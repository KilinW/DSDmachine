# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

#Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/EXTERNAL/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C++ standards
project(DSDcontrol C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME}
        main.cpp
        Gcode_sender.cpp
        Gcode_sender.h
        pwm_unit.cpp
        pwm_unit.h
)

# Create map/bin/hex file etc.
pico_add_extra_outputs(${PROJECT_NAME})

# pull in common dependencies
target_link_libraries(${PROJECT_NAME}
        pico_stdlib
        pico_multicore
        hardware_adc
        hardware_dma
        hardware_uart
        hardware_gpio
        hardware_pio
        hardware_gpio
        hardware_uart
        hardware_pwm
)

# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 0)
pico_enable_stdio_uart(${PROJECT_NAME} 1)