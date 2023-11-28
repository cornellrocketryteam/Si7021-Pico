add_library(Si7021-Pico INTERFACE)

target_sources(Si7021-Pico INTERFACE
    ${CMAKE_CURRENT_LIST_DIR}/si7021.cpp
)

target_link_libraries(Si7021-Pico INTERFACE
    hardware_i2c
    pico_stdlib
)
