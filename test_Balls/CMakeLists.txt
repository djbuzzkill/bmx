
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project (test_Balls)


set (BALLS_HEADER_FILES
	Balls.h
	)
	
set (BALLS_SOURCE_FILES
	Balls.cpp
	)

add_executable (test_Balls
	${BALLS_HEADER_FILES}
	${BALLS_SOURCE_FILES}
	)


target_link_libraries (test_Balls
	OpenGL32
	glew32
	OpenGL_system
	Dx
	SDL2
	SDL2main
	SDL2_platform
	FreeImage
	Charon
	)


