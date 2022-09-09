
//
#include "script.h"
#include "op_fns.h"
#include "script_types.h"


using namespace af;

namespace privread {

  using namespace bmx; 

  //
  //
  script_command read_script_element (//script_command& cmd,
				       ReadStreamRef   rs,
				       unsigned char   len,
				       size_t&         acc) {

    script_command cmd; 
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
  script_command read_OP_PUSH1 (//script_command& cmd,
				 ReadStreamRef   rs,
				 unsigned char   len,
				 size_t&         acc) {
    script_command cmd; 
    
    unsigned char lendata = 0; 
    acc += rs->Read (&lendata, sizeof(lendata));
    
    printf ("%s:lendata[%u]\n", __FUNCTION__, lendata);

    cmd.bin.resize (lendata); 
    acc += rs->Read (&cmd.bin[0], lendata);

    cmd.typ = command_type::SC_operation;

    return cmd;
  }

  //
  //
  script_command read_OP_PUSH2 (//script_command& cmd,
				 ReadStreamRef   rs,
				 unsigned char   len,
				 size_t&         acc) {
    script_command cmd;
   
    std::uint16_t lendat16 = 0;
    acc += rs->Read (&lendat16, sizeof(lendat16));
#ifdef ARCH_BIG_ENDIAN
    //swap_endian (lendat16); 
#endif
    //printf ("%s:lendata[%u]\n", __FUNCTION__, lendat16);
    cmd.bin.resize (lendat16); 
    acc += rs->Read (&cmd.bin[0], lendat16); 
    
    cmd.typ = command_type::SC_operation;

    return cmd;
  }

} // privread




//
//
size_t bmx::ReadScript (command_list& out, ReadStreamRef rs) {
  // FN_SCOPE ("dsa"); 
  using namespace privread; 
  out.clear (); 
  //
  // readlen : total lenghth of bytes read in this function
  size_t readlen = 0; 
  // scriptlen : the claimed length of the following object
  size_t scriptlen = 0;
  readlen += util::read_varint (scriptlen, rs, 0);
  // printf ( "%s:size(varint) [%zu]\n", __FUNCTION__, readlen );
  // printf ( "%s:scriptlen [%zu]\n", __FUNCTION__, scriptlen );
  size_t accum = 0;
  while (accum < scriptlen) {

    unsigned char leader = 0;
    accum += rs->Read (&leader, 1); 
    
    if (leader > 0 && leader < 76) {
      //printf ( " -> element(%u) \n", leader); 
      out.push_back (read_script_element (rs, leader, accum));
    }
    else if (leader == 76) {
      //printf ( " -> read_OP_PUSH1 \n"); 
      out.push_back (read_OP_PUSH1 (rs, leader, accum));
    }
    else if (leader == 77) {
      //printf ( " -> read_OP_PUSH2 \n"); 
      out.push_back (read_OP_PUSH2 (rs, leader, accum));
    }
    else {
      //printf (" -> OP(%x) \n", leader); 
      out.push_back (script_command (leader)); 
      //
    }
  }
  
  readlen += accum; 
  
  if (accum != scriptlen) {
    // error
    printf ("%s(accum(%zu) != scriptlen(%zu))\n", __FUNCTION__, accum, scriptlen); 
  }
  
  return readlen; 
}

