
CMAKE_MINIMUM_REQUIRED(VERSION 3.1)

project (mtool)




#set ("CMAKE_CXX_FLAGS=-g -O0")
#set ("CMAKE_C_FLAGS=-g -O0")

  
set (MTOOL_HEADER_FILES
  mtool.h

#  common.h
#  utility.h
#  EC_Math.h
#  FE_Math.h
  )
	
set (MTOOL_SOURCE_FILES
  mtool.cpp
#  common.cpp
#  utility.cpp
#  FE_Math.cpp
#  EC_Math.cpp

  )

add_executable (mtool
  ${MTOOL_HEADER_FILES}
  ${MTOOL_SOURCE_FILES}
  )


target_link_libraries (mtool

  curv
  aframe
  ffm

  gcrypt
  gmp
  gmpxx
  #ssl
  #crypt 
  # crypto++
  )



