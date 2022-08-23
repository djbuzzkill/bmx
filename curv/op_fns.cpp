
//
//
#include "op_fns.h"


//
//
int curv::proc_OP_0 (script_env& env) {

  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 

  printf ( "--> %s .\n" , __FUNCTION__);
  
  
  s.push (af::bytearray(1,0)); 
  return 0;
}

//
//
int curv::proc_OP_1 (script_env& env) {

  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 

  s.push (af::bytearray (1, 1)); 
  printf ( "--> %s .\n" , __FUNCTION__);

	  return 0;
  }

//
//
int curv::proc_OP_16 (script_env& env) {

  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 
  printf ( "--> %s .\n" , __FUNCTION__);
  return 0;

}

//
//
int curv::proc_OP_DUP (script_env& env) {

  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 

  printf ( "--> %s .\n" , __FUNCTION__);
  return 0;
}

//
//
int curv::proc_OP_ADD (script_env& env) {

  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 

  printf ( "--> %s .\n" , __FUNCTION__);
  return 0;
}

//
//
int curv::proc_OP_HASH160 (script_env& env) {

  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 

  printf ( "--> %s .\n" , __FUNCTION__);
  return 0;

}

//
//
int curv::proc_OP_CHECKSIG (script_env& env) {

  script_stack&       s    = env.stack; 
  script_stack&       alt  = env.alts;
  const command_list& cmds = env.cmds; 

  printf ( "--> %s .\n" , __FUNCTION__);

  return 0;

}
