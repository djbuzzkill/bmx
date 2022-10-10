#include "merkle_block.h"


using namespace af;
using namespace ffm;
using namespace bmx;


//
//
MerkleTree::Struc& MerkleTree::Init (Struc& mt, int32 total) {

  mt.total     = total;
  mt.max_depth = std::ceil(std::log2 (total)); 
  //
  // always allocate full tree, indexing depends on it
  mt.nodes.resize (std::pow (2, mt.max_depth + 1) + 1, MerkleTree::Node::kNIL); 
  // * pow1 b/c, index starts at 1..(1-based)
  // * pow+1 b/c, index starts at 1..(1-based)
  return mt; 
}

// 
//
MerkleTree::Struc& MerkleTree::Populate (Struc& mt, const bytearray& bits, const hasharray& hashes) {

  FN_SCOPE ();

  const byte b00{0};
  const byte b01{1};

  const byte need_calc = b01; 
  const byte hash_incl = b00; 

  int32 bit_ind  = 0;

  mt.shbukkit.resize (hashes.size() + 8 * bits.size());
  //printf("    flag_bits(%zu), hashes(%zu), bukkit(%zu)\n", bits.size(), hashes.size(), mt.shbukkit.size()); 

  // copy into begining of bukkit
  for (auto i = 0; i < hashes.size (); ++i) {
    std::copy (hashes[i].begin (), hashes[i].end(), mt.shbukkit[i].begin ()); 
  }
  int32 shind = 0;
  // shind is used to 'pop_front' of bukkit

  int32  newsh = hashes.size();
  // newsh is where we put created hashes

  int32 curnode = MerkleTree::Node::Root (); 
  // start at the top
  while ( mt.nodes[curnode] == MerkleTree::Node::kNIL ) {
  //printf ("looP,node[%i], shid[%i], newsh[%i], bit_ind[%i]\n", curnode, shind, newsh, bit_ind); 

   if (MerkleTree::Node::IsLeaf (mt, curnode)) {
   //puts ( " is leaef\n"); 
      bit_ind++;
      mt.nodes[curnode] = shind++; // u git next hash
      curnode = MerkleTree::Node::Parent (curnode);
    }
    else {
    int32 lchild_val = mt.nodes[MerkleTree::Node::LeftChild(curnode)]; // MerkleTree::Node::hash(mt, MerkleTree::Node::LeftChild(curnode));
      if (lchild_val == MerkleTree::Node::kNIL) {
        if (bits[bit_ind++] == b00) {
	  mt.nodes[curnode] = shind++; // u get next hash
	  curnode = MerkleTree::Node::Parent (curnode);
        }
	else {
	   curnode = MerkleTree::Node::LeftChild(curnode);
        }
      }
      else if (MerkleTree::Node::RightChildExists (mt, curnode)) { 
	int32 rchild_val = mt.nodes[MerkleTree::Node::RightChild (curnode)]; 
	if (rchild_val == MerkleTree::Node::kNIL) {
	  curnode = MerkleTree::Node::RightChild(curnode); 
	}
        else {

	  std::array<byte, 64> bytes;
	  int32 hash_ind = mt.nodes[curnode] = newsh++;  // put in bukkit
         //printf (" HASH_LR, hash_ind[%i]", hash_ind); 
	  WriteStreamRef ws = af::CreateWriteMemStream (&bytes, 64);
	  ws->Write (&mt.shbukkit[lchild_val], 32); 
	  ws->Write (&mt.shbukkit[rchild_val], 32);
	  // hashit 
          af::hash256 (mt.shbukkit[hash_ind], &bytes, 64); 
	  curnode = MerkleTree::Node::Parent (curnode); 
	  //printf (" ..done\n"); 
	}
      }
      else {
	std::array<byte, 64> bytes;
	int32 hash_ind = mt.nodes[curnode] = newsh++; 
	//printf (" HASH_LL, hash_ind[%i]", hash_ind); 
	WriteStreamRef ws = af::CreateWriteMemStream (&bytes, 64);
	ws->Write (&mt.shbukkit[lchild_val], 32); 
	ws->Write (&mt.shbukkit[lchild_val], 32); 
	// hashit 
	af::hash256 (mt.shbukkit[hash_ind], &bytes, 64); 
	curnode = MerkleTree::Node::Parent (curnode);
	//printf ("..done\n"); 
      }
    }
  }
  
  // did u use them all?
  assert (shind != hashes.size());

  assert (bit_ind != bits.size());

  return mt; 
}

//
//
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

