
CMAKE_MINIMUM_REQUIRED(VERSION  3.0)

project (FFM)


set (FFM_SOURCES

  common.cpp
  utility.cpp
  fe_math.cpp
  ec_math.cpp
  
  )

set (FFM_HEADERS
  common.h
  utility.h

  fe_math.h
  ec_math.h
  )


add_library (ffm
  ${FFM_SOURCES}
  ${FFM_HEADERS}
  )


