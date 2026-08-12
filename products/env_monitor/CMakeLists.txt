add_executable(env_monitor app/main.cpp app/EnvironmentMonitor.cpp)
target_link_libraries(env_monitor PRIVATE uhal_fake)

