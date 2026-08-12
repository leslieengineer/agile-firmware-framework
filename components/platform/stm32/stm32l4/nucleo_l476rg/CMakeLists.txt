if(NOT DEFINED STM32L4_CMSIS_DEVICE_INCLUDE_DIR OR NOT DEFINED STM32L4_CMSIS_CORE_INCLUDE_DIR OR
    NOT DEFINED STM32L4_LL_INCLUDE_DIR)
    message(FATAL_ERROR "Set STM32L4_CMSIS_DEVICE_INCLUDE_DIR, STM32L4_CMSIS_CORE_INCLUDE_DIR, and STM32L4_LL_INCLUDE_DIR for NUCLEO-L476RG builds.")
endif()

add_library(nucleo_l476rg_low_level STATIC
    low_level/src/NucleoL476rgLowLevel.cpp
)
target_include_directories(nucleo_l476rg_low_level
    PUBLIC low_level/include
    PRIVATE ${STM32L4_CMSIS_DEVICE_INCLUDE_DIR} ${STM32L4_CMSIS_CORE_INCLUDE_DIR}
    ${STM32L4_LL_INCLUDE_DIR}
)
target_compile_definitions(nucleo_l476rg_low_level PRIVATE STM32L476xx)

add_library(nucleo_l476rg_adapters STATIC
    adapters/src/NucleoL476rgAdapters.cpp
)
target_include_directories(nucleo_l476rg_adapters PUBLIC adapters/include)
target_link_libraries(nucleo_l476rg_adapters PUBLIC nucleo_l476rg_low_level uhal_interfaces)
