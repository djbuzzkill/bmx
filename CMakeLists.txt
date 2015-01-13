
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project (Hardcore)


if (CMAKE_CL_64)
	set (SDL2_PLATFORM_LIB_DIR "x64")

else ()
	set (SDL2_PLATFORM_LIB_DIR "x86")
endif ()
	 
include_directories (
	$ENV{HARDCORE_ROOT}
	$ENV{BOOST_SDK_ROOT}/include
	$ENV{FREETYPE2_ROOT}
	$ENV{FREETYPE_GL_ROOT}
	$ENV{GLM_ROOT}
	$ENV{GLEW_ROOT}/include
	$ENV{SDL2_ROOT}/include
	)


link_directories (
	$ENV{SDL2_ROOT}/lib/{SDL2_PLATFORM_LIB_DIR}
	)

# target_link_libraries (
# 	)
#  
# set (BG_SOURCES
# 	BG_renderer.cpp
# 	)
# 
# set (BG_HEADERS
# 	BG_renderer.h
# 	)
# 
# add_library (BG_renderer
# 	${BG_SOURCES}
# 	${BG_HEADERS}
# 	)


add_subdirectory (Dx)
add_subdirectory (SDL2_platform)
