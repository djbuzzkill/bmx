#include "mtool.h"
#include "FFM/EC_Math.h"

//#include <glm/glm.hpp>
//#include <stdio.h>


#include <Cx/System.h>
#include <Hx/Hash.h>


#include<array>
//#include <SDL2/SDL.h>
//#include <vulkan/vulkan.h>

//#include <openssl/sha.h>

//#include <crypto++/cryptlib.h>
//#include <cryptopp/sha.h>
//#include <cryptopp/ripemd.h>

#include "gcrypt.h"
//#include "sha2.h"

// big num math
#include <gmp.h>

// zero mq messaging
#include <zmq.h>

//  SEC256k1  stuff
const char kSEC256k1_p_sz[]       = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f";
const char kSEC256k1_G_x_sz[]     = "0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798";
const char kSEC256k1_G_y_sz[]     = "0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8";
const char kSEC356k1_coeff_a_sz[] = "0x0";
const char kSEC256k1_coeff_b_sz[] = "0x7"; 
const char kSEC256k1_n_sz[]       = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEBAAEDCE6AF48A03BBFD25E8CD0364141"; 
// y^2 = x^3 +ax^2+b 
const size_t kField_bit_precision = 256;


inline void print_bignum (const char *lbl, FFM::FE_t x, FFM::FEConPtr c) {

  std::array<char, 128> vstr; 
  c->Format (std::data(vstr), "%Zd", x); 
  printf ("%s%s\n", lbl, std::data(vstr)); 
}


inline bool checkres (const char* str, bool res)
{
  printf ("%s:%s\n", str, res ? "Yes" : "No"); 
  return res; 
}






void SEC256k1_test ()
{


  POUT ("0");
  FFM::FEConPtr F = FFM::Create_FE_context(kSEC256k1_p_sz, 0);
  FFM::ECConRef EC = FFM::Create_EC_context (F, kSEC356k1_coeff_a_sz, kSEC256k1_coeff_b_sz, 0);  

  const std::string
    Fp = "Fp",
    G = "G",
    Eq = "Eq",
    R = "R",
    s = "s",
    Q = "Q",
    M = "M";
  
  EC->DefElem (Fp, kSEC256k1_p_sz, 0);

  //char Fp_str[256]; 
  //print_bignum ("Fp:", Fp, F); 

  // F->Add ( 
  // 
  EC->DefPoint (G, kSEC256k1_G_x_sz, kSEC256k1_G_y_sz, 0);
  EC->PrintPoint ("G|HEX:", G, FFM::Format::HEX);
  EC->PrintPoint ("G|DEC: ", G, FFM::Format::DEC);
  //
  EC->PrintCoeffs ("eq-> ", FFM::Format::DEC);
  
  
  // EC->DefPoint (R, "0xdeadbeef", "0xdeadf00d", 0);  
  // EC->PrintPoint ("R-> ", R);

  // EC->DefElem (s, "0x04ea32532fd", 0);

//  FFM::EC->Mul (R, s, G);


  EC->Add (Q, G, G);
  POUT("G+G=Q");
  EC->PrintPoint ("Q-> ", Q, FFM::Format::HEX);

  
  // EC_Sub (M, Q, G); 
  // POUT("M=Q-G");
  // EC_Print("Q->", Q);  
  
  //FFM::EC_IsPointOnCurve (); 
  // EC->IsPointOnCurve (Eq, G); 

 
}





int test_gcrypt (const std::vector<std::string>& args)
{

// void SHA256Init(SHA2_CTX *);
// void SHA256Transform(uint32_t state[8], const uint8_t [SHA256_BLOCK_LENGTH]);
// void SHA256Update(SHA2_CTX *, const uint8_t *, size_t);
// void SHA256Pad(SHA2_CTX *);
// void SHA256Final(uint8_t [SHA256_DIGEST_LENGTH], SHA2_CTX *);

  //SHA2_CTX sha_con;
  //memset (&sha_con, 0, sizeof (SHA2_CTX));

  std::array<unsigned char> msg;

  
  gcry_error_t errval =  gcry_md_open (std::data(msg), 0);   
  //auto hay = gcry_md_hash_buffer; 
  //SHA256Init (&sha_con);
  
  
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


int CH3_test(std::vector<std::string> &args)
{
  printf ("%s[args]\n", __FUNCTION__);

  FFM::FEConPtr F = FFM::Create_FE_context (kSEC256k1_p_sz);
  FFM::ECConRef EC = FFM::Create_EC_context (F, kSEC356k1_coeff_a_sz, kSEC256k1_coeff_b_sz, 16);

 
  if (true) 
  {   
    POUT("Ex. 3.6");  
    const std::string G = "G";
    const std::string P = "P";
    
    checkres ("G",  EC->DefPoint (G, kSEC256k1_G_x_sz, kSEC256k1_G_y_sz, 16));
    // Verify whether these signatures are valid:
    // P = (0x887387e452b8eacc4acfde10d9aaf7f6d9a0f975aabb10d006e4da568744d06c,
    // 0x61de6d95231cd89026e286df3b6ae4a894a3378e393e93a0f45b666329a0ae34)
    const char kP_x_sz[] = "0x887387e452b8eacc4acfde10d9aaf7f6d9a0f975aabb10d006e4da568744d06c";
    const char kP_y_sz[] = "0x61de6d95231cd89026e286df3b6ae4a894a3378e393e93a0f45b666329a0ae34";
    
    checkres ("P", EC->DefPoint (P, kP_x_sz, kP_y_sz, 16));

    CryptoPP::PublicKey pk("0x0xf32", "0x223aaaaaaaa");


        


    
    // # signature 1
    // z = 0xec208baa0fc1c19f708a9ca96fdeff3ac3f230bb4a7ba4aede4942ad003c0f60
    // r = 0xac8d1c87e51d0d441be8b3dd5b05c8795b48875dffe00b7ffcfac23010d3a395
    // s = 0x68342ceff8935ededd102dd876ffd6ba72d6a427a3edb13d26eb0781cb423c

    // EC->DefElem ("z1", "0xec208baa0fc1c19f708a9ca96fdeff3ac3f230bb4a7ba4aede4942ad003c0f60", 0);
    // EC->PrintElem ("z1", "z1", FFM::Format::HEX);  

    // EC->DefElem ("r1", "0xac8d1c87e51d0d441be8b3dd5b05c8795b48875dffe00b7ffcfac23010d3a395", 0);
    // EC->PrintElem("r1", "r1", FFM::Format::HEX);


    // EC->DefElem ("s1", "0x68342ceff8935ededd102dd876ffd6ba72d6a427a3edb13d26eb0781cb423c4", 0); 		 
    // EC->PrintElem("s1", "s1", FFM::Format::HEX);

    // F->Inv (EC->El("s1_inv"), EC->El("s1"));
    // EC->PrintElem("s1_inv", "s1_inv", FFM::Format::HEX);
    
    // F->Mul(EC->El("u1"), EC->El("r1"), EC->El("s1_inv"));
    // F->Mul(EC->El("v1"), EC->El("z1"), EC->El("S1_inv")); 

    // EC->PrintElem("u1", "u1", FFM::Format::HEX); 
    // EC->PrintElem("v1", "v1", FFM::Format::HEX); 
    
    //EC->Mul_scalar ("u1G", "u1", "G");
    //EC->Mul_scalar ("v1P", "v1", "P");
    //EC->Add("R1", "u1G", "v1P");
    
    
    
    
    // # signature 2
    // z = 0x7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d
    // r = 0xeff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c
    // s = 0xc7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab6
    
    EC->DefElem ("z2", "0x7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d", 0); 
    EC->DefElem ("r2", "0xeff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c", 0);
    EC->DefElem ("s2", "0xc7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab6", 0); 		 
    // uG + vP = R 
    // u = z/s
    // v = r/s
    // s = (z+re)/k
    
  }

  return 0; 
  
}


//
//
int CH4_test (std::vector<std::string>& args) 
{
  printf ("%s[args]\n", __FUNCTION__); 

  FFM::FEConPtr F = FFM::Create_FE_context (kSEC256k1_p_sz);
  FFM::ECConRef EC = FFM::Create_EC_context (F, kSEC356k1_coeff_a_sz, kSEC256k1_coeff_b_sz, 16);


  const std::string G = "G";
  const std::string P = "P";
  
  checkres ("G",  EC->DefPoint (G, kSEC256k1_G_x_sz, kSEC256k1_G_y_sz, 16));

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
      
      
      EC->DefElem_ui (Sec_A, secr_a);
      EC->DefElem_ui (Sec_B, secr_b);
      EC->DefElem    (Sec_C, secr_c, 0);


      EC->PrintElem (Sec_A, Sec_A, FFM::Format::DEC); 
      EC->PrintElem (Sec_B, Sec_B, FFM::Format::DEC);  
      EC->PrintElem (Sec_C, Sec_C, FFM::Format::HEX);



      
    }






  
  
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
  CH3_test(args); 
  //CH4_test(args); 
  test_gcrypt(args); 
  //  Test_CryptoPP(args); 
  //CH4_test (args);  
  // cx::Mouse_state ms;
  //auto f =   cx::foo  ; 

  return 0; 
}
