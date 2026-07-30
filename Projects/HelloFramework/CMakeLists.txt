add_executable(HelloFramework
    app/app_main.c
)

# Explicitly set output extension to .elf
set_target_properties(HelloFramework PROPERTIES
    SUFFIX ".elf"
)

# Wrap static libraries with --whole-archive so weak vector handlers (like SysTick_Handler) 
# aren't stripped by dead-code elimination
target_link_libraries(HelloFramework PRIVATE
    -Wl,--whole-archive
    EmbeddedFramework
    board_package
    -Wl,--no-whole-archive
)

# Attach detected linker script from board_package target property
target_link_options(HelloFramework PRIVATE
    "-T$<TARGET_PROPERTY:board_package,BOARD_LINKER_SCRIPT>"
    "-Wl,-Map=${CMAKE_CURRENT_BINARY_DIR}/HelloFramework.map"
    "-Wl,--print-memory-usage"
)

# Post-build step to generate .bin, .hex, and print size output
add_custom_command(TARGET HelloFramework POST_BUILD
    COMMAND ${CMAKE_OBJCOPY} -O binary $<TARGET_FILE:HelloFramework> ${CMAKE_CURRENT_BINARY_DIR}/HelloFramework.bin
    COMMAND ${CMAKE_OBJCOPY} -O ihex $<TARGET_FILE:HelloFramework> ${CMAKE_CURRENT_BINARY_DIR}/HelloFramework.hex
    COMMAND ${CMAKE_SIZE} $<TARGET_FILE:HelloFramework>
    COMMENT "Generating .bin and .hex binaries for HelloFramework"
)