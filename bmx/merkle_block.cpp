#include "merkle_block.h"


using namespace af;
using namespace ffm;
using namespace bmx;



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
    const hasharray& hashs,
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
