

////
////from lilarch 
#include "mtool.h"
#include "bmx/secp256k1.h"
#include "bmx/transaction.h"
#include "bmx/script.h"
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

using namespace af;
using namespace ffm; 
using namespace bmx; 
  

// y^2 = x^3 +ax^2+b
template<typename Seq>
inline void pr_hex (const char* lbl, const Seq& bytes) {

  printf ("%s: ", lbl);

  for (auto b : bytes) printf ("%02x ", b);
  
  printf ("\n"); 
  
}


;;
;;
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
int thicnspicy  (std::vector<std::string>& args)  {

  printf ("%s:ENTER\n", __FUNCTION__); 

  pt::map pm;
  el::map em;

  FEConRef F  = Create_FE_context (kSEC256k1_p_sz);
  ECConRef EC = Create_EC_context (F, em, pm,
				   kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz,
				   kSEC256k1_n_sz, 0);
  


  ScopeDeleter dr(F);
  //
  bytearray   ar;
  PublicKey   pubk;
  PrivateKey  privatek; 
  std::string addressa;
  bytearray   amem (256, byte{0x0});
  
  const std::string msgfile = "/home/djbuzzkill/secret.msg"; 
  printf  ( "msgfile:%s\n", msgfile.c_str());    
  
  bytearray secretbytes; 
  
  af::From_file (secretbytes, msgfile); 
  //secretbytes.push_back (0); 

  
  const std::string secretstr (secretbytes.size(), '*'); 

  printf ("secretstr:%s\n", secretstr.c_str()); 
  
  std::string encodedstr = "REPLACE THIS STRING" ; 

  af::From_file (encodedstr, msgfile); 
    
  af::hash256 (privatek, encodedstr.c_str(), encodedstr.size ()); 
  printf ("privatek[%zu]\n", privatek.size ());
  
  //FE_t secr_a = dr(F->New_ui(5002));
  MakePublicKey (pubk, privatek);
  MakeAddress   (addressa, true, false, pubk);
  printf ("_ADDRESS:%s\n", addressa.c_str()); 
  printf ("expected:%s\n", "mmTPbXQFxboEtNRkwfh6K51jvdtHLxGeMA"); 
    
  printf ("%s:EXIT\n", __FUNCTION__); 
  
  return 0; 
}      


void print_point (const std::string& lbl, const bmx::Point& p){

  std::string xstr, ystr; 

  af::hex::encode (xstr, &p.x[0], p.x.size()); 
  af::hex::encode (ystr, &p.y[0], p.y.size()); 

  printf (" %s {x:%s, y:%s}\n", lbl.c_str(), xstr.c_str(),  ystr.c_str()); 
  
}


void print_sig (const std::string& lbl, const bmx::Signature& s){

  std::string rstr, sstr; 

  af::hex::encode (rstr, &s.r[0], s.r.size()); 
  af::hex::encode (sstr, &s.s[0], s.s.size()); 

  printf (" %s {r:%s, s:%s}\n", lbl.c_str(), rstr.c_str(),  sstr.c_str()); 
  
}

void print_sig (const bmx::Signature& s) { 
  std::string lbl = "Sig";  
  print_sig (lbl, s); 
  
}




#include "ch06_ex.cpp"
#include "ch07_ex.cpp"
#include "ch08_ex.cpp"
#include "ch09_ex.cpp"
#include "ch10_ex.cpp"
#include "ch11_ex.cpp"
#include "ch12_ex.cpp"
#include "ch13_ex.cpp"


// ----------------------- main --------------------------
int main (int argv, char** argc) {

  printf ("%s:ENTER\n", __FUNCTION__); 
    // test_gcrypt (args);
  std::vector<std::string> args (argc, argc+argv);
  
  //test_gcrypt (args);
  
  // CH4_Ex(args);
  //thicnspicy (args);
  //test_read_SEC_bin (args); 

  CH6_Ex(args);
  //test_sig_hash (args);

  CH7_Ex(args); 
  CH8_Ex(args);
  CH9_Ex(args); 
  CH10_Ex(args); 
  CH11_Ex(args); 
  CH12_Ex(args); 
  CH13_Ex(args); 
  
    // test_gcrypt (args);
  printf ("%s:EXIT\n", __FUNCTION__); 
  return 0; 
}

