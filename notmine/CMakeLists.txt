
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

project (notmine)



add_compile_options (-g -O0 )

#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")

  
set (THE_NOTMINE_HEADERS
  notmine.h
 )
	
set (THE_NOTMINE_SOURCES
  notmine.cpp
  )

add_executable (notmine

  ${THE_NOTMINE_HEADERS}
  ${THE_NOTMINE_SOURCES}

)

#
#
target_link_libraries (notmine

  crypt
  dl
  m
  gmp
#  gmpxx		      
  aframe 	      
  ffm		      
  bmx
  zmq
)





