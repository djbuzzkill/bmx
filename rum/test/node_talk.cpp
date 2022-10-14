

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
int test_node_talk (const std::vector<std::string> &args) {
  FN_SCOPE ();

  using namespace bmx; 

  const bool mainnet = false; 
  const std::string transport = "tcp://"; 
  const std::string sURL      = "testnet.programmingbitcoin.com"; 
  const std::string port_num  = mainnet ? ":8333" : ":18333";

  std::string address = transport + sURL + port_num;
  printf ( "   connecting to.. %s\n", address.c_str());

  void* zmqcon = zmq_ctx_new ();
  void* sock   = zmq_socket (zmqcon, ZMQ_STREAM);
  //int rc       = zmq_bind (sock, address);

  int conn_res   = zmq_connect (sock, address.c_str());
  assert (conn_res == 0); 
  printf ("    conn res [%i] \n", conn_res); 

  {
    // this should be routing ID
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
    // this should be 0 length
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

// 76 65 72 61 63 6b


// 76 65 72 73 69 6f 6e
    
  // {
  //   std::string verack_str = "verack";
  //   bytearray verackbin;

  //   to_bytes(verackbin, verack_str); 
  //   std::string chars;

  //   hex::encode (chars, &verackbin[0], verackbin.size()); 
  //   printf("   verack[%s]\n", chars.c_str());
  // }

  

#define ENABLE_SEND_ENVELOPE 
#ifdef ENABLE_SEND_ENVELOPE

  int sendflags = 0; // ZMQ_SNDMORE; 
  puts   ("   ..sending Version message \n");
  int sendres = -1;

  // send routing ID separately
  zmq_msg_t msg_routingID; 
  zmq_msg_init_size (&msg_routingID, routinglen); 
  memcpy(zmq_msg_data(&msg_routingID), &routingID, routinglen);
  auto msg_send_routingID_len = zmq_msg_send(&msg_routingID, sock, ZMQ_SNDMORE); 

  // send the actual msg
  zmq_msg_t msg_enve; 
  zmq_msg_init_size (&msg_enve, writelen_send); 
  memcpy(zmq_msg_data(&msg_enve), &sendbuf[0], writelen_send);
  auto msg_send_len = zmq_msg_send(&msg_enve, sock, 0);

  int recvflags = 0; ZMQ_RCVMORE;
  bytearray recvbuf (2048, byte(0)); 
  puts   ( "   .. waiting for \'verack\'  message \n");

  {
  zmq_msg_t msg_recv_a;
  zmq_msg_init(&msg_recv_a); 
  int  msg_recv_size = zmq_msg_recv(&msg_recv_a, sock, 0);
  printf (" --> received : msg_recv_size [%i]\n", msg_recv_size);
  size_t msg_size = zmq_msg_size (&msg_recv_a); 
  printf (" --> msg_size [%zu]\n", msg_size);
  auto   close_res  = zmq_msg_close(&msg_recv_a);
  }

  // An application that processes multi-part messages must use the
  // ZMQ_RCVMORE zmq_getsockopt(3) option after calling zmq_msg_recv()
  // to determine if there are further parts to receive.
  
  {
  zmq_msg_t msg_recv_b;
  zmq_msg_init(&msg_recv_b);
  int recv_b_len = zmq_msg_recv (&msg_recv_b, sock, 0);
  printf (" --> recv_b_len  [%i]\n", recv_b_len);
  
  size_t data_b_size = zmq_msg_size (&msg_recv_b);
  printf (" --> data_b_size  [%zu]\n", data_b_size);

  std::string recv_hex; 
  hex::encode (recv_hex, zmq_msg_data (&msg_recv_b), recv_b_len);

  printf ("    recv_hex[%s]\n ", recv_hex.c_str()); 
  
  // network_envelope  netwenv_r; 
  // Network::Envelope::Read (netwenv_r, CreateReadMemStream (zmq_msg_data (&msg_recv_b), recv_b_len), mainnet);

  auto   close_res_b  = zmq_msg_close(&msg_recv_b);
  }

  
  {
  zmq_msg_t msg_recv_c;
  zmq_msg_init(&msg_recv_c); 
  int  msg_recv_size = zmq_msg_recv(&msg_recv_c, sock, 0);
  printf (" --> recv c : msg_recv_size [%i]\n", msg_recv_size);
  size_t msg_size = zmq_msg_size (&msg_recv_c); 
  printf (" --> c size [%zu]\n", msg_size);
  auto   close_res  = zmq_msg_close(&msg_recv_c);
  }
    
  {
  zmq_msg_t msg_recv_c;
  zmq_msg_init(&msg_recv_c); 
  int  msg_recv_size = zmq_msg_recv(&msg_recv_c, sock, 0);
  printf (" --> recv d : msg_recv_size [%i]\n", msg_recv_size);
  size_t msg_size = zmq_msg_size (&msg_recv_c); 
  printf (" --> d size [%zu]\n", msg_size);

  std::string recv_hex; 
  hex::encode (recv_hex, zmq_msg_data (&msg_recv_c), msg_recv_size);
  printf ("    recv_hex[%s]\n ", recv_hex.c_str()); 

  network_envelope ne; 
  size_t read_ne_len = Network::Envelope::Read (ne, CreateReadMemStream (zmq_msg_data (&msg_recv_c), msg_size), mainnet);

  auto   close_res  = zmq_msg_close(&msg_recv_c);
  }
  
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






//
//
int test_node_connection (const std::vector<std::string> &args) {
  FN_SCOPE ();

  using namespace bmx; 

  const bool mainnet = false; 
  const std::string transport = "tcp://"; 
  const std::string sURL      = "testnet.programmingbitcoin.com"; 
  const std::string port_num  = mainnet ? ":8333" : ":18333";

  int create_flags = 0; 
  std::string address = transport + sURL + port_num;
  af::conn_ref conn = af::Create_TCP_Connection (address, create_flags);


  Network::Message::Version  msg_vers;
  network_envelope           netwenv_w;
  Network::Envelope::Payload (netwenv_w, Network::Message::Default (msg_vers), mainnet);

  bytearray sendbuf (1024, byte(0));
  uint64    writelen_send = 0;

  WriteStreamRef ws_send = CreateWriteMemStream (&sendbuf[0], 1024); 
  // sendres = zmq_send (sock, &routingID, routinglen, sendflags);
  // printf ("   ..sendres [%i] \n", sendres);

  writelen_send += Network::Envelope::Write (ws_send, netwenv_w); 
  printf ("    write env len [%zu] \n", writelen_send); 

  puts   ("   ..sending Version message \n");
  int sendflags = 0;
  int sendlen = conn->Send (&sendbuf[0], writelen_send, sendflags);
  printf ("    315 sendlen [%i] \n",  sendlen); 
  // std::string env_enc_hex; 
  // hex::encode (env_enc_hex, &sendbuf[0], writelen_send); 
  // printf ("  --> [%zu] Envelope(hex) [%s]\n", writelen_send, env_enc_hex.c_str());

  int recvflags = 0; connection::RF_dont_wait; 
  int offs = 0; 
  bytearray recvbuf (1024, byte(0)); 
  int recvlen = conn->Recv (&recvbuf[offs], 1024 - offs, recvflags);
  printf ("    320 recv len [%i] \n", recvlen);
  offs += recvlen; 

  recvlen = conn->Recv (&recvbuf[offs], 1024 - offs, recvflags);
  printf ("    324 recv len_ [%i] \n", recvlen);
  offs += recvlen;

  recvlen = conn->Recv (&recvbuf[offs], 1024 - offs, recvflags);
  printf ("    328 recv len_ [%i] \n", recvlen);
  offs += recvlen;

  // recvlen = conn->Recv (&recvbuf[offs], 1024 - offs, recvflags);
  // printf ("    333 recv len_ [%i] \n", recvlen);
  // offs += recvlen;
  
  network_envelope ne_r;
  size_t readlen_r = Network::Envelope::Read (ne_r, CreateReadMemStream (&recvbuf[0], offs), mainnet);

  network_envelope ne_r_;
  size_t readlen_r_ = Network::Envelope::Read (ne_r_, CreateReadMemStream (&recvbuf[readlen_r], offs - readlen_r), mainnet);
  
  printf ("   offs[%i], readlen_r[%zu], readlen_r_[%zu]\n", offs, readlen_r, readlen_r_); 

  return 0; 
  
}






//
// 
struct Simple : public bmx::netmessage_cb, public af::destructor {

  
  Simple (const std::string& addr, bool mainnet, bool logging)
    : got_vers(false)
    , got_verack (false)
  {
	
        /* def __init__(self, host, port=None, testnet=False, logging=False): */
        /*     if port is None: */
        /*         if testnet: */
        /*             port = 18333 */
        /*         else: */
        /*             port = 8333 */
        /*     self.testnet = testnet */
        /*     self.logging = logging */
        /*     self.socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
         */
        /*     self.socket.connect((host, port)) */
        /*     self.stream = self.socket.makefile('rb', None) */
        /* # end::source4[] */

  }

  virtual ~Simple () {
  }

  virtual void OnVerAck (const bmx::Network::Envelope::Struc& ne, bool mainnet) {
    FN_SCOPE (); 
    got_verack = true; 
    printf ("    >> Received VerAck message\n"); 
  }

  
  virtual void OnVersion (const bmx::Network::Message::Version& msg, const bmx::Network::Envelope::Struc& ne, bool mainnet) {
    FN_SCOPE ();
    got_vers = true; 
    printf ("    >> Received Version message\n"); 

  }

  

  bool ReceivedVersion () { return got_vers; } 

  bool ReceivedVerack () { return got_verack; }   
protected:

  bool got_vers;
  bool got_verack;
  
  Simple () = default;

};


typedef std::shared_ptr<Simple> SimpleRef;


SimpleRef CreateSimpleNode (const std::string& addr, bool mainnet, bool logging) {

  return std::make_shared<Simple> (addr, mainnet, logging);

}


//
//
int test_node_obj (const std::vector<std::string> &args) {
  FN_SCOPE ();

  using namespace bmx; 

  const bool mainnet = false; 
  const std::string transport = "tcp://"; 
  const std::string sURL      = "testnet.programmingbitcoin.com"; 
  const std::string port_num  = mainnet ? ":8333" : ":18333";

  int create_flags = 0; 
  std::string address = transport + sURL + port_num;
  af::conn_ref conn = af::Create_TCP_Connection (address, create_flags);


  // wtf is this for 
  SimpleRef simple =  CreateSimpleNode (address, mainnet, false);
  
  Network::Envelope::Send;
  Network::Envelope::Recv;
    
  Network::Message::Version  msg_vers;
  network_envelope           ne_w;
  Network::Envelope::Payload (ne_w, Network::Message::Default (msg_vers), mainnet);
  int sendflags = 0;

  int sendlen = Network::Envelope::Send (conn, ne_w, sendflags);  

  
  int recvflags = 0; connection::RF_dont_wait;
  int recvd_messages = 0; 
  // while ( !simple->ReceivedVersion ()) { 
  while ( !simple->ReceivedVerack() ) { 
  
    int recvlen = Network::Envelope::Recv (simple.get(), conn, mainnet, recvflags); 
    recvd_messages++; 
  }
  
   printf ("   received messages [%i]\n", recvd_messages); 

  return 0; 
  
}


