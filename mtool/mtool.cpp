

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

  using namespace af;
  using namespace ffm; 
  using namespace bmx; 
  
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


// 
struct CH6_Fn { int operator()(const std::vector<std::string>& args) {
    //
  printf ("%s:ENTER\n", __FUNCTION__); 
  //
  using namespace af;
  using namespace ffm; 
  using namespace bmx;
  
  // pt::map pm;
  // el::map em;
  
  // FEConRef F  = Create_FE_context (kSEC256k1_p_sz);
  // ECConRef EC = Create_EC_context (F, em, pm,
  // 				   kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz,
  // 				   kSEC256k1_n_sz, 0);
  
  //  ScopeDeleter dr(F);
  {
    PR ("CH 6.1.2\n"); 
    // from script import Script
    bytearray z_bin;
    af::digest32 z; 
    // z                       = 0x7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d
    const std::string z_xstr  = "7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d";
    //z                      = 0x7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d
    copy_BE (z, hex::decode (z_bin, z_xstr));
    
    bytearray sec_bin;  
    // sec       = bytes.fromhex('04887387e452b8eacc4acfde10d9aaf7f6d9a0f975aabb10d006e4da568744d06c61de6d95231cd89026e286df3b6ae4a894a3378e393e93a0f45b666329a0ae34')
    const std::string sec_xstr = "04887387e452b8eacc4acfde10d9aaf7f6d9a0f975aabb10d006e4da568744d06c61de6d95231cd89026e286df3b6ae4a894a3378e393e93a0f45b666329a0ae34";
    hex::decode (sec_bin, sec_xstr); 


    PublicKey pubk;
    size_t read_pubkey_size = ReadPoint (pubk, CreateReadMemStream (&sec_bin[0], sec_bin.size())); 
    print_point ("from SECbin", pubk); 
    
  
    bytearray sig_bin; 
    // sig       = bytes.fromhex('3045022000eff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c022100c7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab601')
    const std::string sig_xstr = "3045022000eff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c022100c7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab601";
    hex::decode (sig_bin, sig_xstr);

    bmx::Signature sig;
    size_t sig_size = sig_bin.size () - 1;

    bmx::ReadSignature_DER (sig, sig_size, CreateReadMemStream (&sig_bin[0], sig_size));

    print_sig ("sig_bin", sig); 
    // { PR ("wat"); 
    //   bytearray sig_bin0 = sig_bin;
    //   sig_bin0.pop_back();
    //   bmx::Signature sig;
    //   bmx::ReadSignature_DER (sig, sig_bin0.size (), CreateReadMemStream (&sig_bin0[0], sig_bin0.size ()));

    //   secp256k1 verifier;
    //   if (verifier.Verify (sig, pubk, z)) {
    // 	printf ("verifier.Verify..GOOD!!!!\n");
    //   }
    //   else {
    // 	printf ("verifier.Verify ..BAD :(\n"); 

    // 	}

    // }
    bmx::command_list script_sig = {
      script_element  (sig_bin), 
    };
    
    bmx::command_list script_pubkey = {
      script_element   (sec_bin), 
      script_operation (OP_CHECKSIG), // checksig
    };
    

    printf ( "size(z_bin):%zu\n", z_bin.size()); 
    
    bmx::command_list comb_script; 
    
    for (auto& el : script_sig)
      comb_script.push_back (el);
    
    for (auto& el : script_pubkey)
      comb_script.push_back (el);
    
    if (bmx::EvalScript (comb_script, z)) {
      PR ("!! bmx::EvalScript (comb_script) SUCCESS!!"); 
    }
    else {
      PR ("!! FAILED  bmx::EvalScript (comb_script) !!");
    }
    
    PR ("!! 188) !!");
  }

  
    // script_pubkey = Script([sec, 0xac])
    // script_sig = Script([sig])
    // combined_script = script_sig + script_pubkey
    // print(combined_script.evaluate(z))
    
    
    // z = 0x7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d
    // sec = bytes.fromhex('04887387e452b8eacc4acfde10d9aaf7f6d9a0f975aabb10d006e4da568744d06c61de6d95231cd89026e286df3b6ae4a894a3378e393e93a0f45b666329a0ae34')
    // sig = bytes.fromhex('3045022000eff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c022100c7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab601')
    // script_pubkey = Script([sec, 0xac])
    // script_sig = Script([sig])
    // combined_script = script_sig + script_pubkey
    // print(combined_script.evaluate(z))
PR ("!! 205) !!");
  return 0; 

}};



//
//  
int CH6_Ex (std::vector<std::string>& args) {

  CH6_Fn fn;

  return fn (args); 
  
} //




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
  
    // test_gcrypt (args);
  printf ("%s:EXIT\n", __FUNCTION__); 
  return 0; 
}

