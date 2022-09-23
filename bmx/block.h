#ifndef BMX_BLOCK_INCLUDE
#define BMX_BLOCK_INCLUDE

#include "common.h"
#include "utility.h"


#include "aframe/binary_IO.h"





namespace bmx {

  namespace Block {

    struct Struc {
      uint32   version;
      byte32   prev_block; 
      byte32   merkleroot;
      uint32   timestamp;
      uint32   bits;
      uint32   nonce; 
    };

    uint64 Read  (Struc &oblk, af::ReadStreamRef rs); 
    uint64 Write (af::WriteStreamRef ws, const Struc &oblk);

  } 


  
}


#endif
