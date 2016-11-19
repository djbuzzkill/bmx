cmake_minimum_required (VERSION 2.8)

project (test_Vulkan) 

set (TESTVK_SOURCES
   TestVulkan.cpp
   )

set (TESTVK_HEADER
   TestVulkan.h
   )


add_definitions  (
#   -DHELLO
   -DVK_USE_PLATFORM_WIN32_KHR
   )

add_executable (test_Vulkan
   ${TESTVK_SOURCES} 
   ${TESTVK_HEADER}
   )

target_link_libraries (test_Vulkan
   vulkan-1
#	freetype
#	freetype-gl
#	openjp2 
#	openjpeg

#	OpenGL32
#	glew32

	SDL2
	SDL2main
	SDL2_platform
#	Rn
#	Ma
	Dx
	)


