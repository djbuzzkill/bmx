
#include <zmq.h>
int test_node_talk (const std::vector<std::string> &args); 
int test_zmq      (const std::vector<std::string> &args); 
int test_message_version_defaults (const std::vector<std::string>& args); 

//
//
int net_test  (const std::vector<std::string> &args) {
  //test_zmq (args);
  test_node_talk (args) ;
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
    zmq_recv (sock, buffer, 30, 0);
    printf ("Received Hello\n");
    sleep (1);          //  Do some 'work'
    zmq_send (sock, "World", 5, 0);
  }
  
  return 0; 
}

//
//
int test_message_version_defaults (const std::vector<std::string>& args) {

  FN_SCOPE ();

  using namespace bmx; 
  using namespace std::chrono_literals;


  std::this_thread::sleep_for(101ms);
  Network::Message::Version msg_vers;

  for   (int i = 0; i < 20; ++i) {

  Network::Message::Default (msg_vers);
  std::this_thread::sleep_for(131ms);
  puts ("..."); 
  }  

  return 0; 

}


//
//
int test_node_talk (const std::vector<std::string> &args) {
  FN_SCOPE ();

  using namespace bmx; 

  const bool mainnet = false; 
  const std::string transport = "tcp://"; 
  const std::string sURL      = "testnet.programmingbitcoin.com"; 
  const std::string port_num  = mainnet ? ":8333" : ":18333";

  std::string address = transport + sURL + port_num;
  printf ( "   conntecting to.. %s\n", address.c_str());

  void* zmqcon = zmq_ctx_new ();
  void* sock   = zmq_socket (zmqcon, ZMQ_STREAM);
  //int rc       = zmq_bind (sock, address);

  int conn_res   = zmq_connect (sock, address.c_str());
  assert (conn_res == 0); 
  printf ("    conn res [%i] \n", conn_res); 

  {
    zmq_msg_t msg_recv_ID;
    zmq_msg_init(&msg_recv_ID);
    int msg_recv_size = zmq_msg_recv(&msg_recv_ID, sock, 0);
    printf(" --> recv0  : msg_recv_size [%i]\n", msg_recv_size);
    size_t msg_size = zmq_msg_size(&msg_recv_ID);
    printf(" --> recv0 size [%zu]\n", msg_size);
    std::string msg_ID_hex; 
    hex::encode (msg_ID_hex, zmq_msg_data (&msg_recv_ID), msg_recv_size);
    printf ("msg_ID_hex [%s] \n", msg_ID_hex.c_str()); 
    auto close_res = zmq_msg_close(&msg_recv_ID);

    zmq_msg_t msg_recv_;
    zmq_msg_init(&msg_recv_);
    int recv_b_len = zmq_msg_recv(&msg_recv_, sock, 0);
    printf(" --> recv__len  [%i]\n", recv_b_len);
  }

  std::array<byte, 256>  routingID; 
  size_t  routinglen    = sizeof(routingID); 
  int routing_ID = zmq_getsockopt (sock, ZMQ_ROUTING_ID, &routingID, &routinglen);

  std::string routingID_hex;
  hex::encode (routingID_hex, &routingID, routinglen); 
  printf ("    sockopt (%zu) [%s]  \n", routinglen, routingID_hex.c_str()); 


  
  Network::Message::Version  msg_vers;

  network_envelope           netwenv_w;
  Network::Envelope::Payload (netwenv_w, Network::Message::Default (msg_vers), mainnet);

  bytearray sendbuf (1024, byte(0));
  uint64    writelen_send = 0;

  WriteStreamRef ws_send = CreateWriteMemStream (&sendbuf[0], 1024); 
  // sendres = zmq_send (sock, &routingID, routinglen, sendflags);
  // printf ("   ..sendres [%i] \n", sendres);


  //#define PREFIX_ROUTING_ID
#ifdef PREFIX_ROUTING_ID  
  writelen_send += ws_send->Write (&routingID, routinglen); 
  printf ("    write routingID len [%zu] \n", writelen_send); 
#endif
  

#define USE_OUR_ENVELOPE
#ifdef USE_OUR_ENVELOPE

  writelen_send += Network::Envelope::Write (ws_send, netwenv_w); 
  printf ("    write env len [%zu] \n", writelen_send); 

#else

  const std::string py_hex =  "0b11090776657273696f6e00000000006e000000ba78ebd77f1101000000000000000000db27396300000000000000000000000000000000000000000000ffff00000000208d000000000000000000000000000000000000ffff00000000208da73152d961742b0e182f70726f6772616d6d696e67626974636f696e3a302e312f0000000000";
  bytearray pybin; 
  hex::decode (pybin, py_hex);
  writelen_send += ws_send->Write (&pybin[0], pybin.size());
 
#endif
  
  std::string env_enc_hex; 
  hex::encode (env_enc_hex, &sendbuf[0], writelen_send); 
  printf ("  --> [%zu] Envelope(hex) [%s]\n", writelen_send, env_enc_hex.c_str());


  //#define ENABLE_PRINT_ENVELOPE
#ifdef ENABLE_PRINT_ENVELOPE
  //
  const std::string py_hex =  "0b11090776657273696f6e00000000006e000000ba78ebd77f1101000000000000000000db27396300000000000000000000000000000000000000000000ffff00000000208d000000000000000000000000000000000000ffff00000000208da73152d961742b0e182f70726f6772616d6d696e67626974636f696e3a302e312f0000000000";
  bytearray pybin; 
  hex::decode (pybin, py_hex);

  network_envelope pyenve; 
  Network::Envelope::Read (pyenve, CreateReadMemStream (&pybin[0], pybin.size()), mainnet); 

  std::string pystr; 
  Network::Envelope::Format (pystr, pyenve); 
  printf (" py enve:\n%s\n", pystr.c_str());

  //
  const std::string my_hex =  "0b11090776657273696f6e00000000006e000000c4a774297f1101000000000000000000b028396300000000000000000000000000000000000000000000ffff00000000208d000000000000000000000000000000000000ffff00000000208dc7c73113c0d0739c182f70726f6772616d6d696e67626974636f696e3a302e312f0000000000";
  bytearray mybin ; 
  hex::decode (mybin, my_hex);

  network_envelope myenve; 
  Network::Envelope::Read (myenve, CreateReadMemStream (&mybin[0], mybin.size()), mainnet); 

  std::string mystr; 
  Network::Envelope::Format (mystr, netwenv_w ); 
  printf (" my enve:\n%s\n", mystr.c_str());

#endif  



#define ENABLE_SEND_ENVELOPE 
#ifdef ENABLE_SEND_ENVELOPE

  int sendflags = 0; // ZMQ_SNDMORE; 
  puts   ("   ..sending Version message \n");
  int sendres = -1;


  zmq_msg_t msg_routingID; 
  zmq_msg_init_size (&msg_routingID, routinglen); 
  memcpy(zmq_msg_data(&msg_routingID), &routingID, routinglen);
  auto msg_send_routingID_len = zmq_msg_send(&msg_routingID, sock, ZMQ_SNDMORE); 

  zmq_msg_t msg_enve; 
  zmq_msg_init_size (&msg_enve, writelen_send); 
  memcpy(zmq_msg_data(&msg_enve), &sendbuf[0], writelen_send);
  auto msg_send_len = zmq_msg_send(&msg_enve, sock, 0);

  int recvflags = 0; ZMQ_RCVMORE;
  bytearray recvbuf (2048, byte(0)); 
  puts   ( "   .. waiting for \'verack\'  message \n");

  zmq_msg_t msg_recv_a;
  zmq_msg_init(&msg_recv_a); 
  int  msg_recv_size = zmq_msg_recv(&msg_recv_a, sock, 0);
  printf (" --> received : msg_recv_size [%i]\n", msg_recv_size);
  size_t msg_size = zmq_msg_size (&msg_recv_a); 
  printf (" --> msg_size [%zu]\n", msg_size);
  auto   close_res  = zmq_msg_close(&msg_recv_a);


  zmq_msg_t msg_recv_b;
  zmq_msg_init(&msg_recv_b);
  int recv_b_len = zmq_msg_recv (&msg_recv_b, sock, 0);
  printf (" --> recv_b_len  [%i]\n", recv_b_len);
  
  size_t data_b_size = zmq_msg_size (&msg_recv_b);
  printf (" --> data_b_size  [%zu]\n", data_b_size);
  auto   close_res_b  = zmq_msg_close(&msg_recv_b);

  #endif 


  
  // recvlen += zmq_recv (sock, &readbuf[0], 2048, 0);
  // printf ( "   .. (1)received bytes[%i]  \n", recvlen);

  // recvlen += zmq_recv (sock, &readbuf[0], 2048, 0);
  // printf ( "   .. (2)received bytes[%i]  \n", recvlen);


  
  // std::vector<uint8> recvdbytes; 
  // for (int i = 0; i <  msg_recv_size; ++i ) { 
  //   recvdbytes.push_back (std::to_integer<uint8> (recvbytes[i]));
  //   printf ("  -->  el(%i) [%u]\n ", i, recvdbytes[i] ); 
  // }
  
  //printf ("    .. bytes to chars [%s]\n", recvdbytes.c_str());

  
  // network_envelope netwenv_r;
  // Network::Envelope::Read (netwenv_r, CreateReadMemStream (&readbuf[0], recvlen), mainnet);


  // netwenv_r.command;  
  
  // Network::Message::VerAck msg_verack; 
  // uint64 readlen_verack = Network::Message::Read (msg_verack, CreateReadMemStream (&netwenv_r.payload[0] , netwenv_r.payload.size ()), mainnet ); 

  // Defined in header <ctime>
  std::time_t arg = 0; 
  std::time (&arg);

  //         self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
  //         self.socket.connect((host, port))
  //         self.stream = self.socket.makefile('rb', None)

  //     def handshake(self):
  //         '''Do a handshake with the other node.
  //         Handshake is sending a version message and getting a verack
  //         back.''' # create a version message # send the command # wait for a
  //         verack message version = VersionMessage() self.send(version)
  //         self.wait_for(VerAckMessage)

  //   struct msghandler : public Network::MessageCB {
  //     virtual void Response (Network::Message::VerAck& msg, bool mainnet) {
  //     }

  //     virtual void Response (Network::Message::Pong& msg  , bool mainnet) {
  //     }

  //   };

  //   msghandler                handler;
  //   Network::Message::Version msg_verts;

  //   Network::Handshake (Network::Message::Default (msg_verts) , &handler);
  //




  
  return 0; 
}








// # tag::source4[]
// class SimpleNode:

//     def __init__(self, host, port=None, testnet=False, logging=False):
//         if port is None:
//             if testnet:
//                 port = 18333
//             else:
//                 port = 8333
//         self.testnet = testnet
//         self.logging = logging
//         self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
//         self.socket.connect((host, port))
//         self.stream = self.socket.makefile('rb', None)
//     # end::source4[]

//     def handshake(self):
//         '''Do a handshake with the other node.
//         Handshake is sending a version message and getting a verack back.'''
//         # create a version message
//         # send the command
//         # wait for a verack message
//         version = VersionMessage()
//         self.send(version)
//         self.wait_for(VerAckMessage)

        
//     def send(self, message):  # <1>
//         '''Send a message to the connected node'''
//         envelope = NetworkEnvelope(
//             message.command, message.serialize(), testnet=self.testnet)
//         if self.logging:
//             print('sending: {}'.format(envelope))
//         self.socket.sendall(envelope.serialize())

//     def read(self):  # <2>
//         '''Read a message from the socket'''
//         envelope = NetworkEnvelope.parse(self.stream, testnet=self.testnet)
//         if self.logging:
//             print('receiving: {}'.format(envelope))
//         return envelope

//     def wait_for(self, *message_classes):  # <3>
//         '''Wait for one of the messages in the list'''
//         command = None
//         command_to_class = {m.command: m for m in message_classes}
//         while command not in command_to_class.keys():
//             envelope = self.read()
//             command = envelope.command
//             if command == VersionMessage.command:
//                 self.send(VerAckMessage())
//             elif command == PingMessage.command:
//                 self.send(PongMessage(envelope.payload))
//         return command_to_class[command].parse(envelope.stream())
// # end::source4[]


// class SimpleNodeTest(TestCase):

//     def test_handshake(self):
//         node = SimpleNode('testnet.programmingbitcoin.com', testnet=True)
//         node.handshake()
