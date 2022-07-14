//#include <glm/glm.hpp>
//#include <stdio.h>


#include "mtool.h"
#include "SECzy/secp256k1.h"
#include "aframe/hash.h"
#include "aframe/utility.h"
#include "ffm/fe_math.h"
//#include <openssl/sha.h>

//#include <crypto++/cryptlib.h>
//#include <cryptopp/sha.h>
//#include <cryptopp/ripemd.h>

//#include "sha2.h"

// big num math
// #include <gmp.h>

// zero mq messaging
#include <zmq.h>

//  SECP256k1  stuff
const char kSEC256k1_p_sz[]       = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f";
const char kSEC256k1_G_x_sz[]     = "0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798";
const char kSEC256k1_G_y_sz[]     = "0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8";
const char kSEC256k1_coeff_a_sz[] = "0x0";
const char kSEC256k1_coeff_b_sz[] = "0x7"; 
const char kSEC256k1_n_sz[]       = "0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141"; 
// N = 0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141


// y^2 = x^3 +ax^2+b 
const size_t kField_bit_precision = 256;


inline void print_bignum (const char *lbl, ffm::FE_t x, ffm::FEConPtr c) {

  std::array<char, 128> vstr; 
  c->Format (std::data(vstr), "%Zd", x); 
  printf ("%s%s\n", lbl, std::data(vstr)); 
}



int Test_CryptoPP(const std::vector<std::string>& args)
{
  printf ("%s[...]\n", __FUNCTION__); 


   
  const  char hash_inp[] = "0xA23FD532EG475ADAGFFEG97821ADEFDCBE42GAAFCEFC123564FGSAAAECAB";
  unsigned char outp[64];
  memset (outp, 0, 64);
  printf ("strlen(hash_inp):%zu\n", strlen(hash_inp)); 
  // this sha256 is from ssl
  //  unsigned char* res = SHA256 (hash_inp, 60 , outp); 
 
  //CryptoPP::SHA256::Transform   (0, 0);  
  //iiCryptoPP::RIPEMD160::Transform(0, 0);

  // EC_set_curve(eq, , __)
  return 0;
}




void SEC256k1_test ()
{


  POUT ("0");
  ffm::FEConPtr F = ffm::Create_FE_context(kSEC256k1_p_sz, 0);
  ffm::ECConRef EC = ffm::Create_EC_context (F, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);  

  const std::string
    Fp = "Fp",
    G = "G",
    Eq = "Eq",
    R = "R",
    s = "s",
    Q = "Q",
    M = "M";
  
  EC->MakeElem (Fp, kSEC256k1_p_sz, 0);

  //char Fp_str[256]; 
  //print_bignum ("Fp:", Fp, F); 

  // F->Add ( 
  // 
  EC->MakePoint (G, kSEC256k1_G_x_sz, kSEC256k1_G_y_sz, 0);
  EC->PrintPoint ("G|HEX:", G, ffm::format::hex);
  EC->PrintPoint ("G|DEC: ", G, ffm::format::dec);
  //
  EC->PrintCoeffs ("eq-> ", ffm::format::dec);
  
  
  // EC->DefPoint (R, "0xdeadbeef", "0xdeadf00d", 0);  
  // EC->PrintPoint ("R-> ", R);

  // EC->DefElem (s, "0x04ea32532fd", 0);

//  ffm::EC->Mul (R, s, G);


  EC->AddPoint (Q, G, G);
  POUT("G+G=Q");
  EC->PrintPoint ("Q-> ", Q, ffm::format::hex);

  
  // EC_Sub (M, Q, G); 
  // POUT("M=Q-G");
  // EC_Print("Q->", Q);  
  
  //ffM::EC_ISPOINTONCURVE (); 
  // EC->ISPOINTONCURVE (EQ, G); 

 
}


inline void print_bignum (const char *lbl, ffm::FE_t x, ffm::FEConPtr c) {

  std::array<char, 128> vstr; 
  c->Format (std::data(vstr), "%Zd", x); 
  printf ("%s%s\n", lbl, std::data(vstr)); 
}

//
//

void print_bn (const char* lbl, const af::digest32& bytes) {

  printf ("%s: ", lbl);

  for (auto b : bytes) printf ("%02x ", b);
  
  printf ("\n"); 
  
}


bool ECDSA_Verify (
		   ffm::el::map& em,
		   ffm::pt::map& pm,
		   ffm::FEConPtr F,
		   ffm::ECConRef EC,
		   const SECzy::Signature& sig,
		   const SECzy::PublicKey& pubk,
		   const af::digest32& zhash) {
  printf ("%s|%i:ENTER\n", __FUNCTION__, __LINE__); 
  using namespace ffm;
  
  // uG + vP = R 
  // u   = z/s
  // v   = r/s
  
  // s   = (z+re)/k
  // 1/s = k/(z+re)
  ffm::ScopeDeleter dr (F);
  
  ffm::FE_t z = dr (F->New_bin(std::data(zhash), 32, false)); 
  ffm::FE_t r = dr (F->New_bin(std::data(sig.r), 32, false));
  ffm::FE_t s = dr (F->New_bin(std::data(sig.s), 32, false)); 

  em["z"] = z; 
  em["r"] = r;
  em["s"] = s; 
  // EC->PrintElem (" z>", z, format ::hex);
  // EC->PrintElem (" r>", r, format ::hex);
  // EC->PrintElem (" s>", s, format ::hex);
  // print_bn ("zhash", zhash); 

  
  FE_t Px = F->New_bin (std::data(pubk.x), 32, false);
  FE_t Py = F->New_bin (std::data(pubk.y), 32, false);
  EC->MakePoint("P", Px, Py);
  EC->PrintPoint (" P.", "P", format::hex); 
  
  ffm::FE_t s_inv = dr(F->New());
  
  const std::string n = "n"; 
  const std::string u = "u"; 
  const std::string v = "v";
  const std::string vP = "vP";
  const std::string uG = "uG"; 
  
  // powm (out,  
  ffm::FE_t n_minus_2 = dr(F->New());
  F->Sub_ui(n_minus_2, em[n], 2);
  //EC->PrintElem ("{n-2}", n_minus_2, ffm::format::hex);
  
  F->PowM (s_inv, s, n_minus_2, em[n]); 
  //EC->PrintElem ("s_inv", s_inv, ffm::format::hex); 
  
  EC->MakeElem_ui("u", 0);
  F->MulM (em["u"], z, s_inv, em[n]);

  EC->MakeElem_ui ("v", 0);
  F->MulM (em["v"], r, s_inv, em[n]);
  
  EC->MakePoint_ui (uG, 0, 0);
  bool uG_res = EC->Mul_scalar (uG, "u", "G");
  
  EC->MakePoint_ui (vP, 0, 0);
  bool vP_res = EC->Mul_scalar (vP, "v", "P"); 
  
  EC->PrintPoint (uG, uG, ffm::format::hex);
  EC->PrintPoint (vP, vP, ffm::format::hex);
  
  EC->MakePoint_ui ("R", 0, 0);
  
  EC->AddPoint ("R", "uG", "vP"); 

  
  ffm::pt::struc& R = pm["R"];
  bool oncurve = EC->IsPointOnCurve (pt::x(R), pt::y(R)); 
  if (oncurve) {
    printf ("R is on curve\n"); 
  }
  
  EC->PrintElem ("R.x|", pt::x(R), ffm::format::hex);
  EC->PrintElem ("  r|", r, ffm::format::hex);

  printf ("%s|%i:EXIT\n", __FUNCTION__, __LINE__); 
  return (F->Cmp (pt::x(R), r) == 0);
}

bool ECDSA_Verify (ffm::el::map& elmap, ffm::pt::map& pointmap, ffm::FEConPtr F, ffm::ECConRef EC, const char* sz_z, const char* sz_r, const char* sz_s) {
  printf ("%s|%i:ENTER\n", __FUNCTION__, __LINE__); 
  using namespace ffm;
  
  // uG + vP = R 
  // u   = z/s
  // v   = r/s
  
  // s   = (z+re)/k
  // 1/s = k/(z+re)
  ffm::ScopeDeleter dr (F);
  
  ffm::FE_t z = dr (F->New(sz_z, 0));
  ffm::FE_t r = dr (F->New(sz_r, 0));
  ffm::FE_t s = dr (F->New(sz_s, 0));
  
  ffm::FE_t s_inv = dr(F->New());
  
  const std::string n = "n"; 
  const std::string u = "u"; 
  const std::string v = "v";
  const std::string vP = "vP";
  const std::string uG = "uG"; 
  
  // powm (out,  
  ffm::FE_t n_minus_2 = dr(F->New());
  F->Sub_ui(n_minus_2, elmap[n], 2);
  //EC->PrintElem ("{n-2}", n_minus_2, ffm::format::hex);
  
  F->PowM (s_inv, s, n_minus_2, elmap[n]); 
  //EC->PrintElem ("s_inv", s_inv, ffm::format::hex); 
  
  EC->MakeElem_ui("u", 0);
  F->MulM (elmap["u"], z, s_inv, elmap[n]);

  EC->MakeElem_ui ("v", 0);
  F->MulM (elmap["v"], r, s_inv, elmap[n]);
  
  EC->MakePoint_ui (uG, 0, 0);
  bool uG_res = EC->Mul_scalar (uG, "u", "G");
  
  EC->MakePoint_ui (vP, 0, 0);
  bool vP_res = EC->Mul_scalar (vP, "v", "P"); 
  
  EC->PrintPoint (uG, uG, ffm::format::hex);
  EC->PrintPoint (vP, vP, ffm::format::hex);
  
  EC->MakePoint_ui ("R", 0, 0);
  
  EC->AddPoint ("R", "uG", "vP"); 

  
  ffm::pt::struc& R = pointmap["R"];
  bool oncurve = EC->IsPointOnCurve (pt::x(R), pt::y(R)); 
  if (oncurve) {
    printf ("R is on curve\n"); 
  }
  
  EC->PrintElem ("R.x|", pt::x(R), ffm::format::hex);
  EC->PrintElem ("  r|", r, ffm::format::hex);

  printf ("%s|%i:EXIT\n", __FUNCTION__, __LINE__); 
  return (F->Cmp (pt::x(R), r) == 0);
}


bool ECDSA_Verify (const SECzy::Signature& sig, const SECzy::PublicKey& pubk, const af::digest32& z_msg) {

  using namespace ffm;
  
  // uG + vP = R 
  // u = z/s
  // v = r/s
  // s = (z+re)/k

  SECzy::secp256k1 verifier;



  return verifier.Verify  (sig, pubk, z_msg); 

} 

//
bool ECDSA_Sign (SECzy::Signature& sig, const SECzy::digest32& zbin, const SECzy::PrivateKey& privk) {

  using namespace ffm;
  // eG = P
  ffm::el::map em;
  ffm::pt::map pm;
  
  ffm::FEConPtr F  = ffm::Create_FE_context (kSEC256k1_p_sz, 0);
  ffm::ECConRef EC = ffm::Create_EC_context (F, em, pm, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);
  
  //const unsigned char z_bin[] = ""; // where do we get this from
  //const unsigned char prk_bin[32] = {};
  
  const std::string G = "G"; 
  const std::string P = "P";
  const std::string R = "R";
  const std::string n = "n";

  ScopeDeleter dr (F); 
  // kG = R
  FE_t
    e    = dr(F->New_bin(std::data(privk.e), 32, false)),
    z    = dr(F->New_bin(std::data(zbin), 32, false)),
    s    = dr(F->New ()),
    stmp = dr(F->New()),

    tmp  = dr(F->New()),
    snum = dr(F->New ());

  EC->MakePoint (G, kSEC256k1_G_x_sz, kSEC256k1_G_y_sz, 0); 
  EC->MakePoint_ui(P, 0, 0);

  EC->Mul_scalar (P, e, G); 
  EC->PrintPoint (" eG|", P, format::hex); 

  
  // n-2
  FE_t n_sub_2 = dr(F->New());
  F->Sub_ui(n_sub_2, em[n], 2);
  em["n-2"] = n_sub_2; 

  // n/2
  FE_t n_div_2 = dr(F->New()); 
  F->Div_ui(n_div_2, em[n], 2); 
  em["n/2"] = n_div_2;
  
  // k = rand (n) <-- fix later with real random
  FE_t k = dr(F->New()); 
  F->Rand (k, em[n]);
  em["k"] = k;

  F->Print ("rand k", em["k"]); 
  // 1/k

  FE_t k_inv= dr(F->New ());
  F->PowM (k_inv, k, n_sub_2, em[n]);  
  em["1/k"] = k_inv; 

  // kG = R
  EC->MakePoint_ui (R, 0, 0); // <-- we should just make a plain 'alloc-point'
  EC->Mul_scalar (R, "k", G); // we want R.x
  //EC->PrintPoint ("<[R]>", R, format::hex);
  //PrintPoint

  F->SMul (tmp, pt::x(pm[R]), e);
  F->SAdd (snum, z, tmp);
  F->MulM (stmp, snum, k_inv, em[n]); 
  // s = (z+re)/k mod n
  

  // if s > n/2
  //  s = n - s
  int cmpres = F->Cmp (stmp, em["n/2"]);
  // Compare op1 and op2. Return a positive value if op1 > op2, zero if op1 = op2, or a negative value if op1 < op2.
  if (cmpres > 0) {
    printf ("..(s > n/2)\n");
    F->Sub (s , em[n], stmp); 
  }
  else {
    F->Set (s, stmp);
  }

  // write results, sig is (r,s) => sig(r,s)
  {
    bytearray rraw, sraw;
    F->Raw (rraw, pt::x(pm[R]), false);
    ffm::copy_BE(sig.r, rraw); 

    F->Raw (sraw, s, false);
    ffm::copy_BE(sig.s, sraw); 

    printf ("__signature generated__\n");
  }
  
  printf ("%s : exiting\n", __FUNCTION__); 
  return true; 
  
}




//
//
void copy_padded_BE(af::byte32 &out, const af::bytearray &bytes) {

  if (bytes.size() > out.max_size())
    return;

  out.fill(0);

  // printf ("out.max_size:%zu\n", out.max_size());
  // printf ("bytes.size:%zu\n", bytes.size()); 
   std::copy (bytes.begin(), bytes.end(), out.end () - bytes.size());
   
}
 


int CH3_Ex(std::vector<std::string> &args) {

  printf ("%s[args]\n", __FUNCTION__);

  if (true) {   
    POUT("Ex. 3.6");  
// bool ECDSA_Verify (ffm::el::map& em, ffm::pt::map& pm, ffm::FEConPtr F, ffm::ECConRef EC, const SECzy::Signature& sig, const SECzy::PublicKey& pubk, const af::digest32& zhash) {

    ffm::el::map elmap;
    ffm::pt::map pointmap;

    ffm::FEConPtr F = ffm::Create_FE_context (kSEC256k1_p_sz, 0);
    ffm::ECConRef EC = ffm::Create_EC_context (F, elmap, pointmap, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);

    
    const std::string G = "G";
    const std::string P = "P";
    const std::string n = "n";
    
    EC->MakePoint (G, kSEC256k1_G_x_sz, kSEC256k1_G_y_sz, 0);
    EC->PrintPoint ("[G]", G, ffm::format::hex); 
    // P = (0x887387e452b8eacc4acfde10d9aaf7f6d9a0f975aabb10d006e4da568744d06c,
    // Verify whether these signatures are valid:
    // 0x61de6d95231cd89026e286df3b6ae4a894a3378e393e93a0f45b666329a0ae34)

    SECzy::PublicKey pubk; {
      const char kP_x_sz[] = "0x887387e452b8eacc4acfde10d9aaf7f6d9a0f975aabb10d006e4da568744d06c";
      const char kP_y_sz[] = "0x61de6d95231cd89026e286df3b6ae4a894a3378e393e93a0f45b666329a0ae34";
    
      EC->MakePoint (P, kP_x_sz, kP_y_sz, 0);
      EC->PrintPoint ("[P]", P, ffm::format::hex);

      ffm::bytearray xarr, yarr;
      F->Raw (xarr, ffm::pt::x(pointmap[P]), false); 
      F->Raw (yarr, ffm::pt::y(pointmap[P]), false); 
      ffm::copy_BE(pubk.x, xarr);
      ffm::copy_BE(pubk.y, yarr); 
    }

    SECzy::Signature sigA; {
      // # signature 1
      // z = 0xec208baa0fc1c19f708a9ca96fdeff3ac3f230bb4a7ba4aede4942ad003c0f60
      // r = 0xac8d1c87e51d0d441be8b3dd5b05c8795b48875dffe00b7ffcfac23010d3a395
      // s = 0x68342ceff8935ededd102dd876ffd6ba72d6a427a3edb13d26eb0781cb423c
      const char z1_sz[] = "0xec208baa0fc1c19f708a9ca96fdeff3ac3f230bb4a7ba4aede4942ad003c0f60"; 
      const char r1_sz[] = "0xac8d1c87e51d0d441be8b3dd5b05c8795b48875dffe00b7ffcfac23010d3a395"; 
      const char s1_sz[] = "0x68342ceff8935ededd102dd876ffd6ba72d6a427a3edb13d26eb0781cb423c4";

      ffm::bytearray rarr, sarr, zarr; 
      ffm::FE_t r1 = F->New(r1_sz, 0);
      
      F->Raw(rarr, r1, false);
      ffm::copy_BE (sigA.r, rarr); 
		    

      ffm::FE_t s1 = F->New(s1_sz, 0); 
      F->Raw(sarr, s1, false);
      ffm::copy_BE(sigA.s, sarr); 
            
      af::digest32 zbin;
      ffm::FE_t z_el = F->New (z1_sz, 0);
      F->Raw (zarr, z_el, false); 
      ffm::copy_BE (zbin, zarr); 

      
      af::checkres ("[Verify A]", true == ECDSA_Verify (elmap, pointmap, F, EC, sigA, pubk, zbin));
      // af::checkres ("[sig A]", true == ECDSA_Verify (elmap, pointmap, F, EC, z1_sz, r1_sz, s1_sz));
      
    }
    {
      // # signature 2
      // z = 0x7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d
      // r = 0xeff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c
      // s = 0xc7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab6
      
      const char sz_z2[] = "0x7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d";
      const char sz_r2[] = "0xeff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c";
      const char sz_s2[] = "0xc7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab6";

      af::checkres ("[Verify B]", true == ECDSA_Verify (elmap, pointmap, F, EC, sz_z2, sz_r2, sz_s2)); 
    }
  }

  
  if (true) {
    using namespace ffm;
    POUT ("\nEx. 3.7"); 

    
    SECzy::PrivateKey privk; { // init private key
      
      ffm::FEConRef F = ffm::Create_FE_context ( kSEC256k1_p_sz, 0 );


      ScopeDeleter dr (F);
      ffm::FE_t secret = dr(F->New_ui(12345));

      //
      F->Print("secret", secret); 
      af::bytearray sec_bytes;
      F->Raw (sec_bytes, secret, false);
      //print_digest(sec_bytes);

      ffm::copy_BE (privk.e, sec_bytes);
      // print_digest (privk.sec);

      ffm::FE_t reinit = F->New_bin( std::data(privk.e), 32, false); 
      F->Print ("reinit", reinit);
      //print_digest (privk.sec); 

    }

    //
    SECzy::PublicKey pubk; {
      POUT ("<public key>");
      // P = eG

      const std::string G = "G";
      const std::string P = "P";
      
      ffm::el::map em;
      ffm::pt::map pm; 
      ffm::FEConRef F = Create_FE_context ( kSEC256k1_p_sz, 0 );
      ffm::ECConRef EC = Create_EC_context (F, em, pm, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);


      af::bytearray xarr, yarr;
      ffm::FE_t e = F->New_bin (std::data(privk.e), 32, false);
      em["e"] = e;


      EC->MakePoint (G, kSEC256k1_G_x_sz, kSEC256k1_G_y_sz, 0);

      // EC->PrintPoint(" G|", "G", format::hex);
      // EC->PrintElem (" n|", "n", format::hex);
      // EC->PrintElem ("Fp|", "Fp", format::hex);
      // EC->PrintElem (" e|", "e", format::hex);

      EC->Mul_scalar (P, e, G);  
      EC->PrintPoint (" P|", P, ffm::format::hex); 
      
      F->Raw (xarr, ffm::pt::x(pm[P]), false);
      assert (xarr.size() == 32); 
      ffm::copy_BE (pubk.x, xarr);

      F->Raw (yarr, ffm::pt::y(pm[P]), false);
      assert (yarr.size () == 32); 
      ffm::copy_BE (pubk.y, yarr); 

      // print_bn ("P.x:", pubk.x);
      // print_bn ("P.y:", pubk.y);

    }
    //  
    SECzy::digest32 zbin; { // initialzie z_bin
      ffm::el::map em;
      ffm::pt::map pm; 
      ffm::FEConRef F = Create_FE_context ( kSEC256k1_p_sz, 0 );
      ffm::ECConRef EC = Create_EC_context (F, em, pm, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);

      const char hash_this[] = "Programming Bitcoin!";
      printf ("hash_this:%s\n", hash_this);

      af::hash256 (zbin, hash_this, strlen(hash_this));
      af::bytearray zbytes; 
      FE_t hashres = F->New_bin (std::data(zbin), 32, false);

      EC->PrintElem ("zbin:dec", hashres, format::dec); 
      print_bn ("zbin:hex", zbin);  
      };

    // signing it here 
    SECzy::Signature sig; {
      SECzy::secp256k1 signer;
      //af::checkres ("Sign X", ECDSA_Sign (sig, zbin, privk));
      
      af::checkres("Sign[X]", signer.Sign (sig, privk, zbin)); 

      
    }

    if (true) {
      SECzy::secp256k1 verifier; 
      af::checkres("Verify[X]", verifier.Verify (sig,  pubk, zbin)); 
    }
    else {

      ffm::el::map em;
      ffm::pt::map pm;
      
      ffm::FEConPtr F = ffm::Create_FE_context (kSEC256k1_p_sz, 0);
      ffm::ECConRef EC = ffm::Create_EC_context (F, em, pm, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);
      const std::string G = "G";
      const std::string P = "P"; 
      
      EC->MakePoint (G, kSEC256k1_G_x_sz, kSEC256k1_G_y_sz, 0);
      EC->PrintPoint ("[G|417]", G, ffm::format::hex); 
      POUT("580");
      print_bn("zbin", zbin); 
      
      af::checkres ("[verify X]", true == ECDSA_Verify (em, pm, F, EC, sig, pubk, zbin));
     
    } 
    
    // now check it here
    // af::checkres("verify a", ECDSA_Verify (sig, pubk, z_bin)); 
    		  
    
 }
  
  // uG + vP = R 
  // u = z/s
  // v = r/s
  // s = (z+re)/k
  
  printf ("leaving:%s\n", __FUNCTION__); 
  return 0; 
  
}
