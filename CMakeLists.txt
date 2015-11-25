
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


message (STATUS	"FREEIMAGE_ROOT: $ENV{FREEIMAGE_ROOT}")
message (STATUS	"BOOST_SDK_ROOT: $ENV{BOOST_SDK_ROOT}")
message (STATUS	"BOOST_LIB_ROOT: $ENV{BOOST_LIB_ROOT}")

# source_group (
# 	Shaders 
# 	REGULAR_EXPRESSION "shaders/mars_terr.*"
# 	)

link_directories (
	$ENV{SDL2_ROOT}/lib/${SDL2_PLATFORM_LIB_DIR}
	$ENV{GLEW_ROOT}/lib/Release/${GLEW_PLATFORM_LIB_DIR}
	$ENV{FREEIMAGE_ROOT}/lib/${FREEIMAGE_LIB_DIR}
	$ENV{FREETYPE_GL_ROOT}/lib/${CMAKE_CFG_INTDIR}
	$ENV{FREETYPE2_ROOT}/lib/
	)

# add_subdirectory (test_GL4_experiment)

add_subdirectory (Dx)
add_subdirectory (SDL2_platform)
add_subdirectory (OpenGL_system)
add_subdirectory (Terrain_renderer)
add_subdirectory (Charon)

add_subdirectory (test_Mars)
add_subdirectory (test_Cubes)
