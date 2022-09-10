

#include "utility.h"


using namespace af; 
using namespace ffm;
//
// 
size_t bmx::util::read_varint (size_t& out, af::ReadStreamRef rs, const char* trace) {
  
  if (trace)
    printf ( "%s:%s()\n", trace, __FUNCTION__); 
  
  size_t readlen = 0; 
  unsigned char leaderbyte = 0;
  
  readlen += rs->Read (&leaderbyte,  1); 
  
  printf ("%s:leaderbyte:%i\n", __FUNCTION__, leaderbyte);
  
  switch (leaderbyte)  {
  case 253: readlen += rs->Read (&out, 2); break;
  case 254: readlen += rs->Read (&out, 4); break;
  case 255: readlen += rs->Read (&out, 8); break;
  default: out = leaderbyte;               break;
  }

  printf ("varint:%zu\n", out ); 
  return readlen; 
  
}
  
//  
//
size_t bmx::util::write_varint (af::WriteStreamRef ws, size_t v) {
    
  size_t writelen = 0; 
    
  if (v < 253) {
    writelen +=  ws->Write (&v, 1);
  }
  else if (v < 0x10000) { // one_with_four_zeros_following
    const unsigned char leader = 0xfd;
    writelen += ws->Write  (&leader, 1); 
    writelen += ws->Write (&v, 2); 
    // 2 bytes
  }
  else if (v < 0x100000000) { // one_with_eight_zeros_following
    const unsigned char leader = 0xfe; 
    // 4 bytes
    writelen += ws->Write (&leader, 1); 
    writelen += ws->Write (&v, 4); 
  }
  else {
    // must be big if u got this far
    assert (false); 
    
  }
  return writelen; 
  
}

//
//
bytearray& bmx::util::encode_num (bytearray& enc_out, FE_t num, FEConRef F) {
  
  //  printf ("[%s:decinum:%s]\n", __FUNCTION__, decinum.c_str()); 
  enc_out.clear ();
  // only valid strings
  
  const bool true_here = true; 
  int        sign_n = 0;
  bytearray  n_raw; 

  
  if (F->Cmp_ui (num, 0) == 0) {
    return enc_out; 
  }

  //
  F->Raw (n_raw, sign_n, num, true_here);  // false == BE?
  enc_out = n_raw; 

  if (std::to_integer<uint8>(enc_out.back()) & 0x80 ) {
    if (sign_n < 0) {
      enc_out.push_back (byte{0x80}); // printf ("enc_out.push_back(0x80)\n"); 
    }
    else {
      enc_out.push_back (byte{0x0}); // printf ("enc_out.push_back(0x0)\n"); 
    }
  }
  else if (sign_n < 0) {
    enc_out.back () |= byte{0x80}; // printf ("enc_out |= 0x80\n"); 
  }
  return enc_out; 
}


//
//
FE_t bmx::util::decode_num (FE_t num, FEConRef F, const af::bytearray& n_enc) {
  
  if (n_enc.empty ()) {
    F->Set_ui (num, 0); 
    return num;
  }
  
  af::bytearray n_BE = n_enc;
  
  // end_byte knows
  auto end_byte = n_BE.back ();
  
  if (std::to_integer<uint8>(end_byte) & 0x80) {
    n_BE.back () = end_byte & byte{0x7f};
  }
  
  const bool true_here_too = true; 
  F->Set_bin (num, &n_BE[0], n_BE.size(), true_here_too);
  
  if (std::to_integer<uint8>(end_byte) & 0x80) {
    F->SNeg (num, num); 
  }
  
  //std::vector<char> msg (256,'\0'); 
  //printf ("%s:dec_out:[%s]\n", __FUNCTION__, F->Format (&msg[0], "%Zd", num)); 
  return num; 
}

