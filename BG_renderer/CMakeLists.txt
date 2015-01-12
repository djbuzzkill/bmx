
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project (BG_renderer)

include_directories (
	${BOOST_INCLUDE_DIRS}
	)

set (BG_SOURCES
	BG_renderer.cpp
	)

set (BG_HEADERS
	BG_renderer.h
	)

add_library (BG_renderer
	${BG_SOURCES}
	${BG_HEADERS}
	)


