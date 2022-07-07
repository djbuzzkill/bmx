//#include <glm/glm.hpp>
//#include <stdio.h>


#include "mtool.h"
#include "aframe/af.h"

#include <openssl/sha.h>

//#include <crypto++/cryptlib.h>
//#include <cryptopp/sha.h>
//#include <cryptopp/ripemd.h>

//#include "sha2.h"

// big num math
// #include <gmp.h>

// zero mq messaging
#include <zmq.h>

//  SECP256k1  stuff


//
const char kSEC256k1_p_sz[]       = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f";
const char kSEC256k1_G_x_sz[]     = "0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798";
const char kSEC256k1_G_y_sz[]     = "0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8";
const char kSEC356k1_coeff_a_sz[] = "0x0";
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



bool ECDSA_Sign (ffm::el::map& em, ffm::pt::map& pm, ffm::FEConPtr F, ffm::ECConRef EC, const unsigned char* z_bin, const unsigned char* priv_k) 
{
  using namespace ffm;
  // eG = P
  
  const std::string G = "G"; 
  const std::string P = "P";
  const std::string R = "R";
  const std::string n = "n";
  
  ScopeDeleter dr (F); 
  // kG = R
  FE_t
    prk = dr(F->New_bin (priv_k, true, 32)),
    z = dr (F->New_bin (z_bin, true, 32)), 
    s = dr (F->New ()),
    r = dr (F->New ()),
    k = dr (F->New ()),
    tmp = dr(F->New()),
    snum = dr (F->New ()),
    k_inv = dr (F->New ());

  F->Rand (k, em[n]);
  em["k"] = k; 


  F->PowM (k_inv, k, em["n-2"], em[n]);  
  em["1/k"] = k_inv; 

  
  bytearray r_raw, s_raw;
  // kG = R
  EC->MakePoint_ui (R, 0, 0); // <-- we should just make a plain 'alloc-point'
  EC->Mul_scalar (R, "k", G); // we want R.x

  // s = (z+re)/k
  F->Mul (tmp, pt::x(pm[R]), prk);
  F->Add (snum, z, tmp);
  F->Mul (s, snum, k_inv);  

  // get the binary , sig is (r,s) => sig(r,s) 
  F->Raw (r_raw, true, pt::x(pm[R]));
  F->Raw (s_raw, true, s);  
    
  
 return false; 
  
}


bool ECDSA_Verify (ffm::el::map& elmap, ffm::pt::map& pointmap, ffm::FEConPtr F, ffm::ECConRef EC, const char* sz_z, const char* sz_r, const char* sz_s) {

  using namespace ffm;
  
  // uG + vP = R 
  // u = z/s
  // v = r/s
  // s = (z+re)/k
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
  EC->PrintElem ("{n-2}", n_minus_2, ffm::format::hex);
  
  F->PowM (s_inv, s, n_minus_2, elmap[n]); 
  EC->PrintElem ("s_inv", s_inv, ffm::format::hex); 
  
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
  
  EC->PrintElem ("R.x", pt::x(R), ffm::format::hex);
  EC->PrintElem ("r", r, ffm::format::hex);
  return (F->Cmp (pt::x(R), r) == 0);
}
  
 
int CH3_Ex(std::vector<std::string> &args)
{
  printf ("%s[args]\n", __FUNCTION__);


  if (true) 
  {   
    POUT("Ex. 3.6");  

    ffm::el::map elmap;
    ffm::pt::map pointmap;

    ffm::FEConPtr F = ffm::Create_FE_context (kSEC256k1_p_sz, 0);
    ffm::ECConRef EC = ffm::Create_EC_context (F, elmap, pointmap, kSEC356k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);

    
    const std::string G = "G";
    const std::string P = "P";
    const std::string n = "n";
    
    af::checkres ("G",  EC->MakePoint (G, kSEC256k1_G_x_sz, kSEC256k1_G_y_sz, 0));
    EC->PrintPoint ("[G]", G, ffm::format::hex); 
    // P = (0x887387e452b8eacc4acfde10d9aaf7f6d9a0f975aabb10d006e4da568744d06c,
    // Verify whether these signatures are valid:
    // 0x61de6d95231cd89026e286df3b6ae4a894a3378e393e93a0f45b666329a0ae34)
    const char kP_x_sz[] = "0x887387e452b8eacc4acfde10d9aaf7f6d9a0f975aabb10d006e4da568744d06c";
    const char kP_y_sz[] = "0x61de6d95231cd89026e286df3b6ae4a894a3378e393e93a0f45b666329a0ae34";
    
    af::checkres ("P", EC->MakePoint (P, kP_x_sz, kP_y_sz, 0));
    EC->PrintPoint ("[P]", P, ffm::format::hex); 
    {
      // # signature 1
      // z = 0xec208baa0fc1c19f708a9ca96fdeff3ac3f230bb4a7ba4aede4942ad003c0f60
      // r = 0xac8d1c87e51d0d441be8b3dd5b05c8795b48875dffe00b7ffcfac23010d3a395
      // s = 0x68342ceff8935ededd102dd876ffd6ba72d6a427a3edb13d26eb0781cb423c
      const char z1_sz[] = "0xec208baa0fc1c19f708a9ca96fdeff3ac3f230bb4a7ba4aede4942ad003c0f60"; 
      const char r1_sz[] = "0xac8d1c87e51d0d441be8b3dd5b05c8795b48875dffe00b7ffcfac23010d3a395"; 
      const char s1_sz[] = "0x68342ceff8935ededd102dd876ffd6ba72d6a427a3edb13d26eb0781cb423c4";
      
      af::checkres ("sig a", true == ECDSA_Verify (elmap, pointmap, F, EC, z1_sz, r1_sz, s1_sz));
      
    }
    {
      // # signature 2
      // z = 0x7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d
      // r = 0xeff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c
      // s = 0xc7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab6

      const char sz_z2[] = "0x7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d";
      const char sz_r2[] = "0xeff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c";
      const char sz_s2[] = "0xc7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab6";

      af::checkres ("sig b", true == ECDSA_Verify (elmap, pointmap, F, EC, sz_z2, sz_r2, sz_s2)); 
    }
  }

  
  if (true) {
    POUT ("Ex. 3.7"); 

    ffm::el::map El;
    ffm::pt::map Pt;

    ffm::FEConPtr F = ffm::Create_FE_context (kSEC256k1_p_sz, 0);
    ffm::ECConRef EC = ffm::Create_EC_context (F, El, Pt, kSEC356k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);

    const unsigned char z_bin[] = ""; // where do we get this from

    const unsigned char prk_bin[32] = {};

    af::checkres ("sign a", ECDSA_Sign (El, Pt, F, EC, z_bin, prk_bin));
  }
  
  //EC->PrintPoint("R1:", "R1", ffm::format::HEX);
  //EC->PrintElem ("r1:", "r1", ffm::format::HEX);
  
   
  //EC->DefElem ("z2", "0x7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d", 0); 
  //EC->DefElem ("r2", "0xeff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c", 0);
  //EC->DefElem ("s2", "0xc7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab6", 0); 		 
  // uG + vP = R 
  // u = z/s
  // v = r/s
  // s = (z+re)/k
  
  printf ("leaving:%s\n", __FUNCTION__); 
  return 0; 
  
}


//
//
int CH4_test (std::vector<std::string>& args) 
{
  printf ("%s[args]\n", __FUNCTION__); 

  ffm::FEConPtr F = ffm::Create_FE_context (kSEC256k1_p_sz);
  ffm::ECConRef EC = ffm::Create_EC_context (F, kSEC356k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 16);


  const std::string G = "G";
  const std::string P = "P";
  
  af::checkres ("G",  EC->MakePoint (G, kSEC256k1_G_x_sz, kSEC256k1_G_y_sz, 0));

  if (true)
    {
      POUT("Ex. 4.1"); 
      
      // Find the Compressed SEC format for the Public Key where the Private Key secrets are:
      int secr_a = 5000; 
      int secr_b = 20185;  
      char secr_c[] = "0xdeadbeef12345";

      struct symstruc {

	  std::string A;  
	  std::string B;  
	  std::string C;  


        } ;


      const std::string  
	Sec_A = "Sec.A",
	Sec_B = "Sec.B",  
	Sec_C = "Sec.C";  
      
      
      EC->MakeElem_ui (Sec_A, secr_a);
      EC->MakeElem_ui (Sec_B, secr_b);
      EC->MakeElem    (Sec_C, secr_c, 0);


      EC->PrintElem (Sec_A, Sec_A, ffm::format::dec); 
      EC->PrintElem (Sec_B, Sec_B, ffm::format::dec);  
      EC->PrintElem (Sec_C, Sec_C, ffm::format::hex);
      
    }
  return 0; 
}

//
//
void print_digest (const std::array<unsigned char, 32>& dig) {
  for (auto& c : dig) {
    std::string hx = af::hex_uc(c);
    
    printf ("[%i|0x%s|%i]\n", c, hx.c_str(), af::uc_hex (hx));
  }
}

//
//
int test_gcrypt (const std::vector<std::string>& args)
{
  printf ("\n%s:begin\n", __FUNCTION__); 
 
  af::digest32 rnd1, rnd2;

  const char msgtxt[] =
    "i dont have much to say about it";

  const char msg0[] = "fjwekl;4v@#%&WQhjWDDDT2kl+;rwjekl;@:>-{|}fSfsafsa'e vtj34l@vt43j3kqlgjrergej5l!?j54394%$#@^423Q%"; 
  af::sha256 (rnd1, msg0, strlen(msg0));
  af::sha256 (rnd2, std::data(rnd1), rnd1.size ());
  
  print_digest (rnd2); 

  printf ( "\n%s:end\n", __FUNCTION__); 
  return 0; 
}

void printbytes (const std::string& lbl, const ffm::bytearray& bytes)
{
  //printf ("%s|size(bytes):%zu\n", bytes.size()); 
  printf ("bytes.size:%zu\n", bytes.size()); 
  // printf ("%s\n", bytes.size()); 
  for (int i = 0; i < bytes.size(); ++i)
    printf ("%s[%i]:%x\n", lbl.c_str(), i, bytes[i]);
  
}

//
//
//

int test_binary (const std::vector<std::string> &args) {

  printf ("\n%s:begin\n", __FUNCTION__);

  using namespace ffm;

  el::map em;
  pt::map pm;

  FEConPtr F = Create_FE_context ( kSEC256k1_p_sz, 0 );
  ECConRef EC = Create_EC_context(F, em, pm, kSEC356k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);
  
  ScopeDeleter dr (F);
  FE_t vara = dr(F->New_ui(1));
  
  bytearray bina, binb, binc; 

  F->Print ("Fp:", F->p()); 
  //  F->LogiShiftL(vara, 248);
  //printbytes ("bina", bina); 

	       
  F->Raw (binb, F->p(), false); 
  printbytes( "p", binb);
	    
  FE_t varnew = F->New (); 
  printf ("binb.size:%zu\n", binb.size());  
  F->Set_bin (varnew, std::data(binb), binb.size(), false); 

  F->Print ("[new_var]:", varnew);

  
  // print number 1, is it at first or last byte
  return 0;
}


// ----------------------- main --------------------------
int main (int argv, char** argc)
{
  std::vector<std::string> args (argc, argc+argv);

  // glm::vec3 v (2.1f, 2.2f, 2.3f);
  // glm::vec3 x = v + v;

  // test_gmp (args);
  //FE_test (args);

  test_binary (args);
  
  POUT("in between");
  
  //CH3_Ex(args);
    // test_gcrypt (args);
  POUT("af between");
  
  //CH4_test(args); 
  // test_gcrypt(args); 
  //  Test_CryptoPP(args); 
  //CH4_test (args);  
  // cx::Mouse_state ms;
  //auto f =   cx::foo  ; 

  return 0; 
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
  ffm::ECConRef EC = ffm::Create_EC_context (F, kSEC356k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);  

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
  
  //ffm::EC_IsPointOnCurve (); 
  // EC->IsPointOnCurve (Eq, G); 

 
}


