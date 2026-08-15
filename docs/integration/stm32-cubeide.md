# STM32CubeIDE Integration

Use the framework as a Git submodule. Keep CubeIDE startup assembly, linker script and generated board initialization. Add only selected UHAL, reusable logic, Layer 1 and Layer 3 sources.

For a new peripheral vertical slice:

1. Configure exact pins, clocks, DMA and interrupt priority in the board project.
2. Implement the required Layer 1 capability using STM32 HAL/LL/CMSIS.
3. Implement the Layer 3 UHAL adapter and map vendor errors to `uhal::Status`.
4. Extend the matching host fake and contract tests.
5. Implement Layer 4 device/protocol/service logic using only UHAL.
6. Construct and inject objects in the product composition root.
7. Build, flash and verify timing/error behavior on the target.

Do not copy product policy into generated `main.c`, and do not call Layer 4 code directly from an ISR. Prefer a C++ product entry with exactly one `main()`.
