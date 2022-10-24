
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
  FN_SCOPE ();
  
  if (cmds.size () == 3) {

    command_list::const_iterator it = cmds.begin ();

    if (ty(*it) != command_type::SC_operation || op(*it) != OP_HASH160) 
      return false;
    
    ++it;
    if (ty(*it) != command_type::SC_element || arr(*it).size() != 20)
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


extern const std::string& OpName (OpCode o); 

//
//
std::string script_ut::format_script (const command_list& scrip, char delim) {

  std::string ret; 
  std::vector<char> buf (256, 0x0);

  std::string hexs;
  uint32 count = 0;
  for (auto& e : scrip) {
    std::string curs;

    switch (ty(e)) {

    case command_type::SC_element : {
      hex::encode(hexs, &arr(e)[0], arr(e).size()); 
      sprintf (&buf[count], "<el[%zu|0x%s]>", arr(e).size(), hexs.empty() ? "0" : hexs.c_str());  
      curs = &buf[0]; 
    } break;

    case command_type::SC_operation : {
      sprintf (&buf[0], "<%s(0x%02x)>", OpName (op(e)).c_str(), op(e)); 
      curs = &buf[0]; 
    } break;

    default: { } break;
    }
    curs += delim; 
    ret += curs; 
    
  }
  return ret;
}
