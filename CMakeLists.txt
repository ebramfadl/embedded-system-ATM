# Set minimum required version of CMake
cmake_minimum_required(VERSION 3.12)

# Include build functions from Pico SDK
include($ENV{PICO_SDK_PATH}/external/pico_sdk_import.cmake)

# Set name of project (as PROJECT_NAME) and C/C   standards
project(blink C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Creates a pico-sdk subdirectory in our project for the libraries
pico_sdk_init()

# Tell CMake where to find the executable source file
add_executable(${PROJECT_NAME} 
    mainOne.c
    mainTwo.c
    main.c
    LCDops.c
    generalOps.c
    presetChars.c
    presetMessages.c
    ultrasonic.c
)

# Create map/bin/hex/uf2 files
pico_add_extra_outputs(${PROJECT_NAME})

# Link to pico_stdlib (gpio, time, etc. functions)
target_link_libraries(${PROJECT_NAME} 
    pico_stdlib
)
target_link_libraries(${PROJECT_NAME} 
    pico_binary_info
)
target_link_libraries(${PROJECT_NAME} 
    hardware_pwm
)
target_link_libraries(${PROJECT_NAME} 
    hardware_i2c
)
target_link_libraries(${PROJECT_NAME} 
    hardware_adc
)
target_link_libraries(${PROJECT_NAME} 
    hardware_dma
)
target_link_libraries(${PROJECT_NAME} 
    hardware_pio
)
# target_link_libraries(${PROJECT_NAME} 
#     lcd_1602_i2c
# )
# Enable usb output, disable uart output
pico_enable_stdio_usb(${PROJECT_NAME} 1)
pico_enable_stdio_uart(${PROJECT_NAME} 0)