// Converted from vial-keymap.vil
// SPDX-License-Identifier: GPL-2.0-or-later

#include QMK_KEYBOARD_H

enum layers {
    _BASE,
    _NAV, // LT on space / backspace
    _SYM, // LT on escape / enter
    _FUN, // MO from NAV
};

enum tap_dances {
    TD_SCLN, // tap ;  hold GUI   double-tap '
    TD_RBRC, // tap ]  hold Alt   double-tap Ctrl+]
};

// Home row mods
#define HM_A LGUI_T(KC_A)
#define HM_S LALT_T(KC_S)
#define HM_D LCTL_T(KC_D)
#define HM_F LSFT_T(KC_F)
#define HM_J LSFT_T(KC_J)
#define HM_K LCTL_T(KC_K)
#define HM_L LALT_T(KC_L)

// Thumbs
#define NAV_SPC LT(_NAV, KC_SPC)
#define SYM_ESC LT(_SYM, KC_ESC)
#define SYM_ENT LT(_SYM, KC_ENT)
#define NAV_BSP LT(_NAV, KC_BSPC)
#define SFT_TAB LSFT_T(KC_TAB) // Shift when held, Tab on a single tap

// clang-format off
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_split_3x5_3(
        KC_Q,     KC_W,     KC_E,     KC_R,     KC_T,          KC_Y,     KC_U,     KC_I,     KC_O,     KC_P,
        HM_A,     HM_S,     HM_D,     HM_F,     KC_G,          KC_H,     HM_J,     HM_K,     HM_L,     TD(TD_SCLN),
        KC_Z,     KC_X,     KC_C,     KC_V,     KC_B,          KC_N,     KC_M,     KC_COMM,  KC_DOT,   KC_SLSH,
                            NAV_SPC,  SFT_TAB,  SYM_ESC,       TD(TD_RBRC), SYM_ENT, NAV_BSP
    ),
    [_NAV] = LAYOUT_split_3x5_3(
        KC_1,     KC_2,     KC_3,     KC_4,     KC_5,          KC_6,     KC_7,     KC_8,     KC_9,     KC_0,
        KC_LGUI,  KC_LALT,  KC_LCTL,  KC_LSFT,  CW_TOGG,       KC_LEFT,  KC_DOWN,  KC_UP,    KC_RGHT,  KC_QUOT,
        A(KC_1),  A(KC_2),  A(KC_3),  A(KC_4),  A(KC_5),       A(KC_6),  KC_LBRC,  KC_RBRC,  KC_PSCR,  MO(_FUN),
                            _______,  _______,  _______,       MO(_FUN), _______,  KC_DEL
    ),
    [_SYM] = LAYOUT_split_3x5_3(
        KC_EXLM,  KC_AT,    KC_HASH,  KC_DLR,   KC_PERC,       KC_CIRC,  KC_AMPR,  KC_ASTR,  _______,  _______,
        KC_GRV,   KC_MINS,  KC_LPRN,  KC_RPRN,  KC_EQL,        KC_SPC,   KC_LBRC,  KC_RBRC,  KC_QUOT,  KC_DQUO,
        KC_TILD,  KC_UNDS,  KC_LCBR,  KC_RCBR,  KC_PLUS,       KC_BSLS,  KC_PIPE,  KC_LABK,  KC_RABK,  KC_QUES,
                            _______,  _______,  _______,       _______,  _______,  _______
    ),
    [_FUN] = LAYOUT_split_3x5_3(
        KC_F1,    KC_F2,    KC_F3,    KC_F4,    KC_F5,         KC_F6,    KC_F7,    KC_F8,    KC_F9,    KC_F10,
        KC_NO,    KC_LGUI,  KC_LCTL,  KC_LSFT,  KC_LALT,       MS_LEFT,  MS_DOWN,  MS_UP,    MS_RGHT,  KC_F11,
        LSA(KC_1),LSA(KC_2),LSA(KC_3),LSA(KC_4),LSA(KC_5),     LSA(KC_6),KC_NO,    KC_NO,    KC_NO,    KC_F12,
                            KC_NO,    KC_NO,    KC_NO,         MS_BTN3,  MS_BTN1,  MS_BTN2
    ),
};
// clang-format on

/* ---------------------------------------------------------------------------
 * Tap-hold tuning
 * ------------------------------------------------------------------------- */

static bool is_thumb(uint16_t keycode) {
    switch (keycode) {
        case NAV_SPC:
        case SYM_ESC:
        case SYM_ENT:
        case NAV_BSP:
        case SFT_TAB:
            return true;
    }
    return false;
}

// SFT_TAB becomes Shift as soon as another key goes down, so it types like a
// real Shift. Tab only comes out when it is tapped on its own.
bool get_hold_on_other_key_press(uint16_t keycode, keyrecord_t *record) {
    return keycode == SFT_TAB;
}

// No tap-then-hold auto-repeat on SFT_TAB: holding it is always Shift.
uint16_t get_quick_tap_term(uint16_t keycode, keyrecord_t *record) {
    if (keycode == SFT_TAB) {
        return 0;
    }
    return QUICK_TAP_TERM;
}

// Thumbs are excluded from Chordal Hold: a thumb layer key must be able to
// hold with keys on its own side (e.g. NAV_SPC + left-hand numbers and mods).
bool get_chordal_hold(uint16_t tap_hold_keycode, keyrecord_t *tap_hold_record, uint16_t other_keycode, keyrecord_t *other_record) {
    if (is_thumb(tap_hold_keycode)) {
        return true;
    }
    return get_chordal_hold_default(tap_hold_record, other_record);
}

uint16_t get_flow_tap_term(uint16_t keycode, keyrecord_t *record, uint16_t prev_keycode) {
    if (!is_flow_tap_key(keycode) || !is_flow_tap_key(prev_keycode)) {
        return 0;
    }
    if (is_thumb(keycode)) {
        return THUMB_FLOW_TAP_TERM;
    }
    switch (keycode) {
        case HM_F:
        case HM_J:
            return SHIFT_FLOW_TAP_TERM; // shorter, so fast capitals still work
    }
    return FLOW_TAP_TERM;
}

/* ---------------------------------------------------------------------------
 * Encoder (overrides the weak handlers in keyboards/cheapino/encoder.c)
 * ------------------------------------------------------------------------- */

// { counter-clockwise, clockwise }. If the direction feels reversed, swap them.
static const uint16_t PROGMEM encoder_turns[][2] = {
    [_BASE] = { KC_VOLU, KC_VOLD },
    [_NAV]  = { MS_WHLD, MS_WHLU },
    [_SYM]  = { UG_PREV, UG_NEXT },
    [_FUN]  = { UG_NEXT, UG_PREV },
};

static const uint16_t PROGMEM encoder_clicks[] = {
    [_BASE] = KC_MUTE,
    [_NAV]  = MS_BTN3,
    [_SYM]  = KC_MPLY,
    [_FUN]  = UG_TOGG,
};

static void encoder_tap(uint16_t keycode) {
    switch (keycode) {
        case UG_TOGG:
            rgblight_toggle();
            break;
        case UG_NEXT:
            rgblight_step();
            break;
        case UG_PREV:
            rgblight_step_reverse();
            break;
        default:
            tap_code16(keycode);
    }
}

void clicked(void) {
    encoder_tap(pgm_read_word(&encoder_clicks[get_highest_layer(layer_state)]));
}

void turned(bool clockwise) {
    encoder_tap(pgm_read_word(&encoder_turns[get_highest_layer(layer_state)][clockwise ? 1 : 0]));
}

/* ---------------------------------------------------------------------------
 * Tap dances (tap / hold / double tap)
 * ------------------------------------------------------------------------- */

typedef struct {
    uint16_t on_tap;
    uint16_t on_hold;
    uint16_t on_double_tap;
    uint16_t tapping_term;
} td_config_t;

static const td_config_t td_configs[] = {
    [TD_SCLN] = { KC_SCLN, KC_LGUI, KC_QUOT, 250 },
    [TD_RBRC] = { KC_RBRC, KC_LALT, C(KC_RBRC), 250 },
};

#define NUM_TAP_DANCES (sizeof(td_configs) / sizeof(td_configs[0]))

static uint16_t td_registered[NUM_TAP_DANCES];

static void td_finished(tap_dance_state_t *state, void *user_data) {
    uint8_t            idx = (uint8_t)(uintptr_t)user_data;
    const td_config_t *cfg = &td_configs[idx];
    uint16_t           keycode;

    if (state->count == 1) {
        keycode = (state->interrupted || !state->pressed) ? cfg->on_tap : cfg->on_hold;
    } else if (state->count == 2) {
        keycode = cfg->on_double_tap;
    } else {
        for (uint8_t i = 0; i < state->count - 1; i++) {
            tap_code16(cfg->on_tap);
        }
        keycode = cfg->on_tap;
    }

    td_registered[idx] = keycode;
    register_code16(keycode);
}

static void td_reset(tap_dance_state_t *state, void *user_data) {
    uint8_t idx = (uint8_t)(uintptr_t)user_data;

    if (td_registered[idx]) {
        unregister_code16(td_registered[idx]);
        td_registered[idx] = KC_NO;
    }
}

#define TD_ENTRY(idx) \
    { .fn = {NULL, td_finished, td_reset, NULL}, .user_data = (void *)(uintptr_t)(idx) }

tap_dance_action_t tap_dance_actions[] = {
    [TD_SCLN] = TD_ENTRY(TD_SCLN),
    [TD_RBRC] = TD_ENTRY(TD_RBRC),
};

uint16_t get_tapping_term(uint16_t keycode, keyrecord_t *record) {
    if (keycode == SFT_TAB) {
        return SHIFT_TAB_TAPPING_TERM;
    }
    if (is_thumb(keycode)) {
        return THUMB_TAPPING_TERM;
    }
    if (IS_QK_TAP_DANCE(keycode)) {
        return td_configs[QK_TAP_DANCE_GET_INDEX(keycode)].tapping_term;
    }
    return TAPPING_TERM;
}

/* ---------------------------------------------------------------------------
 * Combos
 * ------------------------------------------------------------------------- */

enum combos {
    CB_HOME,
    CB_END,
    CB_TAB_L,
    CB_TAB_R,
    CB_GRV,  // A + S
    CB_MINS, // S + D
    CB_QUOT, // L + ;
};

const uint16_t PROGMEM combo_home[]  = {KC_LEFT, KC_DOWN, COMBO_END};
const uint16_t PROGMEM combo_end[]   = {KC_UP, KC_RGHT, COMBO_END};
const uint16_t PROGMEM combo_tab_l[] = {KC_C, KC_V, COMBO_END};
const uint16_t PROGMEM combo_tab_r[] = {KC_M, KC_COMM, COMBO_END};
const uint16_t PROGMEM combo_grv[]   = {HM_A, HM_S, COMBO_END};
const uint16_t PROGMEM combo_mins[]  = {HM_S, HM_D, COMBO_END};
const uint16_t PROGMEM combo_quot[]  = {HM_L, TD(TD_SCLN), COMBO_END};

combo_t key_combos[] = {
    [CB_HOME]  = COMBO(combo_home, KC_HOME),
    [CB_END]   = COMBO(combo_end, KC_END),
    [CB_TAB_L] = COMBO(combo_tab_l, KC_TAB),
    [CB_TAB_R] = COMBO(combo_tab_r, KC_TAB),
    [CB_GRV]   = COMBO(combo_grv, KC_GRV),
    [CB_MINS]  = COMBO(combo_mins, KC_MINS),
    [CB_QUOT]  = COMBO(combo_quot, KC_QUOT),
};

static bool is_hrm_combo(uint16_t combo_index) {
    return combo_index == CB_GRV || combo_index == CB_MINS || combo_index == CB_QUOT;
}

// Home row combos only fire when tapped. Holding both keys past
// COMBO_HOLD_TERM falls back to the mods (e.g. GUI+Alt on A+S).
bool get_combo_must_tap(uint16_t combo_index, combo_t *combo) {
    return is_hrm_combo(combo_index);
}

// Remember the previous key press, so home row combos can be skipped while
// typing fast (like flow tap). Otherwise rolling "as" in "was" gives a hyphen.
static uint16_t prev_press_time;
static uint16_t prev_press_keycode;
static uint16_t last_press_time;
static uint16_t last_press_keycode;

bool pre_process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed && record->event.type == KEY_EVENT) {
        prev_press_time    = last_press_time;
        prev_press_keycode = last_press_keycode;
        last_press_time    = record->event.time;
        last_press_keycode = keycode;
    }
    return true;
}

bool combo_should_trigger(uint16_t combo_index, combo_t *combo, uint16_t keycode, keyrecord_t *record) {
    if (!is_hrm_combo(combo_index) || !record->event.pressed) {
        return true;
    }
    // Second key of the combo: the first one already passed the check.
    for (const uint16_t *key = combo->keys; pgm_read_word(key) != COMBO_END; key++) {
        if (pgm_read_word(key) == prev_press_keycode) {
            return true;
        }
    }
    return TIMER_DIFF_16(record->event.time, prev_press_time) >= COMBO_IDLE_TERM;
}
