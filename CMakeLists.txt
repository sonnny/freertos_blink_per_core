# Generated Cmake Pico project file

cmake_minimum_required(VERSION 3.13)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)


# Pull in Raspberry Pi Pico SDK (must be before project)
include(pico_sdk_import.cmake)

# Pull in FreeRTOS
include(FreeRTOS_Kernel_import.cmake)


project(SMP C CXX ASM)

# Initialise the Raspberry Pi Pico SDK
pico_sdk_init()

# Add executable. Default name is the project name, version 0.1

add_executable(${CMAKE_PROJECT_NAME} 
    main.c
    freertos_hooks.c
    )

pico_set_program_name(${CMAKE_PROJECT_NAME} ${CMAKE_PROJECT_NAME})
pico_set_program_version(${CMAKE_PROJECT_NAME} "0.1")

pico_enable_stdio_uart(${CMAKE_PROJECT_NAME} 0)
pico_enable_stdio_usb(${CMAKE_PROJECT_NAME} 0)

target_compile_definitions(${CMAKE_PROJECT_NAME} PRIVATE
        PICO_STDIO_STACK_BUFFER_SIZE=64 # use a small printf on stack buffer
)
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE
        ${CMAKE_CURRENT_LIST_DIR}
        )

# Add the standard library to the build
target_link_libraries(${CMAKE_PROJECT_NAME} pico_stdlib FreeRTOS-Kernel FreeRTOS-Kernel-Heap4)

pico_add_extra_outputs(${CMAKE_PROJECT_NAME})

