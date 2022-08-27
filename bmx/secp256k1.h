#ifndef BMX_SECP256K1_H
#define BMX_SECP256K1_H

#include "common.h"
#include <ffm/ffm.h>
#include <aframe/af.h>


namespace bmx {

  //
  //
  typedef af::digest32 digest32;  
  typedef af::fixnum32 fixnum32; 
  //
  //
  struct Point {
    fixnum32 x;
    fixnum32 y; 
  };

  //
  //
  typedef fixnum32 PrivateKey;

  //
  //
  typedef Point PublicKey;
  

  // 
  // 
  struct Signature {
    fixnum32 s;
    fixnum32 r; 
  };

  //

  //
  // 
  Point&       MakePublicKey (Point& out, const PrivateKey& sec); 
  std::string& MakeAddress   (std::string& out, bool compr, bool mainnet, const PublicKey& pubk); 
  // 
  std::string& MakeWIF       (std::string& out, bool compr, bool mainnet, const PrivateKey& prvk); 


  //
  // Serialization 
  size_t WritePoint         (af::WriteStreamRef ws, const Point& pt, bool compressed);
  size_t WriteSignature_DER (af::WriteStreamRef ws, const Signature& sig);  

  size_t ReadPoint          (Point& out , af::ReadStreamRef rs);
  size_t ReadSignature_DER  (Signature& out, af::ReadStreamRef rs);
  //
  
  // soon...
  namespace _secp256k1 {
    
    bool Sign   (Signature& outsig, const PrivateKey& privatekey, const digest32& msghash);
    bool Verify (const Signature& sig, const PublicKey& pubk, const digest32& z); 

  }



  //
  //
  class secp256k1 {
    //  3
  public:
    
    secp256k1 ();
    ~secp256k1 ();
    
    //bool Verify (const char* sz_z, const char* sz_r, const char* sz_s); 
    // bool Sign (unsigned char* z, unsigned char* r); 
    
    bool Sign   (Signature& outsig, const PrivateKey& privatekey, const digest32& msghash);
    bool Verify (const Signature& sig, const PublicKey& pubk, const digest32& z); 

  protected:
    
    ffm::FEConRef F;
    ffm::ECConRef EC;
    ffm::el::map elems;
    ffm::pt::map points; 
  }; 
  

  bool Init_FE_context    (ffm::FEConRef& ofecon); 
  bool Init_secp256k1_Env (ffm::FEConRef& ofecon, ffm::ECConRef& oeccon, ffm::el::map& em, ffm::pt::map& pm);
  

}

#endif
