
#ifndef CURV_SCRIPT_TYPES_H
#define CURV_SCRIPT_TYPES_H

#include "common.h"
#include "aframe/af.h"

#include "opcode.h"


namespace bmx {

  //
  //
  enum command_type : unsigned char {
    SC_operation, SC_element, SC_uninitialized
  }; 

  //
  //
  struct script_command {

    script_command () : typ (command_type::SC_uninitialized), bin() {
    }
    
    command_type  typ;
    af::bytearray bin; 

  }; 

  // convenience
  inline command_type         Ty  (const script_command& sc) { return sc.typ; }
  inline const af::bytearray& arr (const script_command& sc) { return sc.bin; }
  inline OpCode               Op  (const script_command& sc) { return OpCode(sc.bin[0]); }
  //
  //
  typedef std::list<script_command> command_list; 


  //
  //
  typedef std::stack<af::bytearray> script_stack;

  //
  //
  struct script_env {
    command_list cmds;  // current command stack
    script_stack stack; // main stack  
    script_stack alts;  // alt stack 
  }; 

  //

}
#endif
