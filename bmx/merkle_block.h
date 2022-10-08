#ifndef BMX_MERKLE_BLOCK_H
#define BMX_MERKLE_BLOCK_H


#include "common.h"
#include "utility.h"
#include "aframe/binary_IO.h"

namespace bmx {

 namespace  MerkleTree {
 };

 
 namespace  MerkleBlock {

   typedef std::vector<digest32> hash_array; 
   
   struct Struc  {

     uint32 version;     // LE
     byte32 prev_block;  // LE
     byte32 merkle_root; // LE
     uint32 timestamp;   // LE
     
     uint32 bits;   // 
     uint32 nonce;  // 
     uint32 total;  // LE
     hash_array hashes; // read varint
     bytearray flags;   // also read varint 

   };


   uint64 Read  (Struc& , af::ReadStreamRef rs, bool mainnet);
   uint64 Write (af::WriteStreamRef ws, const Struc& mb, bool mainnet);
   
 }; 

 typedef MerkleBlock::Struc merkleblock; 
}

#endif
