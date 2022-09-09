
#include "script.h"
#include "op_fns.h"
#include "script_types.h"


using namespace af;


using bmx::util::encode_num;
using bmx::util::decode_num;


size_t compute_script_size (const bmx::command_list& scrip) {

  using namespace bmx;
  
  size_t expected_script_size = 0; 

  for (const auto& e : scrip) {

    const  bytearray& bytes  = arr(e); 
    size_t            arrlen = bytes.size (); 
    //
    switch (ty (e)) {
      
    case command_type::SC_operation: 
      //expected_script_size += 1;
      if (arrlen == 1) {
	expected_script_size++; 
      }
      
      if(arrlen > 75 && arrlen < 256) {
	expected_script_size += 2;
	expected_script_size += arrlen;
      }
      else if (arrlen > 255 && arrlen < 521) {
	expected_script_size += 3; 
	expected_script_size += arrlen;
      }
      break;
      
    case command_type::SC_element:

      if (arrlen < 76) {
	expected_script_size += 1; 
	expected_script_size += arrlen;
      }
      
      break;
    }
  }

  return expected_script_size;
}


//
//  
size_t bmx::WriteScript (af::WriteStreamRef ws, const command_list& scrip) {
  //FN_SCOPE ("a2"); 

  size_t expected_size = compute_script_size (scrip); 
  // printf ("expected inner size:%zu\n", expected_size); 
  
  const size_t size_workbuf = 1024 * 10; 
  af::bytearray workmem (size_workbuf, 0); 
  // write to inner work stream 
  af::WriteStreamRef  iw = CreateWriteMemStream (&workmem[0], size_workbuf); 
  size_t              innerlen = 0; 

  //
  for (auto e : scrip) {
    const  bytearray& bytes  = arr(e); 
    size_t            arrlen = bytes.size (); 
    //
    switch (ty (e)) {
      
    case command_type::SC_operation: 
      if (arrlen == 1) { 
	// printf   ("SC_operation (%2x) \n", bytes[0]);       
	innerlen += iw->Write (&bytes[0], 1);
      }
      else if (arrlen > 75 && arrlen < 256) {
	const unsigned char uc76 = 76; 
	std::uint8_t  u8len = arrlen;  
	// printf   (" (arrlen > 75 && arrlen < 256) \n");       
	innerlen += iw->Write (&uc76, 1); 
	innerlen += iw->Write (&u8len, 1);
	//
	innerlen += iw->Write (&bytes[0], arrlen);
      }
      else if (arrlen > 255 && arrlen < 521) {
	const unsigned char uc77   = 77; 
	std::uint16_t       u16len = arrlen;  
#ifdef ARCH_BIG_ENDIAN
	// swap_endian (u16len); 
#endif
	// printf   (" (arrlen > 255 && arrlen < 521) \n");       
	innerlen += iw->Write (&uc77, 1); 
	innerlen += iw->Write (&u16len, sizeof(u16len));
	//
	innerlen += iw->Write (&bytes[0], arrlen);
      }
      break;
      
    case command_type::SC_element:
      // printf   ("SC_element (%zu)\n", arrlen);       
      if (arrlen < 76) {
	//printf   (" (arrlen < 76) \n");       
	unsigned char uclen = arrlen; 
	innerlen += iw->Write (&uclen, 1);
	//
	innerlen += iw->Write (&bytes[0], arrlen);
      }
      else {
	// printf (" how did you do this \n");       
	// raise ValueError('too long an cmd')
	return 0; 
      }

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


  size_t varint_size = util::write_varint (ws, innerlen);

  writelen += varint_size; 

  writelen += ws->Write (&workmem[0], innerlen); 
  // printf ("varint_size:%zu\n", varint_size ); 
  // printf ("innerlen:%zu\n"   , innerlen );

  if  (expected_size != innerlen)  {
    printf ("varint_size:%zu\n", varint_size ); 
    printf ("innerlen:%zu\n"   , innerlen );

  }
 
  return writelen;
}

  
