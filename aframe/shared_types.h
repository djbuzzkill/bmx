#ifndef AF_SHARED_TYPES_H
#define AF_SHARED_TYPES_H



#include "common.h"


namespace af
{
  //
  typedef std::vector<unsigned char>         bytearray; 
  
  typedef std::array<unsigned char, 32>      byte32; // 
  typedef std::array<unsigned char, 20>      byte20; //  ripemd160

  typedef byte32                             bigint32;
  typedef byte32                             fixnum32; 



 
  //
  //
  struct destructor {
    
    virtual ~destructor () {}

  protected:
    destructor () {}
  }; 
  
}


#endif
