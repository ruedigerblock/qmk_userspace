key_override_t at_key_override;
key_override_t bsls_key_override;
key_override_t euro_key_override;
key_override_t lcrbr_key_override;
key_override_t lsqbr_key_override;
key_override_t pipe_key_override;
key_override_t rcrbr_key_override;
key_override_t rsqbr_key_override;
key_override_t tilde_key_override;

#define REPLACEMENT_LAYER (1 << 4)

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
            keymap_config.swap_ralt_rgui = true;
            keymap_config.swap_lalt_lgui = true;

            at_key_override    = ko_make_with_layers(0, DE_AT  , A(DE_L)   , REPLACEMENT_LAYER);
            bsls_key_override  = ko_make_with_layers(0, DE_BSLS, S(A(DE_M)), REPLACEMENT_LAYER);
            euro_key_override  = ko_make_with_layers(0, DE_EURO, A(DE_E)   , REPLACEMENT_LAYER);
            lcrbr_key_override = ko_make_with_layers(0, DE_LCBR, A(DE_8)   , REPLACEMENT_LAYER);
            lsqbr_key_override = ko_make_with_layers(0, DE_LBRC, A(DE_5)   , REPLACEMENT_LAYER);
            pipe_key_override  = ko_make_with_layers(0, DE_PIPE, A(DE_7)   , REPLACEMENT_LAYER);
            rcrbr_key_override = ko_make_with_layers(0, DE_RCBR, A(DE_9)   , REPLACEMENT_LAYER);
            rsqbr_key_override = ko_make_with_layers(0, DE_RBRC, A(DE_6)   , REPLACEMENT_LAYER);
            tilde_key_override = ko_make_with_layers(0, DE_TILD, A(DE_N)   , REPLACEMENT_LAYER);

            break;
        case OS_IOS:
            rgb_matrix_sethsv(HSV_GREEN);
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

const key_override_t *key_overrides[] = {
    &at_key_override,
    &bsls_key_override,
    &euro_key_override,
    &lcrbr_key_override,
    &lsqbr_key_override,
    &pipe_key_override,
    &rcrbr_key_override,
    &rsqbr_key_override,
    &tilde_key_override,
};
