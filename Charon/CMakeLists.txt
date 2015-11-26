
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project (Charon)

include_directories (
	${BOOST_INCLUDE_DIRS}
	)


set (CHARON_HEADERS
	Charon.h
	Shader.h
   process_mars_terrain_for_runtime.h
   quantize_height_tiles_to_u16.h
	)


set (CHARON_SOURCES
	Charon.cpp
	Shader.cpp
	process_mars_terrain_for_runtime.cpp
	quantize_height_tiles_to_u16.cpp
	)


add_library (Charon
	${CHARON_SOURCES}
	${CHARON_HEADERS}
	)


