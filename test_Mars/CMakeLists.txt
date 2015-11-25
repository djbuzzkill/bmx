
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project (test_Mars)


set (MARS_HEADER_FILES
	Mars.h

#   "mars_terr.h"
#   "process_mars_terrain_for_runtime.h"
#   "quantize_height_tiles_to_u16.h"
	)
	
	
set (MARS_SOURCE_FILES
	Mars.cpp
	)

add_executable (test_Mars
	${MARS_HEADER_FILES}
	${MARS_SOURCE_FILES}
	)

target_link_libraries (test_Mars
	OpenGL32
	glew32
	OpenGL_system
	Dx
	SDL2
	SDL2main
	SDL2_platform
	FreeImage
	Charon
	Terrain_renderer
	)


