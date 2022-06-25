
CMAKE_MINIMUM_REQUIRED(VERSION 3.0)

project (mtool)




#set ("CMAKE_CXX_FLAGS=-g -00")
#set ("CMAKE_C_FLAGS=-g -00")

  
set (MTOOL_HEADER_FILES
  mtool.h

  Common.h
  Utility.h
#  EC_Math.h
#  FE_Math.h
  )
	
set (MTOOL_SOURCE_FILES
  mtool.cpp
  Common.cpp
  Utility.cpp
#  FE_Math.cpp
#  EC_Math.cpp

  )

add_executable (mtool
  ${MTOOL_HEADER_FILES}
  ${MTOOL_SOURCE_FILES}
  )


target_link_libraries (mtool

  FFM
  gmp
  gmpxx

  #ssl
  crypto++
  )



