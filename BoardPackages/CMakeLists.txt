# 1. Hardware Interface Library
add_library(bsp_flags INTERFACE)

target_compile_options(bsp_flags INTERFACE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
)

target_compile_definitions(bsp_flags INTERFACE
    STM32L433xx
    USE_HAL_DRIVER
    NUCLEO_L433RC_P
)

target_link_options(bsp_flags INTERFACE
    -mcpu=cortex-m4
    -mthumb
    -mfpu=fpv4-sp-d16
    -mfloat-abi=hard
    -specs=nano.specs
    -specs=nosys.specs
    -Wl,--gc-sections
    -Wl,-e,Reset_Handler   # Force Reset_Handler as the entry point
)

# 2. Collect ALL C files from both board/ and generated/
file(GLOB_RECURSE ALL_BSP_C_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/board/*.c"
    "${CMAKE_CURRENT_SOURCE_DIR}/nucleo_l433/generated/*.c"
)

# Exclude generated main.c so application main.c acts as single entry point
#list(FILTER ALL_BSP_C_SOURCES EXCLUDE REGEX ".*/Core/Src/main\\.c$")

# 3. Collect Assembly startup files
file(GLOB_RECURSE ALL_BSP_ASM_SOURCES
    "${CMAKE_CURRENT_SOURCE_DIR}/nucleo_l433/generated/*.s"
    "${CMAKE_CURRENT_SOURCE_DIR}/nucleo_l433/generated/*.S"
    "${CMAKE_CURRENT_SOURCE_DIR}/board/*.s"
    "${CMAKE_CURRENT_SOURCE_DIR}/board/*.S"
)

# 4. Create the BSP Library as STATIC
add_library(board_package STATIC
    ${ALL_BSP_C_SOURCES}
    ${ALL_BSP_ASM_SOURCES}
)

target_link_libraries(board_package PUBLIC bsp_flags)

# Automatically expose all include paths under board/ and generated/
target_include_directories(board_package PUBLIC
    board
    board/inc
    nucleo_l433/generated/Core/Inc
    nucleo_l433/generated/Drivers/STM32L4xx_HAL_Driver/Inc
    nucleo_l433/generated/Drivers/CMSIS/Device/ST/STM32L4xx/Include
    nucleo_l433/generated/Drivers/CMSIS/Include
)

# 5. Automatically locate Linker Script (.ld or .LD)
file(GLOB_RECURSE BOARD_LINKER_SCRIPT_FILE
    "${CMAKE_CURRENT_SOURCE_DIR}/nucleo_l433/generated/*.ld"
    "${CMAKE_CURRENT_SOURCE_DIR}/nucleo_l433/generated/*.LD"
)

list(LENGTH BOARD_LINKER_SCRIPT_FILE SCRIPT_COUNT)

if(SCRIPT_COUNT GREATER 0)
    list(GET BOARD_LINKER_SCRIPT_FILE 0 DETECTED_LINKER_SCRIPT)
    message(STATUS "Found Board Linker Script: ${DETECTED_LINKER_SCRIPT}")
else()
    set(DETECTED_LINKER_SCRIPT "${CMAKE_CURRENT_SOURCE_DIR}/nucleo_l433/generated/STM32L433RCTX_FLASH.ld")
    message(WARNING "No .ld file found via GLOB. Falling back to: ${DETECTED_LINKER_SCRIPT}")
endif()

if(NOT EXISTS "${DETECTED_LINKER_SCRIPT}")
    message(FATAL_ERROR "Error: No valid linker script (.ld) found inside 'board_package' target!")
endif()

# 6. Set Target Property
set_target_properties(board_package PROPERTIES
    BOARD_LINKER_SCRIPT "${DETECTED_LINKER_SCRIPT}"
)