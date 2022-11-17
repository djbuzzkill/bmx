
CMAKE_MINIMUM_REQUIRED(VERSION  3.1)

project (bmx)

set (BMX_SOURCES

  common.cpp
  utility.cpp
  secp256k1.cpp
  transaction.cpp
  op_fns.cpp
  script.cpp
  script_util.cpp
  script_write.cpp
  script_read.cpp
  script_eval.cpp
  script_types.cpp
  block.cpp
  network.cpp
  network_message.cpp
  network_envelope.cpp
  network_node.cpp
  merkle_tree.cpp
  merkle_block.cpp
  bloom_filter.cpp
  )

set (BMX_HEADERS
  common.h
  script_types.h
  utility.h
  secp256k1.h
  transaction.h
  opcode.h
  op_fns.h
  script.h
  block.h
  network.h
  merkle_block.h
  bloom_filter.h
  )

add_library (bmx
  ${BMX_SOURCES}
  ${BMX_HEADERS}
  )


