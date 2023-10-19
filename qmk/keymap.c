
#include QMK_KEYBOARD_H

enum layers {
  _QWERTY,
  _MIRROR,
  _CRL,
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
  MY_FIVE,
  MY_SIX,
  MY_U,
  MY_Y,

  MY_UP,
  MY_LEFT,
  MY_DOWN,
  MY_RIGHT,

  MY_LINE,
  HEART,
  DIAMOND,
  MY_ENTER,
  MY_SPACE
};

bool       shift_carry_on = false;
bool     enter_is_pressed = false;
bool     shift_is_pressed = false;
bool     altgr_is_pressed = false;
bool       crl_is_pressed = false;

bool        up_is_pressed = false;
bool      left_is_pressed = false;
bool      down_is_pressed = false;
bool     right_is_pressed = false;

int      arrow_is_pressed = 0;
bool         magic_action = false;
bool         defer_action = false;
bool continue_cursor_roll = false;
bool         caps_enabled = false;
bool       button_pressed = false;
bool              is_held = false;
bool            first_run = true;
uint16_t   switcher_timer = 0;
uint16_t       next_timer = 0;
uint16_t      magic_timer = 0;
uint16_t     second_timer = 0;

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
  SHIFT,
  NUM,
  NAV,
};

const uint16_t PROGMEM qq[] = {KC_COMM, KC_Q, COMBO_END};
const uint16_t PROGMEM ee[] = {KC_EQL, KC_E, COMBO_END};
const uint16_t PROGMEM rr[] = {KC_DEL, KC_R, COMBO_END};
const uint16_t PROGMEM tt[] = {KC_DEL, KC_T, COMBO_END};
const uint16_t PROGMEM yy[] = {KC_PSCR, KC_Y, COMBO_END};

const uint16_t PROGMEM qf[]    = {KC_Q, KC_F, COMBO_END};
const uint16_t PROGMEM ws[]    = {KC_W, KC_S, COMBO_END};
const uint16_t PROGMEM ws_[]   = {KC_W, MY_LEFT, COMBO_END};
const uint16_t PROGMEM ed[]    = {KC_E, KC_D, COMBO_END};
const uint16_t PROGMEM ra[]    = {KC_R, KC_A, COMBO_END};
const uint16_t PROGMEM tg[]    = {KC_T, KC_G, COMBO_END};
const uint16_t PROGMEM yh[]    = {KC_Y, KC_H, COMBO_END};
const uint16_t PROGMEM yh_[]   = {MY_Y, KC_H, COMBO_END};
const uint16_t PROGMEM uj_[]   = {MY_U, KC_J, COMBO_END};
const uint16_t PROGMEM uj[]    = {KC_U, KC_J, COMBO_END};
const uint16_t PROGMEM ik[]    = {KC_I, KC_K, COMBO_END};

const uint16_t PROGMEM ff[]    = {KC_F, TD(CRL), COMBO_END};
const uint16_t PROGMEM sz[]    = {KC_S, KC_Z, COMBO_END};
const uint16_t PROGMEM dc[]    = {KC_D, KC_C, COMBO_END};
const uint16_t PROGMEM dc_[]   = {MY_DOWN, KC_C, COMBO_END};
const uint16_t PROGMEM ab[]    = {KC_A, KC_B, COMBO_END};
const uint16_t PROGMEM gv[]    = {KC_G, KC_V, COMBO_END};

const uint16_t PROGMEM shift_enter[]     = {KC_ENT, MY_SPACE, COMBO_END};
const uint16_t PROGMEM spaceb_enter[]    = {KC_ENT, KC_SPC, COMBO_END};
const uint16_t PROGMEM space_enter[]     = {KC_ENT, TD(SHIFT), COMBO_END};
const uint16_t PROGMEM zero_plus_minus[] = {KC_PLUS, KC_MINS, COMBO_END};
const uint16_t PROGMEM zero52[]          = {KC_5, KC_2, COMBO_END};
const uint16_t PROGMEM zero12[]          = {KC_1, KC_2, COMBO_END};
const uint16_t PROGMEM zero23[]          = {KC_2, KC_3, COMBO_END};
const uint16_t PROGMEM zero45[]          = {KC_4, KC_5, COMBO_END};
const uint16_t PROGMEM zero56[]          = {KC_5, KC_6, COMBO_END};
const uint16_t PROGMEM zero85[]          = {KC_8, KC_5, COMBO_END};
const uint16_t PROGMEM dot74[]           = {KC_7, KC_4, COMBO_END};
const uint16_t PROGMEM dot96[]           = {KC_9, KC_6, COMBO_END};
const uint16_t PROGMEM sd[]              = {KC_S, KC_D, COMBO_END};
const uint16_t PROGMEM zc[]              = {KC_Z, KC_C, COMBO_END};

combo_t key_combos[] = {

COMBO(qq, KC_F11),
COMBO(ee, KC_HASH),
COMBO(rr, KC_SLSH),
COMBO(tt, KC_F13),
COMBO(yy, KC_F14),

COMBO(qf, KC_NO),
COMBO(ws, C(KC_Z)),
COMBO(ws_, C(KC_Z)),
COMBO(ed, KC_TAB),
COMBO(ra, KC_LPRN),
COMBO(tg, KC_RPRN),
COMBO(yh, KC_LPRN),
COMBO(yh_, KC_RCBR),
COMBO(uj_, KC_LCBR),
COMBO(uj, KC_RPRN),
COMBO(ik, KC_COLN),

COMBO(ff, C(KC_A)),
COMBO(sz, KC_BSPC),
COMBO(dc, KC_TAB),
COMBO(dc_, KC_TAB),
COMBO(ab, KC_DEL),
COMBO(gv, SWITCHER),

COMBO(shift_enter, KC_LGUI),
COMBO(spaceb_enter, KC_LGUI),
COMBO(space_enter, S(KC_LGUI)),
COMBO(zero_plus_minus, KC_0),
COMBO(zero52, KC_0),
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

const key_override_t alt_i        = ko_make_with_layers(MOD_BIT(KC_LALT), KC_I, KC_UP, (1 << _QWERTY));
const key_override_t alt_j        = ko_make_with_layers(MOD_BIT(KC_LALT), KC_J, KC_LEFT, (1 << _QWERTY));
const key_override_t alt_k        = ko_make_with_layers(MOD_BIT(KC_LALT), KC_K, KC_DOWN, (1 << _QWERTY));
const key_override_t alt_l        = ko_make_with_layers(MOD_BIT(KC_LALT), KC_L, KC_RGHT, (1 << _QWERTY));

const key_override_t alt_comma    = ko_make_with_layers_and_negmods(MOD_BIT(KC_LALT), KC_COMM, C(KC_C), (1 << _QWERTY), MOD_MASK_CTRL);
const key_override_t alt_h        = ko_make_with_layers_and_negmods(MOD_BIT(KC_LALT), KC_H, C(KC_F), (1 << _QWERTY), MOD_MASK_CTRL);

const key_override_t escape       = ko_make_basic(MOD_BIT(KC_RALT), KC_BSPC, KC_ESC);
const key_override_t menu         = ko_make_basic(MOD_BIT(KC_RALT), KC_ESC, KC_APP);
const key_override_t underscore   = ko_make_basic(MOD_BIT(KC_RALT), KC_M, KC_UNDS);
const key_override_t tilde        = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_T, KC_TILD, ~0, MOD_MASK_SHIFT);
const key_override_t exclaim      = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_I, KC_EXLM, ~0, MOD_MASK_SHIFT);
const key_override_t grave        = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_G, KC_GRV, ~0, MOD_MASK_SHIFT);
const key_override_t quote        = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_J, KC_QUOT, ~0, MOD_MASK_SHIFT);
const key_override_t double_quote = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_H, KC_DQUO, ~0, MOD_MASK_SHIFT);
const key_override_t question     = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_K, KC_QUES, ~0, MOD_MASK_SHIFT);
const key_override_t backslash    = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_B, KC_BSLS, ~0, MOD_MASK_SHIFT);
const key_override_t dollar       = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_D, KC_DLR, ~0, MOD_MASK_SHIFT);
const key_override_t percent      = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_P, KC_PERC, ~0, MOD_MASK_SHIFT);
const key_override_t circ         = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_V, KC_CIRC, ~0, MOD_MASK_SHIFT);
const key_override_t lcurly       = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_LPRN, KC_LCBR, ~0, MOD_MASK_SHIFT);
const key_override_t rcurly       = ko_make_with_layers_and_negmods(MOD_BIT(KC_RALT), KC_RPRN, KC_RCBR, ~0, MOD_MASK_SHIFT);
const key_override_t semicolon    = ko_make_basic(MOD_MASK_SHIFT, KC_COLN, KC_SCLN);
const key_override_t pipe         = ko_make_basic(MOD_BIT(KC_RALT) | MOD_BIT(KC_LSFT), KC_I, KC_PIPE);

const key_override_t ctrl_f13      = ko_make_basic(MOD_MASK_CTRL, KC_F13, KC_LCBR);
const key_override_t ctrl_f14      = ko_make_basic(MOD_MASK_CTRL, KC_F14, KC_RCBR);
const key_override_t lbracket      = ko_make_basic(MOD_MASK_SHIFT, KC_LPRN, KC_LBRC);
const key_override_t rbracket      = ko_make_basic(MOD_MASK_SHIFT, KC_RPRN, KC_RBRC);
const key_override_t ctrl_switcher = ko_make_basic(MOD_MASK_CTRL, SWITCHER, KC_ENT);

const key_override_t ctrl_w       = ko_make_with_layers(MOD_MASK_CTRL, KC_W, KC_HOME, (1 << _QWERTY));
const key_override_t ctrl_r       = ko_make_with_layers(MOD_MASK_CTRL, KC_R, KC_END, (1 << _QWERTY));
const key_override_t ctrl_g       = ko_make_with_layers(MOD_MASK_CTRL, KC_G, C(KC_F), (1 << _QWERTY));
const key_override_t ctrl_b       = ko_make_with_layers(MOD_MASK_CTRL, KC_B, C(KC_V), (1 << _QWERTY));
const key_override_t ctrl_v       = ko_make_with_layers(MOD_MASK_CTRL, KC_V, C(KC_B), (1 << _QWERTY));
const key_override_t ctrl_z       = ko_make_with_layers(MOD_MASK_CTRL, KC_Z, C(KC_X), ~0);

const key_override_t **key_overrides = (const key_override_t *[]){

&alt_i,
&alt_j,
&alt_k,
&alt_l,

&alt_comma,
&alt_h,

&escape,
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
&lcurly,
&rcurly,
&semicolon,
&pipe,

&ctrl_f13,
&ctrl_f14,
&lbracket,
&rbracket,
&ctrl_switcher,

&ctrl_w,
&ctrl_r,
&ctrl_g,
&ctrl_b,
&ctrl_v,
&ctrl_z,

NULL
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_QWERTY] = LAYOUT(

_______,  KC_COMM,  KC_DOT,   KC_EQL,   KC_DEL,   KC_DEL,   KC_PSCR,  KC_AT,    KC_MINS,  KC_PLUS,  _______,  _______,
KC_ESC,   KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     KC_BSPC,
_______,  KC_F,     KC_S,     KC_D,     KC_A,     KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_N,     KC_ENT,
MY_ENTER, TD(CRL),  KC_Z,     KC_C,     KC_B,     KC_V,     KC_X,     KC_M,     KC_COMM,  KC_DOT,   KC_LALT,  _______,
QK_BOOT,  _______,  _______,  TD(NUM),  TD(SHIFT),MY_SPACE, _______,  TD(NAV),  KC_RALT,  HEART,    DIAMOND,  MY_LEFT

    ),
    [_MIRROR] = LAYOUT(

_______,  _______,  _______,  _______,  _______,  _______,  _______,  KC_F10,   KC_F11,   KC_F12,   _______,  _______,
_______,  KC_P,     KC_O,     KC_I,     MY_U,     MY_Y,     _______,  KC_F7,    KC_F8,    KC_F9,    _______,  _______,
_______,  KC_N,     KC_L,     KC_K,     KC_J,     KC_H,     _______,  KC_F4,    KC_F5,    KC_F6,    _______,  _______,
_______,  _______,  KC_DOT,   KC_COMM,  KC_M,     KC_X,     _______,  KC_F1,    KC_F2,    KC_F3,    _______,  _______,
_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______

    ),
    [_CRL] = LAYOUT(

_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  _______,  MY_UP,    _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  MY_LEFT,  MY_DOWN,  MY_RIGHT, _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______

    ),
    [_NUM] = LAYOUT(

_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  KC_7,     KC_8,     KC_9,     _______,  _______,  KC_7,     KC_8,     KC_9,     _______,  _______,
_______,  KC_0,     KC_4,     KC_5,     KC_6,     KC_PLUS,  KC_ASTR,  KC_4,     KC_5,     KC_6,     KC_0,     _______,
_______,  _______,  KC_1,     KC_2,     KC_3,     KC_MINS,  KC_SLSH,  KC_1,     KC_2,     KC_3,     _______,  _______,
_______,  _______,  _______,  _______,  _______,  KC_SPC,   _______,  _______,  _______,  _______,  _______,  _______

    ),
    [_NUM_NAV] = LAYOUT(

_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  KC_7,     KC_8,     KC_9,     _______,  _______,  KC_HOME,  KC_UP,    KC_END,   _______,  _______,
_______,  KC_0,     KC_4,     KC_5,     KC_6,     KC_PLUS,  _______,  KC_LEFT,  KC_DOWN,  KC_RGHT,  _______,  _______,
_______,  _______,  KC_1,     KC_2,     KC_3,     KC_MINS,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  _______,  _______,  _______,  KC_SPC,   _______,  _______,  _______,  _______,  _______,  _______

    ),
    [_NAV_NUM] = LAYOUT(

_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  KC_HOME,  KC_UP,    KC_END,   _______,  _______,  KC_7,     KC_8,     KC_9,     _______,  _______,
_______,  _______,  KC_LEFT,  KC_DOWN,  KC_RGHT,  _______,  KC_ASTR,  KC_4,     KC_5,     KC_6,     KC_0,     _______,
_______,  _______,  _______,  _______,  _______,  _______,  KC_SLSH,  KC_1,     KC_2,     KC_3,     _______,  _______,
_______,  _______,  _______,  _______,  _______,  KC_SPC,   _______,  _______,  _______,  _______,  _______,  _______

    ),
    [_FUNC] = LAYOUT(

_______,  _______,  KC_F10,   KC_F11,   KC_F12,   _______,  _______,  KC_F10,   KC_F11,   KC_F12,   _______,  _______,
_______,  _______,  KC_F7,    KC_F8,    KC_F9,    _______,  _______,  KC_F7,    KC_F8,    KC_F9,    _______,  _______,
_______,  _______,  KC_F4,    KC_F5,    KC_F6,    _______,  _______,  KC_F4,    KC_F5,    KC_F6,    _______,  _______,
_______,  _______,  KC_F1,    KC_F2,    KC_F3,    _______,  _______,  KC_F1,    KC_F2,    KC_F3,    _______,  _______,
_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______

    ),
    [_NAV] = LAYOUT(

_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  KC_HOME,  MY_UP,    KC_END,   _______,  _______,  KC_HOME,  MY_UP,    KC_END,   _______,  _______,
_______,  KC_LCTL,  MY_LEFT,  MY_DOWN,  MY_RIGHT, _______,  _______,  MY_LEFT,  MY_DOWN,  MY_RIGHT, KC_LALT,  _______,
_______,  _______,  C(KC_X),  C(KC_C),  C(KC_V),  _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  _______,  _______,  _______,  KC_SPC,   _______,  _______,  _______,  _______,  _______,  _______

    ),
    [_CTRL_SPACE] = LAYOUT( // reverting overrides

_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,     KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,     _______,
_______,  KC_F,     KC_S,     KC_D,     KC_GRV,   KC_G,     KC_H,     KC_J,     KC_K,     KC_L,     KC_N,     _______,
_______,  _______,  G(KC_F1), G(KC_F2), KC_N,     G(KC_F4), KC_X,     KC_M,     KC_COMM,  KC_DOT,   _______,  _______,
_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______

    ),
    [_WINDOW] = LAYOUT(

_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  G(KC_Q),  G(KC_W),  G(KC_E),  G(KC_R),  G(KC_T),  G(KC_Y),  G(KC_U),  G(KC_I),  G(KC_O),  G(KC_P),  _______,
_______,  G(KC_F),  G(KC_S),  G(KC_D),  G(KC_A),  G(KC_G),  G(KC_H),  G(KC_J),  G(KC_K),  G(KC_L),  G(KC_N),  _______,
_______,  _______,  G(KC_Z),  G(KC_C),  G(KC_B),  G(KC_V),  G(KC_X),  G(KC_M),  G(KC_COMM),G(KC_DOT),_______, _______,
_______,  _______,  _______,  _______,  _______,  G(KC_SPC),_______,  _______,  _______,  _______,  _______,  _______

    ),
    [_DESKTOP] = LAYOUT(

_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  S(G(KC_7)),S(G(KC_8)),S(G(KC_9)),_______,_______, G(KC_7),  G(KC_8),  G(KC_9),  _______,  _______,
_______,  KC_LCTL,  S(G(KC_4)),S(G(KC_5)),S(G(KC_6)),G(KC_PLUS),_______,G(KC_4),G(KC_5),  G(KC_6),  KC_LALT,  _______,
_______,  _______,  S(G(KC_1)),S(G(KC_2)),S(G(KC_3)),G(KC_MINS),_______,G(KC_1),G(KC_2),  G(KC_3),  _______,  _______,
_______,  _______,  _______,  _______,  _______,  G(KC_SPC),_______,  _______,  _______,  _______,  _______,  _______

    ),
    [_SWITCHER] = LAYOUT(

_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  MY_FOUR,  MY_FIVE,  MY_SIX,   _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  MY_ONE,   MY_TWO,   MY_THREE, _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______

    ),
    [_CONFIG] = LAYOUT(

_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,
_______,  _______,  _______,  RGB_MOD,  RGB_VAI,  RGB_SAI,  RGB_HUI,  _______,  BL_UP,    _______,  _______,  _______,
_______,  _______,  _______,  RGB_RMOD, RGB_VAD,  RGB_SAD,  RGB_HUD,  _______,  BL_DOWN,  _______,  _______,  _______,
_______,  _______,  _______,  RGB_M_P,  RGB_TOG,  QK_BOOT,  QK_RBT,   EE_CLR,   BL_TOGG,  _______,  _______,  _______,
_______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______,  _______

    )
};

layer_state_t layer_state_set_user(layer_state_t state) {
  state = update_tri_layer_state(state, _NUM, _NAV, _DESKTOP);
  return state;
}

void end_of_arrow(void) {
  if (crl_is_pressed == true) {
    register_code(KC_LCTL);
  } else if (arrow_is_pressed == 0) {
    unregister_code(KC_LCTL);
    layer_off(_CRL);
  }
  if (shift_is_pressed == true) {
    register_code(KC_LSFT);
  }
}

void arrow_is_pressed_zero(void) {
  if ( !(left_is_pressed | right_is_pressed | up_is_pressed | down_is_pressed) ) {
    arrow_is_pressed = 0;
  }
}

void press_the_button(void) {
  switch (arrow_is_pressed) {
    case 1:
      tap_code(KC_UP);
      break;
    case 2:
      tap_code(KC_BSPC);
      break;
    case 3:
      tap_code(KC_DOWN);
      break;
    case 4:
      tap_code(KC_DEL);
      break;
    case 12:
      tap_code(KC_LEFT);
      break;
    case 14:
      tap_code16(KC_RGHT);
      break;
  }
}

void matrix_scan_user(void) {
  if (is_held == true) {
    if (timer_elapsed(next_timer) > 500) {
      next_timer = timer_read();
      tap_code16(KC_PLUS);
    }
  }
  if (magic_action == true) {
    if (first_run == true) {
      if (timer_elapsed(magic_timer) > 300) {
        first_run = false;
        magic_timer = timer_read();
        press_the_button();
      }
    } else if (timer_elapsed(magic_timer) > 150) {
      magic_timer = timer_read();
      press_the_button();
    }
  } else if (continue_cursor_roll == true) {
    if (timer_elapsed(magic_timer) > 50) {
      magic_timer = timer_read();
      press_the_button();
    }
  }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {

    case KC_MINS:
      if (record->event.pressed) {
        if ( !(get_mods() & (MOD_BIT(KC_LCTL) | MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI) ) ) ) {
          if (get_mods() & (MOD_BIT(KC_RALT) | MOD_BIT(KC_LSFT) ) ) {
            del_mods(MOD_BIT(KC_RALT) );
            del_mods(MOD_BIT(KC_LSFT) );
            register_code(KC_SLSH);
            if (altgr_is_pressed == true) {
              add_mods(MOD_BIT(KC_RALT) );
            }
            if (shift_is_pressed == true) {
              add_mods(MOD_BIT(KC_LSFT) );
            }
            return false;
          }
        }
      } else {
        unregister_code(KC_SLSH);
      }
      return true;

    case KC_PLUS:
      if (record->event.pressed) {
        if ( !(get_mods() & (MOD_BIT(KC_LCTL) | MOD_BIT(KC_LALT) | MOD_BIT(KC_LGUI) ) ) ) {
          if (get_mods() & (MOD_BIT(KC_RALT) | MOD_BIT(KC_LSFT) ) ) {
            del_mods(MOD_BIT(KC_RALT) );
            del_mods(MOD_BIT(KC_LSFT) );
            register_code16(KC_ASTR);
            if (altgr_is_pressed == true) {
              add_mods(MOD_BIT(KC_RALT) );
            }
            if (shift_is_pressed == true) {
              add_mods(MOD_BIT(KC_LSFT) );
            }
            return false;
          }
        }
      } else {
        unregister_code16(KC_ASTR);
      }
      return true;

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
            tap_code16(S(RALT(KC_H) ));
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
        if (shift_is_pressed == true) {
          button_pressed = false;
          del_mods(MOD_BIT(KC_LSFT));
          shift_carry_on = true;
          enter_is_pressed = true;
          switcher_timer = timer_read();
          register_code(KC_LGUI);
          layer_on(_SWITCHER);
        } else {
          register_code(KC_ENT);
        }
      } else {
        enter_is_pressed = false;
        if (timer_elapsed(switcher_timer) < 200 && button_pressed == false && shift_carry_on == true) {
          tap_code16(LCAG(KC_Z));
          if (shift_is_pressed == false) {
            tap_code16(LCAG(KC_C));
            shift_carry_on = false;
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
          tap_code16(S(RALT(KC_H) ));
        }
        tap_code(KC_ENT);
        return 0;
      } else {
      }
      return true;

    case MY_ONE:
      if (record->event.pressed) {
        button_pressed = true;
        tap_code(KC_1);
      }
      return true;

    case MY_TWO:
      if (record->event.pressed) {
        button_pressed = true;
        tap_code(KC_2);
      }
      return true;

    case MY_THREE:
      if (record->event.pressed) {
        button_pressed = true;
        tap_code(KC_3);
      }
      return true;

    case MY_FOUR:
      if (record->event.pressed) {
        button_pressed = true;
        tap_code(KC_4);
      }
      return true;

    case MY_FIVE:
      if (record->event.pressed) {
        button_pressed = true;
        tap_code(KC_5);
      }
      return true;

    case MY_SIX:
      if (record->event.pressed) {
        button_pressed = true;
        tap_code(KC_6);
      }
      return true;

    case MY_U:
      if (record->event.pressed) {
        register_code(KC_U);
      } else {
        unregister_code(KC_U);
      }
      return true;

    case MY_Y:
      if (record->event.pressed) {
        register_code(KC_Y);
      } else {
        unregister_code(KC_Y);
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
      if (get_mods() & (MOD_BIT(KC_LCTL) | MOD_BIT(KC_LALT) )) {
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
        tap_code16(RALT(KC_H));
      } else {
      }
      return true;

    case DIAMOND: // ♦️
      if (record->event.pressed) {
        tap_code16(S(RALT(KC_Q)));
        tap_code16(RALT(KC_H));
      } else {
      }
      return true;

    case MY_LEFT:  
      if (record->event.pressed) {
        unregister_code(KC_LCTL);
        unregister_code(KC_LSFT);
        left_is_pressed = true;
        if (shift_is_pressed == true) {
          arrow_is_pressed = 12;
          register_code(KC_LEFT);
        } else {
          arrow_is_pressed = 2;
          register_code(KC_BSPC);
        }
      } else {
        left_is_pressed = false;
        arrow_is_pressed_zero();
        unregister_code(KC_LEFT);
        unregister_code(KC_BSPC);
        magic_action = false;
        continue_cursor_roll = false;
        end_of_arrow();
      }
      return true;

    case MY_RIGHT:
      if (record->event.pressed) {
        unregister_code(KC_LCTL);
        unregister_code(KC_LSFT);
        right_is_pressed = true;
        if (shift_is_pressed == true) {
          arrow_is_pressed = 14;
          register_code(KC_RGHT);
        } else {
          arrow_is_pressed = 4;
          register_code16(KC_DEL);
        }
      } else {
        right_is_pressed = false;
        arrow_is_pressed_zero();
        unregister_code(KC_RGHT);
        unregister_code(KC_DEL);
        magic_action = false;
        continue_cursor_roll = false;
        end_of_arrow();
      }
      return true;

    case MY_UP:
      if (record->event.pressed) {
        unregister_code(KC_LCTL);
        unregister_code(KC_LSFT);
        up_is_pressed = true;
        if (shift_is_pressed == true) {
          arrow_is_pressed = 11;
          register_code(KC_WH_U);
        } else {
          arrow_is_pressed = 1;
          register_code(KC_UP);
        }
      } else {
        up_is_pressed = false;
        arrow_is_pressed_zero();
        unregister_code(KC_UP);
        unregister_code(KC_WH_U);
        magic_action = false;
        continue_cursor_roll = false;
        end_of_arrow();
      }
      return true;

    case MY_DOWN:
      if (record->event.pressed) {
        unregister_code(KC_LCTL);
        unregister_code(KC_LSFT);
        down_is_pressed = true;
        if (shift_is_pressed == true) {
          arrow_is_pressed = 13;
          register_code(KC_WH_D);
        } else {
          arrow_is_pressed = 3;
          register_code(KC_DOWN);
        }
      } else {
        down_is_pressed = false;
        arrow_is_pressed_zero();
        unregister_code(KC_DOWN);
        unregister_code(KC_WH_D);
        magic_action = false;
        continue_cursor_roll = false;
        end_of_arrow();
      }
      return true;

    case KC_RALT:
      if (record->event.pressed) {
        altgr_is_pressed = true;
        if (crl_is_pressed == true) {
          layer_on(_WINDOW);
          return false;
        }
      } else {
        altgr_is_pressed = false;
        layer_off(_WINDOW);
      }
      return true;

  }
  return true;
}

int tap_dance (tap_dance_state_t *state) {
  if (state->count == 1) {
    if (!state->pressed)  return SINGLE_TAP;
    else return SINGLE_HOLD;
  }
  else if (state->count == 2) {
    if (state->pressed) return DOUBLE_HOLD;
    else return DOUBLE_TAP;
  }
  else if (state->count == 3) {
    if (!state->pressed) return TRIPLE_TAP;
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
      crl_is_pressed = true;
      first_run = true;
      layer_on(_CRL);
      if (arrow_is_pressed != 0 && magic_action == true) {
        unregister_code(KC_LCTL);
        magic_action = false;
        defer_action = false;
        continue_cursor_roll = true;
        magic_timer = timer_read();
        state->finished = true;
      } else {
        register_code(KC_LCTL);
      }
      if (altgr_is_pressed == true) {
        unregister_code(KC_RALT);
        layer_on(_WINDOW);
      }
      break;
    case 2:
      del_mods(MOD_BIT(KC_LCTL));
      tap_code16(LCAG(KC_3));
      wait_ms(500);
      tap_code(KC_DOWN);
      tap_code(KC_DOWN);
      state->finished = true;
      break;
  }
}

void crl_finished (tap_dance_state_t *state, void *user_data) {
  crl_state.state = tap_dance(state);
  switch (crl_state.state) {
    case SINGLE_TAP:
      layer_off(_CRL);
      unregister_code(KC_LCTL);
      crl_is_pressed = false;
      tap_code16(LCAG(KC_4));
      break;
    default: 
      break;
  }
}

void crl_reset (tap_dance_state_t *state, void *user_data) {
  switch (crl_state.state) {
    default:
      crl_is_pressed = false;
      continue_cursor_roll = false;
      layer_off(_WINDOW);
      if (altgr_is_pressed == true) {
        register_code(KC_RALT);
      }
      if (arrow_is_pressed != 0 && arrow_is_pressed != 11 && arrow_is_pressed != 13) {
        magic_timer = timer_read();
        magic_action = true;
        unregister_code(KC_UP);
        unregister_code(KC_LEFT);
        unregister_code(KC_DOWN);
        unregister_code(KC_RGHT);
        unregister_code(KC_BSPC);
        unregister_code(KC_DEL);
        register_code(KC_LCTL);
      } else {
        layer_off(_CRL);
        unregister_code(KC_LCTL);
      }
      press_the_button();

  }
  crl_state.state = 0;
}

static tap shift_state = {
  .is_press_action = true,
  .state = 0
};

void shift_each(tap_dance_state_t *state, void *user_data) {
  if (arrow_is_pressed == 0) {
  switch (state->count) {
    case 1:
      caps_word_off();

      shift_is_pressed = true;
      if (arrow_is_pressed == 0) {
        register_code(KC_LSFT);
      }
      break;
    case 2:
      unregister_code(KC_LSFT);
      shift_is_pressed = false;
      if (arrow_is_pressed == 0) {
        layer_on(_MIRROR);
      }
      break;
    case 3:
      layer_off(_MIRROR);
      if (arrow_is_pressed == 0) {
        caps_enabled = true;
        tap_code(KC_CAPS);
      }
      break;
  }
  } else {
    magic_action = false;
  }
}

void shift_finished (tap_dance_state_t *state, void *user_data) {
  shift_state.state = tap_dance(state);
}

void shift_reset (tap_dance_state_t *state, void *user_data) {
  switch (shift_state.state) {
    case SINGLE_TAP:
      unregister_code(KC_LSFT);
      shift_is_pressed = false;
      if (caps_enabled == true) {
        caps_enabled = false;
        tap_code(KC_CAPS);
      }
      break;
    case SINGLE_HOLD:
      unregister_code(KC_LSFT);
      shift_is_pressed = false;
      if (enter_is_pressed == false && shift_carry_on == true) {
        tap_code16(LCAG(KC_C));
        shift_carry_on = false;
      }
      break;
    case DOUBLE_TAP:
      layer_off(_MIRROR);
      caps_word_on();
      break;
    case DOUBLE_HOLD:
      layer_off(_MIRROR);
      break;
    default:
      break;
  }
  shift_state.state = 0;
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
      register_code(KC_LGUI);
      break;
    case 3:
      unregister_code(KC_LGUI);
      break;
  }
}

void num_finished (tap_dance_state_t *state, void *user_data) {
  num_state.state = tap_dance(state);
}

void num_reset (tap_dance_state_t *state, void *user_data) {
  switch (num_state.state) {
    case DOUBLE_TAP:
      layer_on(_NUM_NAV);
      unregister_code(KC_LGUI);
      break;
    default:
      layer_off(_NUM);
      unregister_code(KC_LGUI);
      break;
  }
  num_state.state = 0;
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
  nav_state.state = tap_dance(state);
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

tap_dance_action_t tap_dance_actions[] = {
  [CRL] = ACTION_TAP_DANCE_FN_ADVANCED(crl_each, crl_finished, crl_reset),
  [SHIFT] = ACTION_TAP_DANCE_FN_ADVANCED(shift_each, shift_finished, shift_reset),
  [NUM] = ACTION_TAP_DANCE_FN_ADVANCED(num_each, num_finished, num_reset),
  [NAV] = ACTION_TAP_DANCE_FN_ADVANCED(nav_each, NULL, nav_reset),
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    case QK_TAP_DANCE ... QK_TAP_DANCE_MAX:
      return 350;
    default:
      return TAPPING_TERM;
  }
}
