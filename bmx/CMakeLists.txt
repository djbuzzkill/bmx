
CMAKE_MINIMUM_REQUIRED(VERSION  3.1)

project (curv)


set (CURV_SOURCES
  op_fns.cpp
  script_write.cpp
  script_read.cpp
  script_eval.cpp
  secp256k1.cpp
  transaction.cpp
  utility.cpp
  common.cpp
  )

set (CURV_HEADERS
  opcode.h
  op_fns.h
  script.h
  secp256k1.h
  transaction.h
  utility.h
  common.h
  )

add_library (curv
  ${CURV_SOURCES}
  ${CURV_HEADERS}
  )


