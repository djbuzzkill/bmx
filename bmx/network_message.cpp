
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
  digest32 thegest; 
  writelen += ws->Write (&af::hash256(thegest, &nv.payload[0], nv.payload.size ())[0], 4);
  
  // payload 
  writelen += ws->Write (&nv.payload[0], nv.payload.size ()); 
  
  return writelen; 
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bmx::Network::Message::Version& bmx::Network::Message::Default (bmx::Network::Message::Version& msg) {
  FN_SCOPE (); 
  msg.version = 70015;
  msg.services = 0;
  msg.timestamp = 0; // unixtime;

  const std::array<uint8, 4> default_ip   = { 0x00, 0x00, 0x00, 0x00 }; 
  const uint16 default_port = 8333 ; 
  
  msg.receiver_services = 0;
  copy_BE (msg.receiver_IP, to_bytes (default_ip)); 
  msg.receiver_port = default_port;
	     
  msg.sender_services = 0;
  copy_BE (msg.sender_IP,   to_bytes (default_ip )); 
  msg.sender_port = default_port;
  msg.nonce = 0 ; //  self.nonce = int_to_little_endian(randint(0, 2**64), 8)
  const std::string useragent = "/programmingbitcoin:0.1/";
  to_bytes (msg.user_agent, useragent );
  
  msg.latest_block = 0;
  msg.relay = byte { 0x0 }; 

  return msg;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
uint64 bmx::Network::Message::Read (bmx::Network::Message::Version& ver, af::ReadStreamRef rs, bool mainnet) {

  uint64 readlen = 0; 

  // if !timestamp:
  // timestamp int (time.time())

  return 0; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
uint64 bmx::Network::Message::Write (af::WriteStreamRef ws, const bmx::Network::Message::Version& msg) {
  
  uint64 writelen = 0;

  const std::string ip_prefix = "00000000000000000000ffff";
  bytearray prefixbin; 
  hex::decode (prefixbin, ip_prefix);
  
  const byte bytex00 {0x00};  
  const byte bytex01 {0x01};

  uint16 receiver_port = msg.receiver_port;
  uint16 sender_port   = msg.sender_port; 
    
  writelen += ws->Write (&msg.version, sizeof(uint32)); 
  writelen += ws->Write (&msg.services, sizeof(uint64)); 
  writelen += ws->Write (&msg.timestamp, sizeof(uint64));  // 20

  writelen += ws->Write (&msg.receiver_services, sizeof(uint64)); //8 
  writelen += ws->Write (&prefixbin[0], prefixbin.size());        //12   
  writelen += ws->Write (&msg.receiver_IP, 4);                    //4
  writelen += ws->Write (swap_endian<uint16>(&receiver_port), 2);                  //2

  writelen += ws->Write (&msg.sender_services, sizeof(uint64));   //8
  writelen += ws->Write (&prefixbin[0], prefixbin.size());        //12
  writelen += ws->Write (&msg.sender_IP, 4);                      //4
  writelen += ws->Write (swap_endian<uint16>(&sender_port), 2);                    //2

  writelen += ws->Write (&msg.nonce, sizeof(uint64));             //8

  writelen += util::write_varint (ws, msg.user_agent.size ()); 
  writelen += ws->Write (&msg.user_agent[0], msg.user_agent.size ());

  writelen += ws->Write (&msg.latest_block, sizeof(uint32));
  writelen += ws->Write (&msg.relay, 1);
  
  return writelen; 
}



// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
uint64 bmx::Network::Message::Read  (Pong& msg, af::ReadStreamRef rs, bool mainnet) { return 0; } 
uint64 bmx::Network::Message::Write (af::WriteStreamRef ws, const Pong& msg, bool mainnet)  { return 0; } 

uint64 bmx::Network::Message::Read  (GetHeaders& msg, af::ReadStreamRef rs, bool mainnet)  { return 0; } 
uint64 bmx::Network::Message::Write (af::WriteStreamRef ws, const GetHeaders& msg, bool mainnet) { return 0; } 

uint64 bmx::Network::Message::Read  (Headers& msg, af::ReadStreamRef rs, bool mainnet)  { return 0; } 
uint64 bmx::Network::Message::Write (af::WriteStreamRef ws, const Headers& msg, bool mainnet)  { return 0; }


