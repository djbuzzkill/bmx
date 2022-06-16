
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

project (Terrain_renderer)

include_directories (
	${BOOST_INCLUDE_DIRS}
	)

set (TERRAINRENDERER_SOURCES
	Terrain_renderer.cpp
	)

set (TERRAINRENDERER_HEADERS
	Terrain_renderer.h
	)

add_library (Terrain_renderer
	${TERRAINRENDERER_SOURCES}
	${TERRAINRENDERER_HEADERS}
	)


