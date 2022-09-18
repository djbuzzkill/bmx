
//
//
#include "secp256k1.h"
#include <bitset>


#include "aframe/hash.h"
#include "aframe/utility.h"

const char ksecp256k1_p_sz[]       = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f";
const char ksecp256k1_G_x_sz[]     = "0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798";
const char ksecp256k1_G_y_sz[]     = "0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8";
const char ksecp256k1_coeff_a_sz[] = "0x0";
const char ksecp256k1_coeff_b_sz[] = "0x7"; 
const char ksecp256k1_n_sz[]       = "0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141"; 


using namespace ffm;
using namespace af;


namespace
{
  using namespace bmx;
  const std::string G = "G";
  const std::string n = "n";

  //  const char n_minus_2[] = "n-2"; 

  void printbin (const char* lbl, const af::digest32& bytes) {
    printf ("%s: ", lbl);
    for (auto b : bytes) printf ("%02x ", b);
    printf ("\n"); 
  }

  
    // def deterministic_k(self, z):
    //     k = b'\x00' * 32
    //     v = b'\x01' * 32
  

    //     if z > N:
    //         z -= N
    //     z_bytes = z.to_bytes(32, 'big')
    //     secret_bytes = self.secret.to_bytes(32, 'big')
    //     s256 = hashlib.sha256
    //     k = hmac.new(k, v + b'\x00' + secret_bytes + z_bytes, s256).digest()
    //     v = hmac.new(k, v, s256).digest()
    //     k = hmac.new(k, v + b'\x01' + secret_bytes + z_bytes, s256).digest()
    //     v = hmac.new(k, v, s256).digest()
    //     while True:
    //         v = hmac.new(k, v, s256).digest()
    //         candidate = int.from_bytes(v, 'big')
    //         if candidate >= 1 and candidate < N:
    //             return candidate
    //         k = hmac.new(k, v + b'\x00', s256).digest()
    //         v = hmac.new(k, v, s256).digest()

}


namespace bmx {

  //
  class _secp256k1 {
    //  3
  public:
    
    _secp256k1 ();
    ~_secp256k1 ();
    
    //bool Verify (const char* sz_z, const char* sz_r, const char* sz_s); 
    // bool Sign (unsigned char* z, unsigned char* r); 
    
    bool Sign   (Signature& outsig, const PrivateKey& privatekey, const digest32& msghash);
    bool Verify (const Signature& sig, const PublicKey& pubk, const digest32& z); 

  protected:
    
    ffm::FEConRef F;
    ffm::ECConRef EC;
    ffm::el::map elems;
    ffm::pt::map points; 
  }; 
  

}


//
//
bmx::Point& bmx::MakePublicKey (Point& out, const PrivateKey& secr) {

  pt::map pm;
  el::map em;
  // FEConRef F  = Create_FE_context (ksecp256k1_p_sz);
  // ECConRef EC = Create_EC_context (F, em, pm, ksecp256k1_coeff_a_sz, ksecp256k1_coeff_b_sz, ksecp256k1_n_sz, 0);

  FFM_Env env; 
  Init_secp256k1_Env (env);
  FEConRef const& F  = env.F; 
  ECConRef const& EC = env.EC; 
  // checkres (G,  EC->MakePoint (G, ksecp256k1_G_x_sz, ksecp256k1_G_y_sz, 0));
  EC->MakePoint (G, ksecp256k1_G_x_sz, ksecp256k1_G_y_sz, 0); 

  bytearray wsbuf (256);  
  WriteStreamRef ws = CreateWriteMemStream (std::data(wsbuf), wsbuf.size()); 
  
    
  ScopeDeleter dr     (F);
  bytearray    arrtmp (256, byte{0});
    
  FE_t e = dr(F->New_bin (std::data(secr), secr.size(), false));
  
  const std::string v = "xzd8"; 
  EC->MakePoint  (v, dr (F->New()), dr (F->New()));
  EC->Mul_scalar (v, e, G); 

  F->Raw (arrtmp, pt::x(pm[v]), false);
  printf ("[%zu]pt::x\n",  arrtmp.size ());
  //assert (arrtmp.size() == 32);
  copy_BE (out.x, arrtmp); 
  
  F->Raw (arrtmp, pt::y(pm[v]), false);
  printf ("[%zu]pt::y\n",  arrtmp.size ());
  // assert (arrtmp.size() == 32);
  
  copy_BE (out.y, arrtmp); 

  return out; 

}


  
  //
  // 
size_t bmx::ReadPoint (Point& out , ReadStreamRef rs) {
  
    size_t readlen = 0;
    // read prefix byte
    unsigned char pref = 0;
    readlen += rs->Read (&pref, 1);
    
    if (pref == 4) {

      //printf ("%s:pref == 4\n", __FUNCTION__); 
      // both coord 
      readlen += read_byte32 (out.x, rs);
      readlen += read_byte32 (out.y, rs);
    }
    else { // just the x coord

      printf ("%s:[pref != 4]\n", __FUNCTION__); 

      FEConRef F (nullptr); 
      Init_FE_context (F); 
      ScopeDeleter dr (F); 

      bool y_even = (pref == 2);

      readlen += read_byte32 (out.x, rs);

      FE_t Px    = dr (F->New_bin (&out.x[0], out.x.size(), false)); 
      FE_t xxx   = dr (F->New()); // x^3
      FE_t rhs   = dr (F->New()); 
      FE_t alpha = dr (F->New()); 
      FE_t beta  = dr (F->New()); 

      // compute: y^2 = X^3 + 7
      F->Pow_ui (xxx, Px, 3);
      F->Add    (rhs, xxx, dr (F->New_ui(7))); 
      F->Sqrt   (beta, rhs);
      //
      FE_t even_beta = fe_null;
      FE_t odd_beta  = fe_null;
      bool beta_even = (0 == F->TestBit (beta, 0))? true : false;
      if (beta_even) { //beta % 2
	even_beta = beta;
	odd_beta = dr(F->New ());
	F->Sub (odd_beta, F->p(), beta); 
      }
      else {
	even_beta = dr(F->New());  
	F->Sub (even_beta, F->p(), beta); 
	odd_beta = beta;   
      }
      //
      bytearray rawtmp; 
      if (y_even) {
	// y = even_beta;
	copy_BE (out.y, F->Raw (rawtmp, even_beta, false)); 
      }
      else {
	// y = odd_beta; 
	copy_BE (out.y, F->Raw (rawtmp, odd_beta, false)); 
      }
    }
    
    return readlen;
}


size_t bmx::WritePoint (WriteStreamRef ws, const Point& p, bool compr) {
  
  size_t write_len = 0;

  // lest sig byte in y
  const std::bitset<8> lobyte (std::to_integer<uint8>(p.y[31])); 
  
  if (compr) { // figure out if y is odd or even
    byte pref = ( lobyte[0] ? byte{0x3} : byte{0x2}); 
    //byte pref = (p.y[31] & byte{0x1} ? byte{0x3} : byte{0x2}); 
    write_len += ws->Write    (&pref, 1); 
    write_len += write_byte32 ( ws, p.x); 
    //return write_len ; //  = 33);
  }
  else {      // write both coords
    byte  pref {0x4};
    write_len += ws->Write    (&pref, 1); 
    write_len += write_byte32 ( ws, p.x); 
    write_len += write_byte32 ( ws, p.y); 
    //return write_len; // == 65); 
  }
  
  return write_len; 
}


//
//
size_t bmx::WriteSignature_DER (WriteStreamRef ws, const Signature& sig) {

  const byte startmarker { 0x30 };
  const byte zerobyte    { 0 }; 
  const byte markerbyte  { 0x02 }; 


  uint8  remptydigs = 0; 
  while (sig.r[remptydigs] == zerobyte && remptydigs < 32) ++remptydigs;

  uint8  semptydigs = 0; 
  while (sig.s[semptydigs] == zerobyte && semptydigs < 32) ++semptydigs; 

  uint8 rlen = sig.r.size () - remptydigs;
  if (std::to_integer<uint8>(sig.r[0]) & 0x80) ++rlen; 
  
  uint8 slen = sig.s.size () - semptydigs; 
  if (std::to_integer<uint8>(sig.s[0]) & 0x80) ++slen; 

  size_t begpos = ws->GetPos (); 

  size_t write_len = 0; 

  uint8 sizesig = slen + rlen + 6; 
  
  // startbyte
  write_len += ws->Write (&startmarker, sizeof(startmarker)); 

  // size size
  write_len += ws->Write (&sizesig, sizeof(sizesig));


  // marker
  write_len += ws->Write (&markerbyte, sizeof(markerbyte)); 
  // rlen
  write_len += ws->Write (&rlen, sizeof(rlen)); 
  // write r, leading '0' if needed

  if (std::to_integer<uint8>(sig.r[0]) & 0x80)
    write_len += ws->Write (&zerobyte, sizeof(zerobyte)); 
  write_len += ws->Write (&sig.r[0], sig.r.size()); 
  //write_len += af::write_byte32 (ws, sig.r);
  
  // marker
  write_len += ws->Write (&markerbyte, sizeof(markerbyte)); 

  write_len += ws->Write (&slen, sizeof(slen)); 
  // write s,...
  if (std::to_integer<uint8>(sig.s[0]) & 0x80)
      write_len += ws->Write (&zerobyte, sizeof(zerobyte)); 
  write_len += ws->Write (&sig.s[0], sig.s.size ()); 

  size_t final_size = begpos = ws->GetPos ();
  //assert (final_size == write_len); 

  printf ( "%s:write_len[%zu]\n",     __FUNCTION__, write_len);
  //printf ( "%s:expected_size[%zu]\n", __FUNCTION__, expected_size);
  //printf ( "%s:final_size[%zu]\n",    __FUNCTION__, final_size);
  // total write size 
  //return (final_size == expected_size); 

  return write_len; 
 }


    // def der(self):
    //     rbin = self.r.to_bytes(32, byteorder='big')
    //     # remove all null bytes at the beginning
    //     rbin = rbin.lstrip(b'\x00')
    //     # if rbin has a high bit, add a \x00
    //     if rbin[0] & 0x80:
    //         rbin = b'\x00' + rbin
    //     result = bytes([2, len(rbin)]) + rbin  # <1>
    //     sbin = self.s.to_bytes(32, byteorder='big')
    //     # remove all null bytes at the beginning
    //     sbin = sbin.lstrip(b'\x00')
    //     # if sbin has a high bit, add a \x00
    //     if sbin[0] & 0x80:
    //         sbin = b'\x00' + sbin
    //     result += bytes([2, len(sbin)]) + sbin
    //     return bytes([0x30, len(result)]) + result






//
//
#ifdef __OLD_WRITE_SIG_DER__
 
 size_t WriteSignature_DER (WriteStreamRef ws, const Signature& sig) {

  const uint8 startmarker = 0x30;
  const uint8 zerobyte    = 0; 
  const uint8 markerbyte  = 0x02; 

  uint8  remptydigs = 0; 
  while (sig.r[remptydigs] == 0 && remptydigs < 32) ++remptydigs;

  uint8  semptydigs = 0; 
  while (sig.s[semptydigs] == 0 && semptydigs < 32) ++semptydigs; 

  uint8 rlen = sig.r.size () - remptydigs;
  if (sig.r[0] & 0x80) ++rlen; 
  
  uint8 slen = sig.s.size () - semptydigs; 
  if (sig.s[0] & 0x80) ++slen; 


  size_t begpos = ws->GetPos (); 

  size_t write_len = 0; 

  uint8 sizesig = slen + rlen + 6; 
  
  // startbyte
  write_len += ws->Write (&startmarker, sizeof(startmarker)); 

  // size size
  write_len += ws->Write (&sizesig, sizeof(sizesig)); 
  
  // marker
  write_len += ws->Write (&markerbyte, sizeof(markerbyte)); 
  // rlen
  write_len += ws->Write (&rlen, rlen); 
  // write r, leading '0' if needed

  if (sig.r[0] & 0x80)
    write_len += ws->Write (&zerobyte, sizeof(zerobyte)); 
  write_len += ws->Write (&sig.r[0], sig.r.size()); 
  //write_len += af::write_byte32 (ws, sig.r);
  
  // marker
  write_len += ws->Write (&markerbyte, sizeof(markerbyte)); 

  write_len += ws->Write (&slen, sizeof(slen)); 
  // write s,...
  if (sig.s[0] & 0x80)
      write_len += ws->Write (&zerobyte, sizeof(zerobyte)); 
  write_len += ws->Write (&sig.s[0], sig.s.size ()); 

  size_t final_size = begpos = ws->GetPos ();
  //assert (final_size == write_len); 

  printf ( "%s:write_len[%zu]\n",     __FUNCTION__, write_len);
  //printf ( "%s:expected_size[%zu]\n", __FUNCTION__, expected_size);
  //printf ( "%s:final_size[%zu]\n",    __FUNCTION__, final_size);
  // total write size 
  //return (final_size == expected_size); 

  return write_len; 
 }

 #endif
 


 
//
//
size_t bmx::ReadSignature_DER (Signature& osig, size_t binsize, ReadStreamRef rs) {
  
  size_t readlen = 0; 
  const unsigned char startmarker = 0x30;
  const unsigned char zerobyte    = 0; 
  const unsigned char markerbyte  = 0x02; 

  //const unsigned char sizesig = 4 + rlen + slen;
  //printf ("binsize:%zu\n", binsize);   
  // def parse(cls, signature_bin):
  //     s = BytesIO(signature_bin)
  unsigned char compound = 0;
  readlen +=  rs->Read (&compound, 1); 
  // compound = s.read(1)[0]

  if (compound != startmarker) {
    printf ("exiting, l:%i\n",  __LINE__); 
    return readlen;
  }
    // if compound != 0x30:
  //     raise SyntaxError("Bad Signature")
  unsigned char length = 0;
  readlen += rs->Read (&length, 1);
  // length = s.read(1)[0]
  if ( (length + 2) != binsize) {
    // raise SyntaxError("Bad Signature Length")
    printf ("exiting, l:%i\n", __LINE__); 
    return readlen;
  }
  // if length + 2 != len(signature_bin):
  //     raise SyntaxError("Bad Signature Length")

  unsigned char marker = 0;
  readlen += rs->Read (&marker, 1);
  // marker = s.read(1)[0]
  if (marker != markerbyte) {
    // raise SyntaxError("Bad Signature")
    printf ("exiting, l:%i\n", __LINE__); 
    return readlen;
  }
  //     if marker != 0x02:
  //         raise SyntaxError("Bad Signature")
  unsigned char rlen = 0;
  readlen += rs->Read (&rlen, 1); 
  //  rlength = s.read(1)[0]

  bytearray rtmp (rlen);

  readlen += rs->Read (&rtmp[0], rlen);

  int rbegind = rlen - 32;
  if (rbegind > 0) 
    std::copy (&rtmp[rbegind], &rtmp[rbegind+32], &osig.r[0]); 
  else
    copy_BE (osig.r, rtmp);
  // r = int.from_bytes(s.read(rlength), 'big')

  marker = 0; 
  readlen += rs->Read (&marker, 1); 
  //     marker = s.read(1)[0]
  if (marker != markerbyte) {
  // raise SyntaxError("Bad Signature")
    printf ("exiting, l:%i\n", __LINE__); 
    return readlen; 
  }
  // if marker != 0x02:
  //     raise SyntaxError("Bad Signature")

  unsigned char slen = 0;
  readlen += rs->Read (&slen, 1);

  // slength = s.read(1)[0]
  bytearray stmp(slen);
  //printf ("<slen %i>\n" , slen); 
  readlen += rs->Read (&stmp[0], slen);

  int sbegind = slen - 32;
  if (sbegind > 0) 
    std::copy (&stmp[sbegind], &stmp[sbegind+32], &osig.s[0]);
  else
    copy_BE (osig.s, stmp); 
  //     s = int.from_bytes(s.read(slength), 'big')

  if ((6 + rlen + slen) == binsize) {
    printf ("..sig read is good\n", __FUNCTION__); 
  }
  else {
    printf ("%s [len(sig) != binsize }\n", __FUNCTION__); 
    // raise SyntaxError("Signature too long")
  }

  return readlen;
}


//
std::string& bmx::MakeAddress (std::string& out, bool compr, bool mainnet, const PublicKey &pubk) {

  //1
  unsigned char  netprefix = mainnet ? 0x0 : 0x6f;

  //2
  bytearray pmem     (128);
  size_t plen = WritePoint (CreateWriteMemStream (&pmem[0], 128), pubk, compr); 
  af::digest20 digest160; 
  af::hash160 (digest160, &pmem[0], plen);
  
  //3
  size_t         comblen = 0; 
  WriteStreamRef pws     = CreateWriteMemStream (&pmem[0], 128); 
  comblen += pws->Write (&netprefix, 1);
  comblen += pws->Write (&digest160[0], 20);

  // // 4
  // af::digest32 digest256;
  // af::hash256 (digest256, &pmem[0], comblen);

  // // 5
  // size_t writelen = 0;
  // bytearray checksummem (comblen+4); 
  // WriteStreamRef ws = CreateWriteMemStream (&checksummem[0], comblen+4);
  // writelen += ws->Write (&pmem[0], comblen);
  // writelen += ws->Write (&digest256[0], 4);
  
  // return base58::encode (out, &endmem[0] , writelen);

  return base58::encode_checksum (out, &pmem[0], comblen); 
  
  
}


//
// Wallet Import Format
std::string& bmx::MakeWIF (std::string& out, bool compr, bool mainnet, const PrivateKey& prvk) {

  bytearray mem (128);

  WriteStreamRef ws      = CreateWriteMemStream (&mem[0], 128); 
  unsigned char  compsuf = 0x01; 
  unsigned char  netpref = mainnet ? 0x80 : 0xef;

  size_t writelen = 0; 
  writelen += ws->Write (&netpref, 1); 
  writelen += write_byte32 (ws, prvk);
  if (compr) writelen += ws->Write (&compsuf, 1);


  return base58::encode_checksum (out, &mem[0], writelen); 
  
}

//
//
bool bmx::secp256k1::Verify (const Signature& sig, const PublicKey& pubk, const digest32& zmsg) { 

  using namespace af;
  using namespace ffm;
    //    printf ("%s:enter\n", __FUNCTION__); 

  FFM_Env env; 
  Init_secp256k1_Env (env);
  FEConRef&     F      = env.F;
  ECConRef&     EC     = env.EC;
  ffm::pt::map& points = env.pm; 
  ffm::el::map& elems = env.em; 

  ScopeDeleter dr (F);
  
  FE_t z = dr (F->New_bin(std::data(zmsg), 32, false));
  FE_t r = dr (F->New_bin(std::data(sig.r), 32, false));
  FE_t s = dr (F->New_bin(std::data(sig.s), 32, false));
  
  FE_t s_inv = dr(F->New());

  const std::string n = "n"; 
  const std::string u = "u"; 
  const std::string v = "v";
  const std::string vP = "vP";
  const std::string uG = "uG"; 

  const std::string R = "R";
  const std::string P = "P"; 

  FE_t Gx = F->New (ksecp256k1_G_x_sz, 0);
  FE_t Gy = F->New (ksecp256k1_G_y_sz, 0);
  EC->MakePoint (G, Gx, Gy); 
  //    EC->PrintPoint ("<G>", G, ffm::format::hex); 


  FE_t Px = F->New_bin (std::data(pubk.x), 32, false);
  FE_t Py = F->New_bin (std::data(pubk.y), 32, false); 
  EC->MakePoint (P, Px, Py); 
  //    EC->PrintPoint ("<P>", P, ffm::format::hex); 

  FE_t n_minus_2 = dr(F->New());
  F->Sub_ui(n_minus_2, elems[n], 2);
  //EC->PrintElem ("{n-2}", n_minus_2, ffm::format::hex);
    
  F->PowM (s_inv, s, n_minus_2, elems[n]); 
    
  EC->MakeElem_ui(u, 0);
  F->MulM (elems[u], z, s_inv, elems[n]);
    
  EC->MakeElem_ui (v, 0);
  F->MulM (elems[v], r, s_inv, elems[n]);
    
  EC->MakePoint_ui (uG, 0, 0);
  EC->Mul_scalar (uG, u, G);
    
  EC->MakePoint_ui (vP, 0, 0);
  EC->Mul_scalar (vP, v, P); 
    
  EC->MakePoint_ui (R, 0, 0);
  EC->AddPoint (R, uG, vP); 
    
  ffm::pt::struc& Rref = points[R];
  bool oncurve = EC->IsPointOnCurve (pt::x(Rref), pt::y(Rref)); 
  if (oncurve) {
    printf ("R is on curve\n"); 
  }
    
  EC->PrintElem ("R.x", pt::x(Rref), ffm::format::hex);
  EC->PrintElem ("  r", r, ffm::format::hex);
  
  return (F->Cmp (pt::x(Rref), r) == 0);
}



// soon...
bool bmx::secp256k1::Sign (Signature& sig, const PrivateKey& privk, const digest32& zbin) {
  FN_SCOPE (); 
  using namespace ffm;
  // eG = P
  FFM_Env env; 
  Init_secp256k1_Env (env);
  FEConRef&     F      = env.F;
  ECConRef&     EC     = env.EC;
  ffm::pt::map& points = env.pm; 
  ffm::el::map& elems  = env.em; 
  
  //    const std::string G = "G"; 
  const std::string P = "P";
  const std::string R = "R";
  //const std::string n = "n";
  
  ScopeDeleter dr (F); 
  // kG = R
  FE_t
    e    = dr(F->New_bin(std::data(privk), 32, false)),
    z    = dr(F->New_bin(std::data(zbin), 32, false)),
    s    = dr(F->New ()),
    stmp = dr(F->New()),
    
    tmp  = dr(F->New()),
    snum = dr(F->New ());
  
  //
  EC->MakePoint_ui(P, 0, 0);
  EC->Mul_scalar (P, e, G); 
  
  // n-2
  FE_t n_sub_2 = dr(F->New());
  F->Sub_ui(n_sub_2, elems[n], 2);
  elems["n-2"] = n_sub_2; 
  
  // n/2
  FE_t n_div_2 = dr(F->New()); 
  F->Div_ui(n_div_2, elems[n], 2); 
  elems["n/2"] = n_div_2;
  
  // k = rand (n) <-- fix later
  FE_t k = dr(F->New());


  // >>>>>>> deterministricK
  digest32  determK;
  //bytearray determK;

  Deterministic_K (determK, privk, zbin);

  printf ("\n %s\n    -> [%s] \n", __PRETTY_FUNCTION__  , fmthex (determK).c_str()); 
  
  //digest32&  Deterministic_K (digest32& ok, const PrivateKey& priv, const digest32& z) {
  // <<<<<<<<<< USING NEW Deterministic

  
  F->Rand (k, elems[n]);


  elems["k"] = k;
  
  // 1/k
  FE_t k_inv= dr(F->New ());
  F->PowM (k_inv, k, n_sub_2, elems[n]);  
  elems["1/k"] = k_inv; 
  
  EC->MakePoint_ui (R, 0, 0); // <-- we should just make a plain 'alloc-point'
  EC->Mul_scalar (R, "k", G); // we want R.x
  // kG = R
  
  F->SMul (tmp, pt::x(points[R]), e);
  F->SAdd (snum, z, tmp);
  F->MulM (stmp, snum, k_inv, elems[n]); 
  // s = (z+re)/k mod n
  
  // if s > n/2
  //  s = n - s
  int cmpres = F->Cmp (stmp, elems["n/2"]);
  // Compare op1 and op2. Return a positive value if op1 > op2, zero if op1 = op2, or a negative value if op1 < op2.
  if (cmpres > 0) {
    // printf ("..(s > n/2)\n");
    F->Sub (s , elems[n], stmp); 
  }
  else {
    F->Set (s, stmp);
  }
  
  // write results, sig is (r,s) => sig(r,s)
  {
    af::bytearray rraw, sraw;
    F->Raw (rraw, pt::x(points[R]), false);
    ffm::copy_BE(sig.r, rraw); 
    
    F->Raw (sraw, s, false);
    ffm::copy_BE(sig.s, sraw); 
    
    printf ("\n__SIGNATURE_GENERATED__\n");
  }
  
  return true; 
  
}

//
//


//
//
bmx::_secp256k1::_secp256k1 () : elems (), points() { 
  
  F = ffm::Create_FE_context (ksecp256k1_p_sz, 0);
  EC = ffm::Create_EC_context (F, elems, points, ksecp256k1_coeff_a_sz, ksecp256k1_coeff_b_sz, ksecp256k1_n_sz, 0);
    
  EC->MakePoint (G, ksecp256k1_G_x_sz, ksecp256k1_G_y_sz, 0);
  
  ffm::ScopeDeleter dr (F);
  ffm::FE_t t0 = dr (F->New ()); 
  //EC->PrintElem ("SECzy:n", "n", format::hex);
  //    Ec->PrintElem ("
  
  //checkres ("n", EC->DefElem ("n", ksecp256k1_n_sz, 0));
  
  //F->Set (t0, "0x2", 0);
  //EC->DefElem ("n-2", 
    
    }

//
//
bmx::_secp256k1::~_secp256k1 () {
  
}

 
  //
  /*
  bool secp256k1::Verify (const char* sz_z, const char* sz_r, const char* sz_s)  {
    // uG + vP = R 
    // u = z/s
    // v = r/s
    // s = (z+re)/k
    ffm::ScopeDeleter dr (F);

    POUT ("USING THIS");
    

    ffm::FE_t z = dr (F->New(sz_z, 0));
    ffm::FE_t r = dr (F->New(sz_r, 0));
    ffm::FE_t s = dr (F->New(sz_s, 0));
    
    ffm::FE_t s_inv = dr(F->New());

    const std::string R  = "R";
    const std::string P  = "P"; 
    const std::string vP = "vP";
    const std::string uG = "uG"; 
    
    // powm (out,  
    ffm::FE_t n_minus_2 = dr(F->New());
    F->Sub_ui(n_minus_2, elems[n], 2);
    EC->PrintElem ("{n-2}", n_minus_2, ffm::format::hex);
   
    F->PowM (s_inv, s, n_minus_2, elems[n]); 
    EC->PrintElem ("s_inv", s_inv, ffm::format::hex); 
    
    ffm::FE_t u = F->New ();
    F->MulM (u, z, s_inv, elems[n]);
    elems["u"] = u;
    
    ffm::FE_t v = F->New ();
    F->MulM (v, r, s_inv, elems[n]);
    elems["v"] = v; 

    EC->MakePoint_ui (uG, 0,  0);
    bool uG_res = EC->Mul_scalar (uG, "u", G);
      
    EC->MakePoint_ui (vP, 0, 0);
    bool vP_res = EC->Mul_scalar (vP, "v", P); 

    EC->PrintPoint (uG, uG, ffm::format::hex);
    EC->PrintPoint (vP, vP, ffm::format::hex);

    EC->MakePoint_ui (R, 0, 0);

    EC->AddPoint (R, uG, vP); 
    
	      
    pt::struc& Rref = points[R];
    bool oncurve = EC->IsPointOnCurve (pt::x(Rref), pt::y(Rref)); 
    if (oncurve) {
      printf ("R is on curve\n"); 
    }
    else
    
    EC->PrintElem ("R.x", pt::x(Rref), ffm::format::hex);
    EC->PrintElem ("  r", r, ffm::format::hex);
    return (F->Cmp (pt::x(Rref), r) == 0);
  }
  
  */



//
  //
bool bmx::_secp256k1::Verify (const Signature& sig, const PublicKey& pubk, const digest32& z_msg) {
    //    printf ("%s:enter\n", __FUNCTION__); 
  
    ffm::ScopeDeleter dr (F);
  
    ffm::FE_t z = dr (F->New_bin(std::data(z_msg), 32, false));
    ffm::FE_t r = dr (F->New_bin(std::data(sig.r), 32, false));
    ffm::FE_t s = dr (F->New_bin(std::data(sig.s), 32, false));
    
    ffm::FE_t s_inv = dr(F->New());

    const std::string n = "n"; 
    const std::string u = "u"; 
    const std::string v = "v";
    const std::string vP = "vP";
    const std::string uG = "uG"; 

    const std::string R = "R";
    const std::string P = "P"; 

    ffm::FE_t Gx = F->New (ksecp256k1_G_x_sz, 0);
    ffm::FE_t Gy = F->New (ksecp256k1_G_y_sz, 0);
    EC->MakePoint (G, Gx, Gy); 
    EC->PrintPoint ("<G>", G, ffm::format::hex); 


    ffm::FE_t Px = F->New_bin (std::data(pubk.x), 32, false);
    ffm::FE_t Py = F->New_bin (std::data(pubk.y), 32, false); 
    EC->MakePoint (P, Px, Py); 
    EC->PrintPoint ("<P>", P, ffm::format::hex); 

    // powm (out,  
    ffm::FE_t n_minus_2 = dr(F->New());
    F->Sub_ui(n_minus_2, elems[n], 2);
    //EC->PrintElem ("{n-2}", n_minus_2, ffm::format::hex);
    
    F->PowM (s_inv, s, n_minus_2, elems[n]); 
    
    EC->MakeElem_ui(u, 0);
    F->MulM (elems[u], z, s_inv, elems[n]);
    
    EC->MakeElem_ui (v, 0);
    F->MulM (elems[v], r, s_inv, elems[n]);
    
    EC->MakePoint_ui (uG, 0, 0);
    EC->Mul_scalar (uG, u, G);
    
    EC->MakePoint_ui (vP, 0, 0);
    EC->Mul_scalar (vP, v, P); 
    
    EC->MakePoint_ui (R, 0, 0);
    EC->AddPoint (R, uG, vP); 
    
    ffm::pt::struc& Rref = points[R];
    bool oncurve = EC->IsPointOnCurve (pt::x(Rref), pt::y(Rref)); 
    if (oncurve) {
      printf ("R is on curve\n"); 
    }
    
    EC->PrintElem ("R.x", pt::x(Rref), ffm::format::hex);
    EC->PrintElem ("  r", r, ffm::format::hex);

    return (F->Cmp (pt::x(Rref), r) == 0);
  }





//
  //
bool bmx::_secp256k1::Sign (Signature& sig, const PrivateKey& privk, const digest32& zbin) {
  
  using namespace ffm;
  // eG = P
  
  //    const std::string G = "G"; 
  const std::string P = "P";
  const std::string R = "R";
  //const std::string n = "n";
  
  ScopeDeleter dr (F); 
  // kG = R
  FE_t
    e    = dr(F->New_bin(std::data(privk), 32, false)),
    z    = dr(F->New_bin(std::data(zbin), 32, false)),
    s    = dr(F->New ()),
    stmp = dr(F->New()),
    
    tmp  = dr(F->New()),
    snum = dr(F->New ());
  
  //
  EC->MakePoint_ui(P, 0, 0);
  EC->Mul_scalar (P, e, G); 
  POUT ("224");
  
  
  // n-2
  FE_t n_sub_2 = dr(F->New());
  F->Sub_ui(n_sub_2, elems[n], 2);
  elems["n-2"] = n_sub_2; 
  
  // n/2
  FE_t n_div_2 = dr(F->New()); 
  F->Div_ui(n_div_2, elems[n], 2); 
  elems["n/2"] = n_div_2;
  
  // k = rand (n) <-- fix later
  FE_t k = dr(F->New()); 
  POUT("241")
    F->Rand (k, elems[n]);
  elems["k"] = k;
  
  // 1/k
  FE_t k_inv= dr(F->New ());
  F->PowM (k_inv, k, n_sub_2, elems[n]);  
  elems["1/k"] = k_inv; 
  
  EC->MakePoint_ui (R, 0, 0); // <-- we should just make a plain 'alloc-point'
  EC->Mul_scalar (R, "k", G); // we want R.x
  // kG = R
  
  F->SMul (tmp, pt::x(points[R]), e);
  F->SAdd (snum, z, tmp);
  F->MulM (stmp, snum, k_inv, elems[n]); 
  // s = (z+re)/k mod n
  
  // if s > n/2
  //  s = n - s
  int cmpres = F->Cmp (stmp, elems["n/2"]);
  // Compare op1 and op2. Return a positive value if op1 > op2, zero if op1 = op2, or a negative value if op1 < op2.
  if (cmpres > 0) {
    printf ("..(s > n/2)\n");
    F->Sub (s , elems[n], stmp); 
  }
  else {
    F->Set (s, stmp);
  }
  
  // write results, sig is (r,s) => sig(r,s)
  {
    af::bytearray rraw, sraw;
    F->Raw (rraw, pt::x(points[R]), false);
    ffm::copy_BE(sig.r, rraw); 
    
    F->Raw (sraw, s, false);
    ffm::copy_BE(sig.s, sraw); 
    
    printf ("__SIGNATURE_GENERATED__\n");
  }
  
  return true; 
  
}




bool bmx::Init_FE_context (ffm::FEConRef& oFE) { 

  if (oFE)
    return false;
  
  if (oFE = Create_FE_context (ksecp256k1_p_sz))
    return true; 

  return false; 

}

bool bmx::Init_secp256k1_Env (FEConRef& oFE, ECConRef& oEC, el::map& em, pt::map& pm) {

  if (oFE)
    return false;
  
  if (oEC)
    return false;
  
  //oFE = Create_FE_context (ksecp256k1_p_sz);

  if (Init_FE_context (oFE))  {
  
    oEC = Create_EC_context (oFE, em, pm, ksecp256k1_coeff_a_sz, ksecp256k1_coeff_b_sz, ksecp256k1_n_sz, 0);
  
    em["G.x"] = oFE->New (ksecp256k1_G_x_sz, 0); 
    em["G.y"] = oFE->New (ksecp256k1_G_y_sz, 0); 

    oEC->MakePoint ("G", em["G.x"], em["G.y"]);

    return (oFE && oEC);
  }

  return false; 

}


//
//
digest32&  bmx::Deterministic_K (digest32& ok, const PrivateKey& priv, const digest32& z) {
  
  FN_SCOPE (); 

  printf ("%s INPUT<private key [%s]\n",  __FUNCTION__ , fmthex(priv).c_str());
  printf ("%s INPUT<message hash [%s]\n",   __FUNCTION__ , fmthex(z).c_str());
  std::vector<char> msgbuf (256, 0); 
  
  const byte byte0x0 = byte {0x0}; 
  const byte byte0x1 = byte {0x1};
  ok.fill (byte0x0); 
  
  FEConRef F (nullptr); 
  Init_FE_context (F); 
  ScopeDeleter dr (F);
  
  FE_t fe_z = dr(F->New_bin (&z[0], 32, false));
  printf( "\n%s INPUT <fe_z [%s]\n", __FUNCTION__, F->fmt (&msgbuf[0], "%Zx", fe_z)) ; 

  FE_t N    = dr(F->New (ksecp256k1_n_sz, 0));
  printf( "%s INPUT <N [%s]\n", __FUNCTION__, F->fmt (&msgbuf[0], "%Zx", N)); 
	  //	  inline  char*         fmt        (char* out, const char* fmt, FE_t x) { return Format (out, fmt, x); } 
  
  digest32 k; k.fill (byte{0x00}); 
  digest32 v; v.fill (byte{0x01});
  
  // Compare op1 and op2. Return a positive value if op1 > op2, zero if op1 =
  // op2, or a negative value if op1 < op2.
  if (F->Cmp (fe_z, N) > 0) {
    F->SSub (fe_z, fe_z, N);
  }
  
  digest32 z1; {
    bytearray tmp;
    int z_sign = 0;
    F->Raw(tmp, z_sign, fe_z, false);
    copy_BE(z1, tmp);
  }
  
  digest32 zbin ;
  size_t max_txt_size = 512; 
  bytearray input_txt (max_txt_size, byte{0}); 
  WriteStreamRef wstxt = CreateWriteMemStream (&input_txt[0], max_txt_size);
  
  // k = hmac.new(k, v + b'\x00' + secret_bytes + z_bytes, s256).digest()
  wstxt->SetPos (0, byte_stream::SeekMode::Abs); 
  wstxt->Write (&v[0], 32);
  wstxt->Write (&byte0x0, 1);
  wstxt->Write (&priv[0], 32);
  wstxt->Write (&z1[0], 32); 
  af::hmac_sha256 (k, k, &input_txt[0], wstxt->GetPos ());
  
  // v = hmac.new(k, v, s256).digest()
  wstxt->SetPos (0, byte_stream::SeekMode::Abs); 
  wstxt->Write (&v[0], 32); 
  af::hmac_sha256 (v, k, &input_txt[0], wstxt->GetPos ()); 
  
  // k = hmac.new(k, v + b'\x01' + secret_bytes + z_bytes, s256).digest()
  wstxt->SetPos (0, byte_stream::SeekMode::Abs); 
  wstxt->Write (&v[0], 32);
  wstxt->Write (&byte0x1, 1);
  wstxt->Write (&priv[0], 32);
  wstxt->Write (&z1[0], 32); 
  af::hmac_sha256 (k, k, &input_txt[0], wstxt->GetPos ());
  
  // v = hmac.new(k, v, s256).digest()
  wstxt->SetPos (0, byte_stream::SeekMode::Abs);
  wstxt->Write (&v[0], 32); 
  af::hmac_sha256 (v, k, &input_txt[0], wstxt->GetPos ());
  
  FE_t fe_candidate = dr (F->New()); 
  digest32 candidate; 
  while (true) {
    
    wstxt->SetPos(0, byte_stream::SeekMode::Abs);
    wstxt->Write(&v[0], 32);
    af::hmac_sha256 (v, k, &input_txt[0], wstxt->GetPos ());
    
    candidate = v;
    F->Set_bin (fe_candidate, &candidate[0], 32, false); 
    int cmp_0 = F->Cmp (fe_candidate, 0);
    int cmp_N = F->Cmp (fe_candidate, N);
    // Compare op1 and op2. Return a positive value if op1 > op2, zero if op1 =
    // op2, or a negative value if op1 < op2.
    if ( (cmp_0 > 0) &&  (cmp_N < 0)) {
      ok = candidate;
      return ok; 
    }
    
    //k = hmac.new(k, v + b'\x00', s256).digest()
    wstxt->SetPos (0, byte_stream::SeekMode::Abs); 
    wstxt->Write (&v[0], 32);
    wstxt->Write (&byte0x0, 1);
    af::hmac_sha256 (k, k, &input_txt[0], wstxt->GetPos ());
    
    //v = hmac.new(k, v, s256).digest()
    wstxt->SetPos (0, byte_stream::SeekMode::Abs); 
    wstxt->Write (&v[0], 32);
    af::hmac_sha256 (v, k, &input_txt[0], wstxt->GetPos ());
    
  }
  
  
  return ok;
}









