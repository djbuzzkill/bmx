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


  // --------------------------------------------------------------------
  {
    POUT("Ex. 4.9");

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

    bytearray secretbytes; 
    af::From_file (secretbytes, msgfile); 
    // ReadStreamRef rs = CreateReadFileStream (msgfile);
    // size_t filesize =     af::SizeOf_file (msgfile);
    // printf ("filesize:%zu\n", filesize ); 
 
    // bytearray secretbytes (filesize, 0); 
    // rs->Read (&secretbytes[0], filesize );
    secretbytes.push_back (0); 
      
    const std::string secretstr = (const char*) &secretbytes[0]; 
    printf ("secretstr:%s\n", secretstr.c_str()); 

    std::string encodedstr = myencoding (secretstr); 

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

  }


  
  printf ("%s[args]:exit\n", __FUNCTION__); 
  
  return 0; 
}




void print_pk_hex (const std::string& lbl, const bmx::Point& p){

  std::string xstr, ystr; 

  af::hex::encode (xstr, &p.x[0], p.x.size()); 
  af::hex::encode (ystr, &p.y[0], p.y.size()); 

  printf (" %s {x:%s, y:%s}\n", lbl.c_str(), xstr.c_str(),  ystr.c_str()); 
  
}

void print_pk_hex (const bmx::Point& p){
  std::string lbl = "Point"; 
  print_pk_hex (lbl, p);
}

int test_read_SEC_bin (std::vector<std::string>& args) {

  printf ("%s:ENTER\n", __FUNCTION__); 

  using namespace af;
  using namespace ffm; 
  using namespace bmx; 
  
  FFM_Env env; 
  Init_secp256k1_Env (env);

  FEConRef const&  F  = env.F;
  ECConRef const&  EC = env.EC;

  ScopeDeleter dr(F);
  // * 5,001
  // * 2,019^5^
  // * 0xdeadbeef54321

  std::array<unsigned char, 256> writebuf; 
  WriteStreamRef ws = CreateWriteMemStream (std::data(writebuf), writebuf.size()); 

  bytearray arrtmp; 

    
  // #1. intialize 'e'
  PrivateKey privk1;
  Point P1; 
  FE_t e1 = dr (F->New_ui (5001));
  F->Raw (arrtmp, e1, false);
  copy_BE (privk1, arrtmp);
    
  // make the point
  MakePublicKey (P1, privk1); 
  // write it
  size_t len_SEC_P1 = WritePoint (ws, P1, true); 
  printf ("len_SEC_P1: %zu\n", len_SEC_P1); 
  print_pk_hex ("P1", P1); 


  // print back out as hex
  std::string hex1;
  hex::encode (hex1, std::data(writebuf), ws->GetPos ());
  printf ( "hexstr1[%zu]: %s\n " , hex1.size(), hex1.c_str());

  Point R1;
  ReadPoint (R1, CreateReadMemStream (&writebuf[0], len_SEC_P1)); 
  print_pk_hex ("R1", R1); 
  //0357a4f368868a8a6d572991e484e664810ff14c05c0fa023275251151fe0e53d1


    
  // 2019**5
  ws->SetPos (0, byte_stream::SeekMode::Abs); 
  fixnum32 privk2;
  Point P2; 
  FE_t e2 = dr(F->New_ui (2019));
  F->Pow_ui (e2, e2, 5); 
  F->Raw (arrtmp, e2, false);
  copy_BE (privk2, arrtmp);
  // make the point
  MakePublicKey (P2, privk2);
  print_pk_hex ("P2", P2); 

  
  // write it
  size_t len_SEC_P2 =  WritePoint (ws, P2, true); 
  printf ("len_SEC_P2: %zu\n", len_SEC_P2); 
  // print back out as hex
  std::string hex2;
  hex::encode (hex2, std::data(writebuf), ws->GetPos ());
  printf ( "hexstr2[%zu]: %s\n " , hex2.size(), hex2.c_str());
  // 02933ec2d2b111b92737ec12f1c5d20f3233a0ad21cd8b36d0bca7a0cfa5cb8701
  
  Point R2;
  ReadPoint (R2, CreateReadMemStream (std::data(writebuf), writebuf.size()));
  print_pk_hex ("R2", R2); 

  
  // 0xdeadbeef54321
  ws->SetPos (0, byte_stream::SeekMode::Abs); 
  fixnum32 privk3;
  Point P3; 
  FE_t e3 = dr(F->New ("0xdeadbeef54321", 0 ));

  F->Raw (arrtmp, e3, false);
  copy_BE (privk3, arrtmp);
    
    // make the point
  MakePublicKey (P3, privk3);
  print_pk_hex ("P3", P3); 

  // write it
  size_t len_SEC_P3 = WritePoint (ws, P3, true); 
  printf ("len_SEC_P1: %zu\n", len_SEC_P3);
  
  // print back out as hex
  std::string hex3;
  hex::encode (hex3, std::data(writebuf), ws->GetPos ());
  printf ( "hexstr3[%zu]: %s\n " , hex3.size(), hex3.c_str());
  
  
  Point R3;
  ReadPoint (R3, CreateReadMemStream (&writebuf[0], writebuf.size()));
  print_pk_hex ("R3", R3); 
  // 0296be5b1292f6c856b3c5654e886fc13511462059089cdf9c479623bfcbe77690

    
  printf ("%s:EXIT\n", __FUNCTION__); 
  
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




oid SEC256k1_test ()
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

//
//
int test_gcrypt (const std::vector<std::string>& args) {
  printf ("\n%s:begin\n", __FUNCTION__); 
 
  af::digest32 rnd1, rnd2;

  const char msgtxt[] =
    "i dont have much to say about it";

  const char msg0[] = "fjwekl;4v@#%&WQhjWDDDT2kl+;rwjekl;@:>-{|}fSfsafsa'e vtj34l@vt43j3kqlgjrergej5l!?j54394%$#@^423Q%"; 
  af::sha256 (rnd1, msg0, strlen(msg0));
  af::sha256 (rnd2, std::data(rnd1), rnd1.size ());


  pr_hex ("from sha256x2", rnd2);

  af::digest32 hashx1;
  af::hash256(hashx1, msg0, strlen(msg0)); 
  pr_hex ("from hash256", hashx1); 
  
  printf ( "\n%s:end\n", __FUNCTION__); 
  return 0; 
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


      printf ("tx.version;%zu\n", tx.version);
      printf ("size(tx.inputs): %zu\n", tx.inputs.size ()); 

      for (int ind = 0; ind < tx.inputs.size (); ++ind) {
	if (ind  == 1) {

          printf("|input:%i:\n", ind);
	  
          // print_txin (tx.inputs[ind], 3);
          // printf ("len(tx.inp[%i].script_sig):%zu\n", ind,
          // tx.inputs[ind].script_sig.size ());

          const curv::command_list &cmds = tx.inputs[ind].script_sig;

          std::string stmp;

          for (auto& cmd : cmds) {
	    printf("> Ty:%s\n", Ty(cmd) == SC_element ? "element" : "op code");
            hex::encode(stmp, &arr(cmd)[0], arr(cmd).size());
            printf("> bin:0x%s\n", stmp.c_str());
	  }
	}
      }

      printf ("size(tx.outputs): %zu\n", tx.outputs.size ());
      for (auto ind = 0; ind < tx.outputs.size (); ++ind) {
	print_txo (tx.outputs[ind], 3); 

      }
      printf ("tx.locktime:%zu\n", tx.locktime); 

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
  
