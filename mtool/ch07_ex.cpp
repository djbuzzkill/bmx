






int tx_test_p1  (std::vector<std::string>& args) {

  //FN_SCOPE ("");

  //printf ("(fn:%s | ln:%i) ENTER \n" , __FUNCTION__, __LINE__); 
  
  bool on_mainnet = false; 

  const std::string txhex =
    "0100000001813f79011acb80925dfe69b3def355fe914bd1d96a3f5f71bf8303c6a989c7d1000000006b483045022100ed81ff192e75a3fd2304004dcadb746fa5e24c5031ccfcf21320b0277457c98f02207a986d955c6e0cb35d446a89d3f56100f4d7f67801c31967743a9c8e10615bed01210349fc4e631e3624a545de3f89f5d8684c7b8138bd94bdd531d2e213bf016b278afeffffff02a135ef01000000001976a914bc3b654dca7e56b04dca18f2566cdaf02e8d9ada88ac99c39800000000001976a9141c4bc762dd5423e332166702cb75f40df79fea1288ac19430600"; 

  bytearray txbin; 
  hex::decode (txbin, txhex); 
  bmx::Transaction tx; 
  ReadTransaction (tx, CreateReadMemStream (&txbin[0],  txbin.size ()));

  {

    //     def test_parse_inputs(self):

    
    std::string want_txid_hex = "d1c789a9c60383bf715f3f6ad9d14b91fe55f3deb369fe5d9280cb1a01793f81"; 
    bytearray  want_txid_bin;
    hex::decode (want_txid_bin, want_txid_hex); 

    std::string prev_txid_hex; 

    hex::encode (prev_txid_hex, &tx.inputs[0].prev_txid[0], tx.inputs[0].prev_txid.size());
    
    if (prev_txid_hex != want_txid_hex) {  
      printf ("\nwant_txid_hex:%s\n", want_txid_hex.c_str()); 
      printf ("\nprev_txid_hex:%s\n", prev_txid_hex.c_str());
    }
    assert (prev_txid_hex == want_txid_hex); 

    const std::string want_sig_hex = "6b483045022100ed81ff192e75a3fd2304004dcadb746fa5e24c5031ccfcf21320b0277457c98f02207a986d955c6e0cb35d446a89d3f56100f4d7f67801c31967743a9c8e10615bed01210349fc4e631e3624a545de3f89f5d8684c7b8138bd94bdd531d2e213bf016b278a"; 
    bytearray script_sig_bin (want_sig_hex.size (), byte{0}); 

    size_t script_sig_len = WriteScript (CreateWriteMemStream (&script_sig_bin[0], script_sig_bin.size()), tx.inputs[0].script_sig); 
    std::string script_sig_hex ; 
    hex::encode (script_sig_hex, &script_sig_bin[0], script_sig_len); 

    if (script_sig_hex != want_sig_hex) { 
      printf ("script_sig_hex :%s\n", script_sig_hex .c_str()); 
      printf ("want_sig_hex:%s\n"   , want_sig_hex .c_str());
    }
    assert (script_sig_hex == want_sig_hex); 
    // self.assertEqual(tx.tx_ins[0].script_sig.serialize(), want)
    assert (tx.inputs[0].sequence ==  0xfffffffe);
  // self.assertEqual(tx.tx_ins[0].sequence, 0xfffffffe)

  }
  


  {
//     def test_parse_outputs(self):
//         raw_tx = bytes.fromhex('0100000001813f79011acb80925dfe69b3def355fe914bd1d96a3f5f71bf8303c6a989c7d1000000006b483045022100ed81ff192e75a3fd2304004dcadb746fa5e24c5031ccfcf21320b0277457c98f02207a986d955c6e0cb35d446a89d3f56100f4d7f67801c31967743a9c8e10615bed01210349fc4e631e3624a545de3f89f5d8684c7b8138bd94bdd531d2e213bf016b278afeffffff02a135ef01000000001976a914bc3b654dca7e56b04dca18f2566cdaf02e8d9ada88ac99c39800000000001976a9141c4bc762dd5423e332166702cb75f40df79fea1288ac19430600')
//         stream = BytesIO(raw_tx)
//         tx = Tx.parse(stream)
    assert (tx.outputs.size () == 2); 
//         self.assertEqual(len(tx.tx_outs), 2)
//         want = 32454049
    assert (tx.outputs[0].amount == 32454049); 

//         self.assertEqual(tx.tx_outs[0].amount, want)
//         want_pubkey_hex  = bytes.fromhex('1976a914bc3b654dca7e56b04dca18f2566cdaf02e8d9ada88ac')

    //printf ("fn:%s | ln:%i\n" , __FUNCTION__, __LINE__); 

    bytearray script_pubkey_bin (1024);
    size_t script_pubkey_len = WriteScript (CreateWriteMemStream (&script_pubkey_bin[0], 1024) , tx.outputs[0].script_pubkey); 

    //printf ("fn:%s | ln:%i\n" , __FUNCTION__, __LINE__); 

    // test_input_pubkey(self):

    std::string script_pubkey_hex; 
    hex::encode (script_pubkey_hex, &script_pubkey_bin[0],  script_pubkey_len); 
    std::string want_pubkey_hex  = "1976a914bc3b654dca7e56b04dca18f2566cdaf02e8d9ada88ac"; 

    printf ("script_pubkey_hex:%s\n", script_pubkey_hex .c_str()); 
    printf ("want_pubkey_hex:%s\n", want_pubkey_hex .c_str()); 
    assert (script_pubkey_hex == want_pubkey_hex);

    //self.assertEqual(tx.tx_outs[0].script_pubkey.serialize(), want)
    assert (tx.locktime == 410393);

    // want = 10011545
    // self.assertEqual(tx.tx_outs[1].amount, want)
    assert (tx.outputs[1].amount == 10011545); 
    printf ("fn:%s | ln:%i\n" , __FUNCTION__, __LINE__); 

    // want = bytes.fromhex('1976a9141c4bc762dd5423e332166702cb75f40df79fea1288ac')
    // self.assertEqual(tx.tx_outs[1].script_pubkey.serialize(), want)
  }

  //printf ("(fn:%s | ln:%i) EXIT \n" , __FUNCTION__, __LINE__); 
  
  return 0;
}

    //script_command obj;  
int tx_test_p2  (std::vector<std::string>& args) {

  //FN_SCOPE ("");
  

  bool on_mainnet = false; 



  const std::string txhex =
    "0100000001813f79011acb80925dfe69b3def355fe914bd1d96a3f5f71bf8303c6a989c7d1000000006b483045022100ed81ff192e75a3fd2304004dcadb746fa5e24c5031ccfcf21320b0277457c98f02207a986d955c6e0cb35d446a89d3f56100f4d7f67801c31967743a9c8e10615bed01210349fc4e631e3624a545de3f89f5d8684c7b8138bd94bdd531d2e213bf016b278afeffffff02a135ef01000000001976a914bc3b654dca7e56b04dca18f2566cdaf02e8d9ada88ac99c39800000000001976a9141c4bc762dd5423e332166702cb75f40df79fea1288ac19430600"; 

  bytearray txbin; 
  hex::decode (txbin, txhex); 
  bmx::Transaction tx; 

  size_t txreadlen =  ReadTransaction (tx, CreateReadMemStream (&txbin[0],  txbin.size ()));

  {
    // def test_serialize(self):
    // raw_tx = bytes.fromhex('0100000001813f79011acb80925dfe69b3def355fe914bd1d96a3f5f71bf8303c6a989c7d1000000006b483045022100ed81ff192e75a3fd2304004dcadb746fa5e24c5031ccfcf21320b0277457c98f02207a986d955c6e0cb35d446a89d3f56100f4d7f67801c31967743a9c8e10615bed01210349fc4e631e3624a545de3f89f5d8684c7b8138bd94bdd531d2e213bf016b278afeffffff02a135ef01000000001976a914bc3b654dca7e56b04dca18f2566cdaf02e8d9ada88ac99c39800000000001976a9141c4bc762dd5423e332166702cb75f40df79fea1288ac19430600')
    // stream = BytesIO(raw_tx)
    // tx = Tx.parse(stream)
    // self.assertEqual(tx.serialize(), raw_tx)

    bytearray txobin (txreadlen, byte{0});

    size_t writeotxlen = WriteTransaction (CreateWriteMemStream (&txobin[0], txobin.size ()), tx); 

    printf ("(fn:%s | ln:%i) writeotxlen:%zu\n" , __FUNCTION__, __LINE__, writeotxlen); 
    printf ("(fn:%s | ln:%i) txreadlen  :%zu\n" , __FUNCTION__, __LINE__, txreadlen  ); 
    PR_CHECK ("writeotxlen == txreadlen", writeotxlen == txreadlen);
    assert (writeotxlen == txreadlen); 

  }

  return 0; 
}


//
//
int tx_test_input_value(std::vector<std::string> &args) {

  const std::string tx_hash_hex      = "d1c789a9c60383bf715f3f6ad9d14b91fe55f3deb369fe5d9280cb1a01793f81";
  const size_t      want             = 42505594;
  const size_t      default_sequence = 0xffffffff;



  bytearray tx_hash_bin;
  bmx::TxIn txin; 
  // construct a txin 
  txin.prev_index = 0; 
  copy_BE (txin.prev_txid, hex::decode (tx_hash_bin, tx_hash_hex)); 
  txin.sequence = default_sequence;
  // 
  PR_CHECK("want == bmx::Amount (txin, true)", want == bmx::Amount (txin, true)); 
  return 0;
}

//
//
int tx_test_fee (std::vector<std::string>& args) {

  bool on_mainnet = true;
  {
    const std::string raw_tx_hex = "0100000001813f79011acb80925dfe69b3def355fe914bd1d96a3f5f71bf8303c6a989c7d1000000006b483045022100ed81ff192e75a3fd2304004dcadb746fa5e24c5031ccfcf21320b0277457c98f02207a986d955c6e0cb35d446a89d3f56100f4d7f67801c31967743a9c8e10615bed01210349fc4e631e3624a545de3f89f5d8684c7b8138bd94bdd531d2e213bf016b278afeffffff02a135ef01000000001976a914bc3b654dca7e56b04dca18f2566cdaf02e8d9ada88ac99c39800000000001976a9141c4bc762dd5423e332166702cb75f40df79fea1288ac19430600";
    bytearray txbin; 
    Transaction tx; 
    hex::decode (txbin, raw_tx_hex);
    size_t readitxlen = ReadTransaction (tx, CreateReadMemStream (&txbin[0], txbin.size()));
    //     self.assertEqual(tx.fee(), 40000)
    PR_CHECK ("40000 == Tx::Fee (tx, on_mainnet)", 40000 == Tx::Fee (tx, on_mainnet)); 
  }
  // def test_fee(self):
  //     raw_tx = bytes.fromhex('0100000001813f79011acb80925dfe69b3def355fe914bd1d96a3f5f71bf8303c6a989c7d1000000006b483045022100ed81ff192e75a3fd2304004dcadb746fa5e24c5031ccfcf21320b0277457c98f02207a986d955c6e0cb35d446a89d3f56100f4d7f67801c31967743a9c8e10615bed01210349fc4e631e3624a545de3f89f5d8684c7b8138bd94bdd531d2e213bf016b278afeffffff02a135ef01000000001976a914bc3b654dca7e56b04dca18f2566cdaf02e8d9ada88ac99c39800000000001976a9141c4bc762dd5423e332166702cb75f40df79fea1288ac19430600')
  //     stream = BytesIO(raw_tx)
  //     tx = Tx.parse(stream)
  //     self.assertEqual(tx.fee(), 40000)

  {
    const std::string raw_tx_hex = "010000000456919960ac691763688d3d3bcea9ad6ecaf875df5339e148a1fc61c6ed7a069e010000006a47304402204585bcdef85e6b1c6af5c2669d4830ff86e42dd205c0e089bc2a821657e951c002201024a10366077f87d6bce1f7100ad8cfa8a064b39d4e8fe4ea13a7b71aa8180f012102f0da57e85eec2934a82a585ea337ce2f4998b50ae699dd79f5880e253dafafb7feffffffeb8f51f4038dc17e6313cf831d4f02281c2a468bde0fafd37f1bf882729e7fd3000000006a47304402207899531a52d59a6de200179928ca900254a36b8dff8bb75f5f5d71b1cdc26125022008b422690b8461cb52c3cc30330b23d574351872b7c361e9aae3649071c1a7160121035d5c93d9ac96881f19ba1f686f15f009ded7c62efe85a872e6a19b43c15a2937feffffff567bf40595119d1bb8a3037c356efd56170b64cbcc160fb028fa10704b45d775000000006a47304402204c7c7818424c7f7911da6cddc59655a70af1cb5eaf17c69dadbfc74ffa0b662f02207599e08bc8023693ad4e9527dc42c34210f7a7d1d1ddfc8492b654a11e7620a0012102158b46fbdff65d0172b7989aec8850aa0dae49abfb84c81ae6e5b251a58ace5cfeffffffd63a5e6c16e620f86f375925b21cabaf736c779f88fd04dcad51d26690f7f345010000006a47304402200633ea0d3314bea0d95b3cd8dadb2ef79ea8331ffe1e61f762c0f6daea0fabde022029f23b3e9c30f080446150b23852028751635dcee2be669c2a1686a4b5edf304012103ffd6f4a67e94aba353a00882e563ff2722eb4cff0ad6006e86ee20dfe7520d55feffffff0251430f00000000001976a914ab0c0b2e98b1ab6dbf67d4750b0a56244948a87988ac005a6202000000001976a9143c82d7df364eb6c75be8c80df2b3eda8db57397088ac46430600";
    bytearray txbin; 
    Transaction tx; 
    hex::decode (txbin, raw_tx_hex);
    size_t readitxlen = ReadTransaction (tx, CreateReadMemStream (&txbin[0], txbin.size()));
    PR_CHECK ("140500 == Tx::Fee (tx, on_mainnet)", 140500 == Tx::Fee (tx, on_mainnet));
  }

  //     raw_tx = bytes.fromhex('010000000456919960ac691763688d3d3bcea9ad6ecaf875df5339e148a1fc61c6ed7a069e010000006a47304402204585bcdef85e6b1c6af5c2669d4830ff86e42dd205c0e089bc2a821657e951c002201024a10366077f87d6bce1f7100ad8cfa8a064b39d4e8fe4ea13a7b71aa8180f012102f0da57e85eec2934a82a585ea337ce2f4998b50ae699dd79f5880e253dafafb7feffffffeb8f51f4038dc17e6313cf831d4f02281c2a468bde0fafd37f1bf882729e7fd3000000006a47304402207899531a52d59a6de200179928ca900254a36b8dff8bb75f5f5d71b1cdc26125022008b422690b8461cb52c3cc30330b23d574351872b7c361e9aae3649071c1a7160121035d5c93d9ac96881f19ba1f686f15f009ded7c62efe85a872e6a19b43c15a2937feffffff567bf40595119d1bb8a3037c356efd56170b64cbcc160fb028fa10704b45d775000000006a47304402204c7c7818424c7f7911da6cddc59655a70af1cb5eaf17c69dadbfc74ffa0b662f02207599e08bc8023693ad4e9527dc42c34210f7a7d1d1ddfc8492b654a11e7620a0012102158b46fbdff65d0172b7989aec8850aa0dae49abfb84c81ae6e5b251a58ace5cfeffffffd63a5e6c16e620f86f375925b21cabaf736c779f88fd04dcad51d26690f7f345010000006a47304402200633ea0d3314bea0d95b3cd8dadb2ef79ea8331ffe1e61f762c0f6daea0fabde022029f23b3e9c30f080446150b23852028751635dcee2be669c2a1686a4b5edf304012103ffd6f4a67e94aba353a00882e563ff2722eb4cff0ad6006e86ee20dfe7520d55feffffff0251430f00000000001976a914ab0c0b2e98b1ab6dbf67d4750b0a56244948a87988ac005a6202000000001976a9143c82d7df364eb6c75be8c80df2b3eda8db57397088ac46430600')
  //     stream = BytesIO(raw_tx)
  //     tx = Tx.parse(stream)
  //     self.assertEqual(tx.fee(), 140500)
  return 0; 
  }




//
int test_sig_hash (std::vector<std::string>& args) {

  FN_SCOPE ();
  //
  //printf ("%s:ENTER\n", __FUNCTION__);

  bmx::Transaction tx; 
  bmx::TxFetcher   txf;
  //bytearray        txbin;

  // these are ver 2
  //std::string      txid_hex   = "c8258fd502b2593c1d77466ba3d64503e05c3f091d5ab652e16a15891a0c3bdd"; 
  //std::string      txid_hex   = "3db68a2171756cfb0c7af980ac8780b4b5c892412f50cd8c4808182c7408aeb8";

  //
  // use this instead
  std::string      txid_hex   = "0d6fe5213c0b3291f208cba8bfb59b7476dffacc4e5cb66f6eb20a080843a299"; 
  const bool       on_mainnet = false;

  if (txf.Fetch (tx, txid_hex , on_mainnet) ) { 

    printf  ("TxID:%s\n", txid_hex.c_str()); 
    printf  ("  ver:%i\n", tx.version); 
    printf  ("  num txis:%zu\n", tx.inputs.size ()); 
    printf  ("  num txos:%zu\n", tx.outputs.size ()); 
    printf  ("  locktime:%zu\n", tx.locktime);


  }
  else {
    printf  ("  fetching tx failed\n", tx.locktime); 
  }

  
  
  //printf ("%s:EXIT\n", __FUNCTION__); 
  return 0;
}


  //
//
int tx_test_sig_hash (std::vector<std::string>& args) {

  FN_SCOPE (); 
  //prscope scope (__FUNCTION__, __LINE__, "tst"); 

  
  const bool        on_mainnet = true; 
  const std::string wanthex    = "27e0c5994dec7824e56dec6b2fcb342eb7cdb0d0957c2fce9882f715e85d81a6"; 

  bmx::Transaction  tx;
  bmx::TxFetcher    txf;
  txf.Fetch (tx, "452c629d67e41baec3ac6f04fe744b4b9617f8f859c63b3002f8684e7a4fee03", on_mainnet); 

  digest32 testhash; 
  Tx::SignatureHash  (testhash, tx, 0, on_mainnet);

  std::string testhex; 
  hex::encode (testhex,  &testhash[0], testhash.size()); 

  printf ("(fn:%s | ln:%i) wanthex:%s\n" , __FUNCTION__, __LINE__, wanthex.c_str()); 
  printf ("(fn:%s | ln:%i) testhex:%s\n" , __FUNCTION__, __LINE__, testhex.c_str()); 

   
  PR_CHECK  (std::string(__FUNCTION__).c_str() , testhex == wanthex);

  return 0; 
}


int tx_test_verify_p2pkh (std::vector<std::string>& args) {
  FN_SCOPE (); 
  bool on_mainnet = false;
  bool on_testnet = !on_mainnet;
  TxFetcher txf;

  puts ("terw525fas32"); 
  {
    Transaction tx; 
    txf.Fetch (tx, "452c629d67e41baec3ac6f04fe744b4b9617f8f859c63b3002f8684e7a4fee03", on_testnet);
    PR_CHECK ("verify 1", Tx::Verify (tx, on_testnet)); 
  }

  puts ("525fas32"); 
  // def test_verify_p2pkh(self):
  //     tx = TxFetcher.fetch('452c629d67e41baec3ac6f04fe744b4b9617f8f859c63b3002f8684e7a4fee03')
  //     self.assertTrue(tx.verify())

  {
    Transaction tx; 
    txf.Fetch (tx, "5418099cc755cb9dd3ebc6cf1a7888ad53a1a3beb5a025bce89eb1bf7f1650a2", on_mainnet); 
    PR_CHECK ("verify 2",  Tx::Verify (tx, on_mainnet)); 
  }
  puts ("iop525fas32"); 

  //     tx = TxFetcher.fetch('5418099cc755cb9dd3ebc6cf1a7888ad53a1a3beb5a025bce89eb1bf7f1650a2', testnet=True)
  //     self.assertTrue(tx.verify())
  return 0; 
  }

  
int test_verify_p2sh (std::vector<std::string>& args) {
  FN_SCOPE ();
  bool on_mainnet = true;
  // def test_verify_p2sh(self):
  //     tx = TxFetcher.fetch('46df1a9484d0a81d03ce0ee543ab6e1a23ed06175c104a178268fad381216c2b')
  //     self.assertTrue(tx.verify())
  TxFetcher txf;
  Transaction tx; 
  txf.Fetch (tx, "46df1a9484d0a81d03ce0ee543ab6e1a23ed06175c104a178268fad381216c2b", on_mainnet);


  PR_CHECK (std::string(__FUNCTION__).c_str(), Tx::Verify (tx, on_mainnet)); 

  return 0; 
}



//
//
int test_problem_tx (std::vector<std::string>& args) {

  const bool on_mainnet = true; 
  const size_t      default_sequence = 0xffffffff;

  const std::string tx_hex = "0100000001c847414138fc4e86c97bce0adfe0180d8716d0db7f43b955ebb7a80f3cbc2500000000006a47304402202f7e26dda5a70179eaa51e7a995b2fb6b3a705c59c792ae1fde3a4f4a58adaf60220406672081f8f2acfdfbeb327a5c618beb66ab226111da48ac9b150dad0d0ae52012103935581e52c354cd2f484fe8ed83af7a3097005b2f9c60bff71d35bd795f54b67ffffffff0e404b4c00000000001976a91477d946a68a9b95e851afa57006cf2d0c15ae8b3d88ac404b4c00000000001976a914325371fe093e259bdc7beca2c31f795e1b492b2088ac404b4c00000000001976a9144ccf8be232f0b1ee450a5edcc83cc4966703531388ac404b4c00000000001976a9146fe7d8cea1a39739508db7070b029d8497a0d85288ac404b4c00000000001976a91427813ea0d6e3439ffa3e30e47cd768c45bd27ab888ac404b4c00000000001976a914c16ac1981a4c73f1d51cc28f53d4757d3673a45c88ac404b4c00000000001976a9143a1806b04b0f3e14ab9b7c8cb045175d14014ac188ac404b4c00000000001976a914af39e20d8f115ecdbb3b96cda2710239e9259c5288ac404b4c00000000001976a914047357aff1cb49f6a26d71e48b88c1ba7c6ce92788ac404b4c00000000001976a9149637bebfa095f176b6cbffc79cec55fb55bf14de88ac404b4c00000000001976a9142dffa6b5f8ba2bf1ab487d1be1af9d9695350a4b88ac404b4c00000000001976a914fcf0cb53dccea9e4125a8472b8606e7f1769dad388ac404b4c00000000001976a9145a8398af0353464cf727d57a1dd79807eee50b1288ac00639f02000000001976a914d52ad7ca9b3d096a38e752c2018e6fbc40cdf26f88ac00000000";
  Transaction tx;

  bytearray txbin ;
  hex::decode (txbin, tx_hex);
  size_t txlen = ReadTransaction (tx, CreateReadMemStream (&txbin[0],  txbin.size())); 

  printf ("numinputs:%zu\n", tx.inputs.size ());
  printf ("numoutputs:%zu\n", tx.outputs.size ()); 
  
  PR_CHECK ("txlen == txbin.size", txlen == txbin.size ()); 
  return 0; 
  
}



int test_input_pubkey (std::vector<std::string>& args) {
  // FN_SCOPE(); 
  // def test_input_pubkey(self):
  const bool on_mainnet = true; 
  const size_t      default_sequence = 0xffffffff;
  const std::string tx_hash_hex = "d1c789a9c60383bf715f3f6ad9d14b91fe55f3deb369fe5d9280cb1a01793f81"; 
  bytearray txidbin;

  TxIn txin; 
  
  txin.prev_index = 0;
  copy_BE (txin.prev_txid, hex::decode (txidbin, tx_hash_hex));
  txin.sequence = default_sequence;

  // test ScriptBubKey
  command_list script_pubkey; 
  txin.script_sig = ScriptPubKey (script_pubkey, txin, on_mainnet); 
  bytearray scriptbin (1024, byte{0});
  // serialize 
  size_t writescriptlen = WriteScript (CreateWriteMemStream(&scriptbin[0], scriptbin.size()), txin.script_sig);

  std::string test_script_hex;
  hex::encode (test_script_hex, &scriptbin[0], writescriptlen);
  const std::string  want = "1976a914a802fc56c704ce87c42d7c92eb75e7896bdc41ae88ac";
  //PR_CHECK ("test_script_hex == want", test_script_hex == want);
  PR_CHECK (std::string(__FUNCTION__).c_str(), test_script_hex == want);

  return 0;
}



int test_sign_input (std::vector<std::string>& args) {
  FN_SCOPE (); 
  
  FFM_Env env;
  bmx::Init_secp256k1_Env (env);
  ScopeDeleter dr (env.F);

  int prksign = 0;
  // secret=8675309
  
  FE_t privkey = dr(env.F->New_ui (8675309));

  // CL-USER> #xed5f84 -> 15556484 (24 bits, #xED5F84)
  // CL-USER> #x845fed -> 8675309  (24 bits, #x845FED)
  bytearray privatekeybin;  
  env.F->Raw (privatekeybin, prksign, privkey, false);
  //printf ("privatekeybin.prksign -> {%i | %zu]\n", prksign, privatekeybin.size () ); 
  PrivateKey privatekey;
  copy_BE (privatekey, privatekeybin);


  int num_O_lines = 20; 
  // for  (int i = 0; i < num_O_lines; ++i) {
  //   printf ("|-------------------o(O)o----------------------->\n"); 
  // }

  // printf ("\n\n");

  // for (auto e : privatekeybin) { printf ("0x%x " , e); }


  // printf ("\n\n"); 


  // for  (auto e : privatekey)  { printf ("0x%x " , e); }

  // printf ("\n\n"); 
  const bool on_mainnet = false;


  printf ("READING TRANSACTION BEGIN\n");   
  std::string tx_hex = "010000000199a24308080ab26e6fb65c4eccfadf76749bb5bfa8cb08f291320b3c21e56f0d0d00000000ffffffff02408af701000000001976a914d52ad7ca9b3d096a38e752c2018e6fbc40cdf26f88ac80969800000000001976a914507b27411ccf7f16f10297de6cef3f291623eddf88ac00000000"; 
  bytearray txbin;
  hex::decode (txbin, tx_hex); 
  Transaction tx;
  size_t readtxlen = ReadTransaction (tx, CreateReadMemStream (&txbin[0], txbin.size ()));
  printf ("READING TRANSACTION END\n");

  //stream = BytesIO(bytes.fromhex('010000000199a24308080ab26e6fb65c4eccfadf76749bb5bfa8cb08f291320b3c21e56f0d0d00000000ffffffff02408af701000000001976a914d52ad7ca9b3d096a38e752c2018e6fbc40cdf26f88ac80969800000000001976a914507b27411ccf7f16f10297de6cef3f291623eddf88ac00000000'))
  //want = '010000000199a24308080ab26e6fb65c4eccfadf76749bb5bfa8cb08f291320b3c21e56f0d0d0000006b4830450221008ed46aa2cf12d6d81065bfabe903670165b538f65ee9a3385e6327d80c66d3b502203124f804410527497329ec4715e18558082d489b218677bd029e7fa306a72236012103935581e52c354cd2f484fe8ed83af7a3097005b2f9c60bff71d35bd795f54b67ffffffff02408af701000000001976a914d52ad7ca9b3d096a38e752c2018e6fbc40cdf26f88ac80969800000000001976a914507b27411ccf7f16f10297de6cef3f291623eddf88ac00000000'
  // int a1 = 69; 
  // std::string b1=  "wat";
  // float c1 = 1.0f;

  
  {
    bytearray obin (readtxlen, byte{0});
    std::string ohex; 
    Transaction tx_out;
    size_t writetxlen = WriteTransaction (CreateWriteMemStream (&obin[0], obin.size()), tx);
    hex::encode (ohex, &obin[0], obin.size());

    PR_CHECK ("writetxlen == readtxlen", writetxlen == readtxlen);
    PR_CHECK ("ohex == tx_hex", ohex == tx_hex) ; 
  }

  PR_CHECK ("Tx::SignInput", Tx::SignInput (tx, 0, privatekey, on_mainnet));


		       
  return 0;
}

//
//
int test_hmac_sha256 (std::vector<std::string>& args) {
  FN_SCOPE();

  FFM_Env env;
  bmx::Init_secp256k1_Env (env);
  ScopeDeleter dr (env.F);
  FEConRef& F = env.F; 
  int prksign = 0;
  // secret=8675309
 
  bytearray bin; 
  FE_t fe_priv = dr(env.F->New_ui (8675309));
  int prv_sign = 0;
  F->Raw (bin, prv_sign, fe_priv); 
  PrivateKey privatekey;
  copy_BE (privatekey, bin);

  const size_t txt_bin_max_size = 1024;
  bytearray input_txt_bin ( txt_bin_max_size );   

  af::WriteStreamRef wstxt = CreateWriteMemStream ( &input_txt_bin[0], txt_bin_max_size); 
  // cat text here

  digest32 odigest; 
  af::hmac_sha256 (odigest, privatekey, &input_txt_bin[0], wstxt->GetPos ());


  return 0 ;  
}


int test_Deterministic_K (std::vector<std::string>& args) {
  FN_SCOPE (); 

  //digest32&  Deterministic_K (digest32& ok, const PrivateKey& priv, const digest32& z); 
  return 0; 
}


int Ex_7_4 (std::vector<std::string> &args) {
  FN_SCOPE();


  return 0;
}


int Ex_7_5  (std::vector<std::string>& args) {
  FN_SCOPE();
  // Advanced: Get some more testnet coins from a testnet faucet and create a two-input,
  //one-output transaction. One input should be from the faucet, the other should be
  //from the previous exercise; the output can be your own address.
  return 0; 
}

//
int CH7_Ex (std::vector<std::string>& args) {

  FN_SCOPE();
  
  // tx_test_p1          (args); 
  // tx_test_p2          (args); 
  // tx_test_fee         (args);
  // tx_test_input_value (args); 
    
  Ex_7_4 (args); 
  Ex_7_5 (args);
    
  //test_problem_tx (args);
  // tx_test_sig_hash (args); 
  // test_input_pubkey    (args);
  //  tx_test_sig_hash (args);
  //tx_test_verify_p2pkh (args);
    test_verify_p2sh     (args);

  //test_hmac_sha256 (args); 
  // test_Deterministic_K (args) ; 
  //test_sign_input      (args);

  return 0;
}



