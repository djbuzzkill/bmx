
CMAKE_MINIMUM_REQUIRED(VERSION  3.1)

project (aframe)


set (AF_SOURCES
  common.cpp
  utility.cpp
  binary_IO.cpp
  hash.cpp
  task.cpp
  system.cpp
)


set (AF_HEADERS
  common.h
  utility.h
  binary_IO.h
  hash.h
  task.h
  system.h
  )


add_library (aframe
  ${AF_SOURCES}
  ${AF_HEADERS}
  )


