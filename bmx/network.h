

#ifndef BMX_NETWORK_H
#define BMX_NETWORK_H

#include "common.h"
#include "utility.h"

#include <aframe/binary_IO.h>



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
      
      struct VerAck {};
      
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

      struct Pong {};

      uint64 Read  (Pong& msg, af::ReadStreamRef rs, bool mainnet);
      uint64 Write (af::WriteStreamRef ws, const Pong& msg, bool mainnet); 
		    
      struct GetHeaders {};

      uint64 Read  (GetHeaders& msg, af::ReadStreamRef rs, bool mainnet);
      uint64 Write (af::WriteStreamRef ws, const GetHeaders& msg, bool mainnet); 
      
      struct Headers {}; 
      
      uint64 Read  (Headers& msg, af::ReadStreamRef rs, bool mainnet);
      uint64 Write (af::WriteStreamRef ws, const Headers& msg, bool mainnet); 
    }

    
    // ---------------------------------------------------------
    //
    // ---------------------------------------------------------
    namespace Envelope {

      // 
      struct Struc {

	bytearray command;
	bytearray payload;

      };

      uint64 Read  (Struc& env, af::ReadStreamRef rs, bool mainnet);


      uint64 Write (af::WriteStreamRef ws, const Struc& nv, bool mainnet); 
      
    }

    
    // ---------------------------------------------------------
    //
    // ---------------------------------------------------------
    namespace Node {

      struct Simple {
      };
      // ?? struct Full {};
    }

  }

   
  // ---------------------------------------------------------
  //
  // ---------------------------------------------------------
  typedef Network::Envelope::Struc  network_envelope;
  typedef Network::Node::Simple     simple_node; 

  typedef Network::Message::Version message_version; 
  
}

#endif 
