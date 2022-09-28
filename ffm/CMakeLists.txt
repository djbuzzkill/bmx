
CMAKE_MINIMUM_REQUIRED(VERSION  3.1)

project (FFM)



message (STATUS "FFM:${CMAKE_CURRENT_SOURCE_DIR}")
message (STATUS "FFM:${CMAKE_SOURCE_DIR}")

include_directories (
  ${CMAKE_CURRENT_SOURCE_DIR}
  ${CMAKE_SOURCE_DIR}
  )


set (FFM_SOURCES

  common.cpp
  utility.cpp
  fe_math.cpp
  ec_math.cpp

  fe_context.cpp
  )

set (FFM_HEADERS
  common.h
  utility.h
  shared_types.h
  fe_math.h
  ec_math.h

  fe_context.h
  )


add_library (ffm
  ${FFM_SOURCES}
  ${FFM_HEADERS}
  )


