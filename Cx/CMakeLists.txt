
CMAKE_MINIMUM_REQUIRED(VERSION  3.0)

project (Cx)


set (CX_SOURCES
  Common.cpp
  Utility.cpp
  Hash.cpp
  Task.cpp
  System.cpp
)


set (CX_HEADERS
  Common.h
  Utility.h
  Hash.h
  Task.h
  System.h
  )


add_library (Cx
  ${CX_SOURCES}
  ${CX_HEADERS}
  )


