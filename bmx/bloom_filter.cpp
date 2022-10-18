
//
#include "bloom_filter.h"
#include "utility.h"

using namespace af;
using namespace ffm;
using namespace bmx;


namespace { // flag bits
  const byte F_ON  {1}; 
  const byte F_OFF {0};
}

//  
// 
BloomFilter::Struc& BloomFilter::Init (BloomFilter::Struc& bf, uint8 size, uint8 numfns, uint32 tweak) {

  bf.size = size; 
  bf.bitfield.resize (size * 8, F_OFF);
  bf.function_count = numfns;
  bf.tweak = tweak;
  return bf; 
}

// 
//
BloomFilter::Struc& BloomFilter::Add (BloomFilter::Struc& bf, const bytearray& item) {

  const uint32 bf_size = bf.bitfield.size(); 

  for (auto i = 0; i < bf.function_count; ++i) {

    uint32 seed = i * kBIP0037_CONSTANT + bf.tweak; 

    uint32 b = murmur3_x86_32 (&item[0], item.size(), seed); 

    bf.bitfield[b % bf_size] = F_ON; 
  }

  return bf; 
}

//
//
uint64 BloomFilter::Write (WriteStreamRef ws, const BloomFilter::Struc& bf, byte flag) {

  uint64 writelen = 0; 

  // size of bitfield
  writelen += util::write_varint (ws, bf.size);


  bytearray bits (bf.size, byte(0));


  
  bitfield_to_bytes (bits, bf.bitfield); 
  // 
  writelen += ws->Write (&bits[0], bf.size);

  // function count
  writelen += ws->Write (&bf.function_count,  sizeof(uint32)); 
  // tweak
  writelen += ws->Write (&bf.tweak,  sizeof(uint32));
  // flag
  assert (flag == F_OFF || flag == F_ON); 
  writelen += ws->Write (&flag , 1);
  return writelen; 
}

// 
uint64 BloomFilter::Read (Struc& bf, byte& flag, af::ReadStreamRef rs) {
  uint64 readlen = 0;

  uint64 sizeOf_field = 0;

  readlen += util::read_varint(sizeOf_field, rs); ; 

  bf.bitfield.resize (sizeOf_field * 8);
  bytearray bits (sizeOf_field, byte(0)); 

  readlen += rs->Read (&bits[0], sizeOf_field); 
  bytes_to_bitfield (bf.bitfield, bits); 
  
  readlen += rs->Read (&bf.function_count, sizeof(uint32)); ; 
  
  readlen += rs->Read (&bf.tweak, sizeof(uint32)); ; 

  readlen += rs->Read (&flag, 1) ; 

  
  return readlen; 
}

#include "test/test_bloom_filter.cpp"
