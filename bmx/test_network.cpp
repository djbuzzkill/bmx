

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
  

    // def test_serialize(self):
    //     msg = bytes.fromhex('f9beb4d976657261636b000000000000000000005df6e0e2')
    //     stream = BytesIO(msg)
    //     envelope = NetworkEnvelope.parse(stream)
    //     self.assertEqual(envelope.serialize(), msg)
    //     msg = bytes.fromhex('f9beb4d976657273696f6e0000000000650000005f1a69d2721101000100000000000000bc8f5e5400000000010000000000000000000000000000000000ffffc61b6409208d010000000000000000000000000000000000ffffcb0071c0208d128035cbc97953f80f2f5361746f7368693a302e392e332fcf05050001')
    //     stream = BytesIO(msg)
    //     envelope = NetworkEnvelope.parse(stream)
    //     self.assertEqual(envelope.serialize(), msg)

  return 0; 
}
