
#include "script_types.h"
#include "script.h"



using namespace af;
using namespace ffm; 
using namespace bmx; 
//
//
bool script_ut::is_p2pkh_script_pubkey (const bmx::command_list& cmds) {

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
bool script_ut::is_p2sh_script_pubkey (const bmx::command_list& cmds) {

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


//  
command_list& script_ut::p2pkh_script(command_list& s, const digest20& addr) {

  bytearray hashbin;
  s = {
    sco(OP_DUP),                 //    = 118, // 0x76 
    sco(OP_HASH160),             //    = 169, // 0xa9 
    sco(std::move(to_bytes(hashbin, addr))),  // hash160
    sco(OP_EQUALVERIFY),         //    = 136, // 0x88 
    sco(OP_CHECKSIG)             //    = 172, // 0xac 
  };
  
  return s;
}


command_list &script_ut::p2sh_script (command_list &s, const digest20 &addr) {

  bytearray hashbin;

  s = {
    sco (OP_HASH160),             // 169, // 0xa9 
    sco (std::move(to_bytes(hashbin, addr))),  // hash160
    sco (OP_EQUAL),               // 135, // 0x87 
  };
  
  return s; 
}
