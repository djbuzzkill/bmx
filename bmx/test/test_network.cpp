


int test_read_network_envelope (const std::vector<std::string>& args) {
  FN_SCOPE ();
  using namespace af; 
  using namespace bmx;

  const bool mainnet = true; 
  {
    const std::string msghex = "f9beb4d976657261636b000000000000000000005df6e0e2";
    const bytearray   verack = to_bytes (std::string ("verack") );

    bytearray  msgbin; 
    hex::decode (msgbin, msghex);

    network_envelope netenv;
    Network::Envelope::Read (netenv, CreateReadMemStream (&msgbin[0], msgbin.size()), mainnet);

    PR_CHECK("command == \'verack\'", eql (verack, netenv.command));  
    PR_CHECK("len (payload) == 0 ",  netenv.payload.empty()); 

  }  
  {
    const std::string msghex  = "f9beb4d976657273696f6e0000000000650000005f1a69d2721101000100000000000000bc8f5e5400000000010000000000000000000000000000000000ffffc61b6409208d010000000000000000000000000000000000ffffcb0071c0208d128035cbc97953f80f2f5361746f7368693a302e392e332fcf05050001";
    const bytearray   version = to_bytes (std::string ("version"));

    bytearray  msgbin; 
    hex::decode (msgbin, msghex);
    
    network_envelope netenv; 
    Network::Envelope::Read (netenv, CreateReadMemStream (&msgbin[0], msgbin.size()), mainnet);

    PR_CHECK("command == \'version\'", eql (version, netenv.command) ); 
    PR_CHECK("payload == msg[24:]", eql (netenv.payload, bytearray (msgbin.begin()+24, msgbin.end()) )); 

  }

  return 0; 
}



int test_write_network_envelope (const std::vector<std::string>& args) {
  FN_SCOPE ();

  using namespace af; 
  using namespace bmx;
  const bool        mainnet = true; 
  const std::string test_hex = "f9beb4d976657261636b000000000000000000005df6e0e2"; 

  bytearray testbin; 
  hex::decode (testbin, test_hex); 
  
  network_envelope netenv;
  auto readlen = Network::Envelope::Read (netenv, CreateReadMemStream (&testbin[0], testbin.size()), mainnet); 

  bytearray writebin (readlen, byte(0)); 
  Network::Envelope::Write (CreateWriteMemStream (&writebin[0], writebin.size ()), netenv);

  PR_CHECK ("testbin == writebin", eql(testbin, writebin)); 
  return 0; 
}


//
//
int test_version_message (const std::vector<std::string>& args) {
  FN_SCOPE (); 
//         self.assertEqual(v.serialize().hex(), '

  std::string testhex =  "7f11010000000000000000000000000000000000000000000000000000000000000000000000ffff00000000208d000000000000000000000000000000000000ffff00000000208d0000000000000000182f70726f6772616d6d696e67626974636f696e3a302e312f0000000000";

  bytearray testbin ;
  hex::decode (testbin , testhex);

  printf ("    testbin[%zu] \n ", testbin.size()); 
  
  bmx::message_version msgver;
  bytearray writebin (testbin.size(), byte{0});
  bmx::Network::Message::Default (msgver);
  msgver.timestamp = 0; // to match  
  msgver.nonce = 0;     // testbin

  bmx::Network::Message::Write (CreateWriteMemStream (&writebin[0], writebin.size ()), msgver); 
 
  PR_CHECK ( "message ver's match" , eql (testbin, writebin)); 

  return 0;
  
}



//
//
struct Simple : public bmx::netmessage_cb, public af::destructor {
  
  Simple (const std::string& addr, bool mainnet, bool logging)
    : got_vers(false)
    , got_verack (false) {
  }

  virtual ~Simple () {
  }

  // change name?? OnReceive ??
  virtual void Rcvd (const bmx::Network::Message::VerAck& msg, const bmx::Network::Envelope::Struc& ne, bool mainnet) {
    got_verack = true; 
    printf ("    --> Simple Received [VerAck] message\n"); 
  }

  virtual void Rcvd (const bmx::Network::Message::Version& msg, const bmx::Network::Envelope::Struc& ne, bool mainnet) {
    got_vers = true;
    printf ("    --> Simple Received [Version] message, mebe shud respd \n"); 

  }

  bool ReceivedVersion () { return got_vers; } 

  bool ReceivedVerack () { return got_verack; }   
protected:

  bool got_vers;
  bool got_verack;
  
  Simple () = default;

};


//
//
int test_handshake (const std::vector<std::string>& args) {

  using namespace bmx; 

  const bool mainnet = false; 
  const std::string transport = "tcp://"; 
  const std::string sURL      = "testnet.programmingbitcoin.com"; 
  const std::string port_num  = mainnet ? ":8333" : ":18333";

  int create_flags = 0; 
  std::string address = transport + sURL + port_num;
  af::conn_ref conn = af::Create_TCP_Connection (address, create_flags);
  
  std::shared_ptr<Simple> simple = std::make_shared<Simple> (address, mainnet, false);

  //
  // >> SEND
  Network::Message::Version  vers;
  network_envelope           ne_w;
  Network::Envelope::Payload (ne_w, Network::Message::Default (vers), mainnet);
  int sendflags = 0;
  int sendlen   = Network::Envelope::Send (conn, ne_w, sendflags);  

  // << RECV 
  int recvflags = 0; connection::RF_dont_wait;
  int recvd_messages = 0; 
  // while ( !simple->ReceivedVersion ()) { 
  while ( !simple->ReceivedVerack() ) { 
    int recvlen = Network::Envelope::Recv (simple.get(), conn, mainnet, recvflags); 
    recvd_messages++; 
  }
  
  printf ("   received messages [%i]\n", recvd_messages); 
  return 0;
  // def test_handshake(self):
  //     node = SimpleNode('testnet.programmingbitcoin.com', testnet=True)
  //     node.handshake()
}

int test_serialize  (const std::vector<std::string>& args) {
  FN_SCOPE (); 

    // def test_serialize(self):
    //     block_hex = '0000000000000000001237f46acddf58578a37e213d2a6edc4884a2fcad05ba3'
    //     gh = GetHeadersMessage(start_block=bytes.fromhex(block_hex))
    //     self.assertEqual(gh.serialize().hex(), '7f11010001a35bd0ca2f4a88c4eda6d213e2378a5758dfcd6af437120000000000000000000000000000000000000000000000000000000000000000000000000000000000')

  return 0;
}

