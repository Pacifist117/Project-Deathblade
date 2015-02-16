project(mainprogram)

find_package(SDL2_image REQUIRED)
find_package(SDL2_gfx REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR} ${SDL2_gfx_INCLUDE_DIR} ${mainprogram_SOURCE_DIR}/../})

MESSAGE("gfx: ${SDL2_gfx_LIBRARY}")

add_executable(mainprogram main.cpp)
target_link_libraries(mainprogram
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${SDL2_gfx_LIBRARY}
    graphicscontrol
    developer_console
    gameobjects
    player
)
install(TARGETS mainprogram RUNTIME DESTINATION ${BIN_DIR})

