
#include "network.h"
#include "aframe/hash.h"


using namespace af;
using namespace ffm; 
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
uint64 bmx::Network::Envelope::Read(Struc& envel, ReadStreamRef rs, bool mainnet) {

  FN_SCOPE ();

  uint64 readlen = 0;

  uint32 readmagic = 0;
  readlen += rs->Read (&readmagic, sizeof(uint32));
  swap_endian<uint32> (&readmagic);

  if (mainnet) { 
    assert (readmagic == Network::kMAINNET_MAGIC);
  }
  else {
    
    assert (readmagic == Network::kTESTNET_MAGIC); 
  }

  envel.command.resize (12, byte(0));  
  readlen += rs->Read (&envel.command[0], 12); 
  while (envel.command.back () == byte{0x0}) envel.command.pop_back (); 
  
  uint32 payloadlen = 0;
  readlen += rs->Read (&payloadlen, sizeof(uint32));

  uint32 rdchecksum = 0; 
  readlen += rs->Read (&rdchecksum, sizeof (uint32)); 

  envel.payload.resize (payloadlen, byte{0});
  readlen += rs->Read(&envel.payload[0], payloadlen); 

  union {
    uint32 checksum;
    byte bytes_cs_[4]; 
  };
  
  digest32 dig ;
  af::hash256 (dig, &envel.payload[0], payloadlen);
  // hash256(nil) = 5df6e0e2761359d30a8275058e299fcc0381534545f55cf43e41983f5d4c9456
  std::copy (dig.begin (), dig.begin() + 4, bytes_cs_);

  if (rdchecksum != checksum)  {
    printf ("    [%s] CHECKSUM DOES NOT MATCH !!  \n", __FUNCTION__); 
    }

  return readlen; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
uint64 bmx::Network::Envelope::Write (WriteStreamRef ws, const Struc& nv, bool mainnet) {
  uint64 writelen = 0;
  
  // magic
  bytearray  magicbytes;
  if (mainnet)
    hex::decode (magicbytes, std::string (Network::kMAINNET_MAGIC_sz)); 
  else 
    hex::decode (magicbytes, std::string (Network::kTESTNET_MAGIC_sz)); 
  writelen += ws->Write (&magicbytes[0], magicbytes.size ()); 
  
  // command
  bytearray cmd_padd = nv.command;
  while (cmd_padd.size () < 12) cmd_padd.push_back (byte (0)); 
  writelen += ws->Write (&cmd_padd[0], 12); 
  
  // payload length
  uint32 len_payload = nv.payload.size (); 
  writelen +=  ws->Write (&len_payload, sizeof (uint32)); 
  
  // checksum bytes
  digest32 dig; 
  af::hash256 (dig, &nv.payload[0], nv.payload.size ());
  byte byte4 [4]; 
  std::copy (dig.begin(), dig.begin ()+4, byte4); 
  writelen += ws->Write (byte4, 4);
  
  // payload 
  writelen += ws->Write (&nv.payload[0], nv.payload.size ()); 
  
  // def serialize(self):
  //     '''Returns the byte serialization of the entire network message'''
  //     # add the network magic
  //     # command 12 bytes
  //     # fill with 0's
  //     # payload length 4 bytes, little endian
  //     # checksum 4 bytes, first four of hash256 of payload
  //     # payload
  //     result = self.magic
  //     result += self.command + b'\x00' * (12 - len(self.command))
  //     result += int_to_little_endian(len(self.payload), 4)
  //     result += hash256(self.payload)[:4]
  //     result += self.payload
  //     return result
  return writelen; 
}


#include "test_network.cpp"
