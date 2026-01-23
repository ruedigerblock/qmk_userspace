#include QMK_KEYBOARD_H
#if __has_include("keymap.h")
#    include "keymap.h"
#endif

// ../../../../../../../qmk/quantum/keymap_extras/keymap_german.h

#include "os.h"
#include "config.h"
#include "keymap_german.h"


void keyboard_post_init_user(void) {
    rgb_matrix_sethsv(255, 255, 255);
}

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

    // DEFAULT

    [0] = LAYOUT_lily58_hlc(
    //      ESC         1           2           3           4           5                                       6           7           8           9           0           BSPC
            KC_ESC,     KC_1,       KC_2,       KC_3,       KC_4,       KC_5,                                   KC_6,       KC_7,       KC_8,       KC_9,       KC_0,       KC_BSPC,
    //      TAB         Q           W           E           R           T                                       Z           U           I           O           P           +
            KC_TAB,     KC_Q,       KC_W,       KC_E,       KC_R,       KC_T,                                   KC_Y,       KC_U,       KC_I,       KC_O,       KC_P,       DE_PLUS,
    //                  A           S           D           F           G                                       H           J           K           L           #           SFT
            KC_LSFT,    KC_A,       KC_S,       KC_D,       KC_F,       KC_G,                                   KC_H,       KC_J,       KC_K,       KC_L,       DE_HASH,    KC_RSFT,
    //      SFT         Y           X           C           V           B                                       N           M           ,           .           -           SPC
            KC_LSFT,    KC_Z,       KC_X,       KC_C,       KC_V,       KC_B,       XXXXXXX,        XXXXXXX,    KC_N,       KC_M,       KC_COMM,    KC_DOT,     DE_MINS,    KC_RSFT,
    //                                          LCTL        OPT         CMD / ALT   SPC | L(1)      SPC         ALTGR       CMD         RCTL
                                                KC_LCTL,    RB_LCMD2,   RB_LCMD1,   LT(1, KC_SPC),  KC_SPC,     RB_RCMD1,   RB_RCMD2,   KC_RCTL,

                                                KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO),

    [1] = LAYOUT_lily58_hlc(
    //      ^                                                                                                                                   ß           ´           DEL
            DE_GRV,     _______,    _______,    _______,    _______,    _______,                            _______,    _______,    _______,    DE_SS,      DE_ACUT,    KC_DELETE,
    //                                                                                                                  Ü                       Ö
            _______,    _______,    _______,    _______,    _______,    _______,                            _______,    DE_UDIA,    _______,    DE_ODIA,    _______,    _______,
    //                  Ä                                                                                   <-          v           ^           ->
            _______,    DE_ADIA,    _______,    _______,    _______,    _______,                            KC_LEFT,    KC_DOWN,    KC_UP,      KC_RIGHT,   _______,    _______,
    //                  <                                                                                   1           2           3           4           5
            _______,    DE_LABK,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,   _______,    _______,
    //                                                                                          ENT
                                                _______,    _______,    _______,    _______,    KC_ENTER,   _______,    _______,    _______,

                                                _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,     _______,    _______),

    [12] = LAYOUT_lily58_hlc(
            _______,    _______,    _______,    _______,    _______,    _______,                            _______,    _______,    _______,    _______,    _______,    _______,
            _______,    _______,    _______,    _______,    _______,    _______,                            _______,    _______,    _______,    _______,    _______,    _______,
            _______,    _______,    _______,    _______,    _______,    _______,                            _______,    _______,    _______,    _______,    _______,    _______,
            _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,
                                                _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______,

                                                _______,    _______,    _______,    _______,    _______,    _______,    _______,    _______),

};
#if defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)
const uint16_t PROGMEM encoder_mapkk[][NUM_ENCODERS][NUM_DIRECTIONS] = {
    [0] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGUP, KC_PGDN), ENCODER_CCW_CW(KC_PGUP, KC_PGDN)},
    [1] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGUP, KC_PGDN), ENCODER_CCW_CW(KC_PGUP, KC_PGDN)},
    [2] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGUP, KC_PGDN), ENCODER_CCW_CW(KC_PGUP, KC_PGDN)},
    [3] = {ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_VOLD, KC_VOLU), ENCODER_CCW_CW(KC_PGUP, KC_PGDN), ENCODER_CCW_CW(KC_PGUP, KC_PGDN)}
};
#endif // defined(ENCODER_ENABLE) && defined(ENCODER_MAP_ENABLE)



#ifdef OTHER_KEYMAP_C
#    include OTHER_KEYMAP_C
#endif // OTHER_KEYMAP_C

