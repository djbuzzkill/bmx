#include "mtool.h"

#include <glm/glm.hpp>
#include <stdio.h>

#include <array>
#include <vector>
#include <string>
#include <algorithm>



#include <SDL2/SDL.h>
#include <vulkan/vulkan.h>

#include <openssl/sha.h>

#include <cryptopp/sha.h>
#include <cryptopp/ripemd.h>


#include <zmq.h>

#include <cx/System.h>




template<unsigned N>
using bytearr =  std::array<unsigned char, N> ;

//  SEC256k1  stuff
const char kSEC256k1_p_sz[]       = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
const char kSEC256k1_G_x_sz[]     = "0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798";
const char kSEC256k1_G_y_sz[]     = "0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8";
const char kSEC356k1_coeff_a_sz[] = "0x0";
const char kSEC256k1_coeff_b_sz[] = "0x7"; 

// y^2 = x^3 +ax^2+b 

typedef bytearr<32> bignum32b;


//FE_add, FE_sub, FE_mul, FE_div; 



unsigned mpz_to_binary (std::vector<unsigned char>& out, mpz_t n, bool out_LE = true)
{
 
  assert (out.size () > 0);
  
  std::string rawmem (out.size() + 4, 0); 

  FILE* memfile = fmemopen (&rawmem[0], 128, "w+");

 if ( !memfile )
    {

      printf ("fmemopen failed\n"); 
      
      return 0; 
    }
  
  unsigned numbytes = 0;

  char* ch = reinterpret_cast<char*> (&numbytes); 
  int out_size =  mpz_out_raw (memfile, n);
  fclose  (memfile);

  std::any_of(ch, ch+out_size, isalnum);

  
  printf ("out_size:%i\n", out_size);
  
  ch[0] = rawmem[3];
  ch[1] = rawmem[2];
  ch[2] = rawmem[1];
  ch[3] = rawmem[0];
  
  printf ("numbytes:%u\n", numbytes);

  for (unsigned i = 0; i < numbytes; ++i)
  {
    
    out[i] = rawmem[4 + numbytes - 1 - i]; 
  }

  
  return numbytes;
}



// FE_add, FE_sub, FE_mul, FE_div 
// FE_curve &EC_set_curve(FE_curve &out, FE_t a, FE_t b)
// {
//   mpz_set (out.a, a);
//   mpz_set (out.b, b);
//   return out; 
  
// }


void FE_test(std::vector<std::string> &args)
{

  FEContextPtr FE = Create_FE_context(256);
  
  FE_t     prime = FE->New (kSEC256k1_p_sz);
  FE_Point G     = { FE->New (kSEC256k1_G_x_sz)    , FE->New (kSEC256k1_G_y_sz) };
  FE_curve eq    = { FE->New (kSEC356k1_coeff_a_sz), FE->New (kSEC256k1_coeff_b_sz) };
  
  
  is_point_on_curve (G, eq);
  // FE_init (eq.a, kSEC356k1_coeff_a_sz);
  // FE_init (eq.b, kSEC256k1_coeff_b_sz);
  // FE_init (&G.x, kSEC256k1_G_x_sz);
  // FE_init (&G.y, kSEC256k1_G_y_sz);
  // FE_init (prime, kSEC256k1_p_sz);

  // for (int i = 0; i < 32; ++i)
  //   FE_init (fe[i], "0x0");
}



void Test_CryptoPP()
{
  int v = 1;

  printf ("v:%i\n", v); 
  swap_endian (&v); 

  printf ("v:%i\n", v); 
   
  const unsigned char hash_inp[] = "ji111089323fdsjklf;sa3424211$#@5321r23jkffsdafsaf443243l23;jww";
  unsigned char outp[64];
  memset (outp, 0, 64); 

  // this sha256 is from ssl?
  //  unsigned char* res = SHA256 (hash_inp, 60 , outp); 
 
  CryptoPP::SHA256::Transform   (0, 0);  
  CryptoPP::RIPEMD160::Transform(0, 0); 
  // EC_set_curve(eq, , __)
  
}
    
void test_gmp(std::vector<std::string> &args)
{

 
  mpz_t bign_a, bign_b, bign_c, bn_res;
  mpz_init2(bign_a, 256);
  mpz_init2(bign_b, 256);
  mpz_init2(bign_c, 256);
  mpz_init2(bn_res, 256);

  mpz_t bn[32];
  for (int i = 0; i < 32 ; ++i)
    mpz_init2(bn[i], 256);
  
  const char hex_str[3][67] = {
    "0x184567afabd2efaefa9864b312343deac43798feadfaf3e3a281a3c23632fda3",
    "0x4ef57a7e099777ecfdf2ccd0e13c2575e38f73b47a893fd257458a321c905327",
    "0xee231212feeb009238feed8a9c984f21c1b3b09324b72e7d77a77f3190021fd3"
  };     

  mpz_set_str (bign_a, hex_str[0], 0);   
  mpz_set_str (bign_b, hex_str[1], 0);

  gmp_printf ("%s is an big_a %Zd\n", "here", bign_a);
  gmp_printf ("%s is an big_b %Zd\n", "here", bign_b);


  gmp_printf ("a%Z", bign_a );
  
  // mpz_add (bign_c, bign_a, bign_b);
  bignum32b a, b, c;
  
  mpz_set (bn[0], bign_a);       
  mpz_set (bn[1], bign_b);


  gmp_printf ("bn0%Z", bn[0]);
  gmp_printf ("bn1%Z", bn[1]);
  mpz_mod (bn_res, bn[1], bn[0]);

  gmp_printf("bn1\%bn0 %Z", bn_res); 


  mpz_set_ui (bn[2], 123212);       
  mpz_set_ui (bn[3], 31);
  gmp_printf ("bn2%Z", bn[2]);
  gmp_printf ("bn3%Z", bn[3]);
  mpz_mod (bn_res, bn[2], bn[3]);

  gmp_printf("bn2\%bn3%Z", bn_res); 



  gmp_printf ("bn2:%Z", bn[2]);
  gmp_printf ("bn3:%Z", bn[3]);



  
  mpz_mod(bign_c, bign_b, bign_a); 
  //gmp_printf ("%s is an big_c %Zd\n", "here", bign_c);
  gmp_printf ("c:%Z", bign_c); 


  std::vector<unsigned char> c_bin (256, 0x0);

  
  unsigned binsize = mpz_to_binary (c_bin, bign_c); 
  
  //
  // memset (strbuf_256, 0, 256); 
  
  // fseek (fraw, 0, SEEK_SET); 
  printf ("out size: %i\n", binsize) ;

  for (int i = 0; i < binsize ; ++i)
  {

  printf ("char: 0x%x\n", c_bin[i]) ;

  }
  
  
}



// ----------------------- main --------------------------
int main (int argv, char** argc)
{
  std::vector<std::string> args (argc, argc+argv);

  // glm::vec3 v (2.1f, 2.2f, 2.3f);
  // glm::vec3 x = v + v;

  // test_gmp (args);
  FE_test (args); 

  cx::Mouse_state ms;
  
  auto f =   cx::foo  ; 

  return 0; 
}
