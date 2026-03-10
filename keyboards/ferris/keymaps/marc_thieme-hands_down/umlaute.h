#pragma once
#include "quantum.h"
#include "unicode.h"

static uint16_t UML_last_vowel = KC_NO;
static uint16_t UML_timer      = 0;
static bool     UML_upper      = false;

static inline bool uml_timed_out(void) {
    return timer_elapsed(UML_timer) > TAPPING_TERM;
}

static inline void uml_send_umlaut(uint16_t vowel, bool upper) {
    if (upper) {
        switch (vowel) {
            case KC_DOT: send_unicode_string("Ö"); return; // Ö
            case KC_A: send_unicode_string("Ä"); return; // Ä
            case KC_U: send_unicode_string("Ü"); return; // Ü
        }
    }
    switch (vowel) {
        case KC_DOT: send_unicode_string("ö"); return; // ö
        case KC_A: send_unicode_string("ä"); return; // ä
        case KC_U: send_unicode_string("ü"); return; // ü
    }
}

static inline void uml_reset(void) { UML_timer = 0; }

static inline bool umlaut_adaptive_process(uint16_t keycode, keyrecord_t *record) {
    if (!record->event.pressed) return true;

    if (keycode == KC_DOT || keycode == KC_A || keycode == KC_U) {
        UML_last_vowel = keycode;
        UML_timer      = timer_read();

        uint8_t mods = get_mods() | get_oneshot_mods();
        bool caps    = host_keyboard_led_state().caps_lock;
        UML_upper    = ((mods & MOD_MASK_SHIFT) || caps);
        return true; // let the vowel through for now
    }
    if (keycode == KC_E && !uml_timed_out()) {
        tap_code(KC_BSPC);             // remove the original vowel
        uml_send_umlaut(UML_last_vowel, UML_upper); // insert umlaut
        return false; // consume 'e'
    }
    uml_reset();

   return true;
}
