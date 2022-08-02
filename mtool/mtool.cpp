//
////
////from lilarch 
#include "mtool.h"
#include "SECzy/secp256k1.h"
#include "aframe/binary_IO.h"
#include "aframe/hash.h"
#include "aframe/utility.h"
#include "ffm/ec_math.h"
#include "ffm/fe_math.h"

// zero mq messaging
#include <string>
#include <zmq.h>

//  SECP25
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


template <typename Ty> 
std::string from (const Ty& srcbin)
{

    std::string ret = ""; 
    for (auto x : srcbin) {
    ret +=   af::hex::from_uc (x);
  }
    
  return ret;
} 

//
//
int CH4_Ex (std::vector<std::string>& args) {
  printf ("%s[args]:ENTER\n", __FUNCTION__);
  using namespace af;
  using namespace ffm; 
  using namespace SECzy;

  
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
    int  secr_b   = 2018; // ^5;  
    char secr_c[] = "0xdeadbeef12345";

    bytearray wsbuf (1024);  
    WriteStreamRef ws = CreateWriteMemStream (std::data(wsbuf), wsbuf.size()); 
    
    PrivateKey prk1, prk2, prk3; {
      
      ScopeDeleter dr (F);
      bytearray    arrtmp;
     
      FE_t e1 = dr(F->New_ui (secr_a));



      unsigned char pref = 4; // '0x4'
     
      ws->Write(&pref, 1);
      
      EC->MakePoint  ("P1", dr (F->New()), dr (F->New()));
      EC->Mul_scalar ("P1", e1, "G"); 

      F->Raw (arrtmp, pt::x(pm["P1"]), false);
      assert (arrtmp.size() == 32);

      ws->Write (std::data(arrtmp), arrtmp.size());

      F->Raw (arrtmp, pt::y(pm["P1"]), false);
      assert (arrtmp.size() == 32);

      ws->Write (std::data(arrtmp), arrtmp.size());

      
      std::string hexstr1;
     
      hex::encode (hexstr1, wsbuf);

      printf ( "hexstr1[%zu]: %s\n " , hexstr1.size(), hexstr1.c_str());
 


      
      copy_BE(prk1.e, arrtmp); 

      std::string hex_str = from (prk1.e); 

      printf ( "e1:%s\n" , hex_str.c_str ()); 
      
      FE_t e2 = dr(F->New_ui (2018));
      F->Pow_ui (e2, e2, 5); // 2018^5
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
  {

    bytearray membuf (1024); 
    
    // 4.2
    Point P;
    {

    }
    WriteStreamRef ws = CreateWriteMemStream (std::data(membuf), membuf.size()); 

    SECzy::WritePoint (ws , P, true);

    // 5001
    // 20195
    // 0xdeadbeef54321

  }


  { // 4.3

    // Find the DER format for a signature whose r and s values are:
    // r =0x37206a0610995c58074999cb9767b87af4c4978db68c06e8e6e81d282047a7c6
    // s =0x8ca63759c1157ebeaec0d03cecca119fc9a75bf8e6d0fa65c841c8e2738cdaec

  }


  { // Exercise 4

    // Convert the following hex to binary and then to Base58:
    // 7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d
    // eff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c
    // c7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab6



  }




  { // Exercise 5

    //Find the address corresponding to Public Keys whose Private Key secrets are:

    // 5002 (use uncompressed SEC, on testnet)
    // 20205 (use compressed SEC, on testnet)
    // 0x12345deadbeef (use compressed SEC on mainnet)

  }

  { // Exercise 6

    // Find the WIF for Private Key whose secrets are:

    // 5003  (compressed, testnet)
    // 20215 (uncompressed, testnet)
    // 0x54321deadbeef (compressed, mainnet)

  }





  
  {
     SECzy::WriteSignature_DER;
  }  



  
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
    af::base58::encode (sec_a_base58, std::data(outbytes) , outbytes.size());
    printf ("base69: %s\n", sec_a_base58.c_str());

    F->Raw(outbytes, sec_b, false);
    std::string sec_b_base58;
    base58::encode (sec_b_base58, std::data(outbytes), outbytes.size());
    printf ("base69: %s\n", sec_b_base58.c_str());
    
    F->Raw(outbytes, sec_c, false);
    std::string sec_c_base58;
    base58::encode (sec_c_base58, std::data(outbytes), outbytes.size()+);
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
