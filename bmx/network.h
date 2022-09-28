

#ifndef BMX_NETWORK_H
#define BMX_NETWORK_H

#include "common.h"
#include "utility.h"

#include <aframe/binary_IO.h>



namespace bmx {

  namespace Network {

    const uint32 kMAINNET_MAGIC = 0xf9beb4d9;
    const uint32 kTESTNET_MAGIC = 0x0b110907;

    const char kMAINNET_MAGIC_sz[] = "f9beb4d9";
    const char kTESTNET_MAGIC_sz[] = "0b110907";
    // ---------------------------------------------------------
    //
    // ---------------------------------------------------------
    namespace Message {
      
      struct VerAck {};
      
      struct Version {};
      
      struct Pong {};
      
      struct GetHeaders {};
      
      struct Headers {}; 
      
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

}

#endif 
