add_executable(env_monitor app/main.cpp)
target_link_libraries(env_monitor PRIVATE environment_monitor uhal_fake)

option(ENV_MONITOR_BUILD_STM32L476RG "Build the NUCLEO-L476RG firmware image" OFF)

if(ENV_MONITOR_BUILD_STM32L476RG)
    if(NOT DEFINED STM32L476RG_STARTUP_SOURCE OR NOT DEFINED STM32L476RG_LINKER_SCRIPT)
        message(FATAL_ERROR "Set STM32L476RG_STARTUP_SOURCE and STM32L476RG_LINKER_SCRIPT.")
    endif()

    enable_language(ASM)

    add_subdirectory(
        ${CMAKE_CURRENT_SOURCE_DIR}/../../components/platform/stm32/stm32l4/nucleo_l476rg
        ${CMAKE_CURRENT_BINARY_DIR}/nucleo_l476rg_platform
    )

    add_executable(env_monitor_stm32l476rg
        board/nucleo_l476rg/main.cpp
        board/nucleo_l476rg/interrupts.cpp
        ${STM32L476RG_STARTUP_SOURCE}
    )
    target_link_libraries(env_monitor_stm32l476rg PRIVATE environment_monitor nucleo_l476rg_adapters)
    target_link_options(env_monitor_stm32l476rg PRIVATE -T${STM32L476RG_LINKER_SCRIPT})
    target_compile_options(env_monitor_stm32l476rg PRIVATE
        -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
        -ffunction-sections -fdata-sections -fno-exceptions -fno-rtti
    )
    target_link_options(env_monitor_stm32l476rg PRIVATE
        -mcpu=cortex-m4 -mthumb -mfpu=fpv4-sp-d16 -mfloat-abi=hard
        -Wl,--gc-sections --specs=nano.specs --specs=nosys.specs
    )
endif()

