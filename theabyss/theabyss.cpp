#include "theabyss.h"

#include "curv/script.h"

using namespace af;





    
//
//
int test_zmq (const std::vector<std::string> &args) {


  // the abyss is a server 
  // 
  void* zmqcon = zmq_ctx_new ();
  void* sock   = zmq_socket (zmqcon, ZMQ_REP );
  int rc       = zmq_bind (sock, "tcp://127.0.0.1:2022");


  // ZMQ_PUB
  // ZMQ_SUB
  // ZMQ_PUSH
  // ZMQ_PULL
  // ZMQ_CLIENT
  //  ZMQ_SERVER
  
    while (1) {
    char buffer [10];
    zmq_recv (sock, buffer, 10, 0);
    printf ("Received Hello\n");
    //    sleep (1);          //  Do some 'work'
    zmq_send (sock, "World", 5, 0);
  }


    curv::OpCode op = curv::OP_DUP; 
    unsigned char uc = op;

    curv::OpCode op2 = curv::OpCode(uc);
        
  return 0; 
}


// -----------------------------------------
int main (int argv, char** argc) {

  std::vector<std::string> args (argc, argc + argv);

  return test_zmq (args);

  }


