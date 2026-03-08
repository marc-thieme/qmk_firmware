#pragma once
#include "quantum.h"

// ==================== Config ====================

// If you want a custom timeout, define UML_TAPPING_TERM_MS before including this header.
// Otherwise we use QMK's TAPPING_TERM when available, else fall back to 200 ms.
#ifndef UML_TAPPING_TERM_MS
#    ifdef TAPPING_TERM
#        define UML_TAPPING_TERM_MS TAPPING_TERM
#    else
#        define UML_TAPPING_TERM_MS 200
#    endif
#endif

// ==================== Internal state ====================

typedef struct {
    uint16_t vowel;       // KC_A / KC_O / KC_U or KC_NO
    uint16_t started_ms;  // timer_read() at vowel press
    bool     upper;       // uppercase intent captured at vowel press
} uml_state_t;

static uml_state_t uml = {KC_NO, 0, false};

// ==================== Helpers ====================

static inline void uml_reset(void) {
    uml.vowel = KC_NO;
    uml.upper = false;
}

static inline bool uml_is_active(void) {
    return uml.vowel != KC_NO;
}

static inline bool uml_expired(void) {
    return uml_is_active() && (timer_elapsed(uml.started_ms) > UML_TAPPING_TERM_MS);
}

static inline bool uml_is_target_vowel(uint16_t kc) {
    return kc == KC_A || kc == KC_O || kc == KC_U;
}

static inline void uml_send_umlaut(uint16_t vowel, bool upper) {
    uint16_t cp = 0; // unicode code point
    switch (vowel) {
        case KC_O: cp = upper ? 0x00D6 : 0x00F6; break; // Ö / ö
        case KC_A: cp = upper ? 0x00C4 : 0x00E4; break; // Ä / ä
        case KC_U: cp = upper ? 0x00DC : 0x00FC; break; // Ü / ü
    }
    if (cp) tap_code16(UC(cp));
}

// ==================== Public API ====================
//
// Call this at the TOP of process_record_user().
// If it returns false, it consumed the event (do not process further).

static inline bool umlaut_adaptive_process(uint16_t keycode, keyrecord_t *record) {
    if (uml_expired()) {
        uml_reset();
    }

    if (!record->event.pressed) {
        return true; // only act on key presses
    }

    // Step 1: capture vowel (A/O/U)
    if (uml_is_target_vowel(keycode)) {
        uml.vowel      = keycode;
        uml.started_ms = timer_read();

        // Capture uppercase intent at the time of the vowel press
        uint8_t mods = get_mods() | get_oneshot_mods();
        bool caps    = host_keyboard_led_state().caps_lock;
        uml.upper    = ((mods & MOD_MASK_SHIFT) || caps);
        return true; // let the vowel through for now
    }

    // Step 2: on 'E', if a recent vowel exists and not expired → replace with umlaut
    if (keycode == KC_E && uml_is_active() && !uml_expired()) {
        tap_code(KC_BSPC);                  // remove the original vowel
        uml_send_umlaut(uml.vowel, uml.upper); // insert umlaut
        uml_reset();
        return false; // consume 'E'
    }

    // Any other key cancels the pending vowel
    uml_reset();
    return true;
}

// Optional idle cleaner: call from housekeeping_task_user() or a periodic task if desired.
static inline void umlaut_adaptive_task(void) {
    if (uml_expired()) {
        uml_reset();
    }
}
