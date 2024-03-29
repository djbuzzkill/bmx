

#ifndef BMX_NETWORK_H
#define BMX_NETWORK_H

#include "common.h"
#include "utility.h"
#include "block.h"

#include <aframe/binary_IO.h>
#include <aframe/shared_types.h>



namespace bmx {

  namespace Network {

    const uint32 kMAINNET_MAGIC = 0xf9beb4d9;
    const uint32 kTESTNET_MAGIC = 0x0b110907;

    /* const char kMAINNET_MAGIC_sz[] = "f9beb4d9"; */
    /* const char kTESTNET_MAGIC_sz[] = "0b110907"; */

    // ---------------------------------------------------------
    //
    // ---------------------------------------------------------
    namespace Message {

      // 
      // Message [VerAck]  .. not necessary 
      // struct VerAck { };
      /* VerAck& Default (VerAck& msg);  */
      /* uint64   Read    (VerAck& msg, af::ReadStreamRef rs, bool mainnet); */
      /* uint64   Write   (af::WriteStreamRef ws, const VerAck& msg, bool mainnet); */

      //
      // Message [Version] 
      struct Version {
	uint32 version;
	uint64 services;
	uint64 timestamp;

        uint64 receiver_services;
        std::array<byte, 4> receiver_IP;
        uint16 receiver_port;

        uint64 sender_services; 
	std::array<byte, 4> sender_IP;
	uint16 sender_port; 
	
	uint64 nonce;
	bytearray user_agent; 
	uint32  latest_block;
	byte   relay; 
      };
      
      Version& Default (Version& msg); 
      uint64   Read    (Version& msg, af::ReadStreamRef rs, bool mainnet);
      uint64   Write   (af::WriteStreamRef ws, const Version& msg);
      uint64   SizeOf  (const bmx::Network::Message::Version& vers);

      //
      // MESSAGE [Ping]  ..not necessary 
      // struct Ping {
      // 	uint64 nonce;
      // };
      // uint64 Read  (Ping& msg, af::ReadStreamRef rs, bool mainnet);
      // uint64 Write (af::WriteStreamRef ws, const Ping& msg, bool mainnet);

      //
      // MESSAGE [Pong] ..not necessary 
      // struct Pong {
      // 	uint64 nonce;
      // };
      // uint64 Read  (Pong& msg, af::ReadStreamRef rs, bool mainnet);
      // uint64 Write (af::WriteStreamRef ws, const Pong& msg, bool mainnet);

      //
      // Message [GetHeaders]  
      struct GetHeaders {

	uint32   version;     // 4
	uint64   num_hashes;  // varint
	digest32 start_block; // 32bytes
	digest32 end_block;   // 32bytes

      };

      GetHeaders& Default (GetHeaders& gh);
      
      uint64 Read   (GetHeaders& msg, af::ReadStreamRef rs, bool mainnet);
      uint64 Write  (af::WriteStreamRef ws, const GetHeaders& msg, bool mainnet);
      uint64 SizeOf (const bmx::Network::Message::GetHeaders& hdrs);
      // GetHeaders& Init  (GetHeaders& gh, uint32 ver, uint64 num_hashes, const digest32& start_block);
      // GetHeaders& Init  (GetHeaders& gh, uint32 ver, uint64 num_hashes, const digest32& start_block, const digest32& end_block);


      //
      // Message [Headers]  
      typedef blockarray Headers;  // deceit
      // 	blockarray blocks;
      // }; 

      uint64 Read  (Headers& blocks, af::ReadStreamRef rs, bool mainnet);
      uint64 Write (af::WriteStreamRef ws, const Headers& blocks, bool mainnet);

      // 
      // MerkleBlock Message 
      struct MerkleBlock { int dumb; }; 
      uint64 Read  (MerkleBlock& msg, af::ReadStreamRef rs, bool mainnet);
      uint64 Write (af::WriteStreamRef ws, const MerkleBlock& msg, bool mainnet); 
      //



      // 
      // Message [GetData]
      struct GetData {

	enum GETDATA_TYPE {
	  GD_kTX_DATA_TYPE = 1,
	  GD_BLOCK_DATA_TYPE = 2,
	  GD_FILTERED_BLOCK_DATA_TYPE = 3,
	  GD_COMPACT_BLOCK_DATA_TYPE = 4}; 

	struct getdata_tuple {
          GETDATA_TYPE type;
          bytearray identifier; };

	typedef std::vector<getdata_tuple>  DataArray;

        DataArray data; 
      }; 
      // 
      uint64   Read    (GetData& gd, af::ReadStreamRef rs, bool mainnet);
      uint64   Write   (af::WriteStreamRef ws, const GetData& gd, bool mainnet); 
      GetData& AddData (GetData& gd, GetData::GETDATA_TYPE dt, const bytearray& identifier);
      uint64   SizeOf  (Network::Message::GetData& gd); 

    }

    // ---------------------------------------------------------
    //
    // ---------------------------------------------------------
    struct MessageReceiver; 
    
    // ---------------------------------------------------------
    //
    // ---------------------------------------------------------
    namespace Envelope {
      // 
      struct Struc {

	bytearray command;
	uint32    magic; 
	bytearray payload;
      };

      Struc& PayloadVerAck (Struc& ne, bool mainnet);
      Struc& PayloadPing   (Struc& ne, uint64 nonce, bool mainnet);
      Struc& PayloadPong   (Struc& ne, uint64 nonce, bool mainnet);
      Struc& PayloadHeaders(Struc& ne, const blockarray& blocks, bool mainnet);
      Struc& PayloadGeneric(Struc& ne, const std::string& cmd, const bytearray& payload, bool mainnet);

      Struc& Payload       (Struc& ne, const Message::Version&     vers, bool mainnet);
      Struc& Payload       (Struc& ne, const Message::GetHeaders&  msg, bool mainnet);

      Struc& Payload (Struc& oenve, const Message::MerkleBlock& msg, bool mainnet);
      Struc& Payload (Struc& oenve, const Message::MerkleBlock& msg, bool mainnet);
      
      uint64 Read    (Struc& env, af::ReadStreamRef rs, bool mainnet);
      uint64 Write   (af::WriteStreamRef ws, const Struc& nv);

      int    Send    (af::conn_ref conn, const Struc& ne, int flags) ;
      int    Recv    (MessageReceiver* const cb, af::conn_ref conn, bool mainnet, int flags);
      
      std::string& Format   (std::string&  str, const Struc& enve);
     
    }
    
    // ---------------------------------------------------------
    // 4 if u want 2 rcv messages
    // ---------------------------------------------------------
    struct MessageReceiver { 
      // change name?? OnReceive ??
      virtual void OnVerAck (const Envelope::Struc& ne, bool mainnet) {} 
      virtual void OnVersion (const Message::Version& msg, const Envelope::Struc& ne, bool mainnet) {} 
      virtual void OnPing (uint64 nonce, const Envelope::Struc& ne, bool mainnet) {} 
      virtual void OnPong (uint64 nonce, const Envelope::Struc& ne, bool mainnet) {}
      virtual void OnMerkleBlock (const Message::MerkleBlock& msg, const Envelope::Struc& ne, bool mainnet) {}
      virtual void OnGetHeaders (const Message::GetHeaders& msg, const Envelope::Struc& ne, bool mainnet) {} 
      virtual void OnHeaders (const blockarray& blocks, const Envelope::Struc& ne, bool mainnet) {} 
      virtual void OnGetData (const Message::GetData& gd, const Envelope::Struc& ne, bool mainnet) {}

      virtual void OnFilterLoad (const Message::GetData& gd, const Envelope::Struc& ne, bool mainnet) {} 
      virtual void OnFilterAdd  (const Message::GetData& gd, const Envelope::Struc& ne, bool mainnet) {} 
      
    protected: 
      MessageReceiver () = default;
    };


    void DispatchMessage (MessageReceiver* cb, const std::string& cmd_s, const Network::Envelope::Struc& ne); 
    
    // ---------------------------------------------------------
    // 
    // ---------------------------------------------------------
    namespace Node {
      // a node is like water
    }

    // ---------------------------------------------------------
    //
    // ---------------------------------------------------------
    bool Milkshake (const Message::Version& ver, MessageReceiver* cb);

  } // Network 

  // ---------------------------------------------------------
  // 
  // ---------------------------------------------------------
  typedef Network::Envelope::Struc      network_envelope;
  typedef Network::MessageReceiver      netmessage_cb;

  typedef Network::Message::Version     message_version; 
  typedef Network::Message::MerkleBlock message_merkleblock; 
  // typedef Network::Message::Ping        message_ping; 
  // typedef Network::Message::Pong        message_pong; 
  typedef Network::Message::GetHeaders  message_getheaders; 
  //typedef Network::Message::Headers     message_headers; 
  typedef Network::Message::Version     message_version;
  
}

#endif 
