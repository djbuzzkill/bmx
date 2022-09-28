
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



  

  return writelen; 
}






int test_read_network_envelope (const std::vector<std::string>& args) {

  FN_SCOPE ();

  using namespace af; 
  using namespace bmx;

  PR ("95");   
  const bool mainnet = true; 

  {
    std::string msghex = "f9beb4d976657261636b000000000000000000005df6e0e2";

    bytearray verack = { byte{'v'}, byte{'e'}, byte{'r'}, byte{'a'}, byte{'c'}, byte{'k'} } ;

    bytearray  msgbin; 
    hex::decode (msgbin, msghex);

    ReadStreamRef rs = CreateReadMemStream (&msgbin[0], msgbin.size());
    PR ("107");   

    network_envelope netenv;
    Network::Envelope::Read (netenv, rs , mainnet);

    PR_CHECK("command == \'verack\'", eql (verack, netenv.command));  
    PR_CHECK("payload is empty[%zu]",  netenv.payload.empty()); 

  }  
  // def test_parse () : 
  //       msg = bytes.fromhex('f9beb4d976657261636b000000000000000000005df6e0e2')
  //       stream = BytesIO(msg)
  //       envelope = NetworkEnvelope.parse(stream)
  //       self.assertEqual(envelope.command, b'verack')
  //       self.assertEqual(envelope.payload, b'')

  {
    std::string msghex = "f9beb4d976657273696f6e0000000000650000005f1a69d2721101000100000000000000bc8f5e5400000000010000000000000000000000000000000000ffffc61b6409208d010000000000000000000000000000000000ffffcb0071c0208d128035cbc97953f80f2f5361746f7368693a302e392e332fcf05050001";
    bytearray  msgbin; 
    hex::decode (msgbin, msghex);

    printf ("      msgbin[%zu]\n",  msgbin.size ()); 
    pr_a (msgbin);

    
    bytearray version = {
      byte{'v'},
      byte{'e'},
      byte{'r'},
      byte{'s'},
      byte{'i'},
      byte{'o'},
      byte{'n'}
    };
    
    printf ("      145[%zu]\n",  msgbin.size ());

    network_envelope netenv; 
    Network::Envelope::Read (netenv, CreateReadMemStream (&msgbin[0], msgbin.size()), mainnet);

    PR_CHECK("command == \'version\'", eql (version, netenv.command) ); 
    PR_CHECK("payload == msg[24:]", eql (netenv.payload, bytearray (msgbin.begin()+24, msgbin.end()) )); 

  }

  return 0; 
}



int test_write_network_envelope (const std::vector<std::string>& args) {
  

    // def test_serialize(self):
    //     msg = bytes.fromhex('f9beb4d976657261636b000000000000000000005df6e0e2')
    //     stream = BytesIO(msg)
    //     envelope = NetworkEnvelope.parse(stream)
    //     self.assertEqual(envelope.serialize(), msg)
    //     msg = bytes.fromhex('f9beb4d976657273696f6e0000000000650000005f1a69d2721101000100000000000000bc8f5e5400000000010000000000000000000000000000000000ffffc61b6409208d010000000000000000000000000000000000ffffcb0071c0208d128035cbc97953f80f2f5361746f7368693a302e392e332fcf05050001')
    //     stream = BytesIO(msg)
    //     envelope = NetworkEnvelope.parse(stream)
    //     self.assertEqual(envelope.serialize(), msg)

  return 0; 
}
