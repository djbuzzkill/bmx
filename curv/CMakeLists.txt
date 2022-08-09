
CMAKE_MINIMUM_REQUIRED(VERSION  3.1)

project (curv)


set (CURV_SOURCES
  secp256k1.cpp
  transaction.cpp
  common.cpp
  )

set (CURV_HEADERS
  secp256k1.h
  transaction.h
  common.h
  )

add_library (curv
  ${CURV_SOURCES}
  ${CURV_HEADERS}
  )


