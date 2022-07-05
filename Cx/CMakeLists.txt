
CMAKE_MINIMUM_REQUIRED(VERSION  3.0)

project (aframe)


set (AF_SOURCES
  common.cpp
  utility.cpp
  hash.cpp
  task.cpp
  system.cpp
)


set (AF_HEADERS
  common.h
  utility.h
  hash.h
  task.h
  system.h
  )


add_library (aframe
  ${AF_SOURCES}
  ${AF_HEADERS}
  )


