

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
  FN_SCOPE (); 

  using namespace ffm; 
  FEConRef F (nullptr);
  Init_FE_context (F);
  ScopeDeleter dr (F); 
  ScopeDeleter fmt (F); 

  FE_t fe_lowest  = dr (F->New ()); 
  FE_t fe_base    = dr (F->New_ui(256)); 
  F->SPow_ui (fe_base, fe_base, 0x1d - 3);
  F->SMul_ui (fe_lowest, fe_base, 0xffff); 

  fixnum32 target; 
  Block::Target (target, blk);
  FE_t fe_target = dr (F->New_bin (&target, 32, false));
  FE_t fe_res = dr (F->New ()); 
  F->SDiv (fe_res, fe_lowest, fe_target); 

  bytearray resbin;
  int res_sign = 0; 
  F->Raw (resbin, res_sign, fe_res, false); 
  return copy_BE (odiff, resbin); 
}




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

  printf ("    prevbits[0x%x|%u], timediff [%u]\n", prevbits, prevbits, timediff);
  
  
  fixnum32 newtarg; 
  bits_to_target (newtarg, prevbits) ; 

  FEConRef F (nullptr);
  Init_FE_context (F);
  ScopeDeleter dr (F);
  Formatter    fmt(F);

  FE_t fe_newtarg = dr (F->New_bin (&newtarg, 32, false));
    F->SMul_ui (fe_newtarg, fe_newtarg, timediff);

  bytearray bin;
  int targ_sign = 0; 
  F->Raw (bin, targ_sign, fe_newtarg, false);

  return target_to_bits (copy_BE (newtarg, bin)); 
}


// --------------------------------------------------------------------
// TESTS
// --------------------------------------------------------------------


int test_read_block(std::vector<std::string> &args) {
  FN_SCOPE (); 

  
  bytearray blockbin; 
  hex::decode (blockbin, "020000208ec39428b17323fa0ddec8e887b4a7c53b8c0a0a220cfd0000000000000000005b0750fce0a889502d40508d39576821155e9c9e3f5c3157f961db38fd8b25be1e77a759e93c0118a4ffd71d");
  ReadStreamRef rs = CreateReadMemStream (&blockbin[0], blockbin.size ());

  block blk; 
  Block::Read (blk , rs);

  PR_CHECK ("   version == 0x20000002", blk.version == 0x20000002 );


  std::string want_prev_block_hex = "000000000000000000fd0c220a0a8c3bc5a7b487e8c8de0dfa2373b12894c38e"; 
  //self.assertEqual(block.prev_block, want)


  std::string want_merkle_root_hex = "be258bfd38db61f957315c3f9e9c5e15216857398d50402d5089a8e0fc50075b"; 
  //self.assertEqual(block.merkle_root, want)

  std::string timestamp_hex; 
  uint32 rtimestamp = blk.timestamp;
  swap_endian (&rtimestamp);
  
  //PR_CHECK ("   timestamp == 0x59a7771e ",  hex::encode(timestamp_hex, &rtimestamp, 4) == std::string ("59a7771e"));
  PR_CHECK ("   timestamp == 0x59a7771e ", blk.timestamp ==  0x59a7771e ); 
  printf    ("   timestamp_hex:%s\n" , timestamp_hex.c_str ()); 
  //  self.assertEqual(block.timestamp, 0x59a7771e)

  std::string bits_hex;
  uint32 rbits = blk.bits;
  swap_endian<uint32>(&rbits); 
  //PR_CHECK ("   bits == e93c0118 ",  hex::encode (bits_hex, &rbits, 4) == std::string("e93c0118")); 
  PR_CHECK ("   bits == e93c0118 ",  rbits == 0xe93c0118); 
  printf    ("   bits_hex:%s\n" , bits_hex.c_str ()); 
  // self.assertEqual(block.bits, bytes.fromhex('e93c0118'))


  std::string nonce_hex;
  uint32 rnonce = blk.nonce;
  swap_endian<uint32>(&rnonce);    
  //PR_CHECK ("  nonce == a4ffd71d ", hex::encode(nonce_hex, &rnonce, 4) == std::string ( "a4ffd71d"))
  PR_CHECK ("  nonce == a4ffd71d ", rnonce == 0xa4ffd71d); 
  printf    ("  nonce_hex:%s\n" , nonce_hex.c_str ()); 
  //self.assertEqual(block.nonce, bytes.fromhex('a4ffd71d'))


  return 0;
}



int test_write_block(std::vector<std::string> &args) {

  std::string block_raw_hex =
    "020000208ec39428b17323fa0ddec8e887b4a7c53b8c0a0a220cfd0000000000000000005b0750fce0a889502d40508d39576821155e9c9e3f5c3157f961db38fd8b25be1e77a759e93c0118a4ffd71d";

  bytearray inbin; 
  hex::decode (inbin, block_raw_hex);

  block blk ;
  Block::Read (blk, CreateReadMemStream (&inbin[0], inbin.size()));

  bytearray outbin (inbin.size(), byte(0)); 
  Block::Write (CreateWriteMemStream (&outbin[0], outbin.size()), blk);

  std::string  outhex;
  hex::encode (outhex, &outbin[0], outbin.size());
  //printf ("    ihex [%s]\n    ohex [%s]\n", block_raw_hex.c_str (),   outhex.c_str()); 
  PR_CHECK ("(eql outbin inbin)", eql (outbin, inbin)); 
  return 0;

  }

int test_bip9 (std::vector<std::string> &args) {

  std::string ahex = "020000208ec39428b17323fa0ddec8e887b4a7c53b8c0a0a220cfd0000000000000000005b0750fce0a889502d40508d39576821155e9c9e3f5c3157f961db38fd8b25be1e77a759e93c0118a4ffd71d"; 

  bytearray abin;
  hex::decode (abin, ahex);

  block ablock; 
  Block::Read (ablock, CreateReadMemStream (&abin[0], abin.size()));

  PR_CHECK ("Block::Cap::bip9 (ablock)", Block::Cap::bip9 (ablock) ); 
  return 0;
}






int test_bip91 (std::vector<std::string> &args) {
  
  std::string ahex = "1200002028856ec5bca29cf76980d368b0a163a0bb81fc192951270100000000000000003288f32a2831833c31a25401c52093eb545d28157e200a64b21b3ae8f21c507401877b5935470118144dbfd1";
  bytearray abin;
  hex::decode (abin, ahex);

  block ablk; 
  uint64 read_alen =  Block::Read (ablk, CreateReadMemStream (&abin[0], abin.size()));
  PR_CHECK ("Block::Cap::bip91 (ablk)", Block::Cap::bip91 (ablk)); 
  

  std::string bhex = "020000208ec39428b17323fa0ddec8e887b4a7c53b8c0a0a220cfd0000000000000000005b0750fce0a889502d40508d39576821155e9c9e3f5c3157f961db38fd8b25be1e77a759e93c0118a4ffd71d";
  bytearray bbin; 
  hex::decode (bbin, bhex);

  block bblk;
  uint64 read_blen = Block::Read (bblk, CreateReadMemStream (&bbin[0], bbin.size()));

  PR_CHECK ("!Block::Cap::bin91(bblk) ", !Block::Cap::bip91 (bblk));
  return 0;
  
}


 

int test_bip141 (std::vector<std::string> &args) {

  {
    std::string ahex = "020000208ec39428b17323fa0ddec8e887b4a7c53b8c0a0a220cfd0000000000000000005b0750fce0a889502d40508d39576821155e9c9e3f5c3157f961db38fd8b25be1e77a759e93c0118a4ffd71d";

    bytearray abin;
    hex::decode (abin, ahex); 

    block ablk;
    uint64 read_alen = Block::Read (ablk, CreateReadMemStream (&abin[0], abin.size())); 
    PR_CHECK ("Block::Cap::bip141 (ablk)", Block::Cap::bip141 (ablk));
  }

  {
    std::string bhex = "0000002066f09203c1cf5ef1531f24ed21b1915ae9abeb691f0d2e0100000000000000003de0976428ce56125351bae62c5b8b8c79d8297c702ea05d60feabb4ed188b59c36fa759e93c0118b74b2618"; 
    bytearray bbin;
    hex::decode (bbin, bhex); 

    block bblk;
    uint64 read_alen = Block::Read (bblk, CreateReadMemStream (&bbin[0], bbin.size())); 
    PR_CHECK ("!Block::Cap::bip141 (bblk)", !Block::Cap::bip141 (bblk)); 
  }  

  return 0;
}

int test_hash_block (std::vector<std::string> &args) {

  const std::string testhex = "020000208ec39428b17323fa0ddec8e887b4a7c53b8c0a0a220cfd0000000000000000005b0750fce0a889502d40508d39576821155e9c9e3f5c3157f961db38fd8b25be1e77a759e93c0118a4ffd71d"; 

  bytearray tbin ;
  hex::decode (tbin, testhex); 

  block tblk; 
  uint64 read_tlen = Block::Read (tblk, CreateReadMemStream (&tbin[0], tbin.size()));

  digest32 thash ; 
  Block::Hash (thash, tblk); 
    // def test_hash(self):
    //     block_raw = bytes.fromhex('020000208ec39428b17323fa0ddec8e887b4a7c53b8c0a0a220cfd0000000000000000005b0750fce0a889502d40508d39576821155e9c9e3f5c3157f961db38fd8b25be1e77a759e93c0118a4ffd71d')
    //     stream = BytesIO(block_raw)
    //     block = Block.parse(stream)

  std::string hexhash; 
  hex::encode (hexhash, &thash[0], 32);


  const std::string wanthex = "0000000000000000007e9e4c586439b0cdbe13b1370bdd9435d76a644d047523"; 

  printf ("test [%s]\nwant [%s]\n", hexhash.c_str (), wanthex.c_str ());


  PR_CHECK ("(hexhash == wanthex)",  hexhash == wanthex ); 
  
  return 0;
}


int test_block_difficulty (std::vector<std::string> &args) {

  std::string blockhex = "020000208ec39428b17323fa0ddec8e887b4a7c53b8c0a0a220cfd0000000000000000005b0750fce0a889502d40508d39576821155e9c9e3f5c3157f961db38fd8b25be1e77a759e93c0118a4ffd71d";
  bytearray bin;
  hex::decode (bin, blockhex); 

  block blk; 
  Block::Read (blk,  CreateReadMemStream (&bin[0], bin.size ())); 

  fixnum32 diff; 
  Block::Difficulty (diff, blk);

  std::string diffhex; 
  hex::encode (diffhex, &diff[0], 32); 
  
  printf ("   difficulty[]\n", diffhex.c_str ());
  
    // def test_difficulty(self):
    //     block_raw = bytes.fromhex('020000208ec39428b17323fa0ddec8e887b4a7c53b8c0a0a220cfd0000000000000000005b0750fce0a889502d40508d39576821155e9c9e3f5c3157f961db38fd8b25be1e77a759e93c0118a4ffd71d')
    //     stream = BytesIO(block_raw)
    //     block = Block.parse(stream)
    //     self.assertEqual(int(block.difficulty()), 888171856257)

  return 0;
}


int test_block_target (std::vector<std::string> &args) {
  FN_SCOPE ();
  
  std::string thex = "020000208ec39428b17323fa0ddec8e887b4a7c53b8c0a0a220cfd0000000000000000005b0750fce0a889502d40508d39576821155e9c9e3f5c3157f961db38fd8b25be1e77a759e93c0118a4ffd71d"; 
  bytearray   bin;
  hex::decode (bin, thex);

  block blk;
  Block::Read (blk, CreateReadMemStream (&bin[0], bin.size ())); 
  //  PR ("b4 Target ()");
  fixnum32 tgt; 
  Block::Target (tgt, blk);

  fixnum32 diff; 
  Block::Difficulty (diff, blk);

  FEConRef F;
  Init_FE_context (F);
  ScopeDeleter dr (F);
  ffm::Formatter f(F);
  
  const std::string want_targ_hex = "0x13ce9000000000000000000000000000000000000000000";
  FE_t fe_wanttarg = dr (F->New (want_targ_hex.c_str(), 0));
  FE_t fe_testtarg = dr (F->New_bin (&tgt, 32, false));

  printf ("    want_targ(hex) [%s]\n"   , f.hx(fe_wanttarg)); 
  printf ("    test_targ(hex) [%s]\n"   , f.hx(fe_testtarg));

  //  self.assertEqual(block.target(), 0x13ce9000000000000000000000000000000000000000000)
  PR_CHECK ( "want targ == test targ", 0 == F->Cmp (fe_testtarg, fe_wanttarg))


  const std::string want_diff_dec = "888171856257"; 
  FE_t fe_wantdiff = dr (F->New (want_diff_dec.c_str(), 10));
  FE_t fe_testdiff = dr (F->New_bin (&diff, 32, false)); 

  printf ("    want diff(dec) [%s]\n", f.dc(fe_wantdiff)); 
  printf ("    test diff(dec) [%s]\n", f.dc(fe_testdiff)); 
  PR_CHECK ( "want difficulty  == test difficulty", 0 == F->Cmp (fe_wantdiff, fe_testdiff))


  // block_raw = bytes.fromhex('020000208ec39428b17323fa0ddec8e887b4a7c53b8c0a0a220cfd0000000000000000005b0750fce0a889502d40508d39576821155e9c9e3f5c3157f961db38fd8b25be1e77a759e93c0118a4ffd71d')
  //  stream = BytesIO(block_raw)
  //  block = Block.parse(stream)
  //  self.assertEqual(block.target(), 0x13ce9000000000000000000000000000000000000000000)
  //  self.assertEqual(int(block.difficulty()), 888171856257)

  return 0;
 
  }



int test_check_pow (std::vector<std::string> &args) { return 0; }



int test_calc_new_bits  (std::vector<std::string> &args) { 
  FN_SCOPE ();

  const uint32  prev_bits         = 0x54d80118; 
  const uint32  time_differential = 302400; 

  // const uint32  reg_bits         = 0x54d80118; 
  // const uint32  rev_bits         = 0x1801d854;
  // printf ("\n    regbits[%u]\n    revbits[%u]\n", reg_bits, rev_bits); 

  uint32 new_bits  = Block::CalculateNewBits (prev_bits, time_differential);


  const uint32 want_bits = 0x00157617; 

  uint32 rwant = want_bits;
  uint32 rnew  = new_bits;



  swap_endian<uint32>(&rwant);
  swap_endian<uint32>(&rnew);


  std::string newhx, wanthx; 
  hex::encode (wanthx, &rwant, 4);
  hex::encode (newhx , &rnew , 4);

  
  printf ("\n    newbits(hex) [%s]\n    wantbits(hex)[%s]\n", newhx.c_str(), wanthx.c_str());   
  
  PR_CHECK ("(new_bits == want_bits)", new_bits == want_bits); 
  return 0;
}



    // def test_calculate_new_bits(self):
    //     prev_bits = bytes.fromhex('54d80118')
    //     time_differential = 302400
    //     want = bytes.fromhex('00157617')
    //     self.assertEqual(calculate_new_bits(prev_bits, time_differential), want)
