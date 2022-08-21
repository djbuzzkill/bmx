
#include "script.h"
#include "op_fns.h"


using namespace af;

namespace curv {
  
  int encode_num (size_t num) {
    return 0; 
    }
  

  int decode_num () {

// def decode_num(element):
//     if element == b'':
//         return 0
//     # reverse for big endian
//     big_endian = element[::-1]
//     # top bit being 1 means it's negative
//     if big_endian[0] & 0x80:
//         negative = True
//         result = big_endian[0] & 0x7f
//     else:
//         negative = False
//         result = big_endian[0]
//     for c in big_endian[1:]:
//         result <<= 8
//         result += c
//     if negative:
//         return -result
//     else:
//         return result

    return 0;

  }

  //
  script_command &script_operation(script_command &obj,
				   unsigned char op_uc) {
    //
    obj.typ = CommandType::COM_operation;
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
      cmd.typ = CommandType::COM_uninitialized; 
      return cmd;
    }

    cmd.typ = CommandType::COM_element;
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

    cmd.typ = CommandType::COM_operation;

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
    
    cmd.typ = CommandType::COM_operation;

    return cmd;
  }



  
} // privates




//
//
size_t curv::ReadScript (command_list& out, ReadStreamRef rs) {

  out.clear (); 

  //
  // readlen : total lenghth of bytes read in this function
  size_t readlen = 0; 

  //
  // scriptlen : the claimed length of the following object
  size_t scriptlen = 0;
  readlen += util::read_varint (scriptlen, rs);
  printf ( "%s:scriptlen[%zu]\n", __FUNCTION__, scriptlen);

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
      out.push_back (script_operation (obj, leader)); 
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

  //
  //  
size_t curv::Writecript (af::WriteStreamRef ws, const command_list& out) {

  size_t writelen = 0;

    // def raw_serialize(self):
    //     result = b''
    //     for cmd in self.cmds:
    //         if type(cmd) == int:  # <1>
    //             result += int_to_little_endian(cmd, 1)
    //         else:
    //             length = len(cmd)
    //             if length < 75:  # <2>
    //                 result += int_to_little_endian(length, 1)
    //             elif length > 75 and length < 0x100:  # <3>
    //                 result += int_to_little_endian(76, 1)
    //                 result += int_to_little_endian(length, 1)
    //             elif length >= 0x100 and length <= 520:  # <4>
    //                 result += int_to_little_endian(77, 1)
    //                 result += int_to_little_endian(length, 2)
    //             else:  # <5>
    //                 raise ValueError('too long an cmd')
    //             result += cmd
    //     return result

  return writelen; 
}

//

typedef std::function<int(curv::script_env&)> op_proc;
typedef std::map<curv::OpCode, op_proc> OpFnMap;


int curv::EvalScript (const command_list& commands) {
	 

  OpFnMap op_map = {
    { OP_0        , proc_OP_0 }, 
    { OP_1        , proc_OP_1 },    
    { OP_16       , proc_OP_16 },
    { OP_DUP      , proc_OP_DUP } ,  
    { OP_ADD      , proc_OP_ADD },  
    { OP_HASH160  , proc_OP_HASH160 }, 
    { OP_CHECKSIG , proc_OP_CHECKSIG } , 

  }; 
  //
  //
  script_env env;
  env.cmds = commands;
  
  command_list cmds = commands; 
  
  while  (cmds.size ()) {
    script_command cmd = cmds.front ();
    cmds.pop_front ();

    switch (Ty (cmd)) {
    // COM_operation 
    case CommandType::COM_operation:

      assert (op_map.count (Op(cmd)));

      if (op_map.count (Op(cmd))) {
	int op_res = op_map[Op(cmd)] (env);
      }
      else {
	// op not found 
      }
      break;
      
    // COM_element:
    case CommandType::COM_element:
      env.stack.push (mem(cmd));

      break;
  
    // COM_uninitialized
    default:
      // wtf 
      return __LINE__;
      break;
    }

    
    if (env.stack.empty ()) {
      return __LINE__;
      // fail
    }
    if (env.stack.top().size () == 0) {
      return __LINE__; 
      // empty string
    }
    
  } // for 

 return 0; 
}

  

  
