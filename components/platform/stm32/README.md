# STM32 Platform Adapter

This directory holds STM32-specific implementations of UHAL interfaces. The STM32H5 family peripheral catalog is under `stm32h5/`.

The catalog is a superset across STM32H5 variants, not a promise that every H5 SKU includes every peripheral. A product must select an exact MCU, read its reference manual and datasheet, then enable only the matching folders in its CMake target.

Peripheral code may link `uhal_interfaces` and STM32Cube HAL/LL. Device and protocol components must never link STM32-specific targets directly.


