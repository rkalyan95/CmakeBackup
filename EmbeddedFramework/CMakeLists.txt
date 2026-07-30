file(GLOB_RECURSE PAL_SOURCES "${CMAKE_CURRENT_SOURCE_DIR}/pal/*.c")

add_library(EmbeddedFramework OBJECT
    ${PAL_SOURCES}
)

# Linking board_package inherits bsp_flags (-mfloat-abi=hard, -mcpu=cortex-m4) and include paths
target_link_libraries(EmbeddedFramework PUBLIC board_package)

target_include_directories(EmbeddedFramework PUBLIC
    "${CMAKE_CURRENT_SOURCE_DIR}/include"
)