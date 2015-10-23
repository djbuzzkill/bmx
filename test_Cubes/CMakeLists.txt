
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project (test_Cubes)


set (CUBES_HEADER_FILES
	Cubes.h
	)
	
set (CUBES_SOURCE_FILES
	Cubes.cpp
	)

add_executable (test_Cubes
	${CUBES_HEADER_FILES}
	${CUBES_SOURCE_FILES}
	)


target_link_libraries (test_Cubes
	OpenGL32
	glew32
	OpenGL_system
	Dx
	SDL2
	SDL2main
	SDL2_platform
	FreeImage
	)


