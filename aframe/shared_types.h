
//
//
#ifndef AF_SHARED_TYPES_H
#define AF_SHARED_TYPES_H


#include "common.h"


namespace af
{
  // ---------------------------------------------------
  //
  // ---------------------------------------------------
  typedef std::uint8_t  uint8;
  typedef std::uint16_t uint16; 
  typedef std::uint32_t uint32; 
  typedef std::uint64_t uint64; 
  
  typedef std::int8_t   int8;
  typedef std::int16_t  int16; 
  typedef std::int32_t  int32; 
  typedef std::int64_t  int64;
  typedef std::byte     byte;


  // ---------------------------------------------------
  //
  // ---------------------------------------------------
  typedef std::vector<byte>         bytearray; 
  typedef std::array<byte, 32>      byte32; // 
  typedef std::array<byte, 20>      byte20; //  ripemd160

  typedef byte32                    bigint32;
  typedef byte32                    fixnum32;


  // ---------------------------------------------------
  // 
  // ---------------------------------------------------
  struct memrange {
    void*  p;
    uint64 len; 
  };

  typedef memrange        mr; 
  typedef std::vector<mr> mrvec;



  // ---------------------------------------------------
  //
  // ---------------------------------------------------
  struct destructor {
    
    virtual ~destructor () {}

  protected:
    destructor () {}
  }; 
  
}


#endif
