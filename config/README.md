# Shared Configuration

Place shared toolchain files, compiler warnings, and reusable CMake functions here. Configuration belongs here only when several products need it.

For example, add `stm32-gcc-toolchain.cmake` when an STM32 product is introduced. Board pin mapping does not belong here; it belongs in that product's `board/` directory.

