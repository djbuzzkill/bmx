#include "rum.h"
#include "rum_msg_generated.h"


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
    char
    unsigned count;
    unsigned flags;
  };
}

// 
int test_curl (const std::vector<std::string>& args) {

  const std::string tx_id = "3db68a2171756cfb0c7af980ac8780b4b5c892412f50cd8c4808182c7408aeb8"; 

  af::bytearray   txbytes;
  curv::TxFetcher fetcher;

  if (fetcher.Fetch (txbytes, tx_id, false)) {

    std::string printstr  (&txbytes[0], &txbytes[txbytes.size()]);
   printf ("\n\n\n\n\n\n"); 
   printf ("contents: %s\n",printstr.c_str()); 

  }
    
  
  

  return 0;
}

//
//
int test_zmq (const std::vector<std::string> &args) {

  void* zmqcon = zmq_ctx_new ();
  void* sock   = zmq_socket (zmqcon, ZMQ_PUB);
  int rc       = zmq_bind (sock, "tcp://127.0.0.1:2022");
// ZMQ_PUB
// ZMQ_SUB
// ZMQ_PUSH
// ZMQ_PULL
  while (1) {
    char buffer [10];
    zmq_recv (sock, buffer, 10, 0);
    printf ("Received Hello\n");
    sleep (1);          //  Do some 'work'
    zmq_send (sock, "World", 5, 0);
  }
  
  return 0; 
}


// -----------------------------------------
int main (int argv, char** argc) {
  std::vector<std::string> args (argc, argc + argv);
  //int watres = Wat (args);
  int testres = test_zmq (args); 
  return testres;
}


//
//
int flatbuffers_test (const std::vector<std::string>& args) {

  flatbuffers::FlatBufferBuilder build; 

  auto         burgername = build.CreateString ("oscar");

  rum::M101_Status stat;
  
  
  unsigned int burgerid   = 69; 
  std::string toppings_str[] = {"bacon", "cheez", "onion", "mayonaise"};
  
  std::vector<flatbuffers::Offset<flatbuffers::String>> toppingsvec;
  toppingsvec.push_back (build.CreateString ("bacon"));
  toppingsvec.push_back (build.CreateString ("cheez"));
  toppingsvec.push_back (build.CreateString ("onion"));
  toppingsvec.push_back (build.CreateString ("mayonaise")); 
  auto toppings = build.CreateVector (toppingsvec); 
  
  auto onionburger = rum::msg::CreateCheezburger (build, burgername, burgerid, toppings); 
  build.Finish (onionburger); 
  
  build.GetSize ();
  build.GetBufferPointer ();
  
  af::bytearray burgermem (build.GetSize()); {
    af::WriteStreamRef ws =
      af::CreateWriteMemStream(&burgermem[0], build.GetSize());

    ws->Write(build.GetBufferPointer(), burgermem.size());
  }

  auto cheezburger = rum::msg::GetCheezburger (&burgermem[0]);

  printf ("cheezburger: %s\n", cheezburger->name()->c_str());

  for (size_t i = 0; i < cheezburger->toppings()->size(); ++i) {
    printf ("toppings:\n");
    printf (" %s\n",  cheezburger->toppings()->GetAsString (i)->c_str());
  }
  
  return 0; 
}

