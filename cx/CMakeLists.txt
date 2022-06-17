
CMAKE_MINIMUM_REQUIRED(VERSION  3.0)

project (cx)


set (CX_SOURCES
  Task.cpp
  System.cpp
  )

set (CX_HEADERS
  Task.h
  System.h
  )

add_library (cx
  ${CX_SOURCES}
  ${CX_HEADERS}
  )


