#ifndef SECP256K1_INCLUDE
#define SECP256K1_INCLUDE

#include "common.h"
#include "ffm/ffm.h"
#include "aframe/af.h"


namespace SECzy {

  typedef af::digest32 digest32;  
  
  struct PrivateKey {
    ffm::fixnum32 sec;
  }; 

  struct PublicKey {
    ffm::fixnum32 x;
    ffm::fixnum32 y;
  }; 
  
  struct Signature {
    ffm::fixnum32 s;
    ffm::fixnum32 r; 
  };
  
    
  class secp256k1 {
    //  
  public:
    
    secp256k1 ();
    ~secp256k1 ();
    
    bool Verify (const char* sz_z, const char* sz_r, const char* sz_s); 
    // bool Sign (unsigned char* z, unsigned char* r); 
    
    bool Sign (Signature& sig, const PrivateKey&, const digest32& z);
    bool Verify (const Signature& sig, const PublicKey& pubk, const digest32& z); 

  protected:
    
    ffm::FEConPtr F;
    ffm::ECConRef EC;
    ffm::el::map elems;
    ffm::pt::map points; 
  }; 
  

}

#endif
