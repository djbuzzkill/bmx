//
////
////from cyberarch
#include "mtool.h"
#include "SECzy/secp256k1.h"
#include "aframe/hash.h"
#include "aframe/utility.h"
#include "ffm/ec_math.h"
#include "ffm/fe_math.h"

// zero mq messaging
#include <string>
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
void pr_hex (const char* lbl, const af::digest32& bytes) {

  printf ("%s: ", lbl);

  for (auto b : bytes) printf ("%02x ", b);
  
  printf ("\n"); 
  
}


//
//
int CH4_Ex (std::vector<std::string>& args) {
  printf ("%s[args]:ENTER\n", __FUNCTION__);
  using namespace af;
  using namespace ffm; 

  pt::map pm;
  el::map em;
  ffm::FEConRef F  = ffm::Create_FE_context (kSEC256k1_p_sz);
  ffm::ECConRef EC = ffm::Create_EC_context (F, em, pm, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);

  const std::string G = "G";
  const std::string P = "P";
  float fwat;

  
  af::checkres ("G",  EC->MakePoint (G, kSEC256k1_G_x_sz, kSEC256k1_G_y_sz, 0));

  if (true) {
    
    POUT("Ex. 4.1"); 
    
    // Find the Compressed SEC format for the Public Key where the Private Key secrets are:
    int  secr_a   = 5000; 
    int  secr_b   = 20185;  
    char secr_c[] = "0xdeadbeef12345";
    
    SECzy::PrivateKey prk1, prk2, prk3; {
      
      ScopeDeleter dr (F);
      bytearray    arrtmp;
      
      FE_t e1 = dr(F->New_ui (secr_a));
      F->Raw (arrtmp, e1, false);
      copy_BE(prk1.e, arrtmp); 
      
      FE_t e2 = dr(F->New_ui (20185));
      F->Raw (arrtmp, e2, false);
      copy_BE(prk2.e, arrtmp); 

      FE_t e3 = dr(F->New (secr_c, 0));
      F->Raw (arrtmp, e3, false);
      copy_BE(prk3.e, arrtmp);
    }

  } // ex 4.1

  
  // compressed SEC for ec points
  // 0x2 (x,y) 
  // 0x3 P.x   --> ripemd160
  // 0x4 P.x   --> ripemd160
  

  // ripemd160 for ec points


  
  // DER (Distinguished Encoding Rules) .. saves UP TO 6 BYTES!11 
  // r
  // s
  {
    // encoding into base58

    FEConRef F = Create_FE_context (kSEC256k1_p_sz, 0);
    ScopeDeleter dr (F);


    FE_t sec_a = dr (F->New_ui(5000));
    FE_t sec_b = dr (F->New_ui(20185));
    FE_t sec_c = dr (F->New ("0xdeadbeef12345", 0));


    bytearray outbytes;


    F->Raw (outbytes, sec_a, false); 
    std::string sec_a_base58; 
    af::base58::encode (sec_a_base58, std::data(outbytes) , outbytes.size(), true);
    printf ("base69: %s\n", sec_a_base58.c_str());

    F->Raw(outbytes, sec_b, false);
    std::string sec_b_base58;
    base58::encode (sec_b_base58, std::data(outbytes), outbytes.size(), false);
    printf ("base69: %s\n", sec_b_base58.c_str());
    
    F->Raw(outbytes, sec_c, false);
    std::string sec_c_base58;
    base58::encode (sec_c_base58, std::data(outbytes), outbytes.size(), false);
    printf ("base69: %s\n", sec_c_base58.c_str());

    

  }
  
  // base58

  // WIF
  // 

  //printf ("numelems:%i\n", numelems); 
  
  printf ("%s[args]:EXIT\n", __FUNCTION__); 
  
  return 0; 
}




// ----------------------- main --------------------------
int main (int argv, char** argc) {

  printf ("%s:ENTER\n", __FUNCTION__); 
    // test_gcrypt (args);
  std::vector<std::string> args (argc, argc+argv);
  
  POUT("af between");
  //test_gcrypt (args);
  
  CH4_Ex(args);

    // test_gcrypt (args);
  printf ("%s:EXIT\n", __FUNCTION__); 
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
