
enum my_keycodes {
  RB_RCMD1 = SAFE_RANGE,
  RB_RCMD2,
  RB_LCMD1,
  RB_LCMD2,
};

bool process_detected_host_os_kb(os_variant_t detected_os) {
    if (!process_detected_host_os_user(detected_os)) {
        return false;
    }
    switch (detected_os) {
        case OS_WINDOWS:
            // nein
        case OS_MACOS:
        case OS_IOS:
            rgb_matrix_sethsv(0, 255, 0);
            #ifndef RB_RCMD1
                #define RB_RCMD1 KC_RGUI
            #endif
            #ifndef RB_RCMD2
                #define RB_RCMD2 KC_RALT
            #endif
            #ifndef RB_LCMD1
                #define RB_LCMD1 KC_LGUI
            #endif
            #ifndef RB_LCMD2
                #define RB_LCMD2 KC_LALT
            #endif
            break;
        case OS_LINUX:
            rgb_matrix_sethsv(0, 0, 255);
            #ifndef RB_RCMD1
                #define RB_RCMD1 KC_RALT
            #endif
            #ifndef RB_RCMD2
                #define RB_RCMD2 KC_RGUI
            #endif
            #ifndef RB_LCMD1
                #define RB_LCMD1 KC_LALT
            #endif
            #ifndef RB_LCMD2
                #define RB_LCMD2 KC_LGUI
            #endif
            break;
        case OS_UNSURE:
            rgb_matrix_sethsv(255, 0, 0);
            break;
    }

    return true;
}

