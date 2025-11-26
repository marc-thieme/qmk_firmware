#pragma once
#include QMK_KEYBOARD_H

static inline bool is_number_key(uint16_t kc) {
    switch (kc) {
        case KC_1: case KC_2: case KC_3: case KC_4: case KC_5:
        case KC_6: case KC_7: case KC_8: case KC_9: case KC_0:
            return true;
    }
    return false;
}

