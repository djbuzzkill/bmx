
#include "network.h"
#include "aframe/hash.h"


using namespace af;
using namespace ffm; 
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------

bmx::Network::Message::VerAck& Default (bmx::Network::Message::VerAck& msg) {

  return msg; 
  
}

uint64 bmx::Network::Message::Read (bmx::Network::Message::VerAck &msg, af::ReadStreamRef rs, bool mainnet) {

  return 0;
}


uint64 bmx::Network::Message::Write (af::WriteStreamRef ws, const bmx::Network::Message::VerAck& msg, bool mainnet) {
  return 0;
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
bmx::Network::Message::Version& bmx::Network::Message::Default (bmx::Network::Message::Version& msg) {
  FN_SCOPE (); 
  msg.version = 70015;
  msg.services = 0;

  
  std::time_t time_res = std::time (nullptr); 
  msg.timestamp = time_res; 

  const std::array<uint8, 4> default_ip   = { 0x00, 0x00, 0x00, 0x00 }; 
  const uint16 default_port = 8333 ; 
  
  msg.receiver_services = 0;
  copy_BE (msg.receiver_IP, to_bytes (default_ip)); 
  msg.receiver_port = default_port;
	     
  msg.sender_services = 0;
  copy_BE (msg.sender_IP,   to_bytes (default_ip )); 
  msg.sender_port = default_port;
  msg.nonce = 0 ;


  
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


