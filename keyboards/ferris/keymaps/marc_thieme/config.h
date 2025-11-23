#pragma once

#define MASTER_LEFT
// #define MASTER_RIGHT // Uncomment if you plug USB into the right side

// Tune these to your typing speed
#define TAPPING_TERM 200
#define COMBO_TERM 40       // Window to press two keys for a combo
#define COMBO_COUNT 4       // We have 4 specific thumb combos

// Helper to make oneshot modifiers stick for one keypress
#define ONESHOT_TAP_TOGGLE 0
#define ONESHOT_TIMEOUT 2000

// // Set the mouse settings to a comfortable speed/accuracy trade-off,
// // assuming a screen refresh rate of 60 Htz or higher
// // The default is 50. This makes the mouse ~3 times faster and more accurate
// #define MOUSEKEY_INTERVAL 16
// // The default is 20. Since we made the mouse about 3 times faster with the previous setting,
// // give it more time to accelerate to max speed to retain precise control over short distances.
// #define MOUSEKEY_TIME_TO_MAX 40
// // The default is 300. Let's try and make this as low as possible while keeping the cursor responsive
// #define MOUSEKEY_DELAY 100
// // It makes sense to use the same delay for the mouseweel
// #define MOUSEKEY_WHEEL_DELAY 100
// // The default is 100
// #define MOUSEKEY_WHEEL_INTERVAL 50
// // The default is 40
// #define MOUSEKEY_WHEEL_TIME_TO_MAX 100

// // Pick good defaults for enabling homerow modifiers
// #define TAPPING_TERM 200
// #define PERMISSIVE_HOLD
// #define QUICK_TAP_TERM 0

// // Underglow configuration
// #ifdef RGBLIGHT_ENABLE
// #    define RGBLIGHT_EFFECT_BREATHING
// #    define RGBLIGHT_EFFECT_RAINBOW_MOOD
// #    define RGBLIGHT_EFFECT_RAINBOW_SWIRL
// #    define RGBLIGHT_EFFECT_SNAKE
// #    define RGBLIGHT_EFFECT_KNIGHT
// #    define RGBLIGHT_EFFECT_CHRISTMAS
// #    define RGBLIGHT_EFFECT_STATIC_GRADIENT
// #    define RGBLIGHT_EFFECT_RGB_TEST
// #    define RGBLIGHT_EFFECT_ALTERNATING
// #    define RGBLIGHT_EFFECT_TWINKLE
// #    define RGBLIGHT_HUE_STEP 8
// #    define RGBLIGHT_SAT_STEP 8
// #    define RGBLIGHT_VAL_STEP 8
// #endif
