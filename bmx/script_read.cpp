
#include "script.h"
#include "op_fns.h"
#include "script_types.h"


using namespace af;

namespace privread {

  using namespace bmx; 

  //
  script_command &script_operation (script_command &obj,
				    unsigned char op_uc) {
    //
    obj.typ = command_type::SC_operation;
    obj.bin.resize(1);
    obj.bin[0] = op_uc;
    return obj;
  }

  //
  //
  script_command& read_script_element (script_command& cmd,
				       ReadStreamRef   rs,
				       unsigned char   len,
				       size_t&         acc) {
    //
    if ( len > 75) {
      cmd.typ = command_type::SC_uninitialized; 
      return cmd;
    }

    cmd.typ = command_type::SC_element;
    cmd.bin.resize (len); 
    acc += rs->Read (&cmd.bin[0], len); 
    return cmd;
  }

  //
  //
  script_command& read_OP_PUSH1 (script_command& cmd,
				 ReadStreamRef   rs,
				 unsigned char   len,
				 size_t&         acc) {

    unsigned char lendata = 0; 
    acc += rs->Read (&lendata, lendata);
    
    printf ("%s:lendata[%u]\n", __FUNCTION__, lendata);

    cmd.bin.resize (lendata); 
    acc += rs->Read (&cmd.bin[0], lendata);

    cmd.typ = command_type::SC_operation;

    return cmd;
  }

  //
  //
  script_command& read_OP_PUSH2 (script_command& cmd,
				 ReadStreamRef   rs,
				 unsigned char   len,
				 size_t&         acc) {

    unsigned short lendat16 = 0;
    acc += rs->Read (&lendat16, sizeof(lendat16));

    printf ("%s:lendata[%i]\n", __FUNCTION__, lendat16);

    cmd.bin.resize (lendat16); 
    acc += rs->Read (&cmd.bin[0], lendat16); 
    
    cmd.typ = command_type::SC_operation;

    return cmd;
  }

} // privread




//
//
size_t bmx::ReadScript (command_list& out, ReadStreamRef rs) {

  using namespace privread; 
    
  out.clear (); 

  //
  // readlen : total lenghth of bytes read in this function
  size_t readlen = 0; 

  //
  // scriptlen : the claimed length of the following object
  size_t scriptlen = 0;
  readlen += util::read_varint (scriptlen, rs, 0); // __FUNCTION__);
  //  printf ( "%s:scriptlen[%zu]\n", __FUNCTION__, scriptlen);

  // accum : sum of all the bytes after initial varint
  // (accum == scriptlen)
  size_t accum = 0;
  //
  while (accum < scriptlen) {
    script_command obj;  
    
    unsigned char leader = 0;
    
    accum += rs->Read (&leader, 1); 
    
    if (leader > 0 && leader < 76) {
      out.push_back (read_script_element (obj, rs, leader, accum));
    }
    else if (leader == 76) {
      out.push_back (read_OP_PUSH1 (obj, rs, leader, accum));
    }
    else if (leader == 77) {
      out.push_back (read_OP_PUSH2 (obj, rs, leader, accum));
    }
    else {
      out.push_back (script_operation (leader)); 
      //
    }
    
  }
  
  readlen += accum; 
  
  if (accum != scriptlen) {
    // error
    printf ("%s(accum != scriptlen)\n", __FUNCTION__); 
  }
  
  
  return readlen; 
}

