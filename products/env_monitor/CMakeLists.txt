add_executable(env_monitor app/main.cpp)
target_link_libraries(env_monitor PRIVATE sht3x modbus_rtu uhal_fake)

