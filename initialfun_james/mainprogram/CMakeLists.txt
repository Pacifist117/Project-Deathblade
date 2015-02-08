project(mainprogram)

find_package(SDL2_image REQUIRED)
include_directories(${SDL2_IMAGE_INCLUDE_DIR} ${mainprogram_SOURCE_DIR}/../})

add_executable(mainprogram main.cpp)
target_link_libraries(mainprogram
    ${SDL2_LIBRARY}
    ${SDL2_IMAGE_LIBRARY}
    ${SDL2_PRIMATIVES_LIBRARY}
    graphicscontrol
    developer_console
    gameobjects
    player
    /usr/lib/i386-linux-gnu/libSDL2_gfx.so
)
install(TARGETS mainprogram RUNTIME DESTINATION ${BIN_DIR})

