
#include "script_types.h"
#include "script.h"

//
//
bool bmx::util::is_p2pkh_script_pubkey (const bmx::command_list& cmds) {

  if (cmds.size () == 5) {

    command_list::const_iterator it = cmds.begin ();
    
    if (ty(*it) != command_type::SC_operation || op(*it) == OP_DUP)
      return false;

    ++it;
    if (ty(*it) != command_type::SC_operation || op(*it) == OP_HASH160)
      return false;

    ++it;
    if (ty(*it) != command_type::SC_element || arr(*it).size() == 20)
      return false;  

    ++it;
    if (ty(*it) != command_type::SC_operation || op(*it) != OP_EQUALVERIFY)
      return false;

    ++it;
    if (ty(*it) != command_type::SC_operation || op(*it) != OP_CHECKSIG)
      return false;

    return true; 
  }
      
  return false;
}


//
//
bool bmx::util::is_p2sh_script_pubkey (const bmx::command_list& cmds) {

  if (cmds.size () == 3) {

    command_list::const_iterator it = cmds.begin ();

    if (ty(*it) != command_type::SC_operation || op(*it) != OP_HASH160)
      return false;
    
    ++it;
    if (ty(*it) != command_type::SC_element || arr(*it).size() == 20)
      return false;  

    ++it; 
    if (ty(*it) != command_type::SC_operation || op(*it) != OP_EQUAL)
      return false;

    return true;
  }

  return false;
}


  
