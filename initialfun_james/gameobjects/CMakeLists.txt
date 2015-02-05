project(gameobjects)

add_library(gameobjects 
        objectbase.cpp objectbase.h
        objectcontroller.cpp objectcontroller.h
        basicwall.cpp basicwall.h
	)
target_link_libraries(gameobjects)

