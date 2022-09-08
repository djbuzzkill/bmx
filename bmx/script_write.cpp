
#include "script.h"
#include "op_fns.h"
#include "script_types.h"


using namespace af;


using bmx::util::encode_num;
using bmx::util::decode_num;


  //
  //  
size_t bmx::WriteScript (af::WriteStreamRef ws, const command_list& wout) {
  printf (">%s:ENTER \n", __FUNCTION__); 

  const size_t size_workbuf = 1024 * 10; 
  af::bytearray workmem (size_workbuf, 0); 
  // write to inner work stream 
  af::WriteStreamRef  iw = CreateWriteMemStream (&workmem[0], size_workbuf); 
  size_t              innerlen = 0; 

  //
  for (auto e : wout) {
    const  bytearray& bytes  = arr(e); 
    size_t            arrlen = bytes.size (); 
    //
    switch (ty (e)) {
      
    case command_type::SC_operation: 
      printf   ("SC_operation (%2x) \n", bytes[0]);       
      innerlen += iw->Write (&bytes[0], 1);
      break;
      
    case command_type::SC_element:
      printf   ("SC_element");       
      if (arrlen < 75) {
	printf   (" (arrlen < 75) \n");       

	unsigned char uclen = arrlen; 
	innerlen += iw->Write (&uclen, 1);
      }
      else if (arrlen > 75 && arrlen < 256) {
	const unsigned char uc76 = 76; 
	std::uint8_t  u8len = arrlen;  
	printf   (" (arrlen > 75 && arrlen < 256) \n");       
	innerlen += iw->Write (&uc76, 1); 
	innerlen += iw->Write (&u8len, 1);
      }
      else if (arrlen > 255 && arrlen < 521) {
	const unsigned char uc77   = 77; 
	std::uint16_t       u16len = arrlen;  
#ifdef ARCH_BIG_ENDIAN
	// swap_endian (u16len); 
#endif
	printf   (" (arrlen > 255 && arrlen < 521) \n");       
	innerlen += iw->Write (&uc77, 1); 
	innerlen += iw->Write (&u16len, sizeof(u16len));
      }
      else {
	printf (" how did you do this \n");       
	// raise ValueError('too long an cmd')
	return 0; 
      }

      //
      innerlen += iw->Write (&bytes[0], arrlen);
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

  writelen += ws->Write (&workmem[0], innerlen); 


  printf ("<%s:EXIT \n", __FUNCTION__); 

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

  
  

