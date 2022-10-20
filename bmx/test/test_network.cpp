


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



int test_headers_message_read (const std::vector<std::string>& args) {
  FN_SCOPE();

  bool mainnet = true; 
  
  using namespace bmx;
  
  std::string hex_msg = "0200000020df3b053dc46f162a9b00c7f0d5124e2676d47bbe7c5d0793a500000000000000ef445fef2ed495c275892206ca533e7411907971013ab83e3b47bd0d692d14d4dc7c835b67d8001ac157e670000000002030eb2540c41025690160a1014c577061596e32e426b712c7ca00000000000000768b89f07044e6130ead292a3f51951adbd2202df447d98789339937fd006bd44880835b67d8001ade09204600"; 

  bytearray msgbin;
  hex::decode (msgbin, hex_msg); 
  ReadStreamRef rs = CreateReadMemStream (&msgbin[0], msgbin.size()); 
  blockarray blocks;

  Network::Message::Read (blocks, rs, mainnet);

  PR_CHECK("num blocks match ", blocks.size() == 2);

  return 0;
}


int test_getheaders_message_write (const std::vector<std::string>& args) {
  FN_SCOPE(); 
  // class GetHeadersMessageTest(TestCase):

  bool mainnet = true; 
  std::string test_start_block_hex = "0000000000000000001237f46acddf58578a37e213d2a6edc4884a2fcad05ba3";
  bytearray startbin;
  hex::decode (startbin, test_start_block_hex);
  
  Network::Message::GetHeaders gh;
  Network::Message::Default (gh); 
  copy_BE (gh.start_block, startbin); 
  
  bytearray  testbin (bmx::Network::Message::SizeOf (gh));

  uint64 writelen_test = Network::Message::Write (CreateWriteMemStream (&testbin[0], testbin.size()), gh, mainnet);
  std::string testhex;
  hex::encode (testhex, &testbin[0], testbin.size());
  //
  const std::string wanthex = "7f11010001a35bd0ca2f4a88c4eda6d213e2378a5758dfcd6af437120000000000000000000000000000000000000000000000000000000000000000000000000000000000";
  bytearray wantbin;
  hex::decode (wantbin, wanthex);

  printf ( "    want [%s]\n    test [%s]\n", wanthex.c_str(), testhex.c_str()); 
  PR_CHECK("write getheaders match",  eql(testbin, wantbin)); 


  
  return 0;
}

int test_ping_message_parse (const std::vector<std::string>& args) {
  //FN_SCOPE(); 
  return 0;
}

int test_pong_message_parse(const std::vector<std::string> &args) {
  //FN_SCOPE(); 
  return 0;
}

int test_merkleblock_message_parse (const std::vector<std::string>& args) {
  //FN_SCOPE(); 
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
  virtual void OnVerAck (const bmx::Network::Envelope::Struc& ne, bool mainnet) {
    got_verack = true;

    std::string ne_str; 
    bmx::Network::Envelope::Format (ne_str, ne); 
    printf ("    --> Simple Received message\n:%s", ne_str.c_str()); 
  }

  virtual void OnVersion (const bmx::Network::Message::Version& msg, const bmx::Network::Envelope::Struc& ne, bool mainnet) {
    got_vers = true;

    std::string ne_str; 
    bmx::Network::Envelope::Format (ne_str, ne);
    printf ("    --> Simple Received message\n:%s", ne_str.c_str()); 


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




int test_getdata_serialize  (const std::vector<std::string>& args) {
  FN_SCOPE (); 

  bool mainnet = true; 
  std::string block1_hex = "00000000000000cac712b726e4326e596170574c01a16001692510c44025eb30";
  bytearray block1; 

  std::string block2_hex = "00000000000000beb88910c46f6b442312361c6693a7fb52065b583979844910"; 
  bytearray block2;

  
  bmx::Network::Message::GetData gd; 
  //  Network::Message::Default (gd);
  
  Network::Message::AddData (gd, Network::Message::GetData::GD_FILTERED_BLOCK_DATA_TYPE, block1);
  Network::Message::AddData (gd, Network::Message::GetData::GD_FILTERED_BLOCK_DATA_TYPE, block2);

  bytearray   gdbin (Network::Message::SizeOf (gd), byte(0)); 
  // get_data.add_data(FILTERED_BLOCK_DATA_TYPE, block1)
  uint64 writelen_gd = Network::Message::Write (CreateWriteMemStream(&gdbin[0], gdbin.size()), gd, mainnet); 
	      
	      
  std::string want_hex = "020300000030eb2540c41025690160a1014c577061596e32e426b712c7ca00000000000000030000001049847939585b0652fba793661c361223446b6fc41089b8be00000000000000";
  bytearray wantbin; 
  hex::decode (wantbin, want_hex); 

  PR_CHECK("getdata serialize  matches", eql(wantbin, gdbin));

//     def test_serialize(self):
//         hex_msg = '020300000030eb2540c41025690160a1014c577061596e32e426b712c7ca00000000000000030000001049847939585b0652fba793661c361223446b6fc41089b8be00000000000000'
//         get_data = GetDataMessage()
//         block1 = bytes.fromhex('00000000000000cac712b726e4326e596170574c01a16001692510c44025eb30')
//         get_data.add_data(FILTERED_BLOCK_DATA_TYPE, block1)
//         block2 = bytes.fromhex('00000000000000beb88910c46f6b442312361c6693a7fb52065b583979844910')
//         get_data.add_data(FILTERED_BLOCK_DATA_TYPE, block2)
//         self.assertEqual(get_data.serialize().hex(), hex_msg)
// K

  
  return 0;
}

