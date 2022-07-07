cmake_minimum_required (VERSION 3.1)

project (gx)



# include
include_directories (
  ./
  )

add_definitions (-DCMAKE_EXPORT_COMPILE_COMMANDS=1)


add_compile_options (-g -O0 )

#message (STATUS	"FREEIMAGE_ROOT: $ENV{FREEIMAGE_ROOT}")
# source_group (
# 	Shaders 
# 	REGULAR_EXPRESSION "shaders/mars_terr.*"
# 	)

link_directories (
  )

# libs
add_subdirectory (aframe)
add_subdirectory (ffm)
add_subdirectory (SECzy)
#add_subdirectory (Hx)


# progs
add_subdirectory (mtool)
add_subdirectory (rum)
