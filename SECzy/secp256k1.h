#ifndef SECP256K1_INCLUDE
#define SECP256K1_INCLUDE

#include "common.h"
#include "ffm/ffm.h"
#include "aframe/af.h"


namespace SECzy {

  //
  //
  typedef af::digest32  digest32;  
  typedef ffm::fixnum32 fixnum32; 
  //
  //
  struct PublicKey 
  {
    fixnum32 x;
    fixnum32 y; 
  };

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
  template<typename Pt>
  inline bool WritePoint (af::WriteStreamRef ws, const Pt& pt, bool compressed) {


    return false;
    
  };     
    
  //
  template<typename Pt>
  inline bool ReadPoint (Pt& out, af::ReadStreamRef rs) {
    return false;

  }
  
  //
  void WriteSignature_DER (af::WriteStreamRef ws, const Signature& sig); 

  //
  //
  class secp256k1 {
    //  
  public:
    
    secp256k1 ();
    ~secp256k1 ();
    
    //bool Verify (const char* sz_z, const char* sz_r, const char* sz_s); 
    // bool Sign (unsigned char* z, unsigned char* r); 
    
    bool Sign   (Signature& outsig, const PrivateKey& privatekey, const digest32& msghash);
    bool Verify (const Signature& sig, const PublicKey& pubk, const digest32& z); 

  protected:
    
    ffm::FEConPtr F;
    ffm::ECConRef EC;
    ffm::el::map elems;
    ffm::pt::map points; 
  }; 
  

}

#endif
