
#include "network.h"
#include "aframe/hash.h"


using namespace af;
using namespace ffm; 

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------

bool bmx::Network::Milkshake (const bmx::Network::Message::Version& ver, bmx::netmessage_cb* cb) {

    // def handshake(self):
    //     '''Do a handshake with the other node.
    //     Handshake is sending a version message and getting a verack back.'''
    //     # create a version message
    //     # send the command
    //     # wait for a verack message
    //     version = VersionMessage()
    //     self.send(version)
    //     self.wait_for(VerAckMessage)
  return false;
}


#include "test/test_network.cpp"


