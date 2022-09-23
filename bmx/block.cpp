

#include "block.h"


//
//    
bmx::uint64 bmx::Block::Read(bmx::Block::Struc &oblk, af::ReadStreamRef rs) {

  uint64 readlen = 0;

// def parse(cls, s):
//     version = little_endian_to_int(s.read(4))
//     prev_block = s.read(32)[::-1]
//     merkle_root = s.read(32)[::-1]
//     timestamp = little_endian_to_int(s.read(4))
//     bits = s.read(4)
//     nonce = s.read(4)
//     return cls(version, prev_block, merkle_root, timestamp, bits, nonce)

  return readlen; 

  
}

//
//
bmx::uint64 bmx::Block::Write(af::WriteStreamRef ws, const Struc &oblk) {

  uint64 writelen = 0;

// def serialize(self):
//     result = int_to_little_endian(self.version, 4)
//     result += self.prev_block[::-1]
//     result += self.merkle_root[::-1]
//     result += int_to_little_endian(self.timestamp, 4)
//     result += self.bits
//     result += self.nonce
//     return result

  return writelen; 

}

