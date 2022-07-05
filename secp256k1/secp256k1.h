#ifndef SECP256K1_INCLUDE
#define SECP256K1_INCLUDE

#include "common.h"
#include "ffm/ffm.h"




class secp256k1
{
public:

  secp256k1 ();
  ~secp256k1 ();
  
  bool Verify (const char* sz_z, const char* sz_r, const char* sz_s); 
 
  bool Sign (unsigned char* z, unsigned char* r); 
  
protected:


 
  ffm::FEConPtr F;
  ffm::ECConRef EC;
  ffm::el::map elems;
  ffm::pt::map points; 
  
}; 




#endif
