
#include "script.h"
#include "op_fns.h"
#include "script_types.h"


using namespace af;


  //
  //  
size_t bmx::WriteScript (af::WriteStreamRef ws, const command_list& out) {

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

