#include "merkle_block.h"
#include "utility.h"


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
  FN_SCOPE();  

  bytearray flag_bits; 
  merkle_ut::bytes_to_bit_field (flag_bits, mb.flags); 


  merkletree mt;
  Init (mt, mb.total);


  bytearray tmpbytes; 
  hasharray hashes (mb.hashes.size());
  // reverse to be BE 
  for (auto i = 0 ; i < mb.hashes.size(); ++i) 
    std::reverse_copy(&mb.hashes[i][0], &mb.hashes[i][32], &hashes[i][0]); 
    
  //
  MerkleTree::Populate (mt, flag_bits, hashes);
  
  digest32 roothash;
  if (MerkleTree::Node::Hash (roothash, mt, MerkleTree::Node::Root())) {
    std::reverse (&roothash[0], &roothash[32]); 
    //printf ( "Hash! block[%s]\ntest[%s]\n", fmthex(mb.merkle_root).c_str(), fmthex(roothash).c_str()); 
    return  mb.merkle_root == roothash; 
  }
  
  return  false; 
}






//
//
uint64 MerkleBlock::Read (MerkleBlock::Struc& mb, ReadStreamRef rs, bool mainnet) {
  FN_SCOPE ();

  uint64 readlen = 0;

  bytearray tmp32(32);
  readlen += rs->Read (&mb.version, sizeof (uint32)); //  LE 
  readlen += rs->Read (&tmp32[0], 32);
  std::reverse_copy  (&tmp32[0] , &tmp32[32], &mb.prev_block[0]); 
  
  readlen += rs->Read (&tmp32[0], 32);
  std::reverse_copy   (&tmp32[0], &tmp32[32], &mb.merkle_root[0]); 

  readlen += rs->Read (&mb.timestamp, sizeof(uint32)); // LE

  readlen += rs->Read (&mb.bits, sizeof(uint32)); 
  readlen += rs->Read (&mb.nonce, sizeof(uint32)); 
  readlen += rs->Read (&mb.total, sizeof(uint32)); // LE

  uint64 num_hashes = 0; // num_hashes <-- varint
  readlen += util::read_varint (num_hashes, rs, "num_hashes");
  printf ("    num_hashes(%zu)\n", num_hashes);
  mb.hashes.resize (num_hashes);
  //printf ("  mb.hashes.size(%zu)\n", mb.hashes.size ());

  // printf ( "    sizeof(hash_array::value_type):%i\n", sizeof(MerkleBlock::hash_array::value_type)); 
  // printf ( "    sizeof digest32 : %i\n",  sizeof (digest32)); 
  printf ("   &mb.hashes:%p\n", &mb.hashes); 
  printf ("   &mb.hashes[0]:%p\n", &mb.hashes[0]); 
  
  for (uint64 i = 0; i < num_hashes; ++i) {
    //digest32& r = mb.hashes[i]; 
    printf ("   readlen[%zu]\n", readlen); 
    readlen += rs->Read (&tmp32[0], sizeof (digest32));
    std::reverse_copy (&tmp32[0], &tmp32[32], mb.hashes[i].begin()); 
  }
  
  uint64  num_flags = 0; 
  readlen += util::read_varint (num_flags, rs, "num_flags");
  printf ("    num_flags(%zu)\n", num_flags);
  mb.flags.resize (num_flags);
  readlen += rs->Read (&mb.flags[0], num_flags);

  printf ("  return readlen %zu\n", readlen); 
  return readlen; 
}




 // namespace  MerkleBlock {

 //   typedef std::vector<digest32> hash_array; 
   
 //   struct Struc  {

 //     uint32 version;     // LE
 //     byte32 prev_block;  // LE
 //     byte32 merkle_root; // LE
 //     uint32 timestamp;   // LE
     
 //     uint32 bits;   // 
 //     uint32 nonce;  // 
 //     uint32 total;  // LE
 //     hash_array hashes; // read varint
 //     bytearray flags;   // also read varint 

 //   };

//
//
uint64 MerkleBlock::Write (WriteStreamRef ws, const MerkleBlock::Struc &mb, bool mainnet) {

  uint64 writelen = 0;


  mb.version;
  mb.prev_block;
  mb.merkle_root;
  mb.timestamp;
  mb.bits;
  mb.nonce;
  mb.total;

  writelen += ws->Write (&mb.version, sizeof(uint32)); 
  writelen += ws->Write (&mb.prev_block, 32);
  
  // bytearray tmp32(32);
  // readlen += rs->Read (&mb.version, sizeof (uint32)); //  LE 
  // readlen += rs->Read (&tmp32[0], 32);
  // std::reverse_copy  (&tmp32[0] , &tmp32[32], mb.prev_block.begin()); 
  
  // readlen += rs->Read (&tmp32[0], 32);
  // std::reverse_copy   (&tmp32[0], &tmp32[32], mb.merkle_root.begin()); 

  // readlen += rs->Read (&mb.timestamp, sizeof(uint32)); // LE

  // readlen += rs->Read (&mb.bits, sizeof(uint32)); 
  // readlen += rs->Read (&mb.nonce, sizeof(uint32)); 
  // readlen += rs->Read (&mb.total, sizeof(uint32)); // LE



  
  return writelen =0; 
  
}


//
bytearray& bmx::merkle_ut::bytes_to_bit_field (bytearray &oflags, const bytearray& bits) {
  FN_SCOPE ();

  const byte b00 {0x0}; 
  const byte b01 {0x1}; 
  
  oflags.clear (); 
  for (auto b : bits) {
    uint8 bitfield =  std::to_integer<uint8>(b); 
    for (auto i = 0; i < 8; ++i) {
      oflags.push_back (bitfield & 0x1 ? b01 : b00);  
      bitfield >>= 1; 
    }
  }

  // # tag::source1[]
  // def bytes_to_bit_field(some_bytes):
  //     flag_bits = []
  //     for byte in some_bytes:
  //         for _ in range(8):
  //             flag_bits.append(byte & 1)
  //             byte >>= 1
  //     return flag_bits
  // # end::source1[]
  return oflags; 
}

//
//
bytearray& bmx::merkle_ut::bit_field_to_bytes (bytearray &obits, const bytearray &flags) {
  FN_SCOPE(); 
  assert ((flags.size () % 8) == 0);
  if ((flags.size () % 8) != 0)
    return obits;

  const byte b01 {0x1}; 
  
  obits.resize (flags.size () / 8); 
  for (auto i = 0; i < flags.size(); ++i) {
    uint8 ibit  = i % 8; 
    uint8 ibyte = i / 8; 

    if (flags[i] == b01) 
      obits[ibyte] |= byte(1 << ibit); 
    
  }
  
  // def bit_field_to_bytes(bit_field):
  //     if len(bit_field) % 8 != 0:
  //         raise RuntimeError('bit_field does not have a length that is divisible by 8')
  //     result = bytearray(len(bit_field) // 8)
  //     for i, bit in enumerate(bit_field):
  //         byte_index, bit_index = divmod(i, 8)
  //         if bit:
  //             result[byte_index] |= 1 << bit_index
  //     return bytes(result)

  return obits; 
  
}

  

//#ifdef DEFINE_BMX_TESTS
#include "test/test_merkle.cpp"
//#endif
