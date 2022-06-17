cmake_minimum_required (VERSION 3.0)

project (Hardcore)



# include
include_directories (
  "./cx"
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

# libs
add_subdirectory (cx)


# progs
add_subdirectory (mtool)
