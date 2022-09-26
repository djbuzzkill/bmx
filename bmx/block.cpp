

#include "block.h"
#include "secp256k1.h"

using namespace bmx;
using namespace ffm;
using namespace af;


constexpr uint32 kTWO_WEEKS = 60 * 60 * 24 * 14; 

//
//    
uint64 Block::Read(bmx::Block::Struc &oblk, af::ReadStreamRef rs) {

  uint64 readlen = 0;

  size_t sizeOf_prev_block = sizeof(oblk.prev_block);
  //printf ("    sizeOf_prev_block:%zu\n", sizeOf_prev_block);
  readlen += rs->Read (&oblk.version  , sizeof(oblk.version));
  readlen += rs->Read (&oblk.prev_block, 32);
  readlen += rs->Read (&oblk.merkleroot, 32); 
  readlen += rs->Read (&oblk.timestamp , sizeof(oblk.timestamp));
  readlen += rs->Read (&oblk.bits      , sizeof(oblk.bits));
  readlen += rs->Read (&oblk.nonce     , sizeof(oblk.nonce)); 

  std::reverse (oblk.prev_block.begin(), oblk.prev_block.end()); 
  std::reverse (oblk.merkleroot.begin(), oblk.merkleroot.end()); 
   
  return readlen; 
}

//
uint64 Block::Write(af::WriteStreamRef ws, const Struc &oblk) {

  uint64 writelen = 0;
  //platform_endian (
  bytearray rprev_block (oblk.prev_block.rbegin(), oblk.prev_block.rend ()); 
  bytearray rmerkleroot (oblk.merkleroot.rbegin(), oblk.merkleroot.rend ()); 

  writelen += ws->Write (&oblk.version  , sizeof(oblk.version));
  writelen += ws->Write (&rprev_block[0], 32);
  writelen += ws->Write (&rmerkleroot[0], 32); 
  writelen += ws->Write (&oblk.timestamp, sizeof(oblk.timestamp));
  writelen += ws->Write (&oblk.bits     , sizeof(oblk.bits));
  writelen += ws->Write (&oblk.nonce    , sizeof(oblk.nonce)); 

  return writelen; 

}

//
digest32& Block::Hash(bmx::digest32& out, const block& oblk) {

  bytearray mem (sizeof(bmx::block), byte(0));
  auto writelen = Block::Write (CreateWriteMemStream (&mem[0], sizeof(bmx::block)), oblk);
  af::hash256 (out, &mem[0], sizeof(Block::Struc));
  std::reverse (out.begin(), out.end()); 
  return out; 
  
}

//
bool bmx::Block::Cap::bip9 (const bmx::block& b) { 
  return (b.version >> 29) == 0b001; 
}

//
bool bmx::Block::Cap::bip91 (const bmx::block& b) {
  return (b.version >> 4) & 1;
}

//
bool bmx::Block::Cap::bip141 (const bmx::block& b) {

  return (b.version >> 1) & 1 ; 
    
}



bmx::fixnum32& bits_to_target (bmx::fixnum32& targ, bmx::uint32 bits) {
  FN_SCOPE ();

  using namespace ffm; 
  FEConRef F (nullptr);
  bmx::Init_FE_context (F);
  ScopeDeleter dr (F); 
  Formatter f(F);

  bmx::uint32 expn = (bits >> 24) & 0xff;
  
  FE_t fe_base = dr (F->New_ui (256)); 
  F->SPow_ui (fe_base,  fe_base, expn - 3); 
  //printf ("    ========= 256^(%u - 3) = %s \n", expn, f.dc (fe_base)); 
  FE_t fe_coeff = dr (F->New_ui ( bits & 0x00ffffff ));

  FE_t fe_res = dr (F->New());
  F->SMul(fe_res, fe_coeff, fe_base); 

  int res_sign = 0;
  bytearray outbytes;
  F->Raw (outbytes, res_sign, fe_res, false); 
  //  pr_a (outbytes, "outbytes >> "); 
  return copy_BE (targ, outbytes); 
}


//
uint32 target_to_bits (const bmx::fixnum32& targ) {

  uint32 bits = 0;
  
  byte zero {0}; 
  bytearray  bytes;

  copy_bytes (bytes, targ);

  auto count0 = 0;


  while (bytes[count0] == zero)
    count0++;

  WriteStreamRef ws = CreateWriteMemStream(&bits, sizeof(bits));
  int add0 = (std::to_integer<uint8>(bytes[count0]) > 0x7f) ? 1 : 0; 
  if (add0) {
    uint8 exponent = 32 - count0 + 1;
    ws->Write (&exponent, 1);
    // coefficient 
    ws->Write (&zero, 1);
    ws->Write (&bytes[count0], 2);
  }
  else{

    uint8 exponent = 32 - count0;
    ws->Write (&exponent, 1); 
    //int32 coefficient (0x00ffffff)
    ws->Write (&bytes[count0], 3);
  }      

  
  return bits; 
}

//
//
fixnum32& Block::Target (fixnum32& targ, const Struc &oblk) {

  return bits_to_target (targ, oblk.bits);
}

// 
// 
fixnum32& Block::Difficulty (fixnum32& odiff, const Struc& blk) {

  using namespace ffm; 
  FEConRef F (nullptr);
  Init_FE_context (F);
  ScopeDeleter dr (F); 
  //lowest = 0xffff * 256**(0x1d - 3)
  FE_t fe_lowest  = dr (F->New ()); 
  FE_t fe_base    = dr (F->New_ui(256)); 
  F->SPow_ui (fe_base, fe_base, 0x1d - 3);
  F->SMul_ui (fe_lowest, fe_base, 0xffff); 

  fixnum32 target; 
  Block::Target (target, blk);
  FE_t fe_target = dr (F->New_bin (&target, 32, false));

  bytearray resbin;
  int res_sign = 0; 
  FE_t fe_res = dr (F->New ());
  F->SDiv (fe_res, fe_lowest, fe_target); 
  F->Raw (resbin, res_sign, fe_res, false);
  // return lowest / target; 
  return copy_BE (odiff, resbin); 
}

//
//
uint32 Block::CalculateNewBits(uint32 prevbits, uint32 timediff) {
  FN_SCOPE ();
  
  if (timediff > kTWO_WEEKS) {
    PR ("> TWO WKS");
    timediff = kTWO_WEEKS * 4;
    }

  if (timediff < (kTWO_WEEKS / 4)) {
    PR ("< TWO WKS");
    timediff =  (kTWO_WEEKS / 4);
  }

  //printf ("    prevbits[0x%x|%u], timediff [%u]\n", prevbits, prevbits, timediff);

  fixnum32 newtarg;
  bits_to_target (newtarg, prevbits) ; 

  FEConRef F (nullptr);
  Init_FE_context (F);
  ScopeDeleter dr (F);
  Formatter    fmt(F);

  FE_t fe_newtarg = dr (F->New_bin (&newtarg, 32, false));
  //printf ("[0]    newtarg [%s]\n", fmt.hx(fe_newtarg) ); 


  F->SMul_ui (fe_newtarg, fe_newtarg, timediff);
  F->SFDiv_ui (fe_newtarg, fe_newtarg, kTWO_WEEKS);

  bytearray bin;
  int targ_sign = 0; 
  F->Raw (bin, targ_sign, fe_newtarg, false);

  //printf ("[1]    newtarg [%s]\n", fmt.hx (fe_newtarg)); 
  
  return target_to_bits (copy_BE (newtarg, bin)); 
}

//
//
bool Block::CheckPoW (const block& blk) { 
  FN_SCOPE (); 
  digest32 dig_hash; 
  Block::Hash (dig_hash, blk);

  fixnum32 target; 
  Block::Target (target, blk); 

  FEConRef F (nullptr);
  Init_FE_context (F);
  ScopeDeleter  dr (F); 
  //Formatter fmt (F);
  FE_t fe_hash = dr (F->New_bin (&dig_hash, 32, false));
  FE_t fe_targ = dr (F->New_bin (&target, 32, false));
  // printf ("    proof[%s]\n", fmt.hx(fe_hash)); 
  // printf ("    targ [%s]\n", fmt.hx(fe_targ)); 
  return (F->Cmp (fe_hash, fe_targ) < 0); 
}



#include "test_block.cpp"
