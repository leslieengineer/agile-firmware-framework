# NUCLEO-L476RG Platform

## Five-Layer Demo

1. **Low layer:** `demo/Stm32L4LowLayer.*` configures clocks, SysTick, PA5, and USART2 on PA2/PA3 with STM32 LL/CMSIS register primitives. It is the only demo layer that accesses STM32 registers.
2. **UHAL ports:** `uhal::IClock`, `uhal::IGpio`, and `uhal::IUart` define hardware-independent virtual contracts.
3. **Adapters:** `demo/Stm32L4Adapters.*` implement those contracts and delegate to the low layer. They do not expose STM32 register types to application code.
4. **Application API:** `demo/EnvironmentMonitor.*` performs sensor simulation, LED control, CLI parsing, and telemetry only through UHAL ports.
5. **Composition root:** `demo/Application.*` creates adapters and application services. CubeIDE `main.c` calls only low-layer initialization and this application entry point.

The sample routes USART2 TX/RX through PA2/PA3 to the ST-LINK Virtual COM interface on a NUCLEO-L476RG. LD2 is PA5.

## CubeIDE Integration

After adding a new `Core/Src/*.cpp` file, refresh the project in STM32CubeIDE and run a clean build. CubeIDE regenerates `Debug/Core/Src/subdir.mk` and `Debug/objects.list`; these generated files must contain `Stm32L4LowLayer.cpp` and `Stm32L4LowLayer.o`.
