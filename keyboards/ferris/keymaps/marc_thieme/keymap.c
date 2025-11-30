#include "action_util.h"
#include "config.h"
#include "keycodes.h"
#include "modifiers.h"
#include "quantum_keycodes.h"
#include "timer.h"
#include QMK_KEYBOARD_H

#define _NEO 0
#define _NEO_SYM 2
#define _NEO_NAV 3
#define _NEO_GREEK 4
#define _NEO_MATH 5
#define _MOUSE_MEDIA 6
#define _FUNCTION_KEYS 7

enum my_keycodes { WIN_OS_L3 = SAFE_RANGE, L3_OS_CTRL, ALT_OS_SHIFT };

static uint16_t win_os_shift_timer;
static uint16_t l3_os_ctrl_timer;
static uint16_t win_os_l3_timer;

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    switch (keycode) {
        case ALT_OS_SHIFT:
            if (record->event.pressed) {
                win_os_shift_timer = timer_read();
                register_mods(MOD_BIT(KC_LALT));
            } else {
                unregister_mods(MOD_BIT(KC_LALT));
                if (!record->tap.interrupted && timer_elapsed(win_os_shift_timer) < TAPPING_TERM) {
                    add_oneshot_mods(MOD_BIT(KC_LEFT_SHIFT));
                }
            }
            return false;
        case L3_OS_CTRL:
            if (record->event.pressed) {
                l3_os_ctrl_timer = timer_read();
                layer_on(_NEO_SYM);
            } else {
                layer_off(_NEO_SYM);
                if (!record->tap.interrupted && timer_elapsed(l3_os_ctrl_timer) < TAPPING_TERM) {
                    add_oneshot_mods(MOD_BIT(KC_LEFT_CTRL));
                }
            }
            return false;
        case WIN_OS_L3:
            if (record->event.pressed) {
                win_os_l3_timer = timer_read();
                register_mods(MOD_BIT(KC_LGUI));
            } else {
                unregister_mods(MOD_BIT(KC_LGUI));
                if (!record->tap.interrupted && timer_elapsed(win_os_l3_timer) < TAPPING_TERM) {
                    set_oneshot_layer(_NEO_SYM, ONESHOT_START);
                    clear_oneshot_layer_state(ONESHOT_PRESSED);
                }
            }
            return false;
    }
    return true;
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_NEO] = LAYOUT(
        // ┌────────┬────────┬────────┬────────┬────────┐       ┌────────┬────────┬────────┬────────┬────────┐
            KC_X    , KC_V   , KC_L   , KC_C   , KC_W           , KC_K   , KC_H   , KC_G   , KC_F   , KC_Q,
        // ├────────┼────────┼────────┼────────┼────────┤       ├────────┼────────┼────────┼────────┼────────┤
            KC_U    , KC_I   , KC_A   , KC_E   , KC_O           , KC_S   , KC_N   , KC_R   , KC_T   , KC_D,
        // ├────────┼────────┼────────┼────────┼────────┤       ├────────┼────────┼────────┼────────┼────────┤
            UC(L'ü'),UC(L'ö'),UC(L'ä'), KC_P   , KC_Z           , KC_B   , KC_M   ,KC_COMMA, KC_DOT , KC_J,
        // └────────┴────────┴────────┼────────┼────────┤       ├────────┴────────┴────┬───┴────────┴┬───────┘
                                WIN_OS_L3, ALT_OS_SHIFT, LT(_NEO_NAV, KC_SPC), L3_OS_CTRL
        //                            └────────┴────────┘       └──────────────────────┴─────────────┘
        ),
    [_NEO_SYM] = LAYOUT(
        // ┌────────┬────────┬────────┬────────┬────────┐       ┌────────┬────────┬────────┬────────┬────────┐
            UC(L'…'), KC_UNDS, KC_LBRC, KC_RBRC, KC_CIRC        , KC_EXLM, KC_LT  ,   KC_GT,  KC_EQL, KC_AMPR,
        // ├────────┼────────┼────────┼────────┼────────┤       ├────────┼────────┼────────┼────────┼────────┤
            KC_BSLS , KC_SLSH, KC_LCBR, KC_RCBR, KC_ASTR        , KC_QUES, KC_LPRN, KC_RPRN, KC_MINS, KC_COLN,
        // ├────────┼────────┼────────┼────────┼────────┤       ├────────┼────────┼────────┼────────┼────────┤
            KC_HASH , KC_DLR , KC_PIPE, KC_TILD, KC_GRV         , KC_PLUS, KC_PERC, KC_DQUO, KC_QUOT, KC_SCLN,
        // └────────┴────────┴────────┼────────┼────────┤       ├────────┼────────┼───────┴────────┴────────┘
        _______, _______, MO(_FUNCTION_KEYS), _______
        //                            └────────┴────────┘       └────────┴────────┘
        ),
    [_NEO_NAV] = LAYOUT(
        // ┌────────┬────────┬────────┬────────┬─────────┐      ┌──────────────┬────────┬────────┬────────┬────────┐
            KC_PGUP , KC_BSPC, KC_UP  , KC_DEL , KC_PGDN        ,OSL(_NEO_GREEK), KC_7  , KC_8   , KC_9   ,UC(L'ß'),
        // ├────────┼────────┼────────┼────────┼─────────┤      ├──────────────┼────────┼────────┼────────┼────────┤
            KC_HOME , KC_LEFT, KC_DOWN, KC_RGHT, KC_END         , KC_0         , KC_4   , KC_5   , KC_6   , KC_Y   ,
        // ├────────┼────────┼────────┼────────┼─────────┤      ├──────────────┼────────┼────────┼────────┼────────┤
             KC_ESC , KC_TAB , KC_INS , KC_ENT , KC_UNDO        ,OSL(_NEO_MATH), KC_1   , KC_2   , KC_3   , KC_LSFT,
        // └────────┴────────┴────────┼────────┼─────────┤      ├──────────────┼────────┼────────┴────────┴────────┘
        _______, _______, _______, MO(_MOUSE_MEDIA)
        //                            └────────┴─────────┘      └──────────────┴────────┘
        ),
    [_NEO_GREEK] = LAYOUT(
        // ┌────────┬────────┬────────┬────────┬─────────┐       ┌────────┬────────┬────────┬────────┬────────┐
            UC(L'ξ'),UC(L'ς'),UC(L'λ'),UC(L'χ'),UC(L'ω')         ,UC(L'κ'),UC(L'ψ'),UC(L'γ'),UC(L'φ'),UC(L'ϕ'),
        // ├────────┼────────┼────────┼────────┼─────────┤       ├────────┼────────┼────────┼────────┼────────┤
            UC(L'υ'),UC(L'ι'),UC(L'α'),UC(L'ε'),UC(L'ο')         ,UC(L'σ'),UC(L'ν'),UC(L'ρ'),UC(L'τ'),UC(L'δ'),
        // ├────────┼────────┼────────┼────────┼─────────┤       ├────────┼────────┼────────┼────────┼────────┤
            XXXXXXX ,UC(L'ϵ'),UC(L'η'),UC(L'π'),UC(L'β')         ,UC(L'μ'),UC(L'ζ'),UC(L'ϱ'),UC(L'ϑ'),UC(L'θ'),
        // └────────┴────────┴────────┼────────┼─────────┤       ├────────┼────────┼────────┴────────┴────────┘
        _______, _______, _______, _______
        //                          └────────┴─────────┘       └────────┴────────┘
        ),
    [_NEO_MATH] = LAYOUT(
        // ┌────────┬────────┬────────┬────────┬────────┐       ┌────────┬────────┬────────┬────────┬────────┐
            UC(L'∘'),UC(L'√'),UC(L'Λ'),UC(L'ℂ'),UC(L'Ω')        ,UC(L'×'),UC(L'Ψ'),UC(L'Γ'),UC(L'Φ'),UC(L'ℚ'),
        // ├────────┼────────┼────────┼────────┼────────┤       ├────────┼────────┼────────┼────────┼────────┤
            UC(L'⊂'),UC(L'∫'),UC(L'∀'),UC(L'∃'),UC(L'∈')        ,UC(L'Σ'),UC(L'ℕ'),UC(L'ℝ'),UC(L'∂'),UC(L'Δ'),
        // └────────┴────────┴────────┼────────┼────────┤       ├────────┼────────┼────────┴────────┴────────┘
            UC(L'∪'),UC(L'∩'),UC(L'∇'),UC(L'Π'),UC(L'ℤ')        ,UC(L'⇐'),UC(L'⇔'),UC(L'⇒'),UC(L'↦'),UC(L'Θ'),
        // └────────┴────────┴────────┼────────┼────────┤       ├────────┼────────┼────────┴────────┴────────┘
        _______, _______, _______, _______
        //                           └────────┴────────┘       └────────┴────────┘
        ),
    [_MOUSE_MEDIA] = LAYOUT(
        // ┌───────┬────────┬────────┬────────┬────────┐       ┌───────┬────────┬────────┬────────┬────────┐
            MS_WHLL, MS_WHLU, MS_UP  , MS_WHLD, MS_WHLR,        KC_LSFT, KC_MPRV, KC_MPLY, KC_MNXT, KC_LSFT,
        // ├───────┼────────┼────────┼────────┼────────┤       ├───────┼────────┼────────┼────────┼────────┤
            MS_BTN2, MS_LEFT, MS_DOWN, MS_RGHT, MS_BTN1,        KC_MUTE, KC_VOLD, KC_VOLU, KC_MSTP, KC_MFFD,
        // ├───────┼────────┼────────┼────────┼────────┤       ├───────┼────────┼────────┼────────┼────────┤
            _______, _______, _______, _______, _______,        _______, KC_BRID, KC_BRIU, KC_EJCT, QK_BOOTLOADER,
        // └───────┴────────┴────────┼────────┼────────┤       ├───────┼────────┼────────┴────────┴────────┘
        _______, _______, _______, _______
        //                           └────────┴────────┘       └────────┴───────┘
        ),
        [_FUNCTION_KEYS] = LAYOUT(
        // ┌─────────┬─────────┬─────────┬─────────┬─────────┐       ┌─────────┬─────────┬─────────┬─────────┬─────────┐
            _______ , _______ , _______ , _______,  _______,          _______  , KC_F9   , KC_F8   , KC_F7   , _______ ,
        // ├─────────┼─────────┼─────────┼─────────┼─────────┤       ├─────────┼─────────┼─────────┼─────────┼─────────┤
            _______ , _______ , _______ , KC_PSCR,  _______,          KC_F10   , KC_F4   , KC_F5   , KC_F6   , _______ ,
        // ├─────────┼─────────┼─────────┼─────────┼─────────┤       ├─────────┼─────────┼─────────┼─────────┼─────────┤
            _______ , _______ , _______ , _______,  _______,          KC_F11   , KC_F1   , KC_F2   , KC_F3   , _______ ,
        // └─────────┴─────────┴─────────┼─────────┼─────────┤       ├─────────┼─────────┴─────────┴─────────┴─────────┘
            _______, _______, _______, _______
        //                               └─────────┴─────────┘       └─────────┴─────────┘
),
};
