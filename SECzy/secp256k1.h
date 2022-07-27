#ifndef SECP256K1_INCLUDE
#define SECP256K1_INCLUDE

#include "common.h"
#include <ffm/ffm.h>
#include <aframe/af.h>


namespace SECzy {

  //
  //
  typedef af::digest32 digest32;  
  typedef af::fixnum32 fixnum32; 
  //
  //
  struct Point 
  {
    fixnum32 x;
    fixnum32 y; 
  };

  typedef Point PublicKey;
  
  //
  struct PrivateKey {
    fixnum32 e;
  }; 

  // 
  // 
  struct Signature {
    fixnum32 s;
    fixnum32 r; 
  };

  //
  // Serialization 
  bool ReadPoint         (Point& out , af::ReadStreamRef rs);
  bool ReadSignature_DER (Signature& out, af::ReadStreamRef rs);
  //
  bool WritePoint         (af::WriteStreamRef ws, const Point& pt, bool compressed);
  bool WriteSignature_DER (af::WriteStreamRef ws, const Signature& sig);  
  
		       

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
  

}

#endif
