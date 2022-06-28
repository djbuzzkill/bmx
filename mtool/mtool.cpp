#include "mtool.h"

//#include <glm/glm.hpp>
//#include <stdio.h>


#include <Cx/System.h>
//#include <SDL2/SDL.h>
//#include <vulkan/vulkan.h>

//#include <openssl/sha.h>

#include <cryptopp/sha.h>
#include <cryptopp/ripemd.h>


#include <gmp.h>

// zero mq messaging
#include <zmq.h>

//  SEC256k1  stuff
const char kSEC256k1_p_sz[]       = "0xFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFFEFFFFFC2F";
const char kSEC256k1_G_x_sz[]     = "0x79BE667EF9DCBBAC55A06295CE870B07029BFCDB2DCE28D959F2815B16F81798";
const char kSEC256k1_G_y_sz[]     = "0x483ADA7726A3C4655DA4FBFC0E1108A8FD17B448A68554199C47D08FFB10D4B8";
const char kSEC356k1_coeff_a_sz[] = "0x0";
const char kSEC256k1_coeff_b_sz[] = "0x7"; 

// y^2 = x^3 +ax^2+b 
const size_t kField_bit_precision = 256;



void print_bignum (const char *lbl, FFM::FE_t x, FFM::FEConPtr c) {

  char vstr[256]; 
  c->Format (vstr, "%Zd", x); 
  printf ("%s%s\n", lbl, vstr); 
}

bool checkres (const char* str, bool res)
{
  printf ("%s:%s\n", str, res ? "Yes" : "No"); 
  return res; 
}



////
void FE_test(std::vector<std::string> &args)
{
  POUT ("0");
  FFM::FEConPtr F = FFM::Create_FE_context("97", 10);
  POUT ("1");  
  
  FFM::FE_t
    x = F->New (),
    u = F->New (),
    v = F->New (),

    d = F->New (),
    m = F->New (),
    t = F->New (),
    j = F->New (), 
    k = F->New (),
    q = F->New (),
    z = F->New (),
    f = F->New(); 
  POUT ("2");

  
  F->Set (v, "95", 10);
  F->Set (m, "45", 10);
  F->Set (x, "31", 10);
  
  F->Mul (k, v, m);
  F->Mul (q, k, x);
  
  print_bignum ("95*45*31=", q, F); 

  // 
  F->Set (x, "17", 10);
  F->Set (u, "13", 10);
  F->Set (v, "19", 10);
  F->Set (d, "44", 10);

  F->Mul (k, x, u);
  F->Mul (q, k, v);
  F->Mul (k, q, d);
  
  print_bignum ("17*13*19*44=", k, F);   

  F->Set (v, "12", 10);
  F->Set (d, "77", 10); 
  F->Set (m, "49", 10);

  F->Set (k, "12", 10); 
  for (size_t i = 1 ; i < 7; ++i)
    F->Mul (k, k, v);

  F->Set (q, "77", 10); 
  for (size_t i = 1; i < 49; ++i)
    F->Mul (q, q, d);

  F->Mul (z, k, q); 

  print_bignum ("12^7 * 77^49=", z, F);
 
  printf ("sizeof(int): %zd \n", sizeof(int)); 
  // excercize

  FFM::ByteArray rawnum;
  F->Raw (rawnum, z);  
  printf ("rawnum.size (%zu)\n\n  bytes:", rawnum.size ());
  
  for (auto x : rawnum)
    {
      printf ("%d ", x);
    }
  printf ("\n");
}







void EC_test (std::vector<std::string>& args) 
{
  POUT ("0");
  FFM::FEConPtr F = FFM::Create_FE_context("223", 10);

  // create a curve for y^2=X^3 + 7
  // FFM::FE_t
  //   x = F->New (),
  //   u = F->New (),
  //   v = F->New (); 

  const std::string A = "A.Point"; 
  const std::string B = "B.Point";
  
  const std::string X = "X.Point"; 
  const std::string Y = "Y.Point"; 

  const std::string U = "U.Point"; 
  const std::string V = "V.Point";
  const std::string N = "N.Point";
  
  const std::string P = "P.Point"; 
  const std::string Q = "Q.Point"; 
  const std::string R = "R.Point"; 

  // Ex. 1
  if (false)
  { 
    FFM::ECConRef EC = FFM::Create_EC_context (F, "0", "7", 10); 
    EC->PrintCoeffs ("Coefficients:", FFM::EC_Format::DEC);
    POUT("Ex. 1"); 
    checkres ("[192, 105]", EC->DefPoint_ui (A, 192, 105));
    checkres ("[17, 56]"  , EC->DefPoint_ui (B, 17, 56));
    checkres ("[200, 119]", EC->DefPoint_ui (U, 200, 119));
    checkres ("[1, 193]"  , EC->DefPoint_ui (V, 1, 193));
    checkres ("[42, 99]"  , EC->DefPoint_ui (X, 42, 99));
  }
  // Ex. 2
  if (false)
  {
    FFM::ECConRef EC = FFM::Create_EC_context (F, "0", "7", 10); 
    POUT("Ex. 2");

    checkres ("X:[170 142]", EC->DefPoint_ui (X, 170, 142)); 
    checkres ("Y:[60 139]", EC->DefPoint_ui (Y, 60, 139));
    checkres ("A:[47 71]", EC->DefPoint_ui (A, 47, 71)); 
    checkres ("B:[17 56]", EC->DefPoint_ui (B, 17, 56)); 
    checkres ("U:[143 98]", EC->DefPoint_ui (U, 143, 98));
    checkres ("V:[76 66]", EC->DefPoint_ui (V, 76, 66));
    

    //EC->PrintPoint ("X:", X); 
    //EC->PrintPoint ("Y:", Y); 
    EC->Add (P, X, Y);
    EC->PrintPoint("X+Y:", P); 
  
    //EC->PrintPoint ("A:", A); 
    //EC->PrintPoint ("B:", B); 
    EC->Add (Q, A, B);
    EC->PrintPoint ("A+B:", Q); 
    
    //EC->PrintPoint ("U:", U); 
    //EC->PrintPoint ("V:", V); 
    EC->Add (R, U, V);
    EC->PrintPoint ("U+V:", R); 
  }

  if (true)
    { 
      POUT("Ex.3");
      
      // class FieldElementTest(TestCase):
      //FFM::ECConRef EC = FFM::Create_EC_context (F, "0", "7", 10);

      FFM::FEConPtr F31 = FFM::Create_FE_context("31", 10);

      {
        //  def test_ne(self):
        //    a = FieldElement(2, 31)
	//    b = FieldElement(2, 31)
        //    c = FieldElement(15, 31)
	//    self.assertEqual(a, b)
        //    self.assertTrue(a != c)
        //    self.assertFalse(a != b)
	FFM::ScopeDeleter dr(F);

	FFM::FE_t eA = dr (F31->New_ui(2));
	FFM::FE_t eB = dr (F31->New_ui(2));
	FFM::FE_t eC = dr (F31->New_ui(15));
	
	checkres ("A==B", 0 == F31->Cmp(eA, eB));
	checkres ("A!=C", 0 != F31->Cmp(eA, eC)); 
	checkres ("A!=B", 0 != F31->Cmp(eA, eB));
      }
      
      {
        //  def test_add(self):
        //    a = FieldElement(2, 31)
        //    b = FieldElement(15, 31)
        //    self.assertEqual(a + b, FieldElement(17, 31))
        //    a = FieldElement(17, 31)
        //    b = FieldElement(21, 31)
        //    self.assertEqual(a + b, FieldElement(7, 31))

	FFM::ScopeDeleter dr(F);
	FFM::FE_t eA = dr (F31->New_ui(2));
	FFM::FE_t eB = dr (F31->New_ui(15));
	FFM::FE_t eRes = dr (F31->New()); 
	FFM::FE_t e17 = dr (F31->New_ui(17));
	F31->Add(eRes, eA, eB);
        checkres("A+B=17", 0 == F31->Cmp(eRes, e17));


	FFM::FE_t e7 = dr(F31->New_ui(7)); 
        F31->Set_ui(eA, 17);
	F31->Set_ui(eB, 21);
	F31->Add(eRes, eA, eB);
	checkres("A+B=7", 0 == F31->Cmp (eRes,e7));  
      }
      
     {
       //  def test_sub(self):
       //    a = FieldElement(29, 31)
       //    b = FieldElement(4, 31)
       //    self.assertEqual(a - b, FieldElement(25, 31))
       //    a = FieldElement(15, 31)
       //    b = FieldElement(30, 31)
       //    self.assertEqual(a - b, FieldElement(16, 31))
	FFM::ScopeDeleter dr(F);
	FFM::FE_t eA = dr (F31->New_ui(29));
	FFM::FE_t eB = dr (F31->New_ui(4));
	FFM::FE_t e25 = dr(F31->New_ui(25));
	
        FFM::FE_t eRes = dr (F31->New()); 
	F31->Sub (eRes , eA, eB);
	checkres("A-B=25", 0 == F31->Cmp(eRes, e25)); 

	F31->Set_ui(eA, 15);
	F31->Set_ui(eB, 30);

        F31->Sub (eRes, eA, eB); 
	FFM::FE_t e16 = dr (F31->New_ui(16));
	checkres("A-B=16", 0 == F31->Cmp(eRes, e16)); 
     }
      
     {
       //  def test_mul(self):
       //    a = FieldElement(24, 31)
       //    b = FieldElement(19, 31)
       //    self.assertEqual(a * b, FieldElement(22, 31))
 	FFM::ScopeDeleter dr(F);
	FFM::FE_t eA = dr (F31->New_ui(24));
	FFM::FE_t eB = dr (F31->New_ui(19));
	FFM::FE_t eRes = dr(F31->New());

        F31->Mul(eRes, eA, eB);

	FFM::FE_t e22 = dr (F31->New_ui(22));
        checkres("A*B=22", 0 == F31->Cmp(eRes, e22));

     }

     {
       //  def test_rmul(self):
       //    a = FieldElement(24, 31) b = 2
       //    self.assertEqual(b * a, a + a)
     }

     {
       // def test_pow(self):
       //   a = FieldElement(17, 31)
       //   self.assertEqual(a**3, FieldElement(15, 31))
       //   a = FieldElement(5, 31)
       //   b = FieldElement(18, 31)
       //   self.assertEqual(a**5 * b, FieldElement(16, 31))
  	FFM::ScopeDeleter dr(F);
	FFM::FE_t eA = dr (F31->New_ui(17));
	FFM::FE_t res = dr(F31->New()); 
        FFM::FE_t r15 = dr (F31->New_ui(15));
	F31->Pow_ui(res, eA, 3);
	checkres("a^3=15", 0 == F31->Cmp(res, r15)); 
		 
	F31->Set_ui(eA, 5);  
        FFM::FE_t eB = dr (F31->New_ui(18));
	FFM::FE_t e16 = dr(F31->New_ui(16));

	F31->Pow_ui(res, eA, 5);
	F31->Mul(res, res, eB);
	checkres("A^5*B=16", 0 == F31->Cmp(res, e16));
     }
     {
       //    def test_div(self):
       //      a = FieldElement(3, 31)
       //      b = FieldElement(24, 31)
       //      self.assertEqual(a / b, FieldElement(4, 31))
       //      a = FieldElement(17, 31)
       //      self.assertEqual(a **-3, FieldElement(29, 31))
       //      a = FieldElement(4, 31)
       //      b = FieldElement(11, 31) self.assertEqual(a **-4 * b, FieldElement(13, 31))
   	FFM::ScopeDeleter dr(F);
	FFM::FE_t eA = dr (F31->New_ui(3));
	FFM::FE_t eB = dr (F31->New_ui(24)); 
        FFM::FE_t e4 = dr (F31->New_ui(4));
	FFM::FE_t res= dr (F31->New()); 
	F31->Div (res, eA, eB); 
	checkres("A/B=4", 0 == F31->Cmp(res, e4));
	    
	F31->Set_ui(eA, 17);
	FFM::FE_t e29 = dr (F31->New_ui(29));
	F31->Pow_si(res, eA, -3);
	checkres ("A^-3=29", 0 == F31->Cmp(res, e29)); 
        // F31->Set_ui(eB
     }


     if (false) 
       {
	 POUT("Ex. 4");
	 FFM::ECConRef EC = FFM::Create_EC_context (F, "0", "7", 10); 
	 
	 EC->DefPoint_ui(A, 192,  105);
	 EC->DefPoint_ui(B, 143, 98); 
	 EC->DefPoint_ui(X, 47, 71);
	 EC->DefPoint_ui(Y, 47, 71);
	 EC->DefPoint_ui(U, 47, 71);
	 EC->DefPoint_ui(V, 47, 71);
	 EC->DefPoint_ui(N, 47, 152);
      

	 POUT(">>>>>>"); 
	 
	 // checkres ("[!]", EC->DefPoint_ui(R, 47, 152));
	 // checkres ("[?]", EC->DefPoint_ui("wat", 36, 112));
	 
	 EC->Mul_scalar_ui("oA", 2, A);
	 EC->Mul_scalar_ui("oB", 2, B);
	 EC->Mul_scalar_ui("oX", 2, X);
	 EC->Mul_scalar_ui("oY", 4, Y);
	 EC->Mul_scalar_ui("oU", 8, U);
	 EC->Mul_scalar_ui("oN", 20, N); 
	 
	 EC->PrintPoint(V, V, FFM::EC_Format::DEC);
	 EC->PrintPoint(N, N, FFM::EC_Format::DEC);
	 EC->Add ("V+N", V, N); 
	 
	 EC->PrintPoint(V, V, FFM::EC_Format::DEC);
	 EC->PrintPoint(N, N, FFM::EC_Format::DEC);
	 EC->PrintPoint("V+N", "V+N", FFM::EC_Format::DEC);
	 //EC->Add (Q, N, R); 
	 
	 
	 //EC->PrintPoint("oA", "oA", FFM::EC_Format::DEC);
	 //EC->PrintPoint("oB", "oB", FFM::EC_Format::DEC);
	 //EC->PrintPoint("oX", "oX", FFM::EC_Format::DEC);
	 //EC->PrintPoint("oY", "oY", FFM::EC_Format::DEC);
	 //EC->PrintPoint("oU", "oU", FFM::EC_Format::DEC);
	 
       }
     
     if(false) 
       {
      POUT("NEW SECTION");
      
      
      FFM::ECConRef EC = FFM::Create_EC_context (F, "0", "7", 10); 
      EC->DefPoint_ui(A, 192,  105);
      EC->DefPoint_ui(B, 143, 98); 
      EC->DefPoint_ui(X, 47, 71);
      EC->DefPoint_ui(Y, 47, 71);
      EC->DefPoint_ui(U, 47, 71);
      EC->DefPoint_ui(V, 47, 71);
      EC->DefPoint_ui(N, 47, 152);
      
      
      EC->Add("V+N", V, N);
      EC->PrintPoint ("V+N:", "V+N");
      
      for (unsigned i = 1; i < 30; ++i)
	{
	  printf ("%i|", i); 
	  EC->Mul_scalar_ui("oV", i, V);
	  EC->PrintPoint("oV", "oV", FFM::EC_Format::DEC);
	}
      POUT("<<<<<"); 
    
       }

     if (true)
       {
	 
	 POUT("Ex. 5");

	 
      FFM::ECConRef EC = FFM::Create_EC_context (F, "0", "7", 10); 
      EC->DefPoint_ui(P, 15, 86);
      EC->DefPoint_ui(Q, 15, 86);
      int count = 1;
      bool add_success = true;
      while (add_success)
	{

	  add_success = EC->Add(Q, Q, P);
	  count++;
	  
	}

      printf ("Ex.5 count:%i\n", count); 
      
       // prime = 223
	 // a = FieldElement(0, prime)
	 // b = FieldElement(7, prime)
	 // x = FieldElement(15, prime)
	 // y = FieldElement(86, prime)
	 // p = Point(x, y, a, b)
	 // inf = Point(None, None, a, b)

	 
	 
	 // # create a product variable
	 // # create a counter variable
	 // # loop until the product is the point at infinity
	 // # add the point to the product and increment counter
	 // # print the counter when exited from loop
       }

     
    }

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
  EC->PrintPoint ("G|HEX:", G, FFM::EC_Format::HEX);
  EC->PrintPoint ("G|DEC: ", G, FFM::EC_Format::DEC);
  //
  EC->PrintCoeffs ("eq-> ", FFM::EC_Format::DEC);
  
  
  // EC->DefPoint (R, "0xdeadbeef", "0xdeadf00d", 0);  
  // EC->PrintPoint ("R-> ", R);

  // EC->DefElem (s, "0x04ea32532fd", 0);

//  FFM::EC->Mul (R, s, G);


  EC->Add (Q, G, G);
  POUT("G+G=Q");
  EC->PrintPoint ("Q-> ", Q, FFM::EC_Format::HEX);

  
  // EC_Sub (M, Q, G); 
  // POUT("M=Q-G");
  // EC_Print("Q->", Q);  
  
  //FFM::EC_IsPointOnCurve (); 
  // EC->IsPointOnCurve (Eq, G); 

 
}



void Test_CryptoPP()
{
  int v = 1;

  printf ("v:%i\n", v); 
  FFM::swap_endian (&v); 

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


/*


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

*/

// ----------------------- main --------------------------
int main (int argv, char** argc)
{
  std::vector<std::string> args (argc, argc+argv);

  // glm::vec3 v (2.1f, 2.2f, 2.3f);
  // glm::vec3 x = v + v;

  // test_gmp (args);
  //FE_test (args); 
  EC_test (args);  
  // cx::Mouse_state ms;
  //auto f =   cx::foo  ; 

  return 0; 
}
