
//
//
#ifndef CURV_SCRIPT_H
#define CURV_SCRIPT_H


#include "common.h"
#include "aframe/af.h"


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


  //
  //
  struct script_commamd {
    enum Type { CE_operation, CE_element }; 

    
    Type          typ;
    af::bytearray bin; 

  }; 
  
  typedef std::list<script_command> command_list; 

  size_t ReadSript (command_list& out, af::ReadStreamRef rs);



  int EvalScript (const CommandList& cmdlist);

  
}


#endif // CURV_SCRIPT_H
