

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
int test_handshake  (const std::vector<std::string>& args) {

// class SimpleNodeTest(TestCase):

//     def test_handshake(self):
//         node = SimpleNode('testnet.programmingbitcoin.com', testnet=True)
//         node.handshake()

  return 0; 

}
