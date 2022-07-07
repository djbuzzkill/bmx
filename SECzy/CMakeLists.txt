
CMAKE_MINIMUM_REQUIRED(VERSION  3.0)

project (secp256k1)


set (SECP256K1_SOURCES
  secp256k1.cpp
  common.cpp
  )

set (SECP256K1_HEADERS
  secp256k1.h
  common.h
  )

add_library (secp256k1
  ${SECP256K1_SOURCES}
  ${SECP256K1_HEADERS}
  )


