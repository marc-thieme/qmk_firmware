#include "action_util.h"
#include "config.h"
#include "keycodes.h"
#include "keymap_us.h"
#include "modifiers.h"
#include "quantum_keycodes.h"
#include "timer.h"
#include "umlaute.h"
#include QMK_KEYBOARD_H

#define _HDP 0
#define _NEO_SYM 2
#define _NEO_NAV 3
#define _NEO_GREEK 4
#define _NEO_MATH 5
#define _MOUSE_FUNCTION 6
#define _MEDIA_KEYS 7
#define _MY_MATH 8

enum my_keycodes { MOUSE_OS_CTRL = SAFE_RANGE };

const uint16_t PROGMEM combo_wgz[]  = {KC_W, KC_G, COMBO_END}; // U + O
const uint16_t PROGMEM combo_gmq[]  = {KC_G, KC_M, COMBO_END}; // D + L
const uint16_t PROGMEM combo_wmqu[] = {KC_W, KC_M, COMBO_END}; // D + L
const uint16_t PROGMEM combo_oe[]   = {KC_O, KC_E, COMBO_END}; // O + E
const uint16_t PROGMEM combo_au[]   = {KC_A, KC_U, COMBO_END}; // O + E
const uint16_t PROGMEM combo_nw[]   = {KC_W, KC_N, COMBO_END}; // O + E
enum { WM, UO, DL, OE, AU, WN };
combo_t key_combos[] = {[UO] = COMBO(combo_wgz, KC_Z), // U + O => Z
                        [DL] = COMBO(combo_gmq, KC_Q), // D + L => Q
                        [WM] = COMBO_ACTION(combo_wmqu),
                        [OE] = COMBO_ACTION(combo_oe), // O + E => custom action: "oe"
                        [AU] = COMBO_ACTION(combo_au), // O + E => custom action: "oe"
                        [WN] = COMBO_ACTION(combo_nw)};
void    process_combo_event(uint16_t idx, bool pressed) {
    if (!pressed) return;
    switch (idx) {
        case WM:
            tap_code(KC_Q);
            tap_code(KC_U);
            return;
        case OE:
            tap_code(KC_O);
            tap_code(KC_E);
            return;
        case AU:
            tap_code(KC_A);
            tap_code(KC_U);
            return;
        case WN:
            tap_code(KC_W);
            tap_code(KC_N);
            return;
    }
}

static uint16_t mouse_os_ctrl_timer;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (!umlaut_adaptive_process(keycode, record)) return false;
    switch (keycode) {
        case MOUSE_OS_CTRL:
            if (record->event.pressed) {
                mouse_os_ctrl_timer = timer_read();
                layer_on(_MOUSE_FUNCTION);
            } else {
                layer_off(_MOUSE_FUNCTION);
                if (!record->tap.interrupted && timer_elapsed(mouse_os_ctrl_timer) < TAPPING_TERM) {
                    add_oneshot_mods(MOD_BIT(KC_LEFT_CTRL));
                }
            }
            return false;
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_HDP] = LAYOUT(
        KC_F, KC_P, LCTL_T(KC_D), KC_L, KC_X,       /**/ KC_EQL, KC_U, LCTL_T(KC_O), KC_Y, KC_B,
        LSFT_T(KC_S), LT(_NEO_SYM,KC_N), LT(_MOUSE_FUNCTION,KC_T), LT(_MOUSE_FUNCTION,KC_H), KC_K,  /**/ KC_COMM, LT(_NEO_NAV,KC_A), LT(_MOUSE_FUNCTION,KC_E), LT(_NEO_SYM,KC_I), LSFT_T(KC_C),
        KC_V, KC_W, KC_G, KC_M, KC_J,               /**/ KC_MINS, KC_DOT, KC_QUOT, KC_QUES, KC_SLSH,
        WIN_T(KC_R), LALT_T(KC_SPC), MOUSE_OS_CTRL, KC_SPC
        ),
    [_NEO_SYM] = LAYOUT(
        UC(L'…'), UC(L'→'), KC_LBRC, KC_RBRC, KC_CIRC,      /**/ KC_EXLM, KC_LT, KC_GT, KC_AMPR, UC(L'ß'),
        KC_BSLS, KC_DLR, KC_LCBR, KC_RCBR, KC_ASTR,         /**/ KC_UNDS, KC_LPRN, KC_RPRN, KC_COLN, KC_SCLN,
        KC_AT, KC_HASH, KC_PIPE, KC_TILD, KC_GRV,           /**/ KC_PLUS, KC_PERC, KC_DQUO, UC(L'ö'), UC(L'ü'),
        _______, _______, _______, MO(_MEDIA_KEYS)
        ),
    [_NEO_NAV] = LAYOUT(
        KC_PGUP, KC_BSPC, KC_UP, KC_DEL, KC_PGDN,           /**/ OSL(_NEO_GREEK), KC_7, KC_8, KC_9, _______,
        LSFT_T(KC_HOME), KC_LEFT, KC_DOWN, KC_RGHT, KC_END, /**/ KC_0, KC_4, KC_5, KC_6, _______,
        KC_ESC, KC_TAB, KC_INS, KC_ENT, KC_UNDO,            /**/ OSL(_NEO_MATH), KC_1, KC_2, KC_3, KC_LSFT,
        _______, LALT_T(KC_0), _______, OSL(_MY_MATH)
        ),
    [_NEO_GREEK] = LAYOUT(
        UC(L'ξ'), UC(L'ς'), UC(L'λ'), UC(L'χ'), UC(L'ω'),   /**/ UC(L'κ'), UC(L'ψ'), UC(L'γ'), UC(L'φ'), UC(L'ϕ'),
        UC(L'υ'), UC(L'ι'), UC(L'α'), UC(L'ε'), UC(L'ο'),   /**/ UC(L'σ'), UC(L'ν'), UC(L'ρ'), UC(L'τ'), UC(L'δ'),
        _______, UC(L'ϵ'), UC(L'η'), UC(L'π'), UC(L'ζ'),    /**/ UC(L'β'), UC(L'μ'), UC(L'ϱ'), UC(L'ϑ'), UC(L'θ'),
        _______, _______, _______, _______
        ),
    [_NEO_MATH] = LAYOUT(
        UC(L'∘'), UC(L'∨'), UC(L'∧'), UC(L'ℂ'), UC(L'×'),   /**/ UC(L'⊕'), UC(L'Ψ'), UC(L'Θ'), UC(L'Φ'), UC(L'ℚ'),
        UC(L'⊂'), UC(L'∫'), UC(L'∀'), UC(L'∃'), UC(L'∈'),   /**/ UC(L'Σ'), UC(L'ℕ'), UC(L'ℝ'), UC(L'∂'), UC(L'Δ'),
        UC(L'∪'), UC(L'∩'), UC(L'⋅'), UC(L'∏'), UC(L'ℤ'),   /**/ UC(L'⋅'), UC(L'∞'), UC(L'₂'), UC(L'₃'), _______,
        _______, _______, _______, _______
        ),
    [_MOUSE_FUNCTION] = LAYOUT(
        _______, MS_WHLU, MS_UP, MS_WHLD, KC_LSFT,          /**/ KC_F12, KC_F7, KC_F8, KC_F9, KC_LEFT_ALT,
        MS_WHLL, MS_LEFT, MS_DOWN, MS_RGHT, MS_WHLR,        /**/ KC_F11, KC_F4, KC_F5, KC_F6, KC_LEFT_CTRL,
        KC_ESC, _______, _______, _______, _______,         /**/ KC_F10, KC_F1, KC_F2, KC_F3, KC_LSFT,
        MS_BTN2, MS_BTN1, _______, _______
        ),
    [_MEDIA_KEYS] = LAYOUT(
        _______, _______, _______, _______, _______,        /**/ _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_LSFT,
        _______, _______, _______, _______, _______,        /**/ KC_MUTE, KC_VOLD, KC_VOLU, KC_MSTP, KC_MFFD,
        _______, _______, _______, _______, _______,        /**/ KC_PSCR, KC_BRID, KC_BRIU, KC_EJCT, QK_BOOTLOADER,
        _______, _______, _______, _______
        ),
    [_MY_MATH] = LAYOUT(
        UC(L'≅'), UC(L'≔'), UC(L'≠'), UC(L'≡'), UC(L'≘'),   /**/ UC(L'≤'), UC(L'⟨'), UC(L'⟩'), UC(L'≥'), UC(L'ⁿ'),
        UC(L'⇐'), UC(L'←'), UC(L'⇔'), UC(L'→'), UC(L'⇒'),   /**/ UC(L'₀'), UC(L'₁'), UC(L'₂'), UC(L'₃'), UC(L'ₙ'),
        UC(L'⫤'), UC(L'∉'), UC(L'∅'), UC(L'↦'), UC(L'⊨'),   /**/ UC(L'⁰'), UC(L'¹'), UC(L'³'), UC(L'²'), UC(L'ᵢ'),
        _______, _______, _______, _______
        ),
};
