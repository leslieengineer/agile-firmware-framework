add_library(sht3x STATIC src/Sht3x.cpp)
target_include_directories(sht3x PUBLIC include)
target_link_libraries(sht3x PUBLIC uhal_interfaces)
