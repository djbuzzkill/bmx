#ifndef BMX_BLOCK_INCLUDE
#define BMX_BLOCK_INCLUDE

#include "common.h"
#include "utility.h"


#include "aframe/binary_IO.h"





namespace bmx {
  //
  namespace Block {

    struct Struc { 
      uint32 version;
      byte32 prev_block; 
      byte32 merkleroot;
      uint32 timestamp;
      uint32 bits;
      uint32 nonce;
    };

    // IO 
    uint64    Read             (Struc &oblk, af::ReadStreamRef rs); 
    uint64    Write            (af::WriteStreamRef ws, const Struc &oblk);
    //
    digest32& Hash             (digest32& out, const Struc &oblk);
    fixnum32& Target           (fixnum32&, const Struc &oblk);
    fixnum32& Difficulty       (fixnum32&, const Struc &oblk); 
    uint32    CalculateNewBits (uint32 prevbits, uint32 timediff); 
    bool      CheckPoW         (const Struc& blk); 

    // test capability of block
    namespace Cap
    { 
      bool bip9   (const Struc& blk); 
      bool bip91  (const Struc& blk); 
      bool bip141 (const Struc& blk);
    }

  }

  //
  typedef Block::Struc       block;
  typedef std::vector<block> blockarray;
}

static_assert (sizeof(bmx::block) == 80, "size of block"); 

#endif
