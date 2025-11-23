/*
Mod Keys:
hold WIN
hold ALT
hold MOD4
hold SHIFT
hold MOD3
hold MOD7
press CTRL
press MOD5
press MOD6

Required Combinations:
MOD4 + WIN
MOD4 + ALT
MOD4 + SHIFT
MOD3 + ALT

Additional Layers
MOD7: Mouse on the left half
MOD7: Media keys on the right half
MOD8: Function Keys
All other layers taken from the neo2 keyboard layout

Keycode meanings:
press R1 → Space
press L1 → Space

R1 → MOD4
L1 → ALT
L2 → WIN
R2 → MOD3

L1 + R2 → SHIFT
R1 + R2 → MOD7
L1 + L2 → MOD7

press R2 → oneshot CTRL
press L2 → oneshot SHIFT
press L1 + L2 → oneshot MOD8
press R1 + R2 → oneshot MOD8
press L1 + R2 → oneshot MOD5
press L2 + R1 → oneshot MOD6
*/

#include QMK_KEYBOARD_H

// --- Layer Definitions ---
enum layers {
    _BASE = 0,
    _L3,  // Neo Mod 3 (Brackets/Symbols)
    _L4,  // Neo Mod 4 (Nav/Numpad)
    _L5,  // Neo Mod 5 (Greek - Placeholder)
    _L6,  // Neo Mod 6 (Math - Placeholder)
    _L7,  // Mouse (Left) / Media (Right)
    _L8,  // F-Keys
};

// --- Custom dummy keys for logic interception ---
// We use F20-F24 as placeholders. They are rarely used in real life.
#define T_R2_CTL KC_F20  // Tap: OS Ctrl, Hold: Layer 3
#define T_L2_SFT KC_F21  // Tap: OS Shift, Hold: Win
#define T_C_MOD5 KC_F22  // Tap: OS Layer 5, Hold: Shift (Combo L1+R2)
#define T_C_MOD8 KC_F23  // Tap: OS Layer 8, Hold: Layer 7 (Combo R1+R2, L1+L2)

// --- Key Definitions to keep the map clean ---
// R1: Tap Space, Hold Layer 4
#define K_R1 LT(_L4, KC_SPC)
// L1: Tap Space, Hold Alt
#define K_L1 MT(MOD_LALT, KC_SPC)
// R2: Logic handled in process_record (LT logic used for hold)
#define K_R2 LT(_L3, T_R2_CTL)
// L2: Logic handled in process_record (MT logic used for hold)
#define K_L2 MT(MOD_LGUI, T_L2_SFT)

// --- Combo Definitions ---
enum combo_events {
    CB_L1R2,
    CB_R1R2,
    CB_L1L2,
    CB_L2R1
};

const uint16_t PROGMEM combo_l1r2[] = {K_L1, K_R2, COMBO_END};
const uint16_t PROGMEM combo_r1r2[] = {K_R1, K_R2, COMBO_END};
const uint16_t PROGMEM combo_l1l2[] = {K_L1, K_L2, COMBO_END};
const uint16_t PROGMEM combo_l2r1[] = {K_L2, K_R1, COMBO_END};

combo_t key_combos[COMBO_COUNT] = {
    // L1 + R2 -> Shift (Hold) / OS Mod 5 (Tap)
    [CB_L1R2] = COMBO(combo_l1r2, MT(MOD_LSFT, T_C_MOD5)),

    // R1 + R2 -> Layer 7 (Hold) / OS Mod 8 (Tap)
    [CB_R1R2] = COMBO(combo_r1r2, LT(_L7, T_C_MOD8)),

    // L1 + L2 -> Layer 7 (Hold) / OS Mod 8 (Tap)
    [CB_L1L2] = COMBO(combo_l1l2, LT(_L7, T_C_MOD8)),

    // L2 + R1 -> OS Mod 6 (Tap Only defined)
    [CB_L2R1] = COMBO_ACTION(combo_l2r1)
};

// --- The Keymap ---
// Layout assumes 34 keys (Ferris Sweep standard)
// Using Neo2 Arrangement on QWERTY base
const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT(
        KC_X,    KC_V,    KC_L,    KC_C,    KC_W,       KC_K,    KC_H,    KC_G,    KC_F,    KC_Q,
        KC_U,    KC_I,    KC_A,    KC_E,    KC_O,       KC_S,    KC_N,    KC_R,    KC_T,    KC_D,
        KC_LBRC, KC_SCLN, KC_QUOT, KC_P,    KC_Y,       KC_B,    KC_M,    KC_COMM, KC_DOT,  KC_J,
                          K_L2,    K_L1,                K_R1,    K_R2
    ),

    [_L3] = LAYOUT( // Neo2 Layer 3 (Brackets & Symbols)
        KC_DOT,  KC_MINS, KC_GRV,  KC_LBRC, KC_BSLS,    KC_EXLM, KC_LT,   KC_GT,   KC_EQL,  KC_AMPR,
        KC_QUES, KC_LPRN, KC_LCBR, KC_EQL,  KC_LBRC,    KC_RBRC, KC_RCBR, KC_RPRN, KC_SLSH, KC_AT,
        KC_COMM, KC_UNDS, KC_RBRC, KC_DLR,  KC_CIRC,    KC_HASH, KC_PERC, KC_PIPE, KC_TILD, KC_GRV,
                          KC_TRNS, KC_TRNS,             KC_TRNS, KC_TRNS
    ),

    [_L4] = LAYOUT( // Neo2 Layer 4 (Nav & Numpad)
        KC_PGUP, KC_BSPC, KC_UP,   KC_DEL,  KC_PGDN,    KC_PPLS, KC_7,    KC_8,    KC_9,    KC_PAST,
        KC_HOME, KC_LEFT, KC_DOWN, KC_RIGHT,KC_END,     KC_PMNS, KC_4,    KC_5,    KC_6,    KC_COMM,
        KC_ESC,  KC_TAB,  KC_INS,  KC_ENT,  KC_NO,      KC_0,    KC_1,    KC_2,    KC_3,    KC_DOT,
                          KC_TRNS, KC_TRNS,             KC_TRNS, KC_TRNS
    ),

    [_L5] = LAYOUT( // Placeholder for Greek
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                          KC_TRNS, KC_TRNS,             KC_TRNS, KC_TRNS
    ),

    [_L6] = LAYOUT( // Placeholder for Math
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
        KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
                          KC_TRNS, KC_TRNS,             KC_TRNS, KC_TRNS
    ),

    // [_L7] = LAYOUT( // Left: Mouse, Right: Media
    //     KC_NO,   KC_BTN1, KC_MS_U, KC_BTN2, KC_NO,      KC_NO,   KC_MPRV, KC_MPLY, KC_MNXT, KC_NO,
    //     KC_NO,   KC_MS_L, KC_MS_D, KC_MS_R, KC_NO,      KC_NO,   KC_VOLD, KC_MUTE, KC_VOLU, KC_NO,
    //     KC_NO,   KC_NO,   KC_WH_U, KC_WH_D, KC_NO,      KC_NO,   KC_NO,   KC_NO,   KC_NO,   KC_NO,
    //                       KC_TRNS, KC_TRNS,             KC_TRNS, KC_TRNS
    // ),

    [_L8] = LAYOUT( // F-Keys
        KC_F12,  KC_F7,   KC_F8,   KC_F9,   KC_NO,      KC_NO,   KC_F7,   KC_F8,   KC_F9,   KC_F12,
        KC_F11,  KC_F4,   KC_F5,   KC_F6,   KC_NO,      KC_NO,   KC_F4,   KC_F5,   KC_F6,   KC_F11,
        KC_F10,  KC_F1,   KC_F2,   KC_F3,   KC_NO,      KC_NO,   KC_F1,   KC_F2,   KC_F3,   KC_F10,
                          KC_TRNS, KC_TRNS,             KC_TRNS, KC_TRNS
    ),
};

// --- Logic Injection ---

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        switch (keycode) {
            // Case 1: R2 (Hold L3, Tap OS-Ctrl)
            // We defined K_R2 as LT(_L3, T_R2_CTL)
            case T_R2_CTL:
                // If we are here, LT() decided it was a TAP, not a HOLD.
                // Instead of sending F20, we set One Shot Ctrl.
                add_oneshot_mods(MOD_BIT(KC_LCTL));
                return false; // Block the original F20 keycode

            // Case 2: L2 (Hold Win, Tap OS-Shift)
            // Defined as MT(MOD_LGUI, T_L2_SFT)
            case T_L2_SFT:
                // MT() decided it was a TAP.
                add_oneshot_mods(MOD_BIT(KC_LSFT));
                return false;

            // Case 3: Combo L1+R2 (Hold Shift, Tap OS-Mod5)
            case T_C_MOD5:
                set_oneshot_layer(_L5, ONESHOT_START);
                return false;

            // Case 4: Combo R1+R2 or L1+L2 (Hold L7, Tap OS-Mod8)
            case T_C_MOD8:
                set_oneshot_layer(_L8, ONESHOT_START);
                return false;
        }
    }
    return true;
}

// Handling the Combo Action for L2 + R1 (OS Mod 6)
void process_combo_event(uint16_t combo_index, bool pressed) {
    switch(combo_index) {
        case CB_L2R1:
            if (pressed) {
                set_oneshot_layer(_L6, ONESHOT_START);
            }
            break;
    }
}
