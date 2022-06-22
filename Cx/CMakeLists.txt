
CMAKE_MINIMUM_REQUIRED(VERSION  3.0)

project (Cx)


set (CX_SOURCES
  Task.cpp
  System.cpp
  )

set (CX_HEADERS
  Task.h
  System.h
  )

add_library (Cx
  ${CX_SOURCES}
  ${CX_HEADERS}
  )


