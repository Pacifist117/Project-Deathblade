project(gameobjects)


add_library(gameobjects 
        objectbase.h objectbase.cpp
        gameobject.cpp gameobject.h
        objectcontroller.cpp objectcontroller.h
        basicwall.cpp basicwall.h
        texturewall.cpp texturewall.h
        star.cpp star.h
	)
target_link_libraries(gameobjects
    ${SDL2_LIBRARY}
    ${SDL2_gfx_LIBRARY}
)

