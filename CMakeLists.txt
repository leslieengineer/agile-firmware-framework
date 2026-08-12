cmake_minimum_required(VERSION 3.20)

project(uhal_component_catalog LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

enable_testing()

add_subdirectory(components/uhal/core)
add_subdirectory(components/uhal/interfaces)
add_subdirectory(components/libraries)
add_subdirectory(components/platform/fake)
add_subdirectory(components/services)
add_subdirectory(components/services/environment_monitor)
add_subdirectory(components/devices/sht3x)
add_subdirectory(components/protocols/modbus-rtu)
add_subdirectory(components/protocols/frame)
add_subdirectory(products/env_monitor)
add_subdirectory(tests)

