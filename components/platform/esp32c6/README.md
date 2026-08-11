# ESP32-C6 Platform Adapter

This directory holds ESP32-C6-specific implementations of UHAL interfaces. The ESP-IDF peripheral catalog is under `esp_idf/`.

Driver filenames intentionally stay stable, such as `I2c.hpp` and `I2c.cpp`. A version suffix on every file would duplicate the same driver on each ESP-IDF upgrade and make application dependencies unstable. Instead, `esp_idf/compatibility/esp_idf_v6.md` records the supported ESP-IDF major version and migration rules. Add a new version manifest or an adapter only when an ESP-IDF API actually changes.

Only product composition roots may select this platform target. Device and protocol components must never include ESP-IDF headers or link ESP32-C6-specific targets.


