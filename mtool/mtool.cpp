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
std::string hexfrom (const Ty& srcbin) {
    std::string ret = ""; 
    for (auto x : srcbin) {
    ret +=   af::hex::from_uc (x);
  }
    
  return ret;
} 


//
//
//
int Intermezzo (std::vector<std::string>& args)  {

  printf ("%s:ENTER\n", __FUNCTION__); 

  using namespace af;
  using namespace ffm; 
  using namespace SECzy; 
  
  pt::map pm;
  el::map em;
  ffm::FEConRef F  = ffm::Create_FE_context (kSEC256k1_p_sz);
  ffm::ECConRef EC = ffm::Create_EC_context (F, em, pm,
					     kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz,
					     kSEC256k1_n_sz, 0);
  ScopeDeleter dr(F);
  //
  bytearray   ar;
  PublicKey   pubk;
  PrivateKey  privatek; 
  std::string addressa;
  bytearray   amem (256, 0x0);
  
  const std::string msgfile = "/home/djbuzzkill/secret.msg"; 
  printf  ( "msgfile:%s\n", msgfile.c_str());    
  
  bytearray secretbytes; 
  
  af::From_file (secretbytes, msgfile); 
  secretbytes.push_back (0); 
  
  const std::string secretstr = (const char*) &secretbytes[0]; 
  printf ("secretstr:%s\n", secretstr.c_str()); 
  
  std::string encodedstr = "REPLACE THIS STRING" ; 

  af::From_file (encodedstr, msgfile); 

  printf ( "does this work? %s\n" , encodedstr.c_str ()); 
  
  if (encodedstr.size ()  == 0) {
    printf ("bad encoding \n");
    return 69; 
  }    

  printf ("encoded secret: %s | %zu\n", encodedstr.c_str(), encodedstr.size());
    
  af::hash256 (privatek, encodedstr.c_str(), encodedstr.size ()); 
  printf ("privatek[%zu]\n", privatek.size ());
  
  //FE_t secr_a = dr(F->New_ui(5002));
  MakePublicKey (pubk, privatek);
  MakeAddress   (addressa, true, false, pubk);
  printf ("__SECRET__:%s\n", addressa.c_str()); 
  printf ("  expected:%s\n", "mmTPbXQFxboEtNRkwfh6K51jvdtHLxGeMA"); 
  
    
  printf ("%s:EXIT\n", __FUNCTION__); 
  
  return 0; 
}      




// ----------------------- main --------------------------
int main (int argv, char** argc) {

  printf ("%s:ENTER\n", __FUNCTION__); 
    // test_gcrypt (args);
  std::vector<std::string> args (argc, argc+argv);
  
  POUT("af between");
  //test_gcrypt (args);
  
  // CH4_Ex(args);

  Intermezzo (args);
  
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
