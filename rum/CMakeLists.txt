
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

project (rum)



add_compile_options (-g -O0 )

#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")

  
set (RUM_HEADER_FILES
  rum.h
 )
	
set (RUM_SOURCE_FILES
  rum.cpp
  )

add_executable (rum
  ${RUM_HEADER_FILES}
  ${RUM_SOURCE_FILES}
  )


target_link_libraries (rum

		      
  gmp
  curv
  curl  
  aframe 
  ffm
  gcrypt
  zmq
)



