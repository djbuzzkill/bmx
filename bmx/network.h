

#ifndef BMX_NETWORK_H
#define BMX_NETWORK_H

#include "common.h"
#include "utility.h"

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
      // VerAck Message 
      struct VerAck {
	VerAck () : dumb (0) {
	}
        uint8  dumb; 
      };
      //
      /* VerAck& Default (VerAck& msg);  */
      /* uint64   Read    (VerAck& msg, af::ReadStreamRef rs, bool mainnet); */
      /* uint64   Write   (af::WriteStreamRef ws, const VerAck& msg, bool mainnet); */

      // Version Message 
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
      
      // Ping Message 
      struct Ping { int dumb; };
      uint64 Read  (Ping& msg, af::ReadStreamRef rs, bool mainnet);
      uint64 Write (af::WriteStreamRef ws, const Ping& msg, bool mainnet);
      //
      //

      // Pong Message
      struct Pong { int dumb; };
      uint64 Read  (Pong& msg, af::ReadStreamRef rs, bool mainnet);
      uint64 Write (af::WriteStreamRef ws, const Pong& msg, bool mainnet);
      //

      // Get Headers Message 
      struct GetHeaders { int dumb; };
      uint64 Read  (GetHeaders& msg, af::ReadStreamRef rs, bool mainnet);
      uint64 Write (af::WriteStreamRef ws, const GetHeaders& msg, bool mainnet);
      //

      // Headers Message 
      struct Headers { int dumb;  }; 
      uint64 Read  (Headers& msg, af::ReadStreamRef rs, bool mainnet);
      uint64 Write (af::WriteStreamRef ws, const Headers& msg, bool mainnet); 
      // 
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
	bytearray payload;
	uint32    magic; 
      };

      Struc& Payload (Struc& oenve, const Message::Version& vers, bool mainnet);
      Struc& Payload (Struc& oenve, const Message::VerAck& msg, bool mainnet);
      Struc& Payload (Struc& oenve, const Message::GetHeaders& msg, bool mainnet);
      Struc& Payload (Struc& oenve, const Message::Headers& msg, bool mainnet);
      Struc& Payload (Struc& oenve, const Message::Ping& msg, bool mainnet);
      Struc& Payload (Struc& oenve, const Message::Pong& msg, bool mainnet);
      
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
      virtual void Rcvd (const Message::VerAck&  msg, const Envelope::Struc& ne, bool mainnet) {} 
      virtual void Rcvd (const Message::Version& msg, const Envelope::Struc& ne, bool mainnet) {} 
      virtual void Rcvd (const Message::Pong&    msg, const Envelope::Struc& ne, bool mainnet) {} 

    protected: 
      MessageReceiver () = default;
    };
    
    // ---------------------------------------------------------
    // not quite anything here yet
    // ---------------------------------------------------------
    namespace Node { 
      // 
      struct Base {
	virtual int Send () = 0; 
	virtual int Recv () = 0; 
	virtual int WaitFor () = 0;
	
      protected:
	Base () = default; 
      };
    }

    // ---------------------------------------------------------
    //
    // ---------------------------------------------------------
    bool Milkshake (const Message::Version& ver, MessageReceiver* cb);


  } // Network 

  // ---------------------------------------------------------
  //
  // ---------------------------------------------------------
  typedef Network::Envelope::Struc  network_envelope;
  //  typedef Network::Node::Simple     simple_node; 
  typedef Network::Message::Version message_version; 

  typedef Network::MessageReceiver  netmessage_cb;
  
  }

#endif 
