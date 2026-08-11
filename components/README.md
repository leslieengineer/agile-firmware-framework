# Components

Each implemented subdirectory is a separately linkable CMake target with one responsibility. A component exposes only its public `include/` API and keeps implementation in `src/`.

Add a dependency only when it follows the UHAL direction. For example, `sht3x` may link `uhal_interfaces`, but it must never link `stm32_hal` or an ESP-IDF target.

The IoT catalog folders are intentional placeholders, not build targets or generated code. Their README files define module responsibility and dependency boundaries before the first implementation.
