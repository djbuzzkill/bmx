
CMAKE_MINIMUM_REQUIRED(VERSION 2.8)

project (Hardcore)


if (CMAKE_CL_64)
	set (FREEIMAGE_LIB_DIR "x64")
	set (SDL2_PLATFORM_LIB_DIR "x64")
	set (GLEW_PLATFORM_LIB_DIR "x64")
else () 
	set (FREEIMAGE_LIB_DIR "x86")
	set (SDL2_PLATFORM_LIB_DIR "x86")
	set (GLEW_PLATFORM_LIB_DIR "Win32")
endif ()

	 
include_directories (
	$ENV{HARDCORE_ROOT}
	$ENV{FREETYPE2_ROOT}
	$ENV{FREETYPE_GL_ROOT}/lib/
	$ENV{GLM_ROOT}
	$ENV{GLEW_ROOT}/include
	$ENV{SDL2_ROOT}/include
	$ENV{FREEIMAGE_ROOT}/include
	$ENV{BOOST_LIB_ROOT}
	)


message (STATUS	"BOOST_SDK_ROOT: $ENV{BOOST_SDK_ROOT}")
message (STATUS	"BOOST_LIB_ROOT: $ENV{BOOST_LIB_ROOT}")

link_directories (
	$ENV{SDL2_ROOT}/lib/${SDL2_PLATFORM_LIB_DIR}
	$ENV{GLEW_ROOT}/lib/Release/${GLEW_PLATFORM_LIB_DIR}
	$ENV{FREETYPE2_ROOT}/lib/
	$ENV{FREETYPE_GL_ROOT}/lib/${CMAKE_CFG_INTDIR}
	$ENV{FREEIMAGE_ROOT}/lib/${FREEIMAGE_LIB_DIR}
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
add_subdirectory (OpenGL_system)

add_subdirectory (test_GL4_experiment)
