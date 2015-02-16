project(gameobjects)

find_package(SDL2_gfx REQUIRED)

add_library(gameobjects 
        objectbase.cpp objectbase.h
        objectcontroller.cpp objectcontroller.h
        basicwall.cpp basicwall.h
        star.cpp star.h
	)
target_link_libraries(gameobjects
    ${SDL2_LIBRARY}
    ${SDL2_gfx_LIBRARY}
)

