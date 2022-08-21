
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

project (theabyss)



add_compile_options (-g -O0 )

#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")

  
set (THE_ABYSS_HEADERS
  theabyss.h
 )
	
set (THE_ABYSS_SOURCES
  theabyss.cpp
  )

add_executable (theabyss

  ${THE_ABYSS_HEADERS}
  ${THE_ABYSS_SOURCES}

)

#
#
target_link_libraries (theabyss

  gmp
#  gmpxx		      
  aframe 	      
  ffm		      
  curv
  zmq

)



