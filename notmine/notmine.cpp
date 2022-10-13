#include "notmine.h"

#include "bmx/script.h"


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


    bmx::OpCode op = bmx::OP_DUP; 
    unsigned char uc = op;

    bmx::OpCode op2 = bmx::OpCode(uc);
        
  return 0; 
}

int test_murmur(const std::vector<std::string> &args) {


  std::string intxt = "hello world" ;
  std::string in2 = "goodbye" ;
  
  uint32 res = af::murmur3_x86_32 (intxt.c_str(), intxt.size(), 0);
  uint32 r2  = af::murmur3_x86_32 (in2.c_str(), in2.size(), 0);

  printf ( "   murmur3.\'%s\' --> %s\n", intxt.c_str(), fmti(res).c_str()); 
  printf ( "   murmur3.\'%s\' --> %s\n", in2.c_str(), fmti(r2).c_str()); 


  return 0; 
}


// bmx/test 
int test_merkle (const std::vector<std::string> &args); 

// -----------------------------------------
int main (int argv, char** argc) {
  FN_SCOPE ();


  std::vector<std::string> args (argc, argc + argv);

  // test_std_math (args); 
  test_merkle (args);
  
  test_murmur(args) ; 
  
  return 0 ;
    
  }


