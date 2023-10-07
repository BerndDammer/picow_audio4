# freertos/lwip template // cmake template  taken from
# pico-examples/pico_w/wifi/freertos/iperf

# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

# Initialise pico_sdk from installed location
# (note this can come from environment, CMake cache etc)
# set(PICO_SDK_PATH "d:/RaspberryPiPico2/pico-sdk")

set(PICO_BOARD pico_w CACHE STRING "Board type")

# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

if (PICO_SDK_VERSION_STRING VERSION_LESS "1.4.0")
  message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.4.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()


# ///////////////////////////////////////////////////// freertos

### # // WORKAROUND: Why must be set now and not before
### set(FREE_RTOS_KERNEL_SMP 1)


set(FREERTOS_KERNEL_PATH ${CMAKE_CURRENT_LIST_DIR}/freertos)
include(FreeRTOS_Kernel_import.cmake)

# ///////////////////////////////


project(usb_audio4 C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.2.1


#target_include_directories(usb_audio4 PRIVATE src )
#target_include_directories(usb_audio4 ${CMAKE_CURRENT_LIST_DIR}/src )

#add_subdirectory(src)
#include_directories(${MyProject_SOURCE_DIR}/src)

add_executable(usb_audio4
	src/entry.c 
	src/panic.c
	src/task_blinker.c
	src/task_console.c
	src/task_network.c
	src/task_tud.c
	src/usb_descriptors.c
	src/board_usb_get_serial.c
)


target_include_directories(usb_audio4 PRIVATE
	freertos/include
	freertos/portable/ThirdParty/GCC/RP2040/include/
)

# don't want NO_SYS (generally this would be in your lwipopts.h)
target_compile_definitions(usb_audio4 PRIVATE NO_SYS=0 )

pico_set_program_name(usb_audio4 "usb_audio4")
pico_set_program_version(usb_audio4 "0.0.2")

pico_enable_stdio_uart(usb_audio4 1)
pico_enable_stdio_usb(usb_audio4 0)

# Add the standard include files to the build
target_include_directories(usb_audio4 PRIVATE
  ${CMAKE_CURRENT_LIST_DIR}
  ${CMAKE_CURRENT_LIST_DIR}/../.. # for our common lwipopts or any other standard includes, if required
)

# Add any user requested libraries
target_link_libraries(usb_audio4
    pico_stdlib
    hardware_spi
    hardware_timer
    hardware_clocks
    hardware_pwm
    pico_cyw43_arch_lwip_sys_freertos
    FreeRTOS-Kernel-Heap4
    tinyusb_device
)

pico_add_extra_outputs(usb_audio4)

