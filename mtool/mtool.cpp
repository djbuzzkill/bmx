

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


void pr_hex (const char* lbl, const af::digest32& bytes) {

  printf ("%s: ", lbl);

  for (auto b : bytes) printf ("%02x ", b);
  
  printf ("\n"); 
  
}


void print_digest (const std::array<unsigned char, 32>& dig) {
  for (auto& c : dig) {
    std::string hx = af::hex_uc(c);
    
    printf ("[%i|0x%s|%i]\n", c, hx.c_str(), af::uc_hex (hx));
  }
}

void print_digest (const af::bytearray& arr) {
  for (auto& c : arr) {
    std::string hx = af::hex_uc(c);
    
    printf ("[%i|0x%s]\n", c, hx.c_str());
  }
}

//
//
int CH4_Ex (std::vector<std::string>& args) {
  printf ("%s[args]\n", __FUNCTION__); 

  ffm::FEConPtr F = ffm::Create_FE_context (kSEC256k1_p_sz);
  ffm::ECConRef EC = ffm::Create_EC_context (F, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 16);


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
  ECConRef EC = Create_EC_context(F, em, pm, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);
  
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

  // test_binary (args);
  
  //POUT("in between");
  
  //CH3_Ex(args);
    // test_gcrypt (args);
  POUT("af between");
  
  CH4_Ex(args);

  return 0; 
}



