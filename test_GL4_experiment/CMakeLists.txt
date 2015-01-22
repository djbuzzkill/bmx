
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project (test_GL4_experiment)


set (TEST_GL4_HEADER_FILES
   "mars_terr.h"
	"test_GL4_experiment.h"
   "process_mars_terrain_for_runtime.h"
   "quantize_height_tiles_to_u16.h"
	)
	
	
set (TEST_GL4_SOURCE_FILES
	"test_GL4_experiment.cpp"
   "process_mars_terrain_for_runtime.cpp"
   "quantize_height_tiles_to_u16.cpp"
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
	FreeImage
	)


