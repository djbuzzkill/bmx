
#ifndef CURV_SCRIPT_TYPES_H
#define CURV_SCRIPT_TYPES_H

#include "common.h"
#include "aframe/af.h"

#include "secp256k1.h"
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

  script_command script_element   (const af::bytearray& b);
  script_command script_operation (unsigned char op); 

  inline script_command screl (const af::bytearray& b) { return script_element (b); } 
  inline script_command scrop (unsigned char op) { return script_operation (op); }
    
     
  // convenience
  inline command_type         ty  (const script_command& sc) { return sc.typ; }
  inline const af::bytearray& arr (const script_command& sc) { return sc.bin; }
  inline OpCode               op  (const script_command& sc) { return OpCode(sc.bin[0]); }
  //
  //
  typedef std::list<script_command> command_list; 


  //
  //
  typedef std::vector<af::bytearray> script_stack;

  //
  //
  struct script_env {
    FFM_Env      ffme;
    command_list cmds;  // current command stack
    script_stack stack; // main stack  
    script_stack alts;  // alt stack 
    af::digest32 z;
  }; 

  //

}
#endif
