
CMAKE_MINIMUM_REQUIRED(VERSION  3.0)

project (FFM)


set (FFM_SOURCES

  Common.cpp
  Utility.cpp
  FE_Math.cpp
  EC_Math.cpp
  
  )

set (FFM_HEADERS
  Common.h
  Utility.h

  FE_Math.h
  EC_Math.h
  )


add_library (FFM
  ${FFM_SOURCES}
  ${FFM_HEADERS}
  )


