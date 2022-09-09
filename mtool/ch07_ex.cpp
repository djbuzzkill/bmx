




//
int test_sig_hash (std::vector<std::string>& args) {

  //FN_SCOPE ("");
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
    bytearray script_sig_bin (want_sig_hex.size (), 0); 

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

    bytearray txobin (txreadlen, 0);

    size_t writeotxlen = WriteTransaction (CreateWriteMemStream (&txobin[0], txobin.size ()), tx); 

    printf ("(fn:%s | ln:%i) writeotxlen:%zu\n" , __FUNCTION__, __LINE__, writeotxlen); 
    printf ("(fn:%s | ln:%i) txreadlen  :%zu\n" , __FUNCTION__, __LINE__, txreadlen  ); 
    PR_CHECK ("writeotxlen == txreadlen", writeotxlen == txreadlen);
    assert (writeotxlen == txreadlen); 

  }

//     def test_input_value(self):
//         tx_hash = 'd1c789a9c60383bf715f3f6ad9d14b91fe55f3deb369fe5d9280cb1a01793f81'
//         index = 0
//         want = 42505594
//         tx_in = TxIn(bytes.fromhex(tx_hash), index)
//         self.assertEqual(tx_in.value(), want)

//     def test_input_pubkey(self):
//         tx_hash = 'd1c789a9c60383bf715f3f6ad9d14b91fe55f3deb369fe5d9280cb1a01793f81'
//         index = 0
//         tx_in = TxIn(bytes.fromhex(tx_hash), index)
//         want = bytes.fromhex('1976a914a802fc56c704ce87c42d7c92eb75e7896bdc41ae88ac')
//         self.assertEqual(tx_in.script_pubkey().serialize(), want)

//     def test_fee(self):
//         raw_tx = bytes.fromhex('0100000001813f79011acb80925dfe69b3def355fe914bd1d96a3f5f71bf8303c6a989c7d1000000006b483045022100ed81ff192e75a3fd2304004dcadb746fa5e24c5031ccfcf21320b0277457c98f02207a986d955c6e0cb35d446a89d3f56100f4d7f67801c31967743a9c8e10615bed01210349fc4e631e3624a545de3f89f5d8684c7b8138bd94bdd531d2e213bf016b278afeffffff02a135ef01000000001976a914bc3b654dca7e56b04dca18f2566cdaf02e8d9ada88ac99c39800000000001976a9141c4bc762dd5423e332166702cb75f40df79fea1288ac19430600')
//         stream = BytesIO(raw_tx)
//         tx = Tx.parse(stream)
//         self.assertEqual(tx.fee(), 40000)
//         raw_tx = bytes.fromhex('010000000456919960ac691763688d3d3bcea9ad6ecaf875df5339e148a1fc61c6ed7a069e010000006a47304402204585bcdef85e6b1c6af5c2669d4830ff86e42dd205c0e089bc2a821657e951c002201024a10366077f87d6bce1f7100ad8cfa8a064b39d4e8fe4ea13a7b71aa8180f012102f0da57e85eec2934a82a585ea337ce2f4998b50ae699dd79f5880e253dafafb7feffffffeb8f51f4038dc17e6313cf831d4f02281c2a468bde0fafd37f1bf882729e7fd3000000006a47304402207899531a52d59a6de200179928ca900254a36b8dff8bb75f5f5d71b1cdc26125022008b422690b8461cb52c3cc30330b23d574351872b7c361e9aae3649071c1a7160121035d5c93d9ac96881f19ba1f686f15f009ded7c62efe85a872e6a19b43c15a2937feffffff567bf40595119d1bb8a3037c356efd56170b64cbcc160fb028fa10704b45d775000000006a47304402204c7c7818424c7f7911da6cddc59655a70af1cb5eaf17c69dadbfc74ffa0b662f02207599e08bc8023693ad4e9527dc42c34210f7a7d1d1ddfc8492b654a11e7620a0012102158b46fbdff65d0172b7989aec8850aa0dae49abfb84c81ae6e5b251a58ace5cfeffffffd63a5e6c16e620f86f375925b21cabaf736c779f88fd04dcad51d26690f7f345010000006a47304402200633ea0d3314bea0d95b3cd8dadb2ef79ea8331ffe1e61f762c0f6daea0fabde022029f23b3e9c30f080446150b23852028751635dcee2be669c2a1686a4b5edf304012103ffd6f4a67e94aba353a00882e563ff2722eb4cff0ad6006e86ee20dfe7520d55feffffff0251430f00000000001976a914ab0c0b2e98b1ab6dbf67d4750b0a56244948a87988ac005a6202000000001976a9143c82d7df364eb6c75be8c80df2b3eda8db57397088ac46430600')
//         stream = BytesIO(raw_tx)
//         tx = Tx.parse(stream)
//         self.assertEqual(tx.fee(), 140500)

//     def test_verify_p2pkh(self):
//         tx = TxFetcher.fetch('452c629d67e41baec3ac6f04fe744b4b9617f8f859c63b3002f8684e7a4fee03')
//         self.assertTrue(tx.verify())
//         tx = TxFetcher.fetch('5418099cc755cb9dd3ebc6cf1a7888ad53a1a3beb5a025bce89eb1bf7f1650a2', testnet=True)
//         self.assertTrue(tx.verify())

//     def test_verify_p2sh(self):
//         tx = TxFetcher.fetch('46df1a9484d0a81d03ce0ee543ab6e1a23ed06175c104a178268fad381216c2b')
//         self.assertTrue(tx.verify())

//     def test_sign_input(self):
//         private_key = PrivateKey(secret=8675309)
//         stream = BytesIO(bytes.fromhex('010000000199a24308080ab26e6fb65c4eccfadf76749bb5bfa8cb08f291320b3c21e56f0d0d00000000ffffffff02408af701000000001976a914d52ad7ca9b3d096a38e752c2018e6fbc40cdf26f88ac80969800000000001976a914507b27411ccf7f16f10297de6cef3f291623eddf88ac00000000'))
//         tx_obj = Tx.parse(stream, testnet=True)
//         self.assertTrue(tx_obj.sign_input(0, private_key))
//         want = '010000000199a24308080ab26e6fb65c4eccfadf76749bb5bfa8cb08f291320b3c21e56f0d0d0000006b4830450221008ed46aa2cf12d6d81065bfabe903670165b538f65ee9a3385e6327d80c66d3b502203124f804410527497329ec4715e18558082d489b218677bd029e7fa306a72236012103935581e52c354cd2f484fe8ed83af7a3097005b2f9c60bff71d35bd795f54b67ffffffff02408af701000000001976a914d52ad7ca9b3d096a38e752c2018e6fbc40cdf26f88ac80969800000000001976a914507b27411ccf7f16f10297de6cef3f291623eddf88ac00000000'
//         self.assertEqual(tx_obj.serialize().hex(), want)
  
  return 0; 
}



//
//
int tx_test_sig_hash (std::vector<std::string>& args) {

  FN_SCOPE ("hashY"); 
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

  PR_CHECK ("testhex == wanthex", testhex == wanthex); 
  // def test_sig_hash(self):
  //     tx = TxFetcher.fetch('452c629d67e41baec3ac6f04fe744b4b9617f8f859c63b3002f8684e7a4fee03')
  //     want = int('27e0c5994dec7824e56dec6b2fcb342eb7cdb0d0957c2fce9882f715e85d81a6', 16)
  //     self.assertEqual(tx.sig_hash(0), want)
  return 0; 
}




//
int CH7_Ex (std::vector<std::string>& args) {
    //
  printf ("%s:ENTER\n", __FUNCTION__);

  tx_test_p1 (args); 
  tx_test_p2 (args); 
  tx_test_sig_hash  (args); 

  
  printf ("%s:EXIT\n", __FUNCTION__); 
  return 0;
}

