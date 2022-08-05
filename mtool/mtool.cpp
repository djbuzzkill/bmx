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
int CH4_Ex (std::vector<std::string>& args) {
  printf ("%s[args]:ENTER\n", __FUNCTION__);
  using namespace af;
  using namespace ffm; 
  using namespace SECzy;

  const std::string G = "G";
  const std::string P = "P";
  
  if (true) {

  POUT("Ex. 4.1"); 
  
  pt::map pm;
  el::map em;
  ffm::FEConRef F  = ffm::Create_FE_context (kSEC256k1_p_sz);
  ffm::ECConRef EC = ffm::Create_EC_context (F, em, pm, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);
    
  af::checkres ("G",  EC->MakePoint (G, kSEC256k1_G_x_sz, kSEC256k1_G_y_sz, 0));
    
  // Find the Compressed SEC format for the Public Key where the Private Key secrets are:
  int  secr_a   = 5000; 
  int  secr_b   = 2018; // ^5;  
  char secr_c[] = "0xdeadbeef12345";

  bytearray wsbuf (1024);  
  WriteStreamRef ws = CreateWriteMemStream (std::data(wsbuf), wsbuf.size()); 
    
  PrivateKey prk1, prk2, prk3; {

  SECzy::fixnum32 e1_bin; 
  SECzy::Point    P1; 

  ScopeDeleter dr (F);
  bytearray    arrtmp;
  // #1. intialize 'e'
  FE_t e1 = dr(F->New_ui (secr_a));
  F->Raw (arrtmp, e1, false);
  copy_BE (prk1, arrtmp);
  
  std::string e1_hex ; 
  hex::encode (e1_hex, std::data(prk1), 32); 
  printf ( "e1:%s\n" , e1_hex.c_str ()); 
  // make the point
  SECzy::MakePublicKey (P1, prk1); 
  // write it
  SECzy::WritePoint (ws, P1, false); 
  // print back out as hex
  std::string hexstr1;
  hex::encode (hexstr1, std::data(wsbuf), ws->GetPos ());
  printf ( "hexstr1[%zu]: %s\n " , hexstr1.size(), hexstr1.c_str());
  // 04ffe558e388852f0120e46af2d1b370f85854a8eb0841811ece0e3e03d282d57c315dc72890a4f10a1481c031b03b351b0dc79901ca18a00cf009dbdb157a1d10
  
  // #2
  ws->SetPos (0, af::byte_stream::SeekMode::Abs); 
  assert (ws->GetPos () == 0);
  SECzy::Point P2; 
  FE_t e2 = dr(F->New_ui (2018));
  F->Pow_ui (e2, e2, 5); // 2018^5
  F->Raw (arrtmp, e2, false);
  copy_BE(prk2, arrtmp);
  
  SECzy::MakePublicKey (P2, prk2); 
  // write it
  SECzy::WritePoint (ws, P2, false); 
  // print back out as hex
     
  std::string hexstr2;
  hex::encode (hexstr2, std::data(wsbuf), ws->GetPos ());
  printf ( "hexstr2[%zu]: %s\n " , hexstr2.size(), hexstr2.c_str());


  // #3
  ws->SetPos (0, af::byte_stream::SeekMode::Abs); 
  assert (ws->GetPos () == 0);
  FE_t e3 = dr(F->New (secr_c, 0));
  F->Raw (arrtmp, e3, false);
  copy_BE(prk3, arrtmp);
  SECzy::Point P3;
  
  SECzy::MakePublicKey (P3, prk3); 
  // write it
  SECzy::WritePoint (ws, P3, false); 
  // print back out as hex
  
  std::string hexstr3;
  hex::encode (hexstr3, std::data(wsbuf), ws->GetPos ());
  printf ( "hexstr3[%zu]: %s\n " , hexstr3.size(), hexstr3.c_str());
  }
    
  } // ex 4.1





  { // 4.2
  
    pt::map pm;
    el::map em;
    ffm::FEConRef F  = ffm::Create_FE_context (kSEC256k1_p_sz);
    ffm::ECConRef EC = ffm::Create_EC_context (F, em, pm, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);


    ScopeDeleter dr (F); 
    // compressed SEC for ec points

    // * 5,001
    // * 2,019^5^
    // * 0xdeadbeef54321



    std::array<unsigned char, 256> writebuf; 
    WriteStreamRef ws = CreateWriteMemStream (std::data(writebuf), writebuf.size()); 

    bytearray arrtmp; 

    
    // #1. intialize 'e'
    PrivateKey privk1;
    SECzy::Point P1; 
    FE_t e1 = dr (F->New_ui (5001));
    F->Raw (arrtmp, e1, false);
    copy_BE (privk1, arrtmp);
    
    // make the point
    SECzy::MakePublicKey (P1, privk1); 
    // write it
    SECzy::WritePoint (ws, P1, true); 
    // print back out as hex
    std::string hex1;
    hex::encode (hex1, std::data(writebuf), ws->GetPos ());
    printf ( "hexstr1[%zu]: %s\n " , hex1.size(), hex1.c_str());
    //0357a4f368868a8a6d572991e484e664810ff14c05c0fa023275251151fe0e53d1


    
    // 2019**5
    ws->SetPos (0, byte_stream::SeekMode::Abs); 
    fixnum32 privk2;
    SECzy::Point P2; 
    FE_t e2 = dr(F->New_ui (2019));
    F->Pow_ui (e2, e2, 5); 
    F->Raw (arrtmp, e2, false);
    copy_BE (privk2, arrtmp);
    // make the point
    SECzy::MakePublicKey (P2, privk2); 
    // write it
    SECzy::WritePoint (ws, P2, true); 
    // print back out as hex
    std::string hex2;
    hex::encode (hex2, std::data(writebuf), ws->GetPos ());
    printf ( "hexstr2[%zu]: %s\n " , hex2.size(), hex2.c_str());
    // 02933ec2d2b111b92737ec12f1c5d20f3233a0ad21cd8b36d0bca7a0cfa5cb8701
    

    // 0xdeadbeef54321
    ws->SetPos (0, byte_stream::SeekMode::Abs); 
    fixnum32 privk3;
    SECzy::Point P3; 
    FE_t e3 = dr(F->New ("0xdeadbeef54321", 0 ));

    F->Raw (arrtmp, e3, false);
    copy_BE (privk3, arrtmp);
    
    // make the point
    SECzy::MakePublicKey (P3, privk3); 
    // write it
    SECzy::WritePoint (ws, P3, true); 
    // print back out as hex
    std::string hex3;
    hex::encode (hex3, std::data(writebuf), ws->GetPos ());
    printf ( "hexstr3[%zu]: %s\n " , hex3.size(), hex3.c_str());
    // 0296be5b1292f6c856b3c5654e886fc13511462059089cdf9c479623bfcbe77690
  }



   // 0x2 (x,y) 
  // 0x3 P.x   --> ripemd160
  // 0x4 P.x   --> ripemd160


  { // 4.3
  POUT("Ex. 4.3"); 
    pt::map pm;
    el::map em;
    ffm::FEConRef F  = ffm::Create_FE_context (kSEC256k1_p_sz);
    ffm::ECConRef EC = ffm::Create_EC_context (F, em, pm, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);


    ScopeDeleter dr (F); 
    //
    
    // Find the DER format for a signature whose r and s values are:
    // r = 0x37206a0610995c58074999cb9767b87af4c4978db68c06e8e6e81d282047a7c6 
    // s = 0x8ca63759c1157ebeaec0d03cecca119fc9a75bf8e6d0fa65c841c8e2738cdaec
    SECzy::Signature sig0; {
      bytearray abuf; 
      const char r0_hex_str[] = "0x37206a0610995c58074999cb9767b87af4c4978db68c06e8e6e81d282047a7c6"; 
      const char s0_hex_str[] = "0x8ca63759c1157ebeaec0d03cecca119fc9a75bf8e6d0fa65c841c8e2738cdaec";
    
      FE_t r0 = dr(F->New (r0_hex_str, 0));
      F->Raw (abuf, r0, false);
      copy_BE(sig0.r, abuf); 
      
      FE_t s0 = dr(F->New (s0_hex_str, 0));
      F->Raw (abuf, s0, false);
      copy_BE(sig0.s, abuf);
    }


    printf ("%s:r[0]:%s\n", __FUNCTION__, hex::from_uc ( sig0.r[0] ).c_str()); 
    printf ("%s:s[0]:%s\n", __FUNCTION__, hex::from_uc ( sig0.s[0] ).c_str()); 
    
    // >>> from ecc import Signature
    // >>> r = 0x37206a0610995c58074999cb9767b87af4c4978db68c06e8e6e81d282047a7c6
    // >>> s = 0x8ca63759c1157ebeaec0d03cecca119fc9a75bf8e6d0fa65c841c8e2738cdaec
    // >>> sig = Signature(r,s)
    // >>> print(sig.der().hex())
    // 3045022037206a0610995c58074999cb9767b87af4c4978db68c06e8e6e81d282047a7c6022100\
    // 8ca63759c1157ebeaec0d03cecca119fc9a75bf8e6d0fa65c841c8e2738cdaec

    std::array<unsigned char, 256> wsmem; 
    WriteStreamRef ws = CreateWriteMemStream (std::data(wsmem), wsmem.size ());  

    SECzy::WriteSignature_DER(ws, sig0);

    std::string hex_DER; 
    hex::encode (hex_DER, std::data(wsmem), ws->GetPos ());
    printf ( "DER[%zu]: %s\n " , hex_DER.size(), hex_DER.c_str());
   
    // 3045022037206a0610995c58074999cb9767b87af4c4978db68c06e8e6e81d282047a7c6022100 8ca63759c1157ebeaec0d03cecca119fc9a75bf8e6d0fa65c841c8e2738cdaec

  }


  { // Exercise 4
    POUT("Ex. 4.4"); 

    pt::map pm;
    el::map em;
    ffm::FEConRef F  = ffm::Create_FE_context (kSEC256k1_p_sz);
    ffm::ECConRef EC = ffm::Create_EC_context (F, em, pm, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);

    ScopeDeleter dr (F); 
    // Convert the following hex to binary and then to Base58:

    // 7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d
    // eff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c
    // c7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab6

    //
    //
    bytearray atmp;

    const std::string hex_a = "7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d";
    FE_t vara = dr (F->New (hex_a.c_str(), 16)); 
    F->Raw (atmp, vara, false);


    std::string b58_a = "";
    base58::encode (b58_a, std :: data (atmp), atmp.size ()); 
    printf ( "base58_a: %s\n " , b58_a.c_str());

    // 
    const std::string hex_b = "eff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c";
    FE_t varb = dr (F->New(hex_b.c_str(), 16));
    F->Raw (atmp, varb, false);

    std::string b58_b = ""; 
    base58::encode (b58_b, std :: data (atmp), atmp.size ()); 
    printf ( "base58_b: %s\n " , b58_b.c_str());


    // 
    const std::string hex_c = "c7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab6";
    FE_t varc = dr (F->New (hex_c.c_str(), 16)); 
    F->Raw (atmp, varc, false);

    std::string b58_c = ""; 
    base58::encode (b58_c, std::data (atmp), atmp.size ()); 
    printf ( "base58_c: %s\n " , b58_c.c_str());
    
    
  }




  { POUT("Ex. 4.5"); // Exercise 5


    pt::map pm;
    el::map em;
    ffm::FEConRef F  = ffm::Create_FE_context (kSEC256k1_p_sz);
    ffm::ECConRef EC = ffm::Create_EC_context (F, em, pm, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);
   
    ScopeDeleter dr(F);
    // Find the address corresponding to Public Keys whose Private Key secrets
    // are:
    bytearray ar;

    PublicKey   pub_a, pub_b, pub_c;    
    PrivateKey  prv_a, prv_b, prv_c;
    std::string addressa, addressb, addressc; 
    bytearray   amem (256, 0x0), bmem (256, 0x0), cmem (256, 0x0);

    FE_t secr_a = dr(F->New_ui(5002));
    MakePublicKey (pub_a, copy_BE(prv_a, F->Raw(ar, secr_a, false)));
    MakeAddress   (addressa, false, false, pub_a);
    printf ("addressa:%s\n", addressa.c_str()); 
    printf ("expected:%s\n", "mmTPbXQFxboEtNRkwfh6K51jvdtHLxGeMA"); 
    
    // >>> priv = PrivateKey(5002) // mmTPbXQFxboEtNRkwfh6K51jvdtHLxGeMA
    // 5002 (use uncompressed SEC, on testnet)
    //

    FE_t secr_b = dr(F->New_ui(2020));
    F->Pow_ui (secr_b, secr_b, 5); 
    MakePublicKey (pub_b, copy_BE (prv_b, F->Raw (ar, secr_b, false)));
    MakeAddress   (addressb, true, false, pub_b); 
    printf ("  addr_b:%s\n", addressb.c_str()); 
    printf ("expected:%s\n", "mopVkxp8UhXqRYbCYJsbeE1h1fiF64jcoH"); 

    // 2020^5 (use compressed SEC, on testnet)
    //

    FE_t secr_c = dr (F->New ("0x12345deadbeef", 0)); 
    MakePublicKey (pub_c, copy_BE (prv_c, F->Raw (ar, secr_c, false)));
    MakeAddress   (addressc, true, true, pub_c); 
    printf ("addressc:%s\n", addressc.c_str()); 
    printf ("expected:%s\n", "1F1Pn2y6pDb68E5nYJJeba4TLg2U7B6KF1"); 
    // 0x12345deadbeef (use compressed SEC on mainnet)
    //
    
  }




  {
    POUT("Ex. 4.6"); 

    pt::map pm;
    el::map em;
    ffm::FEConRef F  = ffm::Create_FE_context (kSEC256k1_p_sz);
    ffm::ECConRef EC =
      ffm::Create_EC_context (F, em, pm, kSEC256k1_coeff_a_sz, kSEC256k1_coeff_b_sz, kSEC256k1_n_sz, 0);
   
    ScopeDeleter dr(F);
    // Find the address corresponding to Public Keys whose Private Key secrets
    // are:
    bytearray ar;
    // Find the WIF for Private Key whose secrets are:

    PrivateKey  prva, prvb, prvc;
    std::string wifa, wifb, wifc; 

    FE_t secra = dr(F->New_ui (5003)); 
    MakeWIF (wifa, true, false, copy_BE (prva, F->Raw (ar, secra, false)));
    printf ("    wifa:%s\n", wifa.c_str());
    const std::string expecta = "cMahea7zqjxrtgAbB7LSGbcQUr1uX1ojuat9jZodMN8rFTv2sfUK"; 
    printf ("expected:%s\n", expecta.c_str()); 
    // 5003  (compressed, testnet)
    //

    FE_t secrb = dr(F->New_ui (2021));
    F->Pow_ui (secrb, secrb, 5);
    MakeWIF (wifb,false, false, copy_BE (prvb, F->Raw (ar, secrb, false))); 
    printf ("    wifb:%s\n", wifb.c_str());
    const std::string expectb = "91avARGdfge8E4tZfYLoxeJ5sGBdNJQH4kvjpWAxgzczjbCwxic";  
    printf ("expected:%s\n", expectb.c_str()); 
    // 2021**5 (uncompressed, testnet)
    //


    FE_t secrc = dr(F->New ("0x54321deadbeef", 0));
    MakeWIF (wifc, true, true, copy_BE (prvc, F->Raw (ar, secrc, false))); 
    printf ("    wifc:%s\n", wifc.c_str());
    const std::string expectc = "KwDiBf89QgGbjEhKnhXJuH7LrciVrZi3qYjgiuQJv1h8Ytr2S53a"; 
    printf ("expected:%s\n", expectc.c_str()); 
    // 0x54321deadbeef (compressed, mainnet)
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
    base58::encode (sec_c_base58, std::data(outbytes), outbytes.size());
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
