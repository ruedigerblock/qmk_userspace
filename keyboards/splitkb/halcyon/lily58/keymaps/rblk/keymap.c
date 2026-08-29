#include QMK_KEYBOARD_H
#if __has_include("keymap.h")
#    include "keymap.h"
#endif

// ../../../../../../../qmk/quantum/keymap_extras/keymap_german.h

#include "keymap_german.h"
#include "os.h"
#include "config.h"

// ── RGB: underglow = OS color, keylight = off ─────────────────────────────────
#define UNDERGLOW_LED_COUNT 5

void keyboard_post_init_user(void) {
    rgb_matrix_set_flags(LED_FLAG_UNDERGLOW);
    rgb_matrix_mode_noeeprom(RGB_MATRIX_SOLID_COLOR);
    rgb_matrix_sethsv_noeeprom(HSV_BLUE);
}

static rgb_t os_color_rgb(void) {
    hsv_t hsv;
    switch (current_os) {
        case OS_MACOS:
            hsv = (hsv_t){HSV_BLUE};
            break;
        case OS_IOS:
            hsv = (hsv_t){HSV_GREEN};
            break;
        case OS_LINUX:
            hsv = (hsv_t){HSV_ORANGE};
            break;
        case OS_UNSURE:
        default:
            hsv = (hsv_t){HSV_RED};
            break;
    }
    return hsv_to_rgb(hsv);
}

bool rgb_matrix_indicators_advanced_user(uint8_t led_min, uint8_t led_max) {
    // Underglow always shows the OS color — master only.
    // Keylight LEDs are always off.
    if (is_keyboard_master()) {
        rgb_t underglow_rgb = os_color_rgb();
        for (uint8_t i = led_min; i < led_max; i++) {
            if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_UNDERGLOW)) {
                rgb_matrix_set_color(i, underglow_rgb.r, underglow_rgb.g, underglow_rgb.b);
            }
        }
    }
    // Turn off all keylight LEDs explicitly
    for (uint8_t i = led_min; i < led_max; i++) {
        if (HAS_FLAGS(g_led_config.flags[i], LED_FLAG_KEYLIGHT)) {
            rgb_matrix_set_color(i, 0, 0, 0);
        }
    }
    return true;
}

enum layer_names {
    _BASE,
    _MOVE,
    _SYMBOL,
    _FUNC,
    _NUMPAD,
    _UMLAUTS,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    [_BASE] = LAYOUT_lily58_hlc(KC_ESC, KC_1, KC_2, KC_3, KC_4, KC_5, KC_6, KC_7, KC_8, KC_9, KC_0, KC_BSPC, KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, DE_PLUS, KC_LSFT, KC_A, KC_S, KC_D, LT(_MOVE, KC_F), KC_G, KC_H, KC_J, KC_K, KC_L, DE_HASH, KC_RSFT, MO(_UMLAUTS), KC_Z, KC_X, KC_C, KC_V, KC_B, MO(_NUMPAD), MO(_FUNC), KC_N, KC_M, KC_COMM, KC_DOT, DE_MINS, KC_ENT, KC_LCTL, KC_LGUI, KC_LALT, LT(_SYMBOL, KC_SPC), LT(_SYMBOL, KC_SPC), KC_RALT, KC_RGUI, KC_RCTL, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),

    [_MOVE] = LAYOUT_lily58_hlc(_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, A(G(KC_BSPC)), _______, _______, _______, _______, _______, _______, DE_CIRC, KC_PGDN, KC_PGUP, S(4), _______, _______, _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_LEFT, KC_DOWN, KC_UP, KC_RIGHT, _______, _______, _______, _______, _______, LCA_T(KC_SPC), KC_ENT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_SYMBOL] = LAYOUT_lily58_hlc(KC_GRAVE, _______, _______, _______, _______, DE_LABK, S(DE_LABK), DE_BSLS, _______, _______, DE_BSLS, DE_ACUT, _______, DE_AT, _______, DE_EURO, _______, DE_LBRC, DE_RBRC, DE_UDIA, _______, DE_ODIA, _______, DE_TILD, _______, DE_ADIA, DE_SS, _______, _______, S(DE_8), S(DE_9), _______, _______, _______, _______, _______, _______, DE_PIPE, _______, _______, _______, DE_LCBR, _______, _______, DE_RCBR, _______, _______, _______, _______, _______, _______, _______, _______, KC_SPC, KC_ENT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_FUNC] = LAYOUT_lily58_hlc(KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_F6, _______, _______, _______, _______, _______, EE_CLR, KC_F7, KC_F8, KC_F9, KC_F10, KC_F11, KC_F12, _______, _______, _______, _______, _______, QK_BOOT, _______, _______, _______, _______, KC_BRIU, KC_VOLU, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_BRID, KC_VOLD, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_NUMPAD] = LAYOUT_lily58_hlc(_______, _______, _______, _______, _______, _______, KC_PEQL, KC_PSLS, KC_PAST, KC_PMNS, _______, _______, _______, _______, _______, _______, _______, _______, KC_7, KC_8, KC_9, KC_PPLS, _______, _______, _______, _______, _______, _______, _______, _______, KC_4, KC_5, KC_6, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_1, KC_2, KC_3, _______, _______, _______, _______, _______, _______, KC_LSFT, _______, KC_0, KC_COMM, KC_DOT, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [_UMLAUTS] = LAYOUT_lily58_hlc(_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, DE_ACUT, KC_DEL, _______, _______, _______, _______, C(KC_R), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, C(KC_D), _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

    [12] = LAYOUT_lily58_hlc(_______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______),

};

#ifdef OTHER_KEYMAP_C
#    include OTHER_KEYMAP_C
#endif // OTHER_KEYMAP_C
