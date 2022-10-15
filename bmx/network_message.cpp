
#include "network.h"
#include "aframe/hash.h"
#include "utility.h"


using namespace af;
using namespace ffm; 
using namespace bmx; 
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------

// bmx::Network::Message::VerAck& Default (bmx::Network::Message::VerAck& msg) {

//   return msg; 
  
// }

// uint64 bmx::Network::Message::Read (bmx::Network::Message::VerAck &msg, af::ReadStreamRef rs, bool mainnet) {

//   FN_SCOPE (); 

//   assert (false); 
//   return 0;
// }


// uint64 bmx::Network::Message::Write (af::WriteStreamRef ws, const bmx::Network::Message::VerAck& msg, bool mainnet) {
//   return 0;
// }

// ---------------------------------------------------------------------
//
//
// ---------------------------------------------------------------------

uint64 bmx::Network::Message::SizeOf (const bmx::Network::Message::Version& vers) {

  const uint64 sizeOf_prefix_bytes = 12; // 00000000000000000000ffff

  uint64 sizeOf_MessageVersion = sizeof(uint32)                // version;
                               + sizeof(uint64)                // services;
                               + sizeof(uint64)                // timestamp;
                               + sizeof(uint64)                // receiver_services;
                               + sizeOf_prefix_bytes + sizeof(std::array<byte, 4>)   // receiver_IP;
                               + sizeof(uint16)                // receiver_port;
                               + sizeof(uint64)                // sender_services; 
                               + sizeOf_prefix_bytes + sizeof(std::array<byte, 4>)   // sender_IP;
                               + sizeof(uint16)                // sender_port; 
                               + sizeof(uint64)                // nonce;
                               + util::SizeOf_varint(vers.user_agent.size())
                               + vers.user_agent.size()        // user_agent
                               + sizeof(uint32)                //latest_block;
                               + sizeof(byte);                 //relay;

  return sizeOf_MessageVersion;     
}

//
bmx::Network::Message::Version& bmx::Network::Message::Default (bmx::Network::Message::Version& msg) {
  // FN_SCOPE (); 
  msg.version = 70015;
  msg.services = 0;

  
  std::time_t time_res = 0;
  std::time (&time_res); 
  msg.timestamp = time_res; 

  printf ("   timestamp generated:%zu\n", msg.timestamp); 
  
  const std::array<uint8, 4> default_ip   = { 0x00, 0x00, 0x00, 0x00 }; 
  const uint16 default_port = 8333 ; 
  
  msg.receiver_services = 0;
  copy_BE (msg.receiver_IP, to_bytes (default_ip)); 
  msg.receiver_port = default_port;
	     
  msg.sender_services = 0;
  copy_BE (msg.sender_IP,   to_bytes (default_ip )); 
  msg.sender_port = default_port;

  std::random_device rd;
  std::uniform_int_distribution<uint64> dist(5000111713);

  uint64 rand_nonce = 0; {
    std::random_device rd;
    auto rnum = msg.timestamp & 0xff;
    std::uniform_int_distribution<uint64> dist(rnum * 9560111713); 
    // auto  rnum = msg.timestamp & 0xff;

    // for (auto i = 0; i < rnum; ++i)
    rand_nonce = dist(rd);
  }
  msg.nonce = rand_nonce;
  printf ("    nonce generated:%zu\n", msg.nonce); 

  
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

  std::array<byte, 256> buf ; 
  
  readlen += rs->Read (&ver.version, sizeof(uint32)); 
  readlen += rs->Read (&ver.services, 8); 
  readlen += rs->Read (&ver.timestamp, 8);

  readlen += rs->Read (&ver.receiver_services, 8); 
  // 00000000000000000000ffff 
  readlen += rs->Read (&buf, 10 + 2);
  readlen += rs->Read (&ver.receiver_IP,  4);
  readlen += rs->Read (&ver.receiver_port, 2);
  swap_endian <uint16>(&ver.receiver_port);

  readlen += rs->Read (&ver.sender_services, 8); 
  // 00000000000000000000ffff 
  readlen += rs->Read (&buf, 10 + 2); 
  readlen += rs->Read (&ver.sender_IP,  4);
  readlen += rs->Read (&ver.sender_port, 2); 
  swap_endian <uint16>(&ver.sender_port); 

  
  readlen += rs->Read (&ver.nonce, sizeof (uint64)); 

  uint64 user_agent_len = 0;
  util::read_varint (user_agent_len, rs);

  ver.user_agent.resize (user_agent_len); 
  readlen += rs->Read (&ver.user_agent[0], user_agent_len);
  printf ( "    user_agent[%s] \n", af::to_string (ver.user_agent).c_str ()); 

  readlen += rs->Read (&ver.latest_block, sizeof(uint32));
  readlen += rs->Read (&ver.relay, sizeof sizeof(uint8)); 

  return readlen; 
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

//
Network::Message::GetHeaders& Network::Message::Default(GetHeaders& gh) {
  gh.version    = 70015; 
  gh.num_hashes = 1;
  const byte b00 {0}; 
  std::fill(&gh.start_block[0], &gh.start_block[32], b00); 
  std::fill(&gh.end_block[0], &gh.end_block[32], b00); 
  return gh; 
}

// getheaders
// Network::Message::GetHeaders& Network::Message::Init (GetHeaders& gh, uint32 ver, uint64 num_hashes, const digest32& start_block) {
//   return gh;
// }

// Network::Message::GetHeaders& Network::Message::Init (GetHeaders& gh, uint32 ver, uint64 num_hashes, const digest32& start_block, const digest32& end_block) {
//   return gh;
// }

//
//
uint64 bmx::Network::Message::Read (GetHeaders& msg, af::ReadStreamRef rs, bool mainnet)  {
  FN_SCOPE();

  uint64 readlen = 0;
  readlen += rs->Read (&msg.version, sizeof(uint32)); 
  readlen += util::read_varint (msg.num_hashes, rs, "num_hashes");

  digest32 hash_LE;
  readlen += rs->Read (&hash_LE, sizeof(digest32));
  std::reverse_copy (&hash_LE[0], &hash_LE[32], &msg.start_block[0]); 
  
  readlen += rs->Read (&hash_LE, sizeof(digest32)); 
  std::reverse_copy (&hash_LE[0], &hash_LE[32], &msg.end_block[0]); 
  
  return readlen;
}

uint64 bmx::Network::Message::Write (af::WriteStreamRef ws, const GetHeaders& msg, bool mainnet) {
  FN_SCOPE(); 
  uint64 writelen = 0;

  writelen += ws->Write (&msg.version, sizeof(uint32));
  printf ("    writelen[%zu]\n", writelen);

  printf ("    num_hashes[%zu]\n", msg.num_hashes);
  writelen += util::write_varint (ws, msg.num_hashes);
  printf ("    writelen[%zu]\n", writelen);

  digest32 rhash;
  std::reverse_copy (&msg.start_block[0] , &msg.start_block[32], &rhash[0]); 
  writelen += ws->Write (&rhash[0], sizeof(digest32));
  printf ("    writelen[%zu]\n", writelen);

  std::reverse_copy (&msg.end_block[0], &msg.end_block[32], &rhash[0]); 
  writelen += ws->Write (&rhash[0], sizeof(digest32));
  //printf ("    num_hashes\n", msg.num_hashes);

  return writelen;
}

uint64  bmx::Network::Message::SizeOf (const bmx::Network::Message::GetHeaders& hdrs) {

  return sizeof(uint32)      // version(4)
    + util::SizeOf_varint(hdrs.num_hashes) // 
    + sizeof(digest32)   // start_block;  // 32bytes
    + sizeof(digest32);  // end_block;    // 32bytes

}




#ifdef USE_BMX_MESSAGE_PING_IO_FNS


// ping
uint64 bmx::Network::Message::Read (Ping& msg, af::ReadStreamRef rs, bool mainnet) {
  return rs->Read (&msg.nonce, sizeof(uint64)); 
} 
uint64 bmx::Network::Message::Write (af::WriteStreamRef ws, const Ping& msg, bool mainnet)  {
  return ws->Write (&msg.nonce, sizeof(uint64));
  }

#endif


#ifdef USE_BMX_MESSAGE_PING_IO_FNS
  // pong
uint64 bmx::Network::Message::Read (Pong& msg, af::ReadStreamRef rs, bool mainnet) {
  return rs->Read (&msg.nonce, sizeof(uint64));
}

uint64 bmx::Network::Message::Write (af::WriteStreamRef ws, const Pong& msg, bool mainnet) {
  return ws->Write (&msg.nonce, sizeof(uint64));
}

#endif



//
// headers 
uint64 bmx::Network::Message::Read (blockarray& blocks, af::ReadStreamRef rs, bool mainnet) {
  uint64 readlen = 0; 

  uint64 num_blocks = 0; 
  readlen += util::read_varint (num_blocks, rs, "num_blocks"); 

  blocks.resize (num_blocks); 
  for (uint64 i = 0; i < num_blocks; ++i) {
    
    readlen += Block::Read (blocks[i], rs);

    uint64 num_txs = 0xffffffffffffffff; 
    readlen += util::read_varint (num_txs, rs, "num_txs"); 
    if (num_txs != 0) {
      printf ("ERROR : [%zu] num_txs != 0", i); 
      return readlen; 
    }
  }

  return readlen; 
}

//
uint64 bmx::Network::Message::Write (af::WriteStreamRef ws, const blockarray& blocks, bool mainnet) {
  uint64 writelen = 0; 

  writelen += util::write_varint (ws, blocks.size()); 

  for (const auto& bl : blocks) {
    //
    writelen += Block::Write (ws, bl);
    //
    writelen += util::write_varint  (ws, 0); 
    
  }
  
  
  
  return writelen; 
}



