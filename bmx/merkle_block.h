#ifndef BMX_MERKLE_BLOCK_H
#define BMX_MERKLE_BLOCK_H


#include "common.h"
#include "utility.h"
#include "aframe/binary_IO.h"

namespace bmx {

  namespace MerkleTree {
    //  
    struct Struc {
      int32      total;
      int32      max_depth;
      nodearray nodes;
      hasharray shbukkit;
    };
    //
    Struc&      Init     (Struc& mt, int32 total) ;
    Struc&      Populate (Struc& mt, const bytearray& bits, const hasharray& hashes); 
    // 
    std::string&       Format (std::string& os, const Struc& mt) ;
    inline std::string Format (const Struc& mt) {
      std::string ret = "";  return Format  (ret, mt);
    }

    //
    namespace Node {

      // values of nodearray (int32) represent
      //  | = index into hash bukkit  
      //  | = nil
      const int32 kNIL      = ~0x0; 
      // just keep inds of nodes 
      inline int32 Root        () { return 1; }
      inline int32 Depth       (int32 ind) { return std::log2(ind); }
      inline int32 Parent      (int32 ind) { return (ind / 2); }
      inline int32 LeftChild   (int32 ind) { return (ind * 2); }
      inline int32 RightChild  (int32 ind) { return (ind * 2) + 1; }
      inline bool  IsLeaf      (const Struc& mt, int32 ind) { return mt.max_depth == Depth (ind); }
      // num nodes in row 
      inline int32 num_nodes (const Struc& mt, int32 depth) {
        return  std::ceil (mt.total / std::pow (2, mt.max_depth - depth));
      }
      // 
      inline bool RightChildExists (const Struc& mt, int32 ind) {
	if (IsLeaf (mt, ind))
	  return false;
	int32 rc = RightChild (ind); 
	int32 depth = Depth (rc);
	int32 diff  = rc - (1 << depth);  // (rc - beginning of row)
	return diff < num_nodes (mt, depth); 
      }
	  
      // get hash if exists
      inline bool Hash (digest32& out, const Struc& mt, int32 ind) {
	if (mt.nodes[ind] != kNIL) {
	  out = mt.shbukkit[mt.nodes[ind]];
	  return true;
	}
	return false;
      }  

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
