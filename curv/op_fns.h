
//
//
#ifndef CURV_OP_FNS_H
#define CURV_OP_FNS_H

#include "common.h"
#include "opcode.h"
#include "script_types.h"
#include "aframe/af.h"

namespace curv {

  int proc_OP_0        (script_env& env);
  int proc_OP_1        (script_env& env);
  int proc_OP_16       (script_env& env);
  int proc_OP_DUP      (script_env& env);
  int proc_OP_ADD      (script_env& env);
  int proc_OP_HASH160  (script_env& env);
  int proc_OP_CHECKSIG (script_env& env);
  
  
}

#endif
