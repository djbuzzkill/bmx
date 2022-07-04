#include "secp256k1.h"


#include "Cx/Utility.h"

const char ksecp256k1_p_sz[]       = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f";
const char ksecp256k1_G_x_sz[]     = "0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798";
const char ksecp256k1_G_y_sz[]     = "0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8";
const char ksecp256k1_coeff_a_sz[] = "0x0";
const char ksecp256k1_coeff_b_sz[] = "0x7"; 
const char ksecp256k1_n_sz[]       = "0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141"; 


namespace
{
const char G[] = "G";
const char n[] = "n";
const char n_minus_2[] = "n-2"; 
}

secp256k1::secp256k1 ()
{
  F = FFM::Create_FE_context (ksecp256k1_p_sz, 0);
  EC = FFM::Create_EC_context (F, elmap, ptmap, ksecp256k1_coeff_a_sz, ksecp256k1_coeff_b_sz, ksecp256k1_n_sz, 0);

 
  ScopeDeleter dr (F);
  FE_t t0 = dr (F->New ()); 
  checkres ("G", EC->DefPoint (G, ksecp256k1_G_x_sz, ksecp256k1_G_y_sz, 0));
  checkres ("n", EC->DefElem ("n", ksecp256k1_n_sz, 0));

  F->Set (t0, "0x2", 0);
  EC->DefElem ("n-2", 
  elmap[]
   
}

secp256k1::~secp256k1 ()
{

}

 
  //
  //
bool secp256k1::Verify (const char* sz_z, const char* sz_r, const char* sz_s)
  {
    // uG + vP = R 
    // u = z/s
    // v = r/s
    // s = (z+re)/k
    ScopeDeleter dr (F);

    //char from_py[] = "100323378640741192763451357826607979131075829390957642811683296770329292192481";
    //FE_t frompy = dr (F->New (from_py, 10));
    
    
    FE_t z = dr (F->New(sz_z, 0));
    FE_t r = dr (F->New(sz_r, 0));
    FE_t s = dr (F->New(sz_s, 0));
    
    FE_t s_inv = dr(F->New());


    // powm (out,  
    FE_t n_minus_2 = dr(F->New());
    F->Sub_ui(n_minus_2, n, 2);
    printElem ("{n-2}", n_minus_2, Format::HEX);
    
    F->PowM (s_inv, s, n_minus_2, n); 
    printElem ("s_inv", s_inv, Format::HEX); 
    
    FE_t u = F->New ();
    F->MulM (u, z, s_inv, n);
    elem("u") = u;
    
    FE_t v = F->New ();
    F->MulM (v, r, s_inv, n);
    elem("v") = v; 

    // printElem ("[u]:", elem("u"), Format::HEX);
    // printElem ("[v]:", elem("v"), Format::HEX);
   
    
    // printPoint ("G", point("G"), Format::HEX);
    // printPoint ("P", point("P"), Format::HEX); 
 
    makePoint (point("uG"), F->New(), F->New());
    bool uG_res = Mul_scalar ("uG", "u", "G");
      

    makePoint (point("vP"), F->New (), F->New());
    bool vP_res = Mul_scalar ("vP", "v", "P"); 

    
    printPoint ("uG", point("uG"), format::hex);
    printPoint ("vP", point("vP"), format::hex);
    

    makePoint (point ("R"), F->New (), F->New ());
    

    AddPoint ("R", "uG", "vP"); 
    
	      
    elem_tuple& R = point("R");
    bool oncurve = IsPointOnCurve (x(R), y(R)); 
    if (oncurve) {
      printf ("R is on curve\n"); 
    }
    
    printElem  ("R.x", x(point("R")), Format::HEX);
    printElem ("r", r, Format::HEX);
    return (F->Cmp (x(R), r) == 0);
  }
  
  

bool secp256k1::Sign (unsigned char* z, unsigned char* r)
{
  return false; 

}
 
