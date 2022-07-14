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
  //  const char n_minus_2[] = "n-2"; 

  void printbin (const char* lbl, const af::digest32& bytes) {
    printf ("%s: ", lbl);
    for (auto b : bytes) printf ("%02x ", b);
    printf ("\n"); 
  }
}

namespace SECzy {

  secp256k1::secp256k1 () : elems (), points() { 
    
    F = ffm::Create_FE_context (ksecp256k1_p_sz, 0);
    EC = ffm::Create_EC_context (F, elems, points, ksecp256k1_coeff_a_sz, ksecp256k1_coeff_b_sz, ksecp256k1_n_sz, 0);
    
    
    EC->MakePoint (G, ksecp256k1_G_x_sz, ksecp256k1_G_y_sz, 0);
		   
    
    ffm::ScopeDeleter dr (F);
    ffm::FE_t t0 = dr (F->New ()); 


    EC->PrintElem ("SECzy:n", "n", format::hex);
    //    Ec->PrintElem ("
    
    //checkres ("n", EC->DefElem ("n", ksecp256k1_n_sz, 0));
    
    //F->Set (t0, "0x2", 0);
    //EC->DefElem ("n-2", 
    
  }

  //
  //
  secp256k1::~secp256k1 () {
    
  }

 
  //
  /*
  bool secp256k1::Verify (const char* sz_z, const char* sz_r, const char* sz_s)  {
    // uG + vP = R 
    // u = z/s
    // v = r/s
    // s = (z+re)/k
    ffm::ScopeDeleter dr (F);

    POUT ("USING THIS");
    

    ffm::FE_t z = dr (F->New(sz_z, 0));
    ffm::FE_t r = dr (F->New(sz_r, 0));
    ffm::FE_t s = dr (F->New(sz_s, 0));
    
    ffm::FE_t s_inv = dr(F->New());

    const std::string R  = "R";
    const std::string P  = "P"; 
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
    bool uG_res = EC->Mul_scalar (uG, "u", G);
      
    EC->MakePoint_ui (vP, 0, 0);
    bool vP_res = EC->Mul_scalar (vP, "v", P); 

    EC->PrintPoint (uG, uG, ffm::format::hex);
    EC->PrintPoint (vP, vP, ffm::format::hex);

    EC->MakePoint_ui (R, 0, 0);

    EC->AddPoint (R, uG, vP); 
    
	      
    pt::struc& Rref = points[R];
    bool oncurve = EC->IsPointOnCurve (pt::x(Rref), pt::y(Rref)); 
    if (oncurve) {
      printf ("R is on curve\n"); 
    }
    else
    
    EC->PrintElem ("R.x", pt::x(Rref), ffm::format::hex);
    EC->PrintElem ("  r", r, ffm::format::hex);
    return (F->Cmp (pt::x(Rref), r) == 0);
  }
  
  */

  //
  //
  bool secp256k1::Verify (const Signature& sig, const PublicKey& pubk, const digest32& z_msg) {
    //    printf ("%s:enter\n", __FUNCTION__); 
  
    ffm::ScopeDeleter dr (F);
  
    ffm::FE_t z = dr (F->New_bin(std::data(z_msg), 32, false));
    ffm::FE_t r = dr (F->New_bin(std::data(sig.r), 32, false));
    ffm::FE_t s = dr (F->New_bin(std::data(sig.s), 32, false));
    
    ffm::FE_t s_inv = dr(F->New());

    const std::string n = "n"; 
    const std::string u = "u"; 
    const std::string v = "v";
    const std::string vP = "vP";
    const std::string uG = "uG"; 

    const std::string R = "R";
    const std::string P = "P"; 

    ffm::FE_t Gx = F->New (ksecp256k1_G_x_sz, 0);
    ffm::FE_t Gy = F->New (ksecp256k1_G_y_sz, 0);
    EC->MakePoint (G, Gx, Gy); 
    EC->PrintPoint ("<G>", G, ffm::format::hex); 


    ffm::FE_t Px = F->New_bin (std::data(pubk.x), 32, false);
    ffm::FE_t Py = F->New_bin (std::data(pubk.y), 32, false); 
    EC->MakePoint (P, Px, Py); 
    EC->PrintPoint ("<P>", P, ffm::format::hex); 

    // powm (out,  
    ffm::FE_t n_minus_2 = dr(F->New());
    F->Sub_ui(n_minus_2, elems[n], 2);
    //EC->PrintElem ("{n-2}", n_minus_2, ffm::format::hex);
    
    F->PowM (s_inv, s, n_minus_2, elems[n]); 
    
    EC->MakeElem_ui(u, 0);
    F->MulM (elems[u], z, s_inv, elems[n]);
    
    EC->MakeElem_ui (v, 0);
    F->MulM (elems[v], r, s_inv, elems[n]);
    
    EC->MakePoint_ui (uG, 0, 0);
    EC->Mul_scalar (uG, u, G);
    
    EC->MakePoint_ui (vP, 0, 0);
    EC->Mul_scalar (vP, v, P); 
    
    EC->MakePoint_ui (R, 0, 0);
    EC->AddPoint (R, uG, vP); 
    
    ffm::pt::struc& Rref = points[R];
    bool oncurve = EC->IsPointOnCurve (pt::x(Rref), pt::y(Rref)); 
    if (oncurve) {
      printf ("R is on curve\n"); 
    }
    
    EC->PrintElem ("R.x", pt::x(Rref), ffm::format::hex);
    EC->PrintElem ("  r", r, ffm::format::hex);

    return (F->Cmp (pt::x(Rref), r) == 0);
  }
  
  //
  //
  bool secp256k1::Sign (Signature& sig, const PrivateKey& prk, const digest32& zbin) {
    
    using namespace ffm;
    // eG = P
    
    //    const std::string G = "G"; 
    const std::string P = "P";
    const std::string R = "R";
    //const std::string n = "n";
 
    ScopeDeleter dr (F); 
    // kG = R
    FE_t
      e    = dr(F->New_bin(std::data(prk.e), 32, false)),
      z    = dr(F->New_bin(std::data(zbin), 32, false)),
      s    = dr(F->New ()),
      stmp = dr(F->New()),
      
      tmp  = dr(F->New()),
      snum = dr(F->New ());
    
    //
    EC->MakePoint_ui(P, 0, 0);
    EC->Mul_scalar (P, e, G); 
    POUT ("224");

    
    // n-2
    FE_t n_sub_2 = dr(F->New());
    F->Sub_ui(n_sub_2, elems[n], 2);
    elems["n-2"] = n_sub_2; 
    
    // n/2
    FE_t n_div_2 = dr(F->New()); 
    F->Div_ui(n_div_2, elems[n], 2); 
    elems["n/2"] = n_div_2;
    
    // k = rand (n) <-- fix later
    FE_t k = dr(F->New()); 
    POUT("241")
    F->Rand (k, elems[n]);
    elems["k"] = k;
    
    // 1/k
    FE_t k_inv= dr(F->New ());
    F->PowM (k_inv, k, n_sub_2, elems[n]);  
    elems["1/k"] = k_inv; 
    
    EC->MakePoint_ui (R, 0, 0); // <-- we should just make a plain 'alloc-point'
    EC->Mul_scalar (R, "k", G); // we want R.x
    // kG = R
    
    F->SMul (tmp, pt::x(points[R]), e);
    F->SAdd (snum, z, tmp);
    F->MulM (stmp, snum, k_inv, elems[n]); 
    // s = (z+re)/k mod n
    
    // if s > n/2
    //  s = n - s
    int cmpres = F->Cmp (stmp, elems["n/2"]);
    // Compare op1 and op2. Return a positive value if op1 > op2, zero if op1 = op2, or a negative value if op1 < op2.
    if (cmpres > 0) {
      printf ("..(s > n/2)\n");
      F->Sub (s , elems[n], stmp); 
    }
    else {
      F->Set (s, stmp);
    }
    
    // write results, sig is (r,s) => sig(r,s)
    {
      bytearray rraw, sraw;
      F->Raw (rraw, pt::x(points[R]), false);
      ffm::copy_BE(sig.r, rraw); 
      
      F->Raw (sraw, s, false);
      ffm::copy_BE(sig.s, sraw); 
      
      printf ("__SIGNATURE_GENERATED__\n");
    }
    
    return true; 
    
  }

} // namespace  


