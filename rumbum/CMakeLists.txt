
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

project (rumbum)



add_compile_options (-g -O0 )

#set ("CMAKE_CXX_FLAGS=-g -00")
#set ("CMAKE_C_FLAGS=-g -00")

  
set (RUMBUM_HEADER_FILES
  rumbum.h
 )
	
set (RUMBUM_SOURCE_FILES
  rumbum.cpp
  )

add_executable (rumbum
  ${RUMBUM_HEADER_FILES}
  ${RUMBUM_SOURCE_FILES}
  )


target_link_libraries (rumbum

  Cx
  crypto++
  FFM
  SDL2
  SDL2main
  )



