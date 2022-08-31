
//
//
#ifndef BMX_SCRIPT_H
#define BMX_SCRIPT_H


#include "common.h"
#include "utility.h"
#include "opcode.h"
#include "script_types.h"
#include "aframe/af.h"

namespace bmx {


  //
  // aka parse
  size_t ReadScript  (command_list& out, af::ReadStreamRef rs);
  size_t WriteScript (af::WriteStreamRef ws, const command_list& out); 
  bool   EvalScript  (script_env& env);
  //bool EvalScript (const command_list& commands, const af::digest32& z);
  
}


#endif // CURV_SCRIPT_H
