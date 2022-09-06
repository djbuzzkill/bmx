
#include "script.h"
#include "op_fns.h"
#include "script_types.h"


using namespace af;


using bmx::util::encode_num;
using bmx::util::decode_num;


  //
  //  
size_t bmx::WriteScript (af::WriteStreamRef ws, const command_list& out) {

  size_t writelen = 0;
  
  size_t begpos = ws->GetPos ();

  for (auto e : out) {

    switch (ty (e)) {
      //
    case command_type::SC_operation: {
      writelen += ws->Write (&arr(e)[0], 1);
    } break;

    case command_type::SC_element: {
      unsigned char len = arr(e).size (); 
      writelen += ws->Write (&len, 1);  
    } break;

    default:
      // fail
      break;
    }
       
  }

  
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


    // def serialize(self):
    //     # get the raw serialization (no prepended length)
    //     result = self.raw_serialize()
    //     # get the length of the whole thing
    //     total = len(result)
    //     # encode_varint the total length of the result and prepend
    //     return encode_varint(total) + result

  
  
  return writelen; 
}

