
int test_node_talk (const std::vector<std::string> &args); 
int test_zmq      (const std::vector<std::string> &args); 

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
int test_node_talk  (const std::vector<std::string> &args) {
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
  int conn_res = zmq_connect (sock, address.c_str());

  network_envelope netwenv_w;
  Network::Envelope::Default (netwenv_w, mainnet);

  Network::Message::Version msg_vers;
  bytearray sendbuf (1024, byte(0)); 
  uint64 vers_writelen = Network::Message::Write (CreateWriteMemStream (&sendbuf[0], sendbuf.size()), Network::Message::Default (msg_vers));


  netwenv_w.payload.assign (&sendbuf[0], &sendbuf[vers_writelen]);
  
  
  int sendflags = 0; 
  puts ( "   ..sending Version message \n");
  zmq_send (sock, &sendbuf[0], vers_writelen , sendflags);

  puts ( "   .. waiting for VerAck  message \n");

  int recvflags = 0; 
  bytearray readbuf (1024, byte(0)); 
  int recvlen = zmq_recv (sock, &readbuf[0], readbuf.size(), recvflags);
  printf ( "   .. received bytes[%i]  \n", recvlen);

  network_envelope netwenv_r;
  Network::Envelope::Read (netwenv_r, CreateReadMemStream (&readbuf[0], recvlen), mainnet);


  netwenv_r.command;  
  
  Network::Message::VerAck msg_verack; 
  uint64 readlen_verack = Network::Message::Read (msg_verack, CreateReadMemStream (&netwenv_r.payload[0] , netwenv_r.payload.size ()), mainnet);

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
