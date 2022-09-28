
#include "network.h"
#include "aframe/hash.h"


using namespace af;
using namespace ffm; 
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
uint64 bmx::Network::Envelope::Read (Struc& envel, ReadStreamRef rs, bool mainnet) {

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
  uint32  magicbytes = mainnet ? Network::kMAINNET_MAGIC : magicbytes = Network::kTESTNET_MAGIC;
  swap_endian<uint32>(&magicbytes); // comment about LE
  writelen += ws->Write (&magicbytes, sizeof(uint32)); 
  
  // command
  bytearray cmdpad = nv.command;
  while (cmdpad.size () < 12) cmdpad.push_back (byte (0)); 
  writelen += ws->Write (&cmdpad[0], 12); 
  
  // payload length
  uint32 payloadlen = nv.payload.size (); 
  writelen +=  ws->Write (&payloadlen, sizeof (uint32)); 
  
  // checksum bytes
  digest32 ges; 
  writelen += ws->Write (&af::hash256(ges, &nv.payload[0], nv.payload.size ())[0], 4);
  
  // payload 
  writelen += ws->Write (&nv.payload[0], nv.payload.size ()); 
  
  return writelen; 
}


#include "test_network.cpp"
