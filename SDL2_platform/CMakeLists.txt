
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project (SDL2_platform)


set (SDL2_PLATFORM_HEADER_FILES
	SDL2_platform.cpp
	)
	
	
set (SDL2_PLATFORM_SOURCE_FILES
	SDL2_platform.h
	)

add_library (SDL2_platform
	${SDL2_PLATFORM_HEADER_FILES}
	${SDL2_PLATFORM_SOURCE_FILES}
	)







