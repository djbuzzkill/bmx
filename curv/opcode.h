
#ifndef CURV_OPCODE_H
#define CURV_OPCODE_H

namespace curv {


  
  //
  enum OpCode : unsigned char {

    OP_0        = 0x0,
    OP_1        = 0x51,
    OP_16       = 0x60,
    OP_DUP      = 0x76,
    OP_ADD      = 0x93,
    OP_HASH160  = 0xa9,
    OP_CHECKSIG = 0xac,

  };  



}; 


#endif
