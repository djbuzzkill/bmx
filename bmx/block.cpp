

#include "block.h"


//
//    
bmx::uint64 bmx::Block::Read(bmx::Block::Struc &oblk, af::ReadStreamRef rs) {

  uint64 readlen = 0;

  size_t sizeOf_prev_block = sizeof(oblk.prev_block);
  printf ("    sizeOf_prev_block:%zu\n", sizeOf_prev_block);

  readlen += rs->Read (&oblk.version  , sizeof(oblk.version));
  readlen += rs->Read (&oblk.prev_block, 32);
  readlen += rs->Read (&oblk.merkleroot, 32); 
  readlen += rs->Read (&oblk.timestamp , sizeof(oblk.timestamp));
  readlen += rs->Read (&oblk.bits      , sizeof(oblk.bits));
  readlen += rs->Read (&oblk.nonce     , sizeof(oblk.nonce)); 

  std::reverse (oblk.prev_block.begin(), oblk.prev_block.end()); 
  std::reverse (oblk.merkleroot.begin(), oblk.merkleroot.end()); 
   
  return readlen; 
}



//
//
bmx::uint64 bmx::Block::Write(af::WriteStreamRef ws, const Struc &oblk) {

  uint64 writelen = 0;
  //platform_endian (

  bytearray rprev_block (oblk.prev_block.rbegin(), oblk.prev_block.rend ()); 
  bytearray rmerkleroot (oblk.merkleroot.rbegin(), oblk.merkleroot.rend ()); 

  writelen += ws->Write (&oblk.version  , sizeof(oblk.version));
  writelen += ws->Write (&rprev_block, 32);
  writelen += ws->Write (&rmerkleroot, 32); 
  writelen += ws->Write (&oblk.timestamp , sizeof(oblk.timestamp));
  writelen += ws->Write (&oblk.bits      , sizeof(oblk.bits));
  writelen += ws->Write (&oblk.nonce     , sizeof(oblk.nonce)); 

  return writelen; 

}

