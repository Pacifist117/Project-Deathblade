project( camera_controller )

add_library( camera_controller
        camera_object.h camera_object.cpp
	cameracontrol.h cameracontrol.cpp
)
target_link_libraries(camera_controller ${SDL2_LIBRARY} ${SDL2_TTF_LIBRARY})

