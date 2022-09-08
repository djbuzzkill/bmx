
#include "script.h"
#include "op_fns.h"
#include "script_types.h"


using namespace af;


using bmx::util::encode_num;
using bmx::util::decode_num;


  //
  //  
size_t bmx::WriteScript (af::WriteStreamRef ws, const command_list& wout) {

  const size_t size_workbuf = 1024 * 10; 
  af::bytearray workmem (size_workbuf, 0); 
  // write to inner work stream 
  af::WriteStreamRef  iw = CreateWriteMemStream (&workmem[0], size_workbuf); 
  size_t              innerlen = 0; 

  //
  for (auto e : wout) {
    size_t            arr_len = arr(e).size ();
    const  bytearray& bytes   = arr(e); 
    //
    switch (ty (e)) {
      
    case command_type::SC_operation: 
      innerlen += iw->Write (&bytes[0], 1);
      break;
      
    case command_type::SC_element:
      
      if (arr_len < 76) {
	unsigned char uclen = arr_len; 
	innerlen  += iw->Write (&uclen, 1);
      }
      else if (arr_len > 75 && arr_len < 256) {
	const unsigned char uc76 = 76; 
	std::uint16_t  u8len = arr_len;  
	innerlen += iw->Write (&uc76, 1); 
	innerlen += iw->Write (&u8len, 1);
      }
      else if (arr_len > 255 && arr_len < 521) {
	const unsigned char uc77   = 77; 
	std::uint16_t       u16len = arr_len;  
#ifdef ARCH_BIG_ENDIAN
	// swap_endian (u16len); 
#endif
	innerlen += iw->Write (&uc77, 1); 
	innerlen += iw->Write (&u16len, sizeof(std::uint16_t));
      }
      else {
	// raise ValueError('too long an cmd')
	return 0; 
      }
      
      innerlen += iw->Write (&bytes[0], arr_len);
      break;

    default: // switch command_type default
      // how
      // not command type at all
      return 0; 
      break;
    } // switch command_type:
  }

  assert (innerlen == iw->GetPos ());
  //
  size_t writelen = 0;

  writelen += util::write_varint (ws, innerlen);
  printf ("%s:writelen_b4:%zu\n", __FUNCTION__, writelen); 

  writelen += ws->Write (&workmem[0], innerlen); 


  printf ("%s:inner_len:%zu\n" , __FUNCTION__, innerlen); 
  printf ("%s:writelen:%zu\n", __FUNCTION__, writelen); 
  
  return writelen;
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

  
  

