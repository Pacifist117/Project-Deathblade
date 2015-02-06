project(developer_console)

find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR})

add_library(developer_console developer_console.h developer_console.cpp)
target_link_libraries(developer_console graphicscontrol ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})

