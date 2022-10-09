#include "merkle_block.h"


using namespace af;
using namespace ffm;
using namespace bmx;


const int32 kNil = -1;
const int32 k_nil_node = kNil;


//
MerkleTree::Struc& MerkleTree::Init (Struc& mt, int32 total) {

  mt.total     = total;
  mt.max_depth = std::ceil(std::log2 (total)); 
  
  //
  // * total+1 b/c, index starts at 1 for binary tree to work
  mt.nodes.resize (total+1, k_nil_node); 
  
  return mt; 
}

//
MerkleTree::Struc& MerkleTree::Populate (Struc& mt, const bytearray& bits, const hasharray& hashes) {

  const byte b00 {0}; 
  auto  hash_ind = 0; 
  auto  bit_ind  = 0; 

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
      else if (MerkleTree::Node::RightChildExists (mt, curnode)) { 

	int32 rchild_val = MerkleTree::Node::Val(mt, MerkleTree::Node::RightChild(curnode));
	if (rchild_val == k_nil_node) {
	  curnode = MerkleTree::Node::RightChild(curnode); 
	}
        else {
          // give it storage
	  int32 new_bukkit_ind = new_ind++; 
	  MerkleTree::Node::Val(mt, curnode) = new_bukkit_ind;   

	  bytearray bytes (64, byte{0});
	  WriteStreamRef ws = af::CreateWriteMemStream (&bytes[0], 64);
	  ws->Write (&mt.hbukkit[lchild_val], 32); 
	  ws->Write (&mt.hbukkit[rchild_val], 32);
	  // hashit 
          af::hash256 (mt.hbukkit[new_bukkit_ind], &bytes[0], 64); 
	  
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

  return mt; 
}


std::string& MerkleTree::Format (std::string& ret, const MerkleTree::Struc& mt) {

  std::array<char, 512> buf; 
    
  sprintf (&buf[0], "{ total:%i, max_depth:%i\n", mt.total, mt.max_depth); 
  ret = &buf[0];

  for (int32 lvl = 0; lvl < mt.max_depth+1; ++lvl) {
    std::string row = "";

    int n = MerkleTree::Node::num_nodes (mt, lvl); 
    sprintf (&buf[0], "  depth:%i | num_nodes:%i\n", lvl, n);
    row = &buf[0]; 
    ret += row;     
  }
  ret += "}"; 
  return ret; 
}


