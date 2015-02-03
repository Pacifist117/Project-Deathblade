project(gameobjects)

add_library(gameobjects 
	objectcontroller.cpp objectcontroller.h
	objectbase.cpp objectbase.h
	)
target_link_libraries(gameobjects)

