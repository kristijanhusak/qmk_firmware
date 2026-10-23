// SPDX-License-Identifier: GPL-2.0-or-later

#pragma once

// Tap-hold
#undef TAPPING_TERM
#define TAPPING_TERM 250 // home row mods
#define TAPPING_TERM_PER_KEY
#define QUICK_TAP_TERM 175
#define QUICK_TAP_TERM_PER_KEY
#define HOLD_ON_OTHER_KEY_PRESS_PER_KEY
#define PERMISSIVE_HOLD_PER_KEY // thumbs only, see get_permissive_hold()
#define CHORDAL_HOLD // thumbs are excluded, see get_chordal_hold()

// Thumb layer-taps (space, esc, enter, backspace)
#define THUMB_TAPPING_TERM 200
#define THUMB_FLOW_TAP_TERM 100 // 0 = no flow tap on thumbs

// Thumb Shift / Tab: tapped alone within this time = Tab, longer = plain Shift
#define SHIFT_TAB_TAPPING_TERM 200

// Flow tap: a tap-hold key pressed within this many ms of the previous
// letter is always a tap. Only applies while typing (not after Ctrl/Alt/GUI).
#define FLOW_TAP_TERM 150
#define SHIFT_FLOW_TAP_TERM 150 // F/J shift

// Combos
#define COMBO_MUST_TAP_PER_COMBO
#define COMBO_SHOULD_TRIGGER
#define COMBO_IDLE_TERM 150 // home row combos need this much pause before them

// Mouse keys
#define MOUSEKEY_DELAY 10
#define MOUSEKEY_INTERVAL 20
#define MOUSEKEY_MOVE_DELTA 8
#define MOUSEKEY_MAX_SPEED 10
#define MOUSEKEY_TIME_TO_MAX 30
#define MOUSEKEY_WHEEL_DELAY 10
#define MOUSEKEY_WHEEL_INTERVAL 80
#define MOUSEKEY_WHEEL_MAX_SPEED 8
#define MOUSEKEY_WHEEL_TIME_TO_MAX 40
