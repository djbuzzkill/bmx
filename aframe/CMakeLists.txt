
CMAKE_MINIMUM_REQUIRED(VERSION  3.1)

project (aframe)


set (AF_SOURCES
  common.cpp
  utility.cpp
  binary_IO.cpp
  connection.cpp
  hash.cpp
  task.cpp
  system.cpp
  af.cpp
)


set (AF_HEADERS
  common.h
  utility.h
  shared_types.h
  binary_IO.h
  connection.h
  hash.h
  task.h
  system.h
  af.h
  )


add_library (aframe
  ${AF_SOURCES}
  ${AF_HEADERS}
  )


