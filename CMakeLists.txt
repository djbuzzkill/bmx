cmake_minimum_required (VERSION 3.1)

project (gx)



# include
include_directories (

  ${CMAKE_SOURCE_DIR}
  )

add_definitions (-DCMAKE_EXPORT_COMPILE_COMMANDS=1)


add_compile_options (-g -O0 -Wall -Wextra -Wshadow -Wpedantic) # -Werror -Wconversion


#
link_directories (
  )

# libs
add_subdirectory (aframe)
add_subdirectory (ffm)
add_subdirectory (bmx)


# progs
add_subdirectory (mtool)
add_subdirectory (rum)
add_subdirectory (notmine)



#message (STATUS "${CMAKE_BINARY_DIR}")
#message (STATUS "${CMAKE_SOURCE_DIR}")
#message (STATUS "${CMAKE_CURRENT_BINARY_DIR}")
#message (STATUS "${CMAKE_CURRENT_SOURCE_DIR}")
