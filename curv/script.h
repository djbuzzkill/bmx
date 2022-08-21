
//
//
#ifndef CURV_SCRIPT_H
#define CURV_SCRIPT_H


#include "common.h"
#include "utility.h"
#include "opcode.h"
#include "script_types.h"
#include "aframe/af.h"

namespace curv {


  //
  // aka parse
  size_t ReadScript (command_list& out, af::ReadStreamRef rs);

  //
  // aka serialize
  size_t Writecript (af::WriteStreamRef rs, const command_list& out); 

  //
  int EvalScript (const command_list& commands);

  
}


#endif // CURV_SCRIPT_H
