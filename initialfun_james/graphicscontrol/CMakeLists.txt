project(graphicscontrol)

find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR})

add_library(graphicscontrol
cameracontrol.h cameracontrol.cpp
tempsettings.h tempsettings.cpp
controlbaseclass.h controlbaseclass.cpp
gameenums.h
)
target_link_libraries(graphicscontrol ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})

