# ESP32-C6 ESP-IDF Platform

`low_level/` is architecture layer 1. It contains ESP-IDF calls, GPIO routing, interrupts, DMA, and peripheral initialization. Every listed peripheral has a header and source placeholder, but none has an implementation or is included in a build target yet.

ESP32-C6 is a single chip line, so this catalog follows its ESP-IDF peripheral APIs rather than being a family superset. Board selection still controls GPIO routing, power, flash/PSRAM configuration, and enabled features.

## Versioning Policy

- Stable project filenames do not include an ESP-IDF version.
- `compatibility/esp_idf_v6.md` records the chosen major-version API baseline.
- Pin an exact ESP-IDF release in the product build or dependency lockfile.
- If an ESP-IDF upgrade breaks a driver API, add a migration note and isolate compatibility code in `compatibility/`; do not fork every peripheral file.

## Rules

- ESP-IDF headers and `esp_err_t` conversion belong only in `low_level/`.
- An adapter in `adapters/`, such as `Esp32C6I2c`, wraps a low-level module and exposes `uhal::II2c`.
- GDMA/async memcpy are shared capabilities. I2C/SPI/UART implementations may use them but must not duplicate a DMA driver.
- Wi-Fi, Bluetooth LE, and IEEE 802.15.4 are ESP-IDF subsystem placeholders, not UHAL bus interfaces.
