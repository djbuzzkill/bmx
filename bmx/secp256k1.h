#ifndef BMX_SECP256K1_H
#define BMX_SECP256K1_H

#include "common.h"
#include <ffm/ffm.h>
#include <aframe/af.h>


namespace bmx {

  //
  typedef af::digest32  digest32;  
  typedef af::fixnum32  fixnum32; 
  typedef af::bytearray bytearray;
  //
  struct Point {
    fixnum32 x;
    fixnum32 y; 
  };

  //
  typedef Point PublicKey;

  //
  typedef fixnum32 PrivateKey;


  // 
  struct Signature {
    fixnum32 r; 
    fixnum32 s;
  };


  //
  namespace secp256k1 
  {
    
    bool Sign   (Signature& outsig, const PrivateKey& privatekey, const digest32& msghash);
    bool Verify (const Signature& sig, const PublicKey& pubk, const digest32& z); 

  }

  //
  // 
  Point&       MakePublicKey (Point& out, const PrivateKey& sec); 
  std::string& MakeAddress   (std::string& out, const PublicKey &pubk, bool compr, bool mainnet); 


  // 
  std::string& MakeWIF       (std::string& out, bool compr, bool mainnet, const PrivateKey& prvk); 
  //
  // Serialization 
  size_t ReadPoint           (Point& out , af::ReadStreamRef rs);
  size_t WritePoint          (af::WriteStreamRef ws, const Point& pt, bool compressed);

  size_t ReadSignature_DER   (Signature& out, size_t binsize, af::ReadStreamRef rs);
  size_t WriteSignature_DER  (af::WriteStreamRef ws, const Signature& sig);  

  //
  inline size_t ReadPublicKey_SEC (Point& out , af::ReadStreamRef rs) {
    return  ReadPoint (out, rs);
  }

  //
  inline size_t WritePublicKey_SEC (af::WriteStreamRef ws, const Point& pt, bool compressed) {
    return WritePoint (ws, pt, compressed);
  } 

  //
  inline bool SECP256k1_Verify (const Signature& sig, const PublicKey& pubk, const digest32& z) {
    return secp256k1::Verify (sig, pubk, z); 
  }

  //
  inline bool SECP256k1_Sign (Signature& sig, const PrivateKey& privk, const digest32& zbin) {
    return secp256k1::Sign (sig, privk, zbin); 
  }

  // all things ffm together
  struct FFM_Env { 
    //
    ffm::FEConRef F;
    ffm::ECConRef EC;
    ffm::el::map  em;
    ffm::pt::map  pm; 

  }; 
  //
  bool        Init_FE_context    (ffm::FEConRef& ofecon); 
  bool        Init_secp256k1_Env (ffm::FEConRef& ofecon, ffm::ECConRef& oeccon, ffm::el::map& em, ffm::pt::map& pm);
  inline bool Init_secp256k1_Env (FFM_Env& env) { return Init_secp256k1_Env (env.F, env.EC, env.em, env.pm); }  

  digest32&  Deterministic_K (digest32& kout, const PrivateKey& priv, const digest32& z);
  
}

#endif
