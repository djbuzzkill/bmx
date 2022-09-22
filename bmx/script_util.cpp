
#include "script_types.h"
#include "script.h"


    // def is_p2pkh_script_pubkey(self):
    //     '''Returns whether this follows the
    //     OP_DUP OP_HASH160 <20 byte hash> OP_EQUALVERIFY OP_CHECKSIG pattern.'''
    //     return len(self.cmds) == 5 and self.cmds[0] == 0x76 \
    //         and self.cmds[1] == 0xa9 \
    //         and type(self.cmds[2]) == bytes and len(self.cmds[2]) == 20 \
    //         and self.cmds[3] == 0x88 and self.cmds[4] == 0xac

    // def is_p2sh_script_pubkey(self):
    //     '''Returns whether this follows the
    //     OP_HASH160 <20 byte hash> OP_EQUAL pattern.'''
    //     return len(self.cmds) == 3 and self.cmds[0] == 0xa9 \
    //         and type(self.cmds[1]) == bytes and len(self.cmds[1]) == 20 \
    //         and self.cmds[2] == 0x87

bool bmx::util::is_p2pkh_script_pubkey(const bmx::command_list &cmds) {

  return false;
}


bool bmx::util::is_p2sh_script_pubkey  (const bmx::command_list& cmds) {

  return false;
  }


  
