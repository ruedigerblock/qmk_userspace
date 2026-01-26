bool process_detected_host_os_kb(os_variant_t detected_os) {
    if (!process_detected_host_os_user(detected_os)) {
        return false;
    }

    switch (detected_os) {
        case OS_WINDOWS:
            // nein
            break;
        case OS_MACOS:
            rgb_matrix_sethsv(HSV_BLUE);
            layer_on(1);
            keymap_config.swap_ralt_rgui = true;
            keymap_config.swap_lalt_lgui = true;
            break;
        case OS_IOS:
            rgb_matrix_sethsv(HSV_BLUE);
            keymap_config.swap_ralt_rgui = true;
            keymap_config.swap_lalt_lgui = true;
            layer_on(1);
            break;
        case OS_LINUX:
            rgb_matrix_sethsv(HSV_ORANGE);
            break;
        case OS_UNSURE:
            rgb_matrix_sethsv(HSV_RED);
            break;
    }

    return true;
}
