
CMAKE_MINIMUM_REQUIRED(VERSION  3.1)

project (bmx)


set (BMX_SOURCES
  op_fns.cpp
  script_util.cpp
  script_write.cpp
  script_read.cpp
  script_eval.cpp
  script_types.cpp
  block.cpp
  secp256k1.cpp
  transaction.cpp
  utility.cpp
  common.cpp
  )

set (BMX_HEADERS
  opcode.h
  op_fns.h
  script.h
  block.h
  script_types.h
  secp256k1.h
  transaction.h
  utility.h
  common.h
  )

add_library (bmx
  ${BMX_SOURCES}
  ${BMX_HEADERS}
  )


