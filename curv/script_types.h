
#ifndef CURV_SCRIPT_TYPES_H
#define CURV_SCRIPT_TYPES_H

#include "common.h"
#include "aframe/af.h"


namespace curv {

  //
  //
  enum CommandType : unsigned char { COM_operation, COM_element, COM_uninitialized }; 

  //
  //
  struct script_command {

    script_command () : typ (CommandType::COM_uninitialized), bin()  { }
    
    CommandType   typ;
    af::bytearray bin; 

  }; 

  inline CommandType          Ty  (const script_command& sc) { return sc.typ; }
  inline const af::bytearray& mem (const script_command& sc) { return sc.bin; }
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
