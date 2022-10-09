#ifndef BMX_MERKLE_BLOCK_H
#define BMX_MERKLE_BLOCK_H


#include "common.h"
#include "utility.h"
#include "aframe/binary_IO.h"

namespace bmx {

  namespace MerkleTree {

    const int32 k_nil_node = ~0x0; 
    // 
    // values of node_array (int32) represent
    //  | = hash ref 
    //  | = nil
    // hash reference or a nil value (!exists)
    typedef std::vector<int32>    node_array;
    // this is where we keep hashes
    typedef std::vector<digest32> hash_array;
    //
    struct Struc {

      int32      total;
      int32      max_depth;
      node_array nodes;
      hash_array hbukkit;
    };
    //
    Struc&      Init     (Struc& mt, int32 total) ;
    std::string Format   (const Struc& mt) ;
    Struc&      Populate (Struc& mt, const bytearray& bits, const hash_array& hashes); 

    //
    namespace Node {
     // just keep inds of nodes 
      inline int32        Root        () { return 1; }
      inline int32        Depth       (int32 ind) { return std::log2(ind); }
      inline int32        Parent      (int32 ind) { return (ind / 2); }
      inline int32        LeftChild   (int32 ind) { return (ind * 2); }
      inline int32        RightChild  (int32 ind) { return (ind * 2) + 1; }


      inline int32 RightChildExists (const Struc& mt, int32 ind) {
	auto num_items = std::ceil (mt.total / (1 << (mt.max_depth - Depth(ind)))); 
        return num_items  > RightChild (ind); 
      }

	  
      //      inline bool         Exists      (const Struc& mt, int32 ind) { return (mt.nodes[ind] != k_nil_node); }
      inline bool         IsLeaf      (const Struc& mt, int32 ind) { return mt.max_depth == std::log2 (ind); } 
      inline int32&       Val         (Struc &mt, int32 ind) { return mt.nodes[ind]; }
      inline const int32& Val         (const Struc &mt, int32 ind) { return mt.nodes[ind]; }
      // inline int32        Right       (int32 ind) { return (Depth (ind) == Depth (ind + 1) ? (ind + 1) : k_nil_node); }
      // inline int32        Left        (int32 ind) { return (Depth (ind) == Depth (ind - 1) ? (ind - 1) : k_nil_node); }
    } // MerkleTree::Node 
  } // MerkleTree

 //
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

   // who rly needs this
   Struc& Init  (Struc &mb, uint32 ver, const byte32& prev_block, const byte32& merkle_root,
		 uint32 timestamp,
		 uint32 bits,
		 uint64 nonce,
		 uint32 total,
		 const hash_array& hashes,
		 const bytearray flags);
   

   bool   IsValid (const Struc& mb); 

   uint64 Read    (Struc& , af::ReadStreamRef rs, bool mainnet);
   uint64 Write   (af::WriteStreamRef ws, const Struc& mb, bool mainnet);
   
 }; 


 //
 //
 typedef MerkleBlock::Struc merkleblock;
 typedef MerkleTree::Struc  merkletree;

 }

#endif
