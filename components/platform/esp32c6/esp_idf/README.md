# ESP32-C6 ESP-IDF Platform

`low_level/` is architecture Layer 1 and owns ESP-IDF calls. GPIO input/output, per-pin interrupts, and the system millisecond clock are implemented for ESP-IDF 6.0.2. Other listed peripherals remain placeholders until a product vertical slice requires them.

`adapters/` is architecture Layer 3. `OutputPin`, `InputPin`, `PinInterrupt`, and `Clock` implement the corresponding UHAL contracts without exposing ESP-IDF types in public headers.

Board selection still owns GPIO routing, active polarity, power, flash/PSRAM configuration, and enabled features.

## Versioning Policy

- Stable driver filenames do not include an ESP-IDF version.
- `compatibility/esp_idf_v6.md` records the exact tested API baseline.
- Compatibility shims belong in `compatibility/`; do not fork every peripheral file.

## Rules

- ESP-IDF headers and `esp_err_t` conversion belong only in Layer 1 source files.
- Layer 3 adapters expose UHAL and delegate hardware access to Layer 1.
- Device, protocol, and product policy must not enter this platform package.
- Only capabilities required by a tested vertical slice are implemented.
