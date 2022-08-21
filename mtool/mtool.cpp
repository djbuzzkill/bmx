

////
////from lilarch 
#include "mtool.h"
#include "curv/secp256k1.h"
#include "curv/transaction.h"
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
struct CH5_Fn {

  int operator()(const std::vector<std::string>& args) {
    
    printf ("%s:ENTER\n", __FUNCTION__); 

    using namespace af;
    using namespace ffm; 
    using namespace curv;

    { POUT ("Ex 5.5"); 
      //
      pt::map pm;
      el::map em;
      FEConRef F  = Create_FE_context (kSEC256k1_p_sz);
      ECConRef EC = Create_EC_context (F, em, pm,
				       kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz,
				       kSEC256k1_n_sz, 0);
      ScopeDeleter dr(F);
      
      //What is the ScriptSig from the second input, ScriptPubKey from
      //the first output and the amount of the second output for this
      //transaction?
      
      // 1332 chars
      std::string tx_hex = "010000000456919960ac691763688d3d3bcea9ad6ecaf875df5339e148a1fc61c6ed7a069e010000006a47304402204585bcdef85e6b1c6af5c2669d4830ff86e42dd205c0e089bc2a821657e951c002201024a10366077f87d6bce1f7100ad8cfa8a064b39d4e8fe4ea13a7b71aa8180f012102f0da57e85eec2934a82a585ea337ce2f4998b50ae699dd79f5880e253dafafb7feffffffeb8f51f4038dc17e6313cf831d4f02281c2a468bde0fafd37f1bf882729e7fd3000000006a47304402207899531a52d59a6de200179928ca900254a36b8dff8bb75f5f5d71b1cdc26125022008b422690b8461cb52c3cc30330b23d574351872b7c361e9aae3649071c1a7160121035d5c93d9ac96881f19ba1f686f15f009ded7c62efe85a872e6a19b43c15a2937feffffff567bf40595119d1bb8a3037c356efd56170b64cbcc160fb028fa10704b45d775000000006a47304402204c7c7818424c7f7911da6cddc59655a70af1cb5eaf17c69dadbfc74ffa0b662f02207599e08bc8023693ad4e9527dc42c34210f7a7d1d1ddfc8492b654a11e7620a0012102158b46fbdff65d0172b7989aec8850aa0dae49abfb84c81ae6e5b251a58ace5cfeffffffd63a5e6c16e620f86f375925b21cabaf736c779f88fd04dcad51d26690f7f345010000006a47304402200633ea0d3314bea0d95b3cd8dadb2ef79ea8331ffe1e61f762c0f6daea0fabde022029f23b3e9c30f080446150b23852028751635dcee2be669c2a1686a4b5edf304012103ffd6f4a67e94aba353a00882e563ff2722eb4cff0ad6006e86ee20dfe7520d55feffffff0251430f00000000001976a914ab0c0b2e98b1ab6dbf67d4750b0a56244948a87988ac005a6202000000001976a9143c82d7df364eb6c75be8c80df2b3eda8db57397088ac46430600"; 

      bytearray tx_bytes; 
      hex::decode (tx_bytes, tx_hex);

      ReadStreamRef rs = CreateReadMemStream (&tx_bytes[0], tx_bytes.size());

      printf  ("tx_bytes.size(%zu)\n", tx_bytes.size()); 
    
      Transaction tx; 
      size_t readlen = ReadTransaction (tx, rs);

      printf ("readlen:%zu\n", readlen); 
  
    }

    { POUT ("Ex 5.6"); 
      //
      pt::map pm;
      el::map em;
      FEConRef F  = Create_FE_context (kSEC256k1_p_sz);
      ECConRef EC = Create_EC_context (F, em, pm,
				       kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz,
				       kSEC256k1_n_sz, 0);
      ScopeDeleter dr(F);
      //

    

    }

  
    printf ("%s:EXIT\n", __FUNCTION__); 
    return 0; 
  }

}; 


//  
int CH5_Ex (std::vector<std::string>& args) {

  CH5_Fn fn;

  return fn (args); 
  
}
  

//
//

//
//
int thicnspicy  (std::vector<std::string>& args)  {

  printf ("%s:ENTER\n", __FUNCTION__); 

  using namespace af;
  using namespace ffm; 
  using namespace SECzy; 
  
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
  bytearray   amem (256, 0x0);
  
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




// ----------------------- main --------------------------
int main (int argv, char** argc) {

  printf ("%s:ENTER\n", __FUNCTION__); 
    // test_gcrypt (args);
  std::vector<std::string> args (argc, argc+argv);
  
  //test_gcrypt (args);
  
  // CH4_Ex(args);
  //thicnspicy (args);
  CH5_Ex(args);
  
    // test_gcrypt (args);
  printf ("%s:EXIT\n", __FUNCTION__); 
  return 0; 
}

