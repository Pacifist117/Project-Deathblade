project(developer_consoletester)

find_package(SDL2_ttf REQUIRED)
include_directories(${SDL2_TTF_INCLUDE_DIR} 
		${CMAKE_SOURCE_DIR}/developer_console)

add_executable(developer_consoletester main.cpp)

link_directories(${CMAKE_SOURCE_DIR}/lib)
target_link_libraries(developer_consoletester 
		${SDL2_LIBRARY} 
		${SDL2_TTF_LIBRARY}
		developer_console
		)

install(TARGETS developer_consoletester RUNTIME DESTINATION ${BIN_DIR})
