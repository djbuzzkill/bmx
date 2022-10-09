#include "merkle_block.h"


using namespace af;
using namespace bmx;


const int32 kNil = -1;
const int32 k_nil_node = kNil; 

//
MerkleTree::Struc& MerkleTree::Init (Struc& mt, int32 total) {

  mt.total     = total;
  mt.max_depth = std::log2 (total);

  //
  // * total+1 b/c, index starts at 1 for  binary tree to work
  mt.nodes.resize (total+1, k_nil_node); 
  
  // mt.cur_depth = 0;
  // mt.cur_index = 0; 

  mt.hbukkit; // do later
  return mt; 
}

MerkleTree::Struc& MerkleTree::Populate (Struc& mt, const bytearray& bits, const hash_array& hashes) {

  const byte b00 {0}; 
  auto hash_ind = 0; 
  auto bit_ind  = 0; 

  // new_ind is where we put created hashes
  auto new_ind = hashes.size(); 
  
  mt.hbukkit.resize (hashes.size() * 2);
  //
  for (auto i = 0; i < hashes.size (); ++i) {
    std::copy (hashes[i].begin (), hashes[i].end(), mt.hbukkit[i].begin ()); 
  }

  // node index
  int32 curnode = MerkleTree::Node::Root (); 

  while (MerkleTree::Node::Val (mt, curnode) == k_nil_node) {

    if (MerkleTree::Node::IsLeaf (mt, curnode)) {
      bit_ind++;
      MerkleTree::Node::Val(mt, curnode) = hash_ind++;
      curnode = MerkleTree::Node::Parent (curnode);
    }
    else {

      int32 lchild_val = MerkleTree::Node::Val(mt, MerkleTree::Node::LeftChild(curnode));
      if (lchild_val == k_nil_node) {

        if (bits[bit_ind++] == b00) {

          MerkleTree::Node::Val(mt, curnode) = hash_ind++;
	  curnode = MerkleTree::Node::Parent (curnode);

        } else {
          curnode = MerkleTree::Node::LeftChild(curnode);
        }

      }
      //else if (MerkleTree::Node::Exists (mt, MerkleTree::Node::RightChild (curnode))) { 
      else if (MerkleTree::Node::RightChildExists (mt, curnode)) { 

	int32 rchild_val = MerkleTree::Node::Val(mt, MerkleTree::Node::RightChild(curnode));
	if (rchild_val == k_nil_node) {
	  curnode = MerkleTree::Node::RightChild(curnode); 
	}
        else {

          // give it storage
	  int32 new_bukkit_ind = new_ind++; 
	  MerkleTree::Node::Val(mt, curnode) = new_bukkit_ind;   

	  bytearray htmp (64, byte{0});
	  WriteStreamRef ws = af::CreateWriteMemStream (&htmp[0], 64);
	  ws->Write (&mt.hbukkit[lchild_val], 32); 
	  ws->Write (&mt.hbukkit[rchild_val], 32);
	  // hashit 
          af::hash256 (mt.hbukkit[new_bukkit_ind], &htmp[0], 64); 
	  
	  curnode = MerkleTree::Node::Parent (curnode); 
	}

      }
      else {

	int32 new_bukkit_ind = new_ind++; 
	MerkleTree::Node::Val(mt, curnode) = new_bukkit_ind ;   

	bytearray htmp (64, byte{0});
	WriteStreamRef ws = af::CreateWriteMemStream (&htmp[0], 64);
	ws->Write (&mt.hbukkit[lchild_val], 32); 
	ws->Write (&mt.hbukkit[lchild_val], 32); 
	// hashit 
	af::hash256 (mt.hbukkit[new_bukkit_ind], &htmp[0], 64); 
	// move cursor up
	curnode = MerkleTree::Node::Parent (curnode);
      }

    }
  }
  
  // did u use them all?
  assert (hash_ind != hashes.size()); 
  assert (bit_ind == bits.size());
  
  
// def populate_tree(self, flag_bits, hashes):
    //     while self.root() is None:  # <1>
    //         if self.is_leaf():  # <2>
    //             flag_bits.pop(0)  # <3>
    //             self.set_current_node(hashes.pop(0))  # <4>
    //             self.up()
    //         else:
    //             left_hash = self.get_left_node()
    //             if left_hash is None:  # <5>
    //                 if flag_bits.pop(0) == 0:  # <6>
    //                     self.set_current_node(hashes.pop(0))
    //                     self.up()
    //                 else:
    //                     self.left()  # <7>
    //             elif self.right_exists():  # <8>
    //                 right_hash = self.get_right_node()
    //                 if right_hash is None:  # <9>
    //                     self.right()
    //                 else:  # <10>
    //                     self.set_current_node(merkle_parent(left_hash, 
    //                     right_hash))
    //                     self.up()
    //             else:  # <11>
    //                 self.set_current_node(merkle_parent(left_hash, left_hash))
    //                 self.up()
    //     if len(hashes) != 0:  # <12>
    //         raise RuntimeError('hashes not all consumed {}'.format(len(hashes)))
    //     for flag_bit in flag_bits:  # <13>
    //         if flag_bit != 0:
    //             raise RuntimeError('flag bits not all consumed')
    // # end::source3[]

  return mt; 
}


//
// int32 MerkleTree::Node::Depth (int32 ind) { return std::log2 (ind); }
// int32 MerkleTree::Node::Root (int32 ind) { return 1; }
// int32 MerkleTree::Node::Parent (int32 ind) { return (ind / 2); }
// int32 MerkleTree::Node::LeftChild (int32 ind) { return (ind * 2); }
// int32 MerkleTree::Node::RightChild (int32 ind) { return (ind * 2 + 1); }
// int32 MerkleTree::Node::Right (int32 ind) { return (std::log2 (ind) == std::log2(ind + 1) ? (ind + 1) : kNil); }
// int32 MerkleTree::Node::Left (int32 ind) { return (std::log2 (ind) == std::log2(ind - 1) ? (ind - 1) : kNil); }
// bool MerkleTree::Node::IsLeaf (const MerkleTree::Struc& mt, int32 ind) {  return ( !Exists(mt, MerkleTree::Node::LeftChild(ind)) && !Exists (mt, MerkleTree::Node::RightChild (ind)) ); }
// bool MerkleTree::Node::Exists (const MerkleTree::Struc& mt, int32 ind) {  return (mt.nodes[ind] != kNil); }

// this is useless
MerkleBlock::Struc& MerkleBlock::Init (
    MerkleBlock::Struc& mb,
    uint32 ver,
    const byte32& prev_bl,
    const byte32& merkle_r,
    uint32 ts,
    uint32 bts,
    uint64 nce,
    uint32 tot,
    const hash_array& hashs,
    const bytearray flags)
{

  // def __init__(self, version, prev_block, merkle_root, timestamp, bits,
  // nonce, total, hashes, flags):
  //     self.version = version
  //     self.prev_block = prev_block
  //     self.merkle_root = merkle_root
  //     self.timestamp = timestamp
  //     self.bits = bits
  //     self.nonce = nonce
  //     self.total = total
  //     self.hashes = hashes
  //     self.flags = flags

  return mb; 
  
} 


bool MerkleBlock::IsValid (const MerkleBlock::Struc& mb) {


  return false; 
  }

//
//
uint64 MerkleBlock::Read (MerkleBlock::Struc& mb, ReadStreamRef rs, bool mainnet) {

  uint64 readlen = 0; 

  return readlen; 
}

//
//
uint64 MerkleBlock::Write (WriteStreamRef ws, const MerkleBlock::Struc &mb, bool mainnet) {

  uint64 writelen = 0;


  return writelen =0; 
  
}


//#ifdef DEFINE_BMX_TESTS
#include "test/test_merkle.cpp"
//#endif
