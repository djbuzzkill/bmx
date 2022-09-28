#ifndef FFM_SHARED_TYPES_H
#define FFM_SHARED_TYPES_H

#include "common.h"
#include <aframe/shared_types.h>


namespace ffm {
  //
  typedef af::int8       int8 ; 
  typedef af::int16      int16 ; 
  typedef af::int32      int32 ; 
  typedef af::int64      int64 ;

  typedef af::uint8      uint8 ; 
  typedef af::uint16     uint16; 
  typedef af::uint32     uint32; 
  typedef af::uint64     uint64; 

  typedef af::fixnum32   fixnum32;  
  typedef af::byte       byte; 
  typedef af::bytearray  bytearray; 

  //
  //
  typedef int32          FE_t;
  const   FE_t           fe_null  = 0;

  inline bool FE_INF (FE_t x) { return (x < 0 ? true : false); }


  
}


#endif
