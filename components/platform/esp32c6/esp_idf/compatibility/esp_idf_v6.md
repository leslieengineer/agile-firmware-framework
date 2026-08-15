# ESP-IDF v6 Compatibility Manifest

- Baseline: ESP-IDF 6.0.2
- Target: ESP32-C6
- Required components: `esp_driver_gpio`, `esp_timer`, `freertos`

GPIO uses `gpio_config`, the per-pin ISR service, and interrupt enable/disable APIs. Clock uses `esp_timer_get_time` and FreeRTOS task delay. ESP-IDF types remain private to Layer 1 sources.

NVS key-value storage is intentionally not represented by offset-based `uhal::IStorage`; products own repository ports for their schemas.
