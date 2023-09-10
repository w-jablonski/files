
#include QMK_KEYBOARD_H

enum layers {
  _QWERTY,
  _NUM,
  _NUM_NAV,
  _NAV_NUM,
  _FUNC,
  _NAV,
  _CTRL_SPACE,
  _WINDOW,
  _DESKTOP,
  _SWITCHER,
  _CONFIG,
  _CRL // dummy
};

enum keycodes {
  SWITCHER = SAFE_RANGE,
  SWITCH_NEXT,
  MY_NEXT,
  MY_PREV,
  MY_ONE,
  MY_TWO,
  MY_THREE,
  MY_FOUR,
  MY_LINE,
  HEART,
  DIAMOND,
  MY_ENTER,
  MY_SPACE
};

bool window_cycling = false;
bool enter_is_pressed = false;
bool caps_enabled = false;
bool button_pressed = false;
bool is_held = false;
uint16_t switcher_timer = 0;
uint16_t next_timer = 0;

typedef struct {
  bool is_press_action;
  int state;
} tap;

enum {
  SINGLE_TAP = 1,
  SINGLE_HOLD = 2,
  DOUBLE_TAP = 3,
  DOUBLE_HOLD = 4,
  DOUBLE_SINGLE_TAP = 5,
  TRIPLE_TAP = 6,
  TRIPLE_HOLD = 7
};

enum my_keycodes {
  CRL,
  SHF,
  NUM,
  NAV,
  ALGR
};

const uint16_t PROGMEM qq[] = {KC_COMM, KC_Q, COMBO_END};
const uint16_t PROGMEM ee[] = {KC_EQL, KC_E, COMBO_END};
const uint16_t PROGMEM rr[] = {KC_DEL, KC_R, COMBO_END};
const uint16_t PROGMEM tt[] = {KC_DEL, KC_T, COMBO_END};
const uint16_t PROGMEM yy[] = {KC_PSCR, KC_Y, COMBO_END};

const uint16_t PROGMEM qa[] = {KC_Q, KC_A, COMBO_END};
const uint16_t PROGMEM ws[] = {KC_W, KC_S, COMBO_END};
const uint16_t PROGMEM ed[] = {KC_E, KC_D, COMBO_END};
const uint16_t PROGMEM rf[] = {KC_R, KC_F, COMBO_END};
const uint16_t PROGMEM tg[] = {KC_T, KC_G, COMBO_END};
const uint16_t PROGMEM yh[] = {KC_Y, KC_H, COMBO_END};
const uint16_t PROGMEM ik[] = {KC_I, KC_K, COMBO_END};

const uint16_t PROGMEM aa[] = {KC_A, TD(CRL), COMBO_END};
const uint16_t PROGMEM sz[] = {KC_S, KC_Z, COMBO_END};
const uint16_t PROGMEM dc[] = {KC_D, KC_C, COMBO_END};
const uint16_t PROGMEM fv[] = {KC_F, KC_V, COMBO_END};
const uint16_t PROGMEM gb[] = {KC_G, KC_B, COMBO_END};

const uint16_t PROGMEM shift_enter[] = {KC_ENT, MY_SPACE, COMBO_END};
const uint16_t PROGMEM spaceb_enter[] = {KC_ENT, KC_SPC, COMBO_END};
const uint16_t PROGMEM space_enter[] = {KC_ENT, TD(SHF), COMBO_END};
const uint16_t PROGMEM zero12[] = {KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM zero23[] = {KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM zero45[] = {KC_4, KC_5, COMBO_END};
const uint16_t PROGMEM zero56[] = {KC_5, KC_6, COMBO_END};
const uint16_t PROGMEM zero85[] = {KC_8, KC_5, COMBO_END};
const uint16_t PROGMEM dot74[] = {KC_7, KC_4, COMBO_END};
const uint16_t PROGMEM dot96[] = {KC_9, KC_6, COMBO_END};
const uint16_t PROGMEM sd[] = {KC_S, KC_D, COMBO_END};
const uint16_t PROGMEM zc[] = {KC_Z, KC_C, COMBO_END};

combo_t key_combos[] = {

COMBO(qq, KC_F11),
COMBO(ee, KC_HASH),
COMBO(rr, KC_SLSH),
COMBO(tt, KC_F13),
COMBO(yy, KC_F14),

COMBO(qa, KC_NO),
COMBO(ws, C(KC_Z)),
COMBO(ed, KC_TAB),
COMBO(rf, KC_DEL),
COMBO(tg, KC_LPRN),
COMBO(yh, KC_RPRN),
COMBO(ik, KC_COLN),

COMBO(aa, C(KC_A)),
COMBO(sz, KC_BSPC),
COMBO(dc, KC_TAB),
COMBO(fv, KC_DEL),
COMBO(gb, SWITCHER),

COMBO(shift_enter, KC_LGUI),
COMBO(spaceb_enter, KC_LGUI),
COMBO(space_enter, S(KC_LGUI)),
COMBO(zero12, KC_0),
COMBO(zero23, KC_0),
COMBO(zero45, KC_0),
COMBO(zero56, KC_0),
COMBO(zero85, KC_0),
COMBO(dot74, KC_DOT),
COMBO(dot96, KC_DOT),
COMBO(sd, KC_BSPC),
COMBO(zc, C(KC_BSPC)),

};

const key_override_t shift_plus      = ko_make_basic(MOD_MASK_SHIFT, KC_PLUS, KC_ASTR);
const key_override_t shift_minus     = ko_make_basic(MOD_MASK_SHIFT, KC_MINS, KC_SLSH);
const key_override_t shift_backspace = ko_make_basic(MOD_MASK_SHIFT, KC_BSPC, C(KC_BSPC));

const key_override_t alt_i        = ko_make_with_layers(MOD_BIT(KC_LALT), KC_I, KC_UP, (1 << _QWERTY));
const key_override_t alt_j        = ko_make_with_layers(MOD_BIT(KC_LALT), KC_J, KC_LEFT, (1 << _QWERTY));
const key_override_t alt_k        = ko_make_with_layers(MOD_BIT(KC_LALT), KC_K, KC_DOWN, (1 << _QWERTY));
const key_override_t alt_l        = ko_make_with_layers(MOD_BIT(KC_LALT), KC_L, KC_RGHT, (1 << _QWERTY));

const key_override_t ctrl_shift_e = ko_make_with_layers(MOD_MASK_CS, KC_E, KC_WH_U, (1 << _QWERTY));
const key_override_t ctrl_shift_s = ko_make_with_layers(MOD_MASK_CS, KC_S, C(KC_LEFT), (1 << _QWERTY));
const key_override_t ctrl_shift_d = ko_make_with_layers(MOD_MASK_CS, KC_D, KC_WH_D, (1 << _QWERTY));
const key_override_t ctrl_shift_f = ko_make_with_layers(MOD_MASK_CS, KC_F, C(KC_RGHT), (1 << _QWERTY));

const key_override_t menu         = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_ESC, KC_APP, ~0, MOD_MASK_SHIFT);
const key_override_t underscore   = ko_make_basic(MOD_BIT(KC_RALT), KC_M, KC_UNDS);
const key_override_t tilde        = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_T, KC_TILD, ~0, MOD_MASK_SHIFT);
const key_override_t exclaim      = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_I, KC_EXLM, ~0, MOD_MASK_SHIFT);
const key_override_t grave        = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_G, KC_GRV, ~0, MOD_MASK_SHIFT);
const key_override_t quote        = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_J, KC_QUOT, ~0, MOD_MASK_SHIFT);
const key_override_t double_quote = ko_make_basic(MOD_BIT(KC_RALT) | MOD_BIT(KC_LSFT), KC_J, KC_DQUO);
const key_override_t question     = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_K, KC_QUES, ~0, MOD_MASK_SHIFT);
const key_override_t backslash    = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_B, KC_BSLS, ~0, MOD_MASK_SHIFT);
const key_override_t dollar       = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_D, KC_DLR, ~0, MOD_MASK_SHIFT);
const key_override_t percent      = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_P, KC_PERC, ~0, MOD_MASK_SHIFT);
const key_override_t circ         = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_V, KC_CIRC, ~0, MOD_MASK_SHIFT);
const key_override_t lesser       = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_LPRN, KC_LT, ~0, MOD_MASK_SHIFT);
const key_override_t greater      = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_RPRN, KC_GT, ~0, MOD_MASK_SHIFT);
const key_override_t semicolon    = ko_make_basic(MOD_MASK_SHIFT, KC_COLN, KC_SCLN);
const key_override_t pipe         = ko_make_basic(MOD_BIT(KC_RALT) | MOD_BIT(KC_LSFT), KC_I, KC_PIPE);

const key_override_t ctrl_f13      = ko_make_basic(MOD_MASK_CTRL, KC_F13, KC_LCBR);
const key_override_t ctrl_f14      = ko_make_basic(MOD_MASK_CTRL, KC_F14, KC_RCBR);
const key_override_t ctrl_lparen   = ko_make_basic(MOD_MASK_CTRL, KC_LPRN, KC_LBRC);
const key_override_t ctrl_rparen   = ko_make_basic(MOD_MASK_CTRL, KC_RPRN, KC_RBRC);
const key_override_t ctrl_switcher = ko_make_basic(MOD_MASK_CTRL, SWITCHER, KC_ENT);

const key_override_t ctrl_w       = ko_make_with_layers(MOD_MASK_CTRL, KC_W, KC_HOME, (1 << _QWERTY));
const key_override_t ctrl_e       = ko_make_with_layers(MOD_MASK_CTRL, KC_E, KC_UP, (1 << _QWERTY));
const key_override_t ctrl_r       = ko_make_with_layers(MOD_MASK_CTRL, KC_R, KC_END, (1 << _QWERTY));
const key_override_t ctrl_s       = ko_make_with_layers(MOD_MASK_CTRL, KC_S, KC_LEFT, (1 << _QWERTY));
const key_override_t ctrl_d       = ko_make_with_layers(MOD_MASK_CTRL, KC_D, KC_DOWN, (1 << _QWERTY));
const key_override_t ctrl_f       = ko_make_with_layers(MOD_MASK_CTRL, KC_F, KC_RGHT, (1 << _QWERTY));
const key_override_t ctrl_g       = ko_make_with_layers(MOD_MASK_CTRL, KC_G, C(KC_F), ~0);
const key_override_t ctrl_z       = ko_make_with_layers(MOD_MASK_CTRL, KC_Z, C(KC_X), ~0);

//const key_override_t ctrl_del = ko_make_basic(MOD_MASK_CTRL, KC_DEL, KC_P0);
//const key_override_t ctrl_m   = ko_make_basic(MOD_MASK_CTRL, KC_M, KC_P1);
//const key_override_t ctrl_com = ko_make_basic(MOD_MASK_CTRL, KC_COMM, KC_P2);
//const key_override_t ctrl_dot = ko_make_basic(MOD_MASK_CTRL, KC_DOT, KC_P3);
//const key_override_t ctrl_j   = ko_make_basic(MOD_MASK_CTRL, KC_J, KC_LEFT);
//const key_override_t ctrl_k   = ko_make_basic(MOD_MASK_CTRL, KC_K, KC_DOWN);
//const key_override_t ctrl_l   = ko_make_basic(MOD_MASK_CTRL, KC_L, KC_RGHT);
//const key_override_t ctrl_u   = ko_make_basic(MOD_MASK_CTRL, KC_U, KC_P7);
//const key_override_t ctrl_i   = ko_make_basic(MOD_MASK_CTRL, KC_I, KC_UP);
//const key_override_t ctrl_o   = ko_make_basic(MOD_MASK_CTRL, KC_O, KC_P9);

const key_override_t **key_overrides = (const key_override_t *[]){

&shift_plus,
&shift_minus,
&shift_backspace,

&alt_i,
&alt_j,
&alt_k,
&alt_l,

&ctrl_shift_e,
&ctrl_shift_s,
&ctrl_shift_d,
&ctrl_shift_f,

&menu,
&underscore,
&tilde,
&exclaim,
&grave,
&quote,
&double_quote,
&question,
&backslash,
&dollar,
&percent,
&circ,
&lesser,
&greater,
&semicolon,
&pipe,

&ctrl_f13,
&ctrl_f14,
&ctrl_lparen,
&ctrl_rparen,
&ctrl_switcher,

&ctrl_w,
&ctrl_e,
&ctrl_r,
&ctrl_s,
&ctrl_d,
&ctrl_f,
&ctrl_g,
&ctrl_z,

//&ctrl_del,
//&ctrl_m,
//&ctrl_com,
//&ctrl_dot,
//&ctrl_j,
//&ctrl_k,
//&ctrl_l,
//&ctrl_u,
//&ctrl_i,
//&ctrl_o,

NULL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(

_______,		KC_COMM,		KC_DOT,			KC_EQL,			KC_DEL,			KC_DEL,			KC_PSCR,		KC_6,				KC_MINS,		KC_PLUS,		KC_VOLU,		KC_VOLU,
KC_ESC,			KC_Q,				KC_W,				KC_E,				KC_R,				KC_T,				KC_Y,				KC_U,				KC_I,				KC_O,				KC_P,				KC_BSPC,
_______,		KC_A,				KC_S,				KC_D,				KC_F,				KC_G,				KC_H,				KC_J,				KC_K,				KC_L,				KC_N,				KC_ENT,
MY_ENTER,		TD(CRL),		KC_Z,				KC_C,				KC_V,				KC_B,				KC_X,				KC_M,				KC_COMM,		KC_DOT,			KC_LALT,		_______,
_______,		_______,		_______,		TD(NUM),		TD(SHF),		MY_SPACE,		_______,		TD(NAV),		TD(ALGR),		HEART,			DIAMOND,		_______

    ),
    [_NUM] = LAYOUT(

_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,
_______,		_______,		KC_7,				KC_8,				KC_9,				_______,		_______,		KC_7,				KC_8,				KC_9,				_______,		_______,
_______,		KC_BSPC,		KC_4,				KC_5,				KC_6,				KC_PLUS,		KC_PLUS,		KC_4,				KC_5,				KC_6,				_______,		_______,
_______,		_______,		KC_1,				KC_2,				KC_3,				KC_MINS,		KC_MINS,		KC_1,				KC_2,				KC_3,				_______,		_______,
_______,		_______,		_______,		_______,		_______,		KC_SPC,			_______,		_______,		_______,		_______,		_______,		_______

    ),
    [_NUM_NAV] = LAYOUT(

_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,
_______,		_______,		KC_7,				KC_8,				KC_9,				_______,		KC_PGUP,		KC_HOME,		KC_UP,			KC_END,			_______,		_______,
_______,		KC_BSPC,		KC_4,				KC_5,				KC_6,				KC_PLUS,		KC_PGDN,		KC_LEFT,		KC_DOWN,		KC_RGHT,		_______,		_______,
_______,		_______,		KC_1,				KC_2,				KC_3,				KC_MINS,		_______,		_______,		_______,		_______,		_______,		_______,
_______,		_______,		_______,		_______,		_______,		KC_SPC,			_______,		_______,		_______,		_______,		_______,		_______

    ),
    [_NAV_NUM] = LAYOUT(

_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,
_______,		_______,		KC_HOME,		KC_UP,			KC_END,			KC_PGUP,		_______,		KC_7,				KC_8,				KC_9,				_______,		_______,
_______,		_______,		KC_LEFT,		KC_DOWN,		KC_RGHT,		KC_PGDN,		KC_PLUS,		KC_4,				KC_5,				KC_6,				_______,		_______,
_______,		_______,		_______,		_______,		_______,		_______,		KC_MINS,		KC_1,				KC_2,				KC_3,				_______,		_______,
_______,		_______,		_______,		_______,		_______,		KC_SPC,			_______,		_______,		_______,		_______,		_______,		_______

    ),
    [_FUNC] = LAYOUT(

_______,		_______,		KC_F10,			KC_F11,			KC_F12,			_______,		_______,		KC_F10,			KC_F11,			KC_F12,			_______,		_______,
_______,		_______,		KC_F7,			KC_F8,			KC_F9,			_______,		_______,		KC_F7,			KC_F8,			KC_F9,			_______,		_______,
_______,		_______,		KC_F4,			KC_F5,			KC_F6,			KC_Y,				_______,		KC_F4,			KC_F5,			KC_F6,			_______,		_______,
_______,		_______,		KC_F1,			KC_F2,			KC_F3,			KC_N,				_______,		KC_F1,			KC_F2,			KC_F3,			_______,		_______,
_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______

    ),
    [_NAV] = LAYOUT(

_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,
_______,		_______,		C(KC_HOME),	C(KC_UP),		C(KC_END),	C(KC_PGUP),	KC_PGUP,		KC_HOME,		KC_UP,			KC_END,			_______,		_______,
_______,		_______,		C(KC_LEFT),	C(KC_DOWN),	C(KC_RGHT),	C(KC_PGDN),	KC_PGDN,		KC_LEFT,		KC_DOWN,		KC_RGHT,		_______,		_______,
_______,		_______,		C(KC_X),		C(KC_C),		C(KC_V),		_______,		_______,		_______,		_______,		_______,		_______,		_______,
_______,		_______,		_______,		_______,		_______,		KC_SPC,			_______,		_______,		_______,		_______,		_______,		_______

    ),
    [_CTRL_SPACE] = LAYOUT( // cancelling overrides

_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,
_______,		KC_Q,				KC_W,				KC_E,				KC_R,				KC_T,				KC_Y,				KC_U,				KC_I,				KC_O,				KC_P,				_______,
_______,		KC_A,				KC_S,				KC_D,				KC_F,				KC_G,				KC_H,				KC_J,				KC_K,				KC_L,				KC_N,				_______,
_______,		_______,		G(KC_F1),		G(KC_F2),		G(KC_F3),		G(KC_F4),		KC_X,				KC_M,				KC_COMM,		KC_DOT,			_______,		_______,
_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______

    ),
    [_WINDOW] = LAYOUT(

_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,
_______,		G(KC_Q),		G(KC_W),		G(KC_E),		G(KC_R),		G(KC_T),		G(KC_Y),		G(KC_U),		G(KC_I),		G(KC_O),		G(KC_P),		_______,
_______,		G(KC_A),		G(KC_S),		G(KC_D),		G(KC_F),		G(KC_G),		G(KC_H),		G(KC_J),		G(KC_K),		G(KC_L),		G(KC_N),		_______,
_______,		_______,		G(KC_Z),		G(KC_C),		G(KC_V),		G(KC_B),		G(KC_X),		G(KC_M),		G(KC_COMM),	G(KC_DOT),	_______,		_______,
_______,		_______,		_______,		_______,		_______,		G(KC_SPC),	_______,		_______,		_______,		_______,		_______,		_______

    ),
    [_DESKTOP] = LAYOUT(

_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,
_______,		_______,		S(G(KC_7)),	S(G(KC_8)),	S(G(KC_9)),	_______,		_______,		G(KC_7),		G(KC_8),		G(KC_9),		_______,		_______,
_______,		_______,		S(G(KC_4)),	S(G(KC_5)),	S(G(KC_6)),	_______,		_______,		G(KC_4),		G(KC_5),		G(KC_6),		_______,		_______,
_______,		_______,		S(G(KC_1)),	S(G(KC_2)),	S(G(KC_3)),	_______,		_______,		G(KC_1),		G(KC_2),		G(KC_3),		_______,		_______,
_______,		_______,		_______,		_______,		_______,		G(KC_SPC),	_______,		_______,		_______,		_______,		_______,		_______

    ),
    [_SWITCHER] = LAYOUT(

_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,
_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,
_______,		_______,		MY_TWO,			MY_NEXT,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,
_______,		MY_PREV,		MY_TWO,			_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,
_______,		_______,		_______,		_______,		_______,		MY_FOUR,		_______,		_______,		_______,		_______,		_______,		_______

    ),
    [_CONFIG] = LAYOUT(

_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,
_______,		_______,		_______,		RGB_MOD,		RGB_VAI,		RGB_SAI,		RGB_HUI,		_______,		BL_UP,			_______,		_______,		_______,
_______,		_______,		_______,		RGB_RMOD,		RGB_VAD,		RGB_SAD,		RGB_HUD,		_______,		BL_DOWN,		_______,		_______,		_______,
_______,		_______,		_______,		RGB_M_P,		RGB_TOG,		QK_BOOT,		QK_RBT,			EE_CLR,			BL_TOGG,		_______,		_______,		_______,
_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______,		_______

    )
};

layer_state_t layer_state_set_user(layer_state_t state) {
  state = update_tri_layer_state(state, _CRL, _NAV, _WINDOW);
  state = update_tri_layer_state(state, _NUM, _NAV, _DESKTOP);
  return state;
}

void matrix_scan_user(void) {
  if (is_held == true) {
    if (timer_elapsed(next_timer) > 500) {
      next_timer = timer_read();
      tap_code16(KC_PLUS);
    }

  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {

    case SWITCH_NEXT:
      if (record->event.pressed) {
        if (!(get_mods() & MOD_BIT(KC_LCTL) )) {
          next_timer = timer_read();
          is_held = true;
          register_code(KC_LGUI);
          layer_on(_SWITCHER);
          tap_code16(KC_PLUS);
        } else {
          del_mods(MOD_BIT(KC_LCTL));
          int i = 60;
          while (i--) {
            tap_code16(RALT(KC_H));
          }
          tap_code(KC_ENT);
          add_mods(MOD_BIT(KC_LCTL));
        }
      } else {
        is_held = false;
        layer_off(_SWITCHER);
        unregister_code(KC_LGUI);
      }
      return true;

    case SWITCHER:
      if (!(get_mods() & MOD_BIT(KC_LCTL) )) {
        if (record->event.pressed) {
          button_pressed = false;
          switcher_timer = timer_read();
          register_code(KC_LGUI);
          layer_on(_SWITCHER);
        } else {
          if (timer_elapsed(switcher_timer) < 200 && button_pressed == false) {
            tap_code16(LCAG(KC_Z));
          }
          unregister_code(KC_LGUI);
          layer_off(_SWITCHER);
          wait_ms(50);
          tap_code16(LCAG(KC_C));
        }
      } else {
        if (!record->event.pressed) {
          unregister_code(KC_LGUI);
          layer_off(_SWITCHER);
        }
      }
      return true;

    case MY_ENTER:
      if (record->event.pressed) {
        if ((get_mods() & MOD_BIT(KC_LSFT) )) {
          window_cycling = true;
          enter_is_pressed = true;
          register_code(KC_LGUI);
          layer_on(_SWITCHER);
        } else {
          register_code(KC_ENT);
        }
      } else {
        if (window_cycling == true) {
          tap_code16(LCAG(KC_Z));
          enter_is_pressed = false;
          if (!(get_mods() & MOD_BIT(KC_LSFT) )) {
            tap_code16(LCAG(KC_C));
            window_cycling = false;
          }
        }
        unregister_code(KC_LGUI);
        layer_off(_SWITCHER);
        unregister_code(KC_ENT);
      }
      return true;

    case MY_LINE:
      if (record->event.pressed) {
        int i = 60;
        while (i--) {
          tap_code16(RALT(KC_H));
        }
        tap_code(KC_ENT);
        return 0;
      } else {
      }
      return true;

    case MY_ONE:
      if (record->event.pressed) {
        button_pressed = true;
        register_code(KC_1);
      } else {
        unregister_code(KC_1);
      }
      return true;

    case MY_TWO:
      if (record->event.pressed) {
        button_pressed = true;
        register_code(KC_2);
      } else {
        unregister_code(KC_2);
      }
      return true;

    case MY_THREE:
      if (record->event.pressed) {
        button_pressed = true;
        register_code(KC_3);
      } else {
        unregister_code(KC_3);
      }
      return true;

    case MY_FOUR:
      if (record->event.pressed) {
        button_pressed = true;
        register_code(KC_4);
      } else {
        unregister_code(KC_4);
      }
      return true;

    case MY_PREV:
      if (record->event.pressed) {
        button_pressed = true;
        register_code(KC_MINS);
      } else {
        unregister_code(KC_MINS);
      }
      return true;

    case MY_NEXT:
      if (record->event.pressed) {
        button_pressed = true;
        register_code16(KC_PLUS);
      } else {
        unregister_code16(KC_PLUS);
      }
      return true;

    case MY_SPACE:
      if (get_mods() & (MOD_BIT(KC_LCTL) | MOD_BIT(KC_LALT))) {
        if (record->event.pressed) {
          layer_on(_CTRL_SPACE);
        } else {
          layer_off(_CTRL_SPACE);
        }
      } else {
        if (record->event.pressed) {
          register_code(KC_SPC);
        } else {
          unregister_code(KC_SPC);
          layer_off(_CTRL_SPACE);
        }
      }
      return true;

        case HEART: // ♥️
            if (record->event.pressed) {
              tap_code16(RALT(KC_Q));
              tap_code16(S(RALT(KC_BSLS)));
            } else {
            }
            return true;

        case DIAMOND: // ♦️
            if (record->event.pressed) {
              tap_code16(S(RALT(KC_Q)));
              tap_code16(S(RALT(KC_BSLS)));
            } else {
            }
            return true;

    }
    return true;
};

int crl_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (!state->pressed)  return SINGLE_TAP;
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  else if (state->count == 3) {
    if (!state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8;
}

static tap crl_state = {
  .is_press_action = true,
  .state = 0
};

void crl_each(tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
    case 1:
      layer_on(_CRL);
      register_code(KC_LCTL);
      break;
    case 2:
      tap_code16(LCAG(KC_3));
      wait_ms(500);
      del_mods(MOD_BIT(KC_LCTL));
      tap_code(KC_DOWN);
      tap_code(KC_DOWN);
      add_mods(MOD_BIT(KC_LCTL));
      state->finished = true;
      break;
  }
}

void crl_finished (tap_dance_state_t *state, void *user_data) {
  crl_state.state = crl_dance(state);
  switch (crl_state.state) {
    case SINGLE_TAP:
      layer_off(_CRL);
      unregister_code(KC_LCTL);
      tap_code16(LCAG(KC_4));
      break;
    default: break;
  }
}

void crl_reset (tap_dance_state_t *state, void *user_data) {
  switch (crl_state.state) {
    default:
      layer_off(_CRL);
      unregister_code(KC_LCTL);
      break;
  }
  crl_state.state = 0;
}

int shf_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (!state->pressed)  return SINGLE_TAP;
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  else if (state->count == 3) {
    if (!state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8;
}

static tap shf_state = {
  .is_press_action = true,
  .state = 0
};

void shf_each(tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
    case 1:
      caps_word_off();
      register_code(KC_LSFT);
      break;
    case 2:
      unregister_code(KC_LSFT);
      register_code(KC_LGUI);
      break;
    case 3:
      unregister_code(KC_LGUI);
      caps_enabled = true;
      tap_code(KC_CAPS);
      break;
  }
}

void shf_finished (tap_dance_state_t *state, void *user_data) {
  shf_state.state = shf_dance(state);
  switch (shf_state.state) {
    default: break;
  }
}

void shf_reset (tap_dance_state_t *state, void *user_data) {
  switch (shf_state.state) {
    case SINGLE_TAP:
      unregister_code(KC_LSFT);
      if (caps_enabled == true) {
        caps_enabled = false;
        tap_code(KC_CAPS);
      }
      break;
    case SINGLE_HOLD:
      unregister_code(KC_LSFT);
      if (enter_is_pressed == false && window_cycling == true) {
        tap_code16(LCAG(KC_C));
        window_cycling = false;
      }
      break;
    case DOUBLE_TAP:
      unregister_code(KC_LGUI);
      caps_word_on();
      break;
    case DOUBLE_HOLD:
      unregister_code(KC_LGUI);
      break;
    default:
      break;
  }
  shf_state.state = 0;
}

int num_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (!state->pressed)  return SINGLE_TAP;
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  else if (state->count == 3) {
    if (!state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8;
}

static tap num_state = {
  .is_press_action = true,
  .state = 0
};

void num_each(tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
    case 1:
      layer_off(_NUM_NAV);
      layer_on(_NUM);
      break;
    case 2:
      layer_off(_NUM);
      layer_on(_FUNC);
      break;
    case 3:
      layer_off(_FUNC);
      break;
  }
}

void num_finished (tap_dance_state_t *state, void *user_data) {
  num_state.state = num_dance(state);
  switch (num_state.state) {
    default: break;
  }
}

void num_reset (tap_dance_state_t *state, void *user_data) {
  switch (num_state.state) {
    case DOUBLE_TAP:
      layer_on(_NUM_NAV);
      layer_off(_FUNC);
      break;
    default:
      layer_off(_NUM);
      layer_off(_FUNC);
      break;
  }
  num_state.state = 0;
}

int nav_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (!state->pressed)  return SINGLE_TAP;
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  else if (state->count == 3) {
    if (!state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8;
}

static tap nav_state = {
  .is_press_action = true,
  .state = 0
};

void nav_each(tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
    case 1:
      layer_off(_NAV_NUM);
      layer_on(_NAV);
      break;
    case 2:
      layer_off(_NAV);
      layer_on(_NAV_NUM);
      break;
    case 3:
      layer_off(_NAV_NUM);
      break;
  }
}

void nav_finished (tap_dance_state_t *state, void *user_data) {
  nav_state.state = nav_dance(state);
  switch (nav_state.state) {
    default: break;
  }
}

void nav_reset (tap_dance_state_t *state, void *user_data) {
  switch (nav_state.state) {
    case DOUBLE_TAP:
      break;
    default:
      layer_off(_NAV_NUM);
      layer_off(_NAV);
      break;
  }
  nav_state.state = 0;
}

int algr_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (!state->pressed)  return SINGLE_TAP;
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  else if (state->count == 3) {
    if (!state->pressed)  return TRIPLE_TAP;
    else return TRIPLE_HOLD;
  }
  else return 8;
}

static tap algr_state = {
  .is_press_action = true,
  .state = 0
};

void algr_each(tap_dance_state_t *state, void *user_data) {
  switch (state->count) {
    case 1:
      layer_off(_CONFIG);
      register_code(KC_RALT);
      break;
    case 2:
      unregister_code(KC_RALT);
      break;
    case 3:
      layer_on(_CONFIG);
      break;
    case 4:
      layer_off(_CONFIG);
      break;
  }
}

void algr_finished (tap_dance_state_t *state, void *user_data) {
  algr_state.state = algr_dance(state);
  switch (algr_state.state) {
    default: break;
  }
}

void algr_reset (tap_dance_state_t *state, void *user_data) {
  switch (algr_state.state) {
    case TRIPLE_TAP:
      break;
    default:
      unregister_code(KC_RALT);
      layer_off(_CONFIG);
      break;
  }
  algr_state.state = 0;
}

tap_dance_action_t tap_dance_actions[] = {
  [CRL] = ACTION_TAP_DANCE_FN_ADVANCED(crl_each, crl_finished, crl_reset),
  [SHF] = ACTION_TAP_DANCE_FN_ADVANCED(shf_each, shf_finished, shf_reset),
  [NUM] = ACTION_TAP_DANCE_FN_ADVANCED(num_each, num_finished, num_reset),
  [NAV] = ACTION_TAP_DANCE_FN_ADVANCED(nav_each, nav_finished, nav_reset),
  [ALGR] = ACTION_TAP_DANCE_FN_ADVANCED(algr_each, algr_finished, algr_reset),
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
      return 350;
    default:
      return TAPPING_TERM;
  }
}


