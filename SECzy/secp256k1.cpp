#include "secp256k1.h"


#include "aframe/utility.h"

const char ksecp256k1_p_sz[]       = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f";
const char ksecp256k1_G_x_sz[]     = "0x79be667ef9dcbbac55a06295ce870b07029bfcdb2dce28d959f2815b16f81798";
const char ksecp256k1_G_y_sz[]     = "0x483ada7726a3c4655da4fbfc0e1108a8fd17b448a68554199c47d08ffb10d4b8";
const char ksecp256k1_coeff_a_sz[] = "0x0";
const char ksecp256k1_coeff_b_sz[] = "0x7"; 
const char ksecp256k1_n_sz[]       = "0xfffffffffffffffffffffffffffffffebaaedce6af48a03bbfd25e8cd0364141"; 



using namespace ffm;

namespace
{
  const char G[] = "G";
  const char n[] = "n";
  const char n_minus_2[] = "n-2"; 
}

secp256k1::secp256k1 ()
  : elems ()
  , points()
{
 
  F = ffm::Create_FE_context (ksecp256k1_p_sz, 0);
  EC = ffm::Create_EC_context (F, elems, points, ksecp256k1_coeff_a_sz, ksecp256k1_coeff_b_sz, ksecp256k1_n_sz, 0);

  
  ffm::ScopeDeleter dr (F);
  ffm::FE_t t0 = dr (F->New ()); 
  //checkres ("G", EC->DefPoint (G, ksecp256k1_G_x_sz, ksecp256k1_G_y_sz, 0));
  //checkres ("n", EC->DefElem ("n", ksecp256k1_n_sz, 0));
 
  //F->Set (t0, "0x2", 0);
  //EC->DefElem ("n-2", 
   
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
    ffm::ScopeDeleter dr (F);

    
    ffm::FE_t z = dr (F->New(sz_z, 0));
    ffm::FE_t r = dr (F->New(sz_r, 0));
    ffm::FE_t s = dr (F->New(sz_s, 0));
    
    ffm::FE_t s_inv = dr(F->New());


    const std::string vP = "vP";
    const std::string uG = "uG"; 
    
    // powm (out,  
    ffm::FE_t n_minus_2 = dr(F->New());
    F->Sub_ui(n_minus_2, elems[n], 2);
    EC->PrintElem ("{n-2}", n_minus_2, ffm::format::hex);
    
    F->PowM (s_inv, s, n_minus_2, elems[n]); 
    EC->PrintElem ("s_inv", s_inv, ffm::format::hex); 
    
    ffm::FE_t u = F->New ();
    F->MulM (u, z, s_inv, elems[n]);
    elems["u"] = u;
    
    ffm::FE_t v = F->New ();
    F->MulM (v, r, s_inv, elems[n]);
    elems["v"] = v; 

    EC->MakePoint_ui (uG, 0,  0);
    bool uG_res = EC->Mul_scalar (uG, "u", "G");
      
    EC->MakePoint_ui (vP, 0, 0);
    bool vP_res = EC->Mul_scalar (vP, "v", "P"); 

    EC->PrintPoint (uG, uG, ffm::format::hex);
    EC->PrintPoint (vP, vP, ffm::format::hex);

    EC->MakePoint_ui ("R", 0, 0);

    EC->AddPoint ("R", "uG", "vP"); 
    
	      
    pt::struc& R = points["R"];
    bool oncurve = EC->IsPointOnCurve (pt::x(R), pt::y(R)); 
    if (oncurve) {
      printf ("R is on curve\n"); 
    }
    
    EC->PrintElem ("R.x", pt::x(R), ffm::format::hex);
    EC->PrintElem ("r", r, ffm::format::hex);
    return (F->Cmp (pt::x(R), r) == 0);
  }
  
  

bool secp256k1::Sign (unsigned char* z, unsigned char* r)
{
  return false; 

}
 
