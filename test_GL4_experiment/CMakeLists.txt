
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project (test_GL4_experiment)


set (TEST_GL4_HEADER_FILES
	"test_GL4_experiment.h"
	)
	
	
set (TEST_GL4_SOURCE_FILES
	"test_GL4_experiment.cpp"
	)

add_executable (test_GL4_experiment
	${TEST_GL4_HEADER_FILES}
	${TEST_GL4_SOURCE_FILES}
	)


target_link_libraries (test_GL4_experiment
	OpenGL32
	glew32
	OpenGL_system
	Dx
	SDL2
	SDL2main
	SDL2_platform
	)


