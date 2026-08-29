# QMK Userspace — Agent Notes

## Project Overview

This is a QMK userspace for the **splitkb Halcyon Lily58 rev2** split keyboard.
The keymap is located at `keyboards/splitkb/halcyon/lily58/keymaps/rblk/`.

### Flash Command

```sh
qmk flash -kb splitkb/halcyon/lily58/rev2 -km rblk -e HLC_NONE=1
```

### Key Files

| File | Purpose |
|------|---------|
| `keymaps/rblk/keymap.c` | Main keymap, layer definitions, RGB init |
| `keymaps/rblk/os.h` | OS detection callbacks (macOS → blue, iOS → green, Linux → orange) |
| `keymaps/rblk/config.h` | RGB Matrix defaults |
| `keymaps/rblk/rules.mk` | Feature flags (RGB_MATRIX, OS_DETECTION, KEY_OVERRIDE, etc.) |
| `users/halcyon_modules/splitkb/halcyon.c` | Keyboard-level init, LED config (`led_config_t`), split sync |
| `users/halcyon_modules/splitkb/os.h` | (not present — OS logic lives in keymap `os.h`) |

---

## Architecture Notes

### Split Keyboard
- **Master half** connects via USB. Only the master is flashed directly via USB reset.
- **Slave half** receives its config via UART (GP28/GP29) after the master boots.
- RGB Matrix sync between halves is handled by `halcyon.c` via `transaction_rpc_send(MODULE_SYNC, ...)`.

### RGB Matrix
- Driver: `ws2812` (RP2040 PIO)
- LED count: 68 total (34 per half)
- `led_config_t` is defined in `users/halcyon_modules/splitkb/halcyon.c` under `#if defined(KEYBOARD_splitkb_halcyon_lily58_rev2)`
- **LED flags in `led_config_t` (Lily58):**
  - LEDs 0–4: flag `0x02` = `LED_FLAG_UNDERGLOW`
  - LEDs 5–67: flag `0x04` = `LED_FLAG_KEYLIGHT`
- The RGB Matrix render loop (`rgb_matrix.c:317`) filters LEDs by `rgb_effect_params.flags`, which is synced from `rgb_matrix_config.flags` every tick.

### EEPROM (Flash wear-leveling)
- The RP2040 has **no hardware EEPROM**. QMK emulates it via wear-leveling on flash.
- `rgb_config_t` is stored as a packed `uint64_t` containing: `enable`, `mode`, `hsv`, `speed`, `flags`.
- **EEPROM survives reflashing** — the flash region used for EEPROM is outside the firmware area.
- `rgb_matrix_init()` reads EEPROM into `rgb_matrix_config` during `keyboard_init()`, which runs **before** `keyboard_post_init_user()`.
- `RGB_MATRIX_DEFAULT_FLAGS` in `config.h` is only applied when `rgb_matrix_config.mode == 0` (blank EEPROM). If EEPROM already has a valid config, defaults are ignored.

---

## Problem: Only 5 LEDs Lit (Underglow Only)

### Symptom
After plugging into macOS, only 5 LEDs (the underglow strip) lit up blue. All per-key LEDs were off.

### Root Cause
The stored EEPROM value for `rgb_matrix_config.flags` was `0x04` (`LED_FLAG_KEYLIGHT`).
Since `LED_FLAG_UNDERGLOW = 0x02` does NOT match `0x04`, the underglow LEDs were excluded —
but the per-key LEDs (flag `0x04`) matched and *should* have lit. Wait — actually it was the
**inverse**: the stored flags only matched `LED_FLAG_KEYLIGHT`, and the 5 underglow LEDs
(flag `0x02`) were the only ones rendered because the mode was showing only underglow.

The actual stored flags were `LED_FLAG_UNDERGLOW` (`0x02`), so only underglow LEDs passed the
`HAS_ANY_FLAGS` filter in `RGB_MATRIX_TEST_LED_FLAGS()`.

### Why Code Changes Alone Didn't Fix It
- `RGB_MATRIX_DEFAULT_FLAGS LED_FLAG_ALL` in `config.h` → ignored because EEPROM already had a valid `mode` (non-zero).
- `rgb_matrix_config.flags = LED_FLAG_ALL` in `keyboard_post_init_user` → correct in theory, but `rgb_matrix_set_flags()` also calls `eeconfig_flag_rgb_matrix(true)` which flushes the struct back to EEPROM, and there may be re-reads on the slave side.
- `rgb_matrix_set_flags(LED_FLAG_ALL)` with EEPROM guard → correct approach but the EEPROM guard condition was never true on the flashed half (EEPROM value on the master already matched from a previous attempt).

### Fix That Worked
Added `EE_CLR` keycode to the `_FUNC` layer (FUNC + Backspace). Pressing it called `eeconfig_init()` which **wiped the entire EEPROM**, forcing QMK to write fresh defaults including `RGB_MATRIX_DEFAULT_FLAGS LED_FLAG_ALL` from `config.h`. After the reboot all 68 LEDs lit blue.

---

## Keymap Layers

| Layer | Index | Access | Purpose |
|-------|-------|--------|---------|
| `_BASE` | 0 | default | QWERTZ base layer (German layout) |
| `_MOVE` | 1 | hold F | Navigation (arrows, page up/down, Ctrl+A/E) |
| `_SYMBOL` | 2 | hold Space | Symbols (brackets, pipes, tildes, etc.) |
| `_FUNC` | 3 | hold right inner thumb | F-keys, brightness, volume, EE_CLR, QK_BOOT |
| `_NUMPAD` | 4 | hold left inner thumb | Numpad |
| `_UMLAUTS` | 5 | hold bottom-left key | German umlauts (ä, ö, ü, ß) |

---

## OS Detection Colour Scheme

| OS | Colour |
|----|--------|
| macOS | 🔵 Blue |
| iOS | 🟢 Green |
| Linux | 🟠 Orange |
| Windows | (none) |
| Unknown | 🔴 Red |

OS detection is in `keymaps/rblk/os.h` via `process_detected_host_os_kb()`.
Each OS case also sets `rgb_matrix_set_flags(LED_FLAG_ALL)` to ensure all LEDs respond.

---

## Special Keys (FUNC Layer)

| Key combo | Keycode | Action |
|-----------|---------|--------|
| FUNC + Backspace | `EE_CLR` | Wipe EEPROM, reboot with defaults |
| FUNC + `\|` key | `QK_BOOT` | Enter RP2040 bootloader (for flashing) |

---

## macOS Key Swaps

On macOS and iOS, the following swaps are applied via `keymap_config`:
- `swap_ralt_rgui = true`
- `swap_lalt_lgui = true`

Symbol key overrides (via `KEY_OVERRIDE_ENABLE`) remap German special characters to their
macOS equivalents (e.g. `DE_AT` → `A(DE_L)` for `@` on macOS).

---

## Boot Animation

On USB plug-in, a rainbow sweep runs on the **underglow LEDs** (indices 0–4) for ~5 seconds, then turns off. The **keylight LEDs** stay solid blue throughout via `rgb_matrix_indicators_advanced_user`.

### Implementation (`keymap.c`)

| Symbol | Value | Purpose |
|--------|-------|---------|
| `UNDERGLOW_LED_COUNT` | 5 | Number of underglow LEDs on the Lily58 |
| `BOOT_ANIM_STEP_MS` | 20ms | Delay per hue step → full 256-step sweep ≈ 5s |
| `boot_hue` | 0–255 | Current hue, incremented each callback tick |
| `boot_anim_done` | bool | Guards `rgb_matrix_indicators_advanced_user` |
| `boot_anim_token` | deferred_token | Handle returned by `defer_exec` |

- `boot_anim_callback()` — sets underglow LEDs to `boot_hue`, increments hue, returns `0` to cancel when hue wraps back to 0
- `rgb_matrix_indicators_advanced_user()` — re-paints all keylight LEDs blue each render frame while `!boot_anim_done`
- Animation started in `keyboard_post_init_user()` via `defer_exec(BOOT_ANIM_STEP_MS, boot_anim_callback, NULL)`

---

## Tap Dance — Umlaut Hold Keys

`TAP_DANCE_ENABLE = yes` in `rules.mk`.

| Key | Tap | Hold |
|-----|-----|------|
| `A` | `a` | `ä` (`DE_ADIA`) |
| `O` | `o` | `ö` (`DE_ODIA`) |
| `U` | `u` | `ü` (`DE_UDIA`) |
| `S` | `s` | `ß` (`DE_SS` = `KC_MINS`) |

### Implementation notes

- Uses `ACTION_TAP_DANCE_FN_ADVANCED(NULL, td_x_finished, td_x_reset)` — one pair per key
- **Hold detection:** `state->count == 1 && state->pressed` in the `finished` callback
- **Single umlaut on hold:** `tap_code16()` sends one press+release — no OS-level key repeat
- **Multiple taps:** replays the base key N times via a loop
- **`ß` special case:** `DE_SS` = `KC_MINS` — macOS German input source maps this to `ß` natively. No `Option+S` or OS branching needed. `tap_code16` bypasses key overrides but the German input source handles it correctly.
- **OS tracking:** `current_os` variable updated via `process_detected_host_os_user()` hook — available for future OS-aware logic. `tap_code16` bypasses the key override pipeline so overrides defined in `os.h` do NOT apply to tap dance outputs.
- **`TAPPING_TERM`** (default 200ms) controls hold threshold — tune in `config.h` if needed