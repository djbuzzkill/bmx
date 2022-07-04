cmake_minimum_required (VERSION 3.0)

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
add_subdirectory (secp256k1)
add_subdirectory (Cx)
#add_subdirectory (Hx)
add_subdirectory (ffm)


# progs
add_subdirectory (mtool)
add_subdirectory (rumbum)
