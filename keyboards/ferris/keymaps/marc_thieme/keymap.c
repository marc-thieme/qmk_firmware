#include "action_util.h"
#include "helpers.h"
#include "keycodes.h"
#include "modifiers.h"
#include "quantum_keycodes.h"
#include QMK_KEYBOARD_H

#define _NEO 0
#define _NEO_SYM 2
#define _NEO_NAV 3
#define _NEO_GREEK 4
#define _NEO_MATH 5
#define _MOUSE_MEDIA 6
#define _FUNCTION_KEYS 7

#undef UNICODE_SELECTED_MODES
#define UNICODE_SELECTED_MODES UC_MAC, UC_LNX, UC_WIN, UC_WINC

enum my_keycodes { WIN_OS_CTRL = SAFE_RANGE, KC_BNCP, KC_BNM4, KC_EN_UC_WIN };

enum combo_events {
    COMBO_GREEK,
    COMBO_MATH,
};

const uint16_t PROGMEM greek_combo[] = {KC_LALT, KC_LWIN, COMBO_END};
const uint16_t PROGMEM math_combo[]  = {KC_LWIN, KC_LCTL, COMBO_END};

combo_t key_combos[] = {
    [COMBO_GREEK] = COMBO_ACTION(greek_combo),
    [COMBO_MATH]  = COMBO_ACTION(math_combo),
};

// void process_combo_event(uint16_t combo_index, bool pressed) {
//     switch (combo_index) {
//         case COMBO_GREEK:
//             if (pressed) {
//                 set_oneshot_layer(_NEO_GREEK, ONESHOT_START);
//             }
//             break;
//         case COMBO_MATH:
//             if (pressed) {
//                 set_oneshot_layer(_NEO_MATH, ONESHOT_START);
//             }
//             break;
//     }
// }
// const key_override_t delete_key_override = ko_make_basic(MOD_MASK_GUI, KC_BSPC5, KC_DEL);
// #@!^%$(*&

// This globally defines all key overrides to be used
const key_override_t *key_overrides[] = {
	// &delete_key_override
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case WIN_OS_CTRL:
            if (record->event.pressed) {
                register_mods(MOD_BIT(KC_LGUI));
            } else {
                unregister_mods(MOD_BIT(KC_LGUI));
                if (record->tap.count && !record->tap.interrupted) {
                    set_oneshot_mods(MOD_BIT(KC_LCTL));
                }
            }
            return false; // Skip further processing
    }
    return true;
}
// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//   switch (keycode) {
//     case MY_GUI_OSM_CTL:
//       if (record->event.pressed) {
//         // Held: act as GUI modifier
//         register_mods(MOD_BIT(KC_LGUI));
//       } else {
//         // On release: distinguish tap vs hold
//         if (record->tap.count && !record->tap.interrupted) {
//           // Tapped: set one-shot Ctrl, and ensure GUI is not registered
//           unregister_mods(MOD_BIT(KC_LGUI));
//           set_oneshot_mods(MOD_BIT(KC_LCTL));
//         } else {
//           // Released after hold: release GUI
//           unregister_mods(MOD_BIT(KC_LGUI));
//         }
//       }
//       return false; // we've handled this keycode

//     default:
//       return true;
//   }
// }

// bool process_record_user(uint16_t keycode, keyrecord_t *record) {
//   switch (keycode) {
//     case KC_RSFT:
//         layer_invert(_NEO_SHIFT);
//         update_tri_layer(_NEO_SYM, _NEO_SHIFT, _NEO_GREEK);
//         return true;
//     case KC_BNCP:
//         layer_invert(_NEO_SYM);
//         update_tri_layer(_NEO_SYM, _NEO_SHIFT, _NEO_GREEK);
//         update_tri_layer(_NEO_NAV, _NEO_SYM, _NEO_MATH);
//         return false;
//     case KC_BNM4:
//         layer_invert(_NEO_NAV);
//         update_tri_layer(_NEO_NAV, _NEO_SYM, _NEO_MATH);
//         return false;
//     case KC_EN_UC_WIN:
//         if(record->event.pressed)
//             SEND_STRING("reg add \"HKCU\\Control Panel\\Input Method\" -v EnableHexNumpad -t REG_SZ -d 1");
//         return false;
//     default:
//          return true;
//   }
// }

void autoshift_press_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
    if (is_number_key(keycode)) {
        register_code16(LSFT(keycode));
        return;
    }
    switch (keycode) {
        case KC_DOT:
            register_code16((!shifted) ? KC_DOT : UC(L'–'));
            break;
        case KC_COMMA:
            register_code16((!shifted) ? KC_COMMA : UC(L'•'));
            break;
        default:
            if (shifted) {
                add_weak_mods(MOD_BIT(KC_LSFT));
            }
            // & 0xFF gets the Tap key for Tap Holds, required when using Retro Shift
            register_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
    }
}
void autoshift_release_user(uint16_t keycode, bool shifted, keyrecord_t *record) {
    if (is_number_key(keycode)) {
        unregister_code16(LSFT(keycode));
        return;
    }
    switch (keycode) {
        case KC_DOT:
            unregister_code16((!shifted) ? KC_DOT : UC(L'–'));
            break;
        case KC_COMMA:
            unregister_code16((!shifted) ? KC_COMMA : UC(L'•'));
            break;
        default:
            // & 0xFF gets the Tap key for Tap Holds, required when using Retro Shift
            // The IS_RETRO check isn't really necessary here, always using
            // keycode & 0xFF would be fine.
            unregister_code16((IS_RETRO(keycode)) ? keycode & 0xFF : keycode);
    }
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_NEO] = LAYOUT(
        // ┌────────┬────────┬────────┬────────┬────────┐       ┌────────┬────────┬────────┬────────┬────────┐
        KC_X, KC_V, KC_L, KC_C, KC_W, KC_K, KC_H, KC_G, KC_F, KC_Q,
        // ├────────┼────────┼────────┼────────┼────────┤       ├────────┼────────┼────────┼────────┼────────┤
        KC_U, KC_I, KC_A, KC_E, KC_O, KC_S, KC_N, KC_R, KC_T, KC_D,
        // ├────────┼────────┼────────┼────────┼────────┤       ├────────┼────────┼────────┼────────┼────────┤
        UC(L'ü'), UC(L'ö'), UC(L'ä'), KC_P, KC_Z, KC_B, KC_M, KC_COMMA, KC_DOT, KC_J,
        // └────────┴────────┴────────┼────────┼────────┤       ├────────┴────────┴────┬───┴────────┴┬───────┘
        WIN_OS_CTRL, ALT_T(KC_SPC), LT(_NEO_NAV, KC_SPC), MO(_NEO_SYM)
        //                            └────────┴────────┘       └──────────────────────┴─────────────┘
        ),
    [_NEO_SYM] = LAYOUT(
        // ┌────────┬────────┬────────┬────────┬────────┐       ┌────────┬────────┬────────┬────────┬────────┐
        UC(L'…'), KC_UNDS, KC_LBRC, KC_RBRC, KC_CIRC, KC_EXLM, KC_LT, KC_GT, KC_EQL, KC_AMPR,
        // ├────────┼────────┼────────┼────────┼────────┤       ├────────┼────────┼────────┼────────┼────────┤
        KC_BSLS, KC_SLSH, KC_LCBR, KC_RCBR, KC_ASTR, KC_QUES, KC_LPRN, KC_RPRN, KC_MINS, KC_COLN,
        // ├────────┼────────┼────────┼────────┼────────┤       ├────────┼────────┼────────┼────────┼────────┤
        KC_HASH, KC_DLR, KC_PIPE, KC_TILD, KC_GRV, KC_PLUS, KC_PERC, KC_DQUO, KC_QUOT, KC_SCLN,
        // └────────┴────────┴────────┼────────┼────────┤       ├────────┼────────┼───────┴────────┴────────┘
        _______, _______, _______, _______
        //                           └────────┴────────┘       └────────┴────────┘
        ),
    [_NEO_NAV] = LAYOUT(
        // ┌────────┬────────┬────────┬────────┬────────┐       ┌────────┬────────┬────────┬────────┬────────┐
        KC_PGUP, KC_BSPC, KC_UP, KC_DEL, KC_PGDN, _______, KC_7, KC_8, KC_9, _______,
        // ├───────┼────────┼────────┼────────┼─────────┤       ├────────┼────────┼────────┼────────┼────────┤
        KC_HOME, KC_LEFT, KC_DOWN, KC_RGHT, KC_END, KC_AT, KC_4, KC_5, KC_6, KC_Y,
        // ├───────┼────────┼────────┼────────┼─────────┤       ├────────┼────────┼────────┼────────┼────────┤
        KC_ESC, KC_TAB, KC_INS, KC_ENT, KC_UNDO, UC(L'ß'), KC_1, KC_2, KC_3, _______,
        // └────────┴────────┴────────┼────────┼────────┤       ├────────┼────────┼────────┴────────┴────────┘
        _______, _______, _______, _______
        //                           └────────┴────────┘       └────────┴────────┘
        ),
    [_NEO_GREEK] = LAYOUT(
        // ┌───────┬────────┬────────┬────────┬─────────┐       ┌────────┬────────┬────────┬────────┬────────┐
        UC(L'ξ'), UC(L'ς'), UC(L'λ'), UC(L'χ'), UC(L'ω'), UC(L'κ'), UC(L'ψ'), UC(L'γ'), UC(L'φ'), UC(L'ϕ'),
        // ├───────┼────────┼────────┼────────┼─────────┤       ├────────┼────────┼────────┼────────┼────────┤
        UC(L'υ'), UC(L'ι'), UC(L'α'), UC(L'ε'), UC(L'ο'), UC(L'σ'), UC(L'ν'), UC(L'ρ'), UC(L'τ'), UC(L'δ'),
        // ├───────┼────────┼────────┼────────┼─────────┤       ├────────┼────────┼────────┼────────┼────────┤
        XXXXXXX, UC(L'ϵ'), UC(L'η'), UC(L'π'), UC(L'β'), UC(L'μ'), UC(L'ζ'), UC(L'ϱ'), UC(L'ϑ'), UC(L'θ'),
        // └───────┴────────┴────────┼────────┼─────────┤       ├────────┼────────┼────────┴────────┴────────┘
        _______, _______, _______, _______
        //                          └────────┴─────────┘       └────────┴────────┘
        ),
    [_NEO_MATH] = LAYOUT(
        // ┌────────┬────────┬────────┬────────┬────────┐       ┌────────┬────────┬────────┬────────┬────────┐
        UC(L'∘'), UC(L'√'), UC(L'Λ'), UC(L'ℂ'), UC(L'Ω'), UC(L'×'), UC(L'Ψ'), UC(L'Γ'), UC(L'Φ'), UC(L'ℚ'),
        // ├────────┼────────┼────────┼────────┼────────┤       ├────────┼────────┼────────┼────────┼────────┤
        UC(L'⊂'), UC(L'∫'), UC(L'∀'), UC(L'∃'), UC(L'∈'), UC(L'Σ'), UC(L'ℕ'), UC(L'ℝ'), UC(L'∂'), UC(L'Δ'),
        // └────────┴────────┴────────┼────────┼────────┤       ├────────┼────────┼────────┴────────┴────────┘
        UC(L'∪'), UC(L'∩'), UC(L'∇'), UC(L'Π'), C(L'ℤ'), UC(L'⇐'), UC(L'⇔'), UC(L'⇒'), UC(L'↦'), UC(L'Θ'),
        // └────────┴────────┴────────┼────────┼────────┤       ├────────┼────────┼────────┴────────┴────────┘
        _______, _______, _______, _______
        //                           └────────┴────────┘       └────────┴────────┘
        ),
    [_MOUSE_MEDIA] = LAYOUT(
        // ┌────────┬────────┬───────┬────────┬────────┐       ┌────────┬────────┬────────┬────────┬────────┐
        MS_WHLL, MS_WHLD, MS_UP, MS_WHLU, MS_WHLR, _______, KC_MPRV, KC_MPLY, KC_MNXT, _______,
        // ├───────┼────────┼────────┼────────┼────────┤       ├────────┼────────┼────────┼────────┼────────┤
        MS_BTN1, MS_LEFT, MS_DOWN, MS_RGHT, MS_BTN2, KC_MUTE, KC_VOLD, KC_VOLU, KC_MSTP, KC_MFFD,
        // ├───────┼────────┼────────┼────────┼────────┤       ├────────┼────────┼────────┼────────┼────────┤
        _______, _______, _______, _______, _______, _______, KC_BRID, KC_BRIU, KC_EJCT, _______,
        // └───────┴────────┴────────┼────────┼────────┤       ├────────┼────────┼────────┴────────┴────────┘
        _______, _______, _______, _______
        //                          └────────┴────────┘       └────────┴────────┘
        )};
