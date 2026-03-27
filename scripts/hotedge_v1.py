#!/usr/bin/env python3

"""
hotedge — A clickable/hoverable trigger window (aka hot corner/hot edge) for Wayland

Create an empty window and execute a custom command if a combination of keyboard modifiers
(optional) and mouse button/wheel/hover occurs over that window. By design, a modifier+hover
event will be detected even if the hover (i.e. mouse entering the window) happens first.
Bindings with NO modifier key (e.g. plain left-click) will never fire in mods-only=yes mode.
Should continue to work after keyboard replug.

Dependencies: pyside6, python-evdev, user must be in the 'input' group
Tested with: Sway, Niri, pyside6 6.11.0, python-evdev 1.9.3

Flags:
    --color=RRGGBBAA      Window fill color as 8 hex digits (default: 00FF0066)
    --app-id=STRING       Wayland app_id as seen by the WM
    --size=WxH            Window size in pixels (default: 200x200)
    --hover-delay=INT     Milliseconds pointer must stay in window to trigger hover (default: 30)
    --mods-only=yes|no    If yes, the window only shows up when a modifier key is held
                          (i.e. it will never catch non-modified mouse events).
                          This uses evdev to monitor keys globally, so the user must be
                          in the 'input' group (or run as root). Default: no
    --act=SPEC            Action spec: [mod+]* + mouse_action
                          Modifiers:     alt  ctrl  shift  win
                          Mouse actions: hover  left  middle  right  back  forward
                                         wheel_up  wheel_down  wheel_left  wheel_right
    --run=CMD             Command to run when any immediately preceding --act fires.
                          Multiple --act flags before one --run are OR'd together
    --debug               Send debug messages to output

Examples:

hotedge_v1.py \
    --color=00FF0066 \
    --app-id=hotedge_left \
    --size=200x200 \
    --hover-delay=150 \
    --mods-only=no \
    --act=ctrl+left \
    --act=win+left \
    --run="my_script --some-arg" \
    --act=ctrl+win+hover \
    --run=my_other_script \
    --act=wheel_up \
    --act=wheel_down \
    --run="some_exe arg1 arg2"
hotedge --app-id=hotedge1 --size=1000x30 --mods-only=no --act=ctrl+hover --act=left --run="firefox"
hotedge --app-id=hotedge2 --size=1000x30 --mods-only=yes --act=win+hover --run="featherpad"

Window manager rule examples:

Sway:

no_focus [app_id="hotedge"]
for_window [app_id="hotedge"] floating enable, border none, sticky enable
for_window [app_id="hotedge1"] resize set 1000 80, move position 230 -80
for_window [app_id="hotedge2"] resize set 1000 80, move position 1955 -80

Niri:

window-rule {
    match app-id="hotedge"
    open-floating true
    open-focused false
    draw-border-with-background false
    border { off; }
}
window-rule {
    match app-id="hotedge1"
    default-floating-position x=-810 y=-50 relative-to="top"
}
window-rule {
    match app-id="hotedge2"
    default-floating-position x=710 y=-50 relative-to="top"
}
"""

import sys
import signal
import select
import subprocess
import shlex
import threading
import time

# Allow Ctrl+C to kill the process even inside the Qt event loop
signal.signal(signal.SIGINT, signal.SIG_DFL)

from PySide6.QtWidgets import QApplication, QWidget
from PySide6.QtCore import Qt, QTimer, QSize, QObject, Signal
from PySide6.QtGui import QColor, QPainter, QRegion

# ── Constants ────────────────────────────────────────────────────────────────

VALID_MODIFIERS = {'alt', 'ctrl', 'shift', 'win'}
VALID_ACTIONS   = {'left', 'middle', 'right', 'back', 'forward',
                   'wheel_up', 'wheel_down', 'wheel_left', 'wheel_right', 'hover'}

# Values stored as plain int so they work with & regardless of PySide6 version.
MODIFIER_MAP = {
    'alt':   Qt.AltModifier.value,
    'ctrl':  Qt.ControlModifier.value,
    'shift': Qt.ShiftModifier.value,
    'win':   Qt.MetaModifier.value,   # Super/Win → Qt.MetaModifier on Linux
}

BUTTON_MAP = {
    'left':    Qt.LeftButton,
    'middle':  Qt.MiddleButton,
    'right':   Qt.RightButton,
    'back':    Qt.BackButton,
    'forward': Qt.ForwardButton,
}

# ── Argument parsing ─────────────────────────────────────────────────────────

def parse_color(s):
    """Parse RRGGBBAA or #RRGGBBAA into a QColor."""
    s = s.lstrip('#')
    assert len(s) == 8, \
        f"--color must be exactly 8 hex digits (RRGGBBAA), got '{s}' ({len(s)} chars)"
    try:
        r, g, b, a = int(s[0:2],16), int(s[2:4],16), int(s[4:6],16), int(s[6:8],16)
    except ValueError:
        assert False, f"--color contains non-hex characters: '{s}'"
    return QColor(r, g, b, a)


def parse_size(s):
    """Parse WxH into (int, int)."""
    parts = s.lower().split('x')
    assert len(parts) == 2, \
        f"--size must be WxH format (e.g. 100x100), got '{s}'"
    try:
        w, h = int(parts[0]), int(parts[1])
    except ValueError:
        assert False, f"--size values must be integers, got '{s}'"
    assert w > 0 and h > 0, f"--size values must be positive, got {w}x{h}"
    return w, h


def parse_act(spec):
    """
    Parse an --act value into (frozenset_of_modifier_names, action_string).
    e.g. 'ctrl+win+hover' -> (frozenset({'ctrl','win'}), 'hover')
         'left'           -> (frozenset(), 'left')
    """
    parts = [p.strip().lower() for p in spec.split('+')]
    modifiers = set()
    action = None
    for part in parts:
        if part in VALID_MODIFIERS:
            modifiers.add(part)
        elif part in VALID_ACTIONS:
            assert action is None, \
                f"--act '{spec}' has more than one mouse action " \
                f"(found '{action}' and '{part}')"
            action = part
        else:
            assert False, \
                f"--act '{spec}' has unknown token '{part}'. " \
                f"Valid modifiers: {sorted(VALID_MODIFIERS)}, " \
                f"valid actions: {sorted(VALID_ACTIONS)}"
    assert action is not None, \
        f"--act '{spec}' has no mouse action. Valid actions: {sorted(VALID_ACTIONS)}"
    return (frozenset(modifiers), action)


def parse_args(raw_argv):
    """
    Parse sys.argv[1:] manually to preserve the ordered --act/--run structure.

    Returns:
        bg_color    : QColor
        app_id      : str | None
        win_size    : (int, int)
        hover_delay : int  (ms)
        mods_only   : bool
        debug       : bool
        bindings    : list of (list_of_(frozenset, action_str), command_str)
    """
    bg_color    = None
    app_id      = None
    win_size    = None
    hover_delay = None
    mods_only   = None
    debug       = False
    act_run_tokens = []

    for arg in raw_argv:
        if arg.startswith('--color='):
            assert bg_color is None, "Duplicate --color flag"
            bg_color = parse_color(arg[len('--color='):])

        elif arg.startswith('--app-id='):
            assert app_id is None, "Duplicate --app-id flag"
            app_id = arg[len('--app-id='):]
            assert app_id, "--app-id value must not be empty"

        elif arg.startswith('--size='):
            assert win_size is None, "Duplicate --size flag"
            win_size = parse_size(arg[len('--size='):])

        elif arg.startswith('--hover-delay='):
            assert hover_delay is None, "Duplicate --hover-delay flag"
            val = arg[len('--hover-delay='):]
            try:
                hover_delay = int(val)
            except ValueError:
                assert False, f"--hover-delay must be an integer, got '{val}'"
            assert hover_delay >= 0, f"--hover-delay must be >= 0, got {hover_delay}"

        elif arg == '--debug':
            debug = True

        elif arg.startswith('--mods-only='):
            assert mods_only is None, "Duplicate --mods-only flag"
            val = arg[len('--mods-only='):].lower()
            assert val in ('yes', 'no'), \
                f"--mods-only must be 'yes' or 'no', got '{val}'"
            mods_only = (val == 'yes')

        elif arg.startswith('--act='):
            act_run_tokens.append(('act', arg[len('--act='):]))

        elif arg.startswith('--run='):
            act_run_tokens.append(('run', arg[len('--run='):]))

        else:
            assert False, f"Unknown argument: '{arg}'"

    # Defaults
    if bg_color    is None: bg_color    = QColor(0, 255, 0, 102)   # 00FF0066
    if win_size    is None: win_size    = (200, 200)
    if app_id      is None: app_id      = 'hotedge'
    if hover_delay is None: hover_delay = 30
    if mods_only   is None: mods_only   = False

    # Build bindings list
    assert len(act_run_tokens) > 0, "At least one --act/--run pair is required"

    bindings = []
    current_acts = []

    for kind, value in act_run_tokens:
        if kind == 'act':
            current_acts.append(parse_act(value))
        else:  # 'run'
            assert len(current_acts) > 0, \
                f"--run='{value}' is not preceded by any --act flag"
            assert value.strip(), "--run value must not be empty"
            bindings.append((list(current_acts), value))
            current_acts = []

    assert len(current_acts) == 0, \
        f"Trailing --act flags with no following --run: {[str(a) for a in current_acts]}"
    assert len(bindings) > 0, "No valid --act/--run bindings were parsed"

    return bg_color, app_id, win_size, hover_delay, mods_only, debug, bindings


# ── Evdev modifier monitor ────────────────────────────────────────────────────

# Maps each of our named modifiers to the set of evdev key codes for that modifier
# (left + right variants).  Only used when --mods-only=yes.
def _build_evdev_modifier_codes():
    try:
        from evdev import ecodes
        return {
            'alt':   {ecodes.KEY_LEFTALT,   ecodes.KEY_RIGHTALT},
            'ctrl':  {ecodes.KEY_LEFTCTRL,  ecodes.KEY_RIGHTCTRL},
            'shift': {ecodes.KEY_LEFTSHIFT, ecodes.KEY_RIGHTSHIFT},
            'win':   {ecodes.KEY_LEFTMETA,  ecodes.KEY_RIGHTMETA},
        }
    except ImportError:
        return None


# All evdev modifier codes flattened into one set for quick membership test
def _all_modifier_codes(mod_code_map):
    result = set()
    for codes in mod_code_map.values():
        result |= codes
    return result


class EvdevBridge(QObject):
    """
    Thin QObject living on the main thread that the evdev background thread
    emits signals into.  PySide6 signals are thread-safe to emit from any thread;
    connected slots are always called on the receiver's thread (queued connection).
    """
    # Carries (any_held: bool, qt_mods: int) — qt_mods is the OR of all
    # currently-held modifier Qt flags, or Qt.NoModifier when none are held.
    modifier_held_changed = Signal(bool, int)


def _find_keyboard_devices(mod_codes_all):
    """Return a list of evdev InputDevice objects that have at least one modifier key."""
    from evdev import InputDevice, ecodes, list_devices
    keyboards = []
    for path in list_devices():
        try:
            dev = InputDevice(path)
            caps = dev.capabilities()
            if ecodes.EV_KEY in caps:
                key_set = set(caps[ecodes.EV_KEY])
                if key_set & mod_codes_all:      # has at least one modifier key
                    keyboards.append(dev)
        except Exception:
            pass
    return keyboards


def _pressed_mods_to_qt_flags(pressed_codes: set, mod_code_map: dict) -> int:
    """
    Convert a set of currently-pressed evdev key codes into a Qt modifier
    flags integer, using the same MODIFIER_MAP that HotspotWindow uses.
    Called from the evdev thread; result is passed via Signal to the main thread.
    """
    from PySide6.QtCore import Qt as _Qt
    result = 0
    _name_to_qt = {
        'alt':   _Qt.AltModifier.value,
        'ctrl':  _Qt.ControlModifier.value,
        'shift': _Qt.ShiftModifier.value,
        'win':   _Qt.MetaModifier.value,
    }
    for name, codes in mod_code_map.items():
        if pressed_codes & codes:   # any key for this modifier is held
            result |= _name_to_qt[name]
    return result   # plain int


def _evdev_monitor_loop(bridge: EvdevBridge, mod_codes_all: set,
                        mod_code_map: dict, stop_event: threading.Event,
                        debug: bool = False):
    """
    Background thread: read raw key events from all keyboard devices via select(),
    track which modifier keys are currently pressed, and emit bridge.modifier_held_changed
    whenever the "any modifier held" boolean flips.

    Tight-loop protection: consecutive errors (select failure or read failure) are
    counted; if they accumulate faster than one per second the thread sleeps
    progressively longer (up to 5 s) to prevent busy-looping after suspend/resume
    or device disconnection.
    """
    from evdev import InputDevice, ecodes

    def dbg(msg):
        if debug:
            print(f"[hotedge:evdev] {msg}", file=sys.stderr, flush=True)

    def rebuild():
        devs = _find_keyboard_devices(mod_codes_all)
        print(f"[hotedge] evdev: monitoring {len(devs)} keyboard device(s):", file=sys.stderr)
        for d in devs:
            print(f"          {d.path}  ({d.name})", file=sys.stderr)
        return devs

    keyboards = rebuild()
    assert len(keyboards) > 0, \
        "evdev: no keyboard devices found under /dev/input/ — " \
        "check that you are in the 'input' group and that /dev/input/ is accessible"

    pressed_mods: set  = set()    # currently held modifier key codes
    was_any_held: bool = False    # last reported state

    # Tight-loop watchdog state
    error_count:    int   = 0
    last_error_time: float = 0.0
    ERROR_WINDOW_S:  float = 1.0    # if >MAX_ERRORS errors in this window → sleep
    MAX_ERRORS:      int   = 5
    BACKOFF_MAX_S:   float = 5.0

    # Debug: loop iteration rate tracking
    if debug:
        iter_count: int   = 0
        rate_window: float = time.monotonic()

    while not stop_event.is_set():

        if debug:
            iter_count += 1
            now = time.monotonic()
            if now - rate_window >= 5.0:
                dbg(f"loop rate: {iter_count / (now - rate_window):.1f} iter/s "
                    f"over last {now - rate_window:.1f}s  "
                    f"(pressed_mods={pressed_mods})")
                iter_count  = 0
                rate_window = now

        # ── select() with 1 s timeout so stop_event is checked regularly ──
        try:
            readable, _, _ = select.select(keyboards, [], [], 1.0)
        except Exception as exc:
            now = time.monotonic()
            error_count += 1
            dbg(f"select() error #{error_count}: {exc}")
            # Tight-loop protection: if errors are arriving faster than
            # MAX_ERRORS/ERROR_WINDOW_S, back off with a sleep.
            if now - last_error_time < ERROR_WINDOW_S:
                sleep_s = min(error_count * 0.5, BACKOFF_MAX_S)
                dbg(f"too many errors in {ERROR_WINDOW_S}s, sleeping {sleep_s:.1f}s")
                time.sleep(sleep_s)
            else:
                error_count = 0   # reset counter when errors are spread out
            last_error_time = now
            # Rebuild device list (handles USB unplug, post-suspend fd invalidity)
            keyboards = rebuild()
            continue

        # ── Process readable devices ──
        for dev in readable:
            try:
                for event in dev.read():
                    if event.type != ecodes.EV_KEY:
                        continue
                    if event.code not in mod_codes_all:
                        continue
                    if event.value == 1:       # key-down
                        pressed_mods.add(event.code)
                        dbg(f"key-down code={event.code}  pressed={pressed_mods}")
                    elif event.value == 0:     # key-up
                        pressed_mods.discard(event.code)
                        dbg(f"key-up   code={event.code}  pressed={pressed_mods}")
                    # value == 2 is key-repeat — ignore

                    any_held = len(pressed_mods) > 0
                    if any_held != was_any_held:
                        was_any_held = any_held
                        qt_mods = _pressed_mods_to_qt_flags(pressed_mods, mod_code_map)
                        dbg(f"modifier_held_changed → {any_held}  qt_mods={qt_mods}")
                        bridge.modifier_held_changed.emit(any_held, qt_mods)
                # Successful read resets error counters
                error_count = 0
            except OSError as exc:
                import errno as _errno
                dbg(f"dev.read() error on {dev.path}: {exc}")
                if exc.errno == _errno.ENODEV:
                    # Device physically gone (USB unplug, suspend/resume).
                    # select() will keep returning a dead fd as readable forever,
                    # so we MUST remove it now — sleeping/retrying does not help.
                    print(f"[hotedge] evdev: device gone ({dev.path}), removing and rebuilding",
                          file=sys.stderr, flush=True)
                    # Discard any modifier keys attributed to this device so we
                    # don't get stuck thinking a modifier is held after the device
                    # disappears mid-keypress.
                    pressed_mods.clear()
                    any_held = False
                    if was_any_held:
                        was_any_held = False
                        bridge.modifier_held_changed.emit(False, 0)
                    try:
                        dev.close()
                    except Exception:
                        pass
                    keyboards = [d for d in keyboards if d is not dev]
                    # Re-scan for the reconnected device.  USB keyboards (especially
                    # QMK firmware) are slow to enumerate: they initialise multiple
                    # HID interfaces before /dev/input/eventN appears, so an
                    # immediate rebuild() will find nothing.  We retry on a short
                    # delay loop regardless of whether other devices are still
                    # present — the previous code only retried when the list hit
                    # zero, so a surviving built-in keyboard would mask the missing
                    # QMK device and we'd never re-add it.
                    REDISCOVER_INTERVAL = 2.0   # seconds between rebuild attempts
                    REDISCOVER_TIMEOUT  = 30.0  # give up trying to rediscover after this
                    rediscover_deadline = time.monotonic() + REDISCOVER_TIMEOUT
                    while not stop_event.is_set():
                        time.sleep(REDISCOVER_INTERVAL)
                        new_devs = rebuild()
                        existing_paths = {d.path for d in keyboards}
                        added = [d for d in new_devs if d.path not in existing_paths]
                        if added:
                            keyboards += added
                            dbg(f"rediscovered {len(added)} device(s): "
                                f"{[d.path for d in added]}")
                            break
                        if keyboards:
                            # Other devices still alive; keep scanning until timeout
                            # so we don't abandon the reconnect attempt, but also
                            # don't block event processing forever.
                            if time.monotonic() > rediscover_deadline:
                                dbg("rediscover timeout — giving up, will catch next reconnect")
                                break
                        # No devices at all: keep retrying indefinitely (suspend/resume)
                        print("[hotedge] evdev: no devices found, retrying...",
                              file=sys.stderr, flush=True)
                    break   # restart the outer select() loop with updated keyboards list
                else:
                    # Transient read error — log and continue, select() will
                    # tell us if the fd is still usable next iteration.
                    now = time.monotonic()
                    error_count += 1
                    if now - last_error_time < ERROR_WINDOW_S:
                        sleep_s = min(error_count * 0.5, BACKOFF_MAX_S)
                        dbg(f"too many read errors, sleeping {sleep_s:.1f}s")
                        time.sleep(sleep_s)
                    else:
                        error_count = 0
                    last_error_time = now


# ── Main window ───────────────────────────────────────────────────────────────

class HotspotWindow(QWidget):
    def __init__(self, bg_color: QColor, win_size: tuple,
                 hover_delay_ms: int, mods_only: bool, debug: bool, bindings: list):
        super().__init__()
        self.bg_color       = bg_color
        self.hover_delay_ms = hover_delay_ms
        self.mods_only      = mods_only
        self.debug          = debug
        self.bindings       = bindings

        # Frameless + translucent
        self.setWindowFlags(Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint)
        self.setAttribute(Qt.WA_TranslucentBackground)
        self.setMouseTracking(True)

        # Minimum size fixes (see comments in earlier revision)
        w, h = win_size
        self.setMinimumSize(1, 1)
        self.setFixedSize(w, h)

        # Hover timer
        self._hover_timer = QTimer(self)
        self._hover_timer.setSingleShot(True)
        self._hover_timer.timeout.connect(self._on_hover_triggered)

        # Pass-through state: start in pass-through mode until a modifier is held.
        # We apply the empty mask after show() in enable_pass_events().
        self._passing_through = False

        # Track whether the pointer is currently inside the window.
        # Used to restart the hover timer when a modifier is pressed while
        # the mouse is already inside (both --mods-only modes).
        self._mouse_inside = False

        # Last modifier state reported by evdev, expressed as Qt modifier flags.
        # Used in _on_hover_triggered instead of queryKeyboardModifiers(),
        # which always returns NoModifier on an unfocused Wayland window.
        self._evdev_qt_mods = 0   # plain int; set by on_modifier_held_changed via evdev

        # Set to True by main() after the evdev thread is successfully started.
        # When True, _mod_poll_timer is NOT started on enterEvent because
        # on_modifier_held_changed already handles "modifier pressed while inside"
        # reliably.  Running both paths simultaneously causes a double-trigger:
        # evdev restarts the hover timer, it fires (action #1), then the poll
        # timer detects the same state change and restarts it again (action #2).
        self._evdev_available = False

        # Modifier-polling timer (used when --mods-only=no).
        # Fires every MOD_POLL_MS while the mouse is inside, checks whether
        # the modifier state has changed, and restarts the hover timer if so.
        # This is the only way to detect 'modifier pressed while already inside'
        # without evdev when the window does not accept keyboard focus.
        MOD_POLL_MS = 40   # ~25 polls/s; low enough to be invisible in CPU profiling
        self._last_polled_mods = None
        self._mod_poll_timer = QTimer(self)
        self._mod_poll_timer.setInterval(MOD_POLL_MS)
        self._mod_poll_timer.timeout.connect(self._on_mod_poll)

    # ── Size hints ────────────────────────────────────────────────────────────

    def minimumSizeHint(self) -> QSize:
        return QSize(1, 1)

    def sizeHint(self) -> QSize:
        return self.size()

    # ── Pass-through control ──────────────────────────────────────────────────

    def enable_pass_events(self):
        """
        Call once after show() to put the window into its initial pass-through state.

        Strategy A (default, USE_HIDE_SHOW_WIDGET=False):
            Sets Qt::WindowTransparentForInput on the underlying QWindow, then
            cycles windowHandle().hide()/show() to force the compositor to
            re-commit the surface properties.  Operating at the QWindow level
            (not QWidget) avoids any widget-level visual flash.

        Strategy B (fallback, USE_HIDE_SHOW_WIDGET=True):
            Hides/shows the QWidget entirely.  Window disappears from screen
            while no modifier is held, but is guaranteed to work on every
            compositor with zero input leakage.
        """
        assert self.windowHandle() is not None, \
            "enable_pass_events() called before window was shown — windowHandle() is None"
        self._set_transparent_for_input(True)
        self._passing_through = True

    # ← flip to True if WindowTransparentForInput still intercepts on your compositor
    USE_HIDE_SHOW_WIDGET = True

    def _set_transparent_for_input(self, transparent: bool):
        if self.USE_HIDE_SHOW_WIDGET:
            if transparent:
                self.hide()
            else:
                self.show()
            return

        # Default: WindowTransparentForInput on the underlying QWindow,
        # then cycle the QWindow to re-commit surface properties.
        handle = self.windowHandle()
        if handle is None:
            return
        handle.setFlag(Qt.WindowTransparentForInput, transparent)
        handle.hide()
        handle.show()

    def on_modifier_held_changed(self, any_held: bool, evdev_qt_mods: int):
        """
        Slot connected to EvdevBridge.modifier_held_changed.
        Always called on the main Qt thread via queued signal delivery.

        Called in BOTH --mods-only=yes and --mods-only=no modes whenever evdev
        detects a modifier state change globally.  In mods-only mode it also
        toggles window pass-through; in both modes it restarts the hover timer
        when a modifier is pressed while the mouse is already inside the window.
        This is the only reliable way to detect modifier changes on an unfocused
        Wayland window (queryKeyboardModifiers() always returns NoModifier there).
        """
        if self.debug:
            print(f"[hotedge:qt] on_modifier_held_changed any_held={any_held}  "
                  f"mods_only={self.mods_only}  "
                  f"passing_through={self._passing_through}  "
                  f"mouse_inside={self._mouse_inside}",
                  file=sys.stderr, flush=True)

        # Always store the latest evdev modifier state so _on_hover_triggered
        # can use it instead of queryKeyboardModifiers() (unreliable on
        # unfocused Wayland windows).
        self._evdev_qt_mods = evdev_qt_mods

        if self.mods_only:
            # ── mods-only=yes: also toggle pass-through ──────────────────────
            if any_held and self._passing_through:
                self._set_transparent_for_input(False)
                self._passing_through = False
            elif not any_held and not self._passing_through:
                self._hover_timer.stop()
                self._set_transparent_for_input(True)
                self._passing_through = True
                return   # window is now hidden/pass-through; nothing more to do

        # ── Both modes: restart hover timer if modifier pressed while inside ──
        if any_held and self._mouse_inside:
            if self.debug:
                print("[hotedge:qt] modifier pressed while mouse inside "
                      "→ restarting hover timer", file=sys.stderr, flush=True)
            self._hover_timer.start(self.hover_delay_ms)
        elif not any_held:
            # Modifier released — cancel any pending hover countdown so a
            # half-elapsed timer from a previous press doesn't fire spuriously.
            self._hover_timer.stop()

    def paintEvent(self, event):
        painter = QPainter(self)
        painter.setRenderHint(QPainter.Antialiasing)
        painter.fillRect(self.rect(), self.bg_color)

    # ── Trigger logic ─────────────────────────────────────────────────────────

    @staticmethod
    def _mods_int(qt_mods) -> int:
        """Normalise qt_mods to a plain int.

        PySide6 >= 6.x returns a Qt.KeyboardModifier flag enum from
        event.modifiers(); older builds and our evdev path already return int.
        Both cases are handled here so the rest of the code stays simple.
        """
        return qt_mods.value if hasattr(qt_mods, 'value') else int(qt_mods)

    def _modifiers_match(self, required_mods: frozenset, qt_mods) -> bool:
        """
        Exact match: every modifier in required_mods must be active,
        and no other modifier may be active.
        """
        mods_int = self._mods_int(qt_mods)
        for name in VALID_MODIFIERS:
            required = name in required_mods
            present  = bool(mods_int & MODIFIER_MAP[name])
            if required != present:
                return False
        return True

    def _try_trigger(self, action: str, qt_mods) -> bool:
        """Find and execute the first matching binding, if any."""
        if self.debug:
            print(f"[hotedge:qt] _try_trigger action={action!r} qt_mods={self._mods_int(qt_mods)}",
                  file=sys.stderr, flush=True)
        for act_list, cmd in self.bindings:
            for (req_mods, req_action) in act_list:
                if req_action == action and self._modifiers_match(req_mods, qt_mods):
                    if self.debug:
                        print(f"[hotedge:qt] MATCH → running {cmd!r}",
                              file=sys.stderr, flush=True)
                    self._run(cmd)
                    return True
        return False

    @staticmethod
    def _run(cmd: str):
        try:
            subprocess.Popen(shlex.split(cmd))
        except Exception as exc:
            print(f"[hotedge] error running '{cmd}': {exc}", file=sys.stderr)

    # ── Mouse events ──────────────────────────────────────────────────────────

    def mousePressEvent(self, event):
        for name, qt_btn in BUTTON_MAP.items():
            if event.button() == qt_btn:
                self._try_trigger(name, event.modifiers())
                break
        event.accept()

    def wheelEvent(self, event):
        dy = event.angleDelta().y()
        dx = event.angleDelta().x()
        if dy != 0:
            action = 'wheel_up' if dy > 0 else 'wheel_down'
            self._try_trigger(action, event.modifiers())
        if dx != 0:
            action = 'wheel_right' if dx > 0 else 'wheel_left'
            self._try_trigger(action, event.modifiers())
        event.accept()

    def enterEvent(self, event):
        self._mouse_inside = True
        # Don't read modifiers here — stale cache on Wayland.
        # queryKeyboardModifiers() is called fresh when the hover timer fires.
        self._hover_timer.start(self.hover_delay_ms)
        # Start polling for modifier changes while inside — but only when evdev
        # is NOT available.  If evdev is running, on_modifier_held_changed already
        # restarts the hover timer reliably; starting the poll timer as well
        # causes a double-trigger (evdev fires the timer, action runs, then the
        # poll detects the same change and fires the timer a second time).
        if not self.mods_only and not self._evdev_available:
            self._last_polled_mods = QApplication.queryKeyboardModifiers()
            self._mod_poll_timer.start()

    def leaveEvent(self, event):
        self._mouse_inside = False
        self._hover_timer.stop()
        self._mod_poll_timer.stop()
        self._last_polled_mods = None

    def _on_mod_poll(self):
        """
        Called every MOD_POLL_MS while the mouse is inside and --mods-only=no.
        If the modifier state has changed since the last poll, restart the hover
        timer so that pressing a modifier while inside triggers a fresh countdown.
        """
        mods = QApplication.queryKeyboardModifiers()
        if mods != self._last_polled_mods:
            if self.debug:
                print(f"[hotedge:qt] mod poll: state changed "
                      f"{self._last_polled_mods!r} → {mods!r}, restarting hover timer",
                      file=sys.stderr, flush=True)
            self._last_polled_mods = mods
            self._hover_timer.start(self.hover_delay_ms)   # restart countdown

    def _on_hover_triggered(self):
        # Use _evdev_qt_mods (set by on_modifier_held_changed) rather than
        # queryKeyboardModifiers(), which always returns NoModifier on an
        # unfocused Wayland window — the root cause of "only fires once".
        self._try_trigger('hover', self._evdev_qt_mods)


# ── Entry point ───────────────────────────────────────────────────────────────

def main():
    bg_color, app_id, win_size, hover_delay, mods_only, debug, bindings = \
        parse_args(sys.argv[1:])

    app = QApplication(sys.argv)

    if app_id:
        app.setDesktopFileName(app_id)   # sets Wayland app_id
        app.setApplicationName(app_id)   # compositor fallback

    win = HotspotWindow(bg_color, win_size, hover_delay, mods_only, debug, bindings)
    win.show()

    # ── Set up evdev monitoring if requested ──────────────────────────────────
    evdev_thread  = None
    stop_event    = None

    # Start evdev modifier monitor whenever evdev is available.
    # Used for pass-through toggling (--mods-only=yes) AND for reliable
    # "modifier pressed while mouse inside" detection in both modes.
    # queryKeyboardModifiers() is unreliable on unfocused Wayland windows,
    # making evdev the only dependable modifier source in either case.
    mod_code_map = _build_evdev_modifier_codes()

    if mod_code_map is None:
        if mods_only:
            assert False, "--mods-only=yes requires the 'evdev' package: pip install evdev"
        else:
            print("[hotedge] WARNING: 'evdev' package not found (pip install evdev). "
                  "Modifier detection while mouse is stationary inside the window "
                  "will be unreliable on Wayland. Mouse movement inside the window "
                  "will still update modifier state via mouseMoveEvent.",
                  file=sys.stderr, flush=True)
    else:
        mod_codes_all = _all_modifier_codes(mod_code_map)

        bridge = EvdevBridge(parent=win)
        bridge.modifier_held_changed.connect(win.on_modifier_held_changed)

        if mods_only:
            # Put window into initial pass-through state.
            # Must be called after show() so the Wayland surface exists.
            win.enable_pass_events()

        stop_event   = threading.Event()
        evdev_thread = threading.Thread(
            target=_evdev_monitor_loop,
            args=(bridge, mod_codes_all, mod_code_map, stop_event, debug),
            daemon=True,
            name='evdev-monitor',
        )
        evdev_thread.start()
        # Tell the window that evdev is handling modifier detection so it does
        # not also start the poll-based fallback timer (which would double-trigger
        # modifier+hover actions in --mods-only=no mode).
        win._evdev_available = True

    exit_code = app.exec()

    # Clean shutdown of the evdev thread (if it was started)
    try:
        stop_event.set()
        evdev_thread.join(timeout=2.0)
    except NameError:
        pass   # evdev was not available, no thread to stop

    sys.exit(exit_code)


if __name__ == '__main__':
    main()
