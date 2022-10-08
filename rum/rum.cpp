#include "rum.h"
#include "bmx/bmx.h"
#include "rum_msg_generated.h"

#include <chrono>
#include <thread>

#include <ctime>


using namespace af;

//
namespace rum {
  // 
  struct MessageBase {

    enum {
      ID_M101_Status = 0,
      ID_M102_ = 0,
      ID_M103 = 0,
      ID_M104 = 0,
    };

    //
    size_t id; 
    char   label[32]; 

  }; 

  // 
  //
  struct M101_Status : public MessageBase {
    enum { ID = ID_M101_Status };
    unsigned count;
    unsigned flags;
  };
}


#include "test/misc.cpp"
#include "test/node_talk.cpp"
//

// bmx/test/test_network
int test_handshake  (const std::vector<std::string>& args); 

// -----------------------------------------
int main (int argv, char** argc) {
  printf ("[ENTER:%s|%s]\n", __FUNCTION__, __FILE_NAME__);

  std::vector<std::string> args (argc, argc + argv);

  //int res =  test_hmac_sha256 (args); 
  //int res =  test_inspect_tx (args); 

  //return int_2_hexes  (args);
  //int watres = Wat (args);
  //int testres = test_zmq (args);

  //test_message_version_defaults (args); 
  // test_node_talk (args); 
  // test_node_connection (args); 
  //test_node_obj (args); 
  //test_handshake (args);


  printf ("[LEAVE:%s|%s]\n", __FUNCTION__, __FILE_NAME__); 
  return 0; 
}


