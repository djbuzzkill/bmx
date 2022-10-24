
// 
int test_curl (const std::vector<std::string>& args) {

  const std::string tx_id = "3db68a2171756cfb0c7af980ac8780b4b5c892412f50cd8c4808182c7408aeb8"; 

  std::string   txbytes;
  bmx::TxFetcher fetcher;

  if (fetcher.Fetch (txbytes, tx_id, false)) {

   printf ("\n\n\n\n\n\n"); 
   //   printf ("contents: %s\n",printstr.c_str()); 

  }
    
  return 0;
}

//
//
int int_2_hexes (const std::vector<std::string> &args) {
  //bmx:: print_verify_mapping ();
  // for (unsigned char i = 79; i < 190; ++i) {
  bmx::test_encode_decode(); 
  //   printf ("int:%i|%s\n", i, af::hex::from_uc(i).c_str()); 
  // }
  return 0; 
}


//
//

int test_hmac_sha256 (const std::vector<std::string> &args) {
  FN_SCOPE ();

  digest32 odigest;
  fixnum32 key;

  const size_t txt_bin_max_size = 1024;
  bytearray input_txt_bin ( txt_bin_max_size );   

  af::WriteStreamRef wstxt = CreateWriteMemStream ( &input_txt_bin[0], txt_bin_max_size); 
  // cat text here
  
  af::hmac_sha256 (odigest, key, &input_txt_bin[0], wstxt->GetPos ());

  //af::digest32&
  //af::hmac_sha256 (digest32& oz, const fixnum32& key, const void* txt, size_t lentxt) ; 
  return 0; 
}
  
//
//
int test_inspect_tx(const std::vector<std::string> &args) {
  FN_SCOPE ();
  
  using namespace af;
  using namespace bmx;

  TxFetcher txf;
  
  // TxID
  bool              on_mainnet  = false;
  const std::string TxID_hex    = "3db68a2171756cfb0c7af980ac8780b4b5c892412f50cd8c4808182c7408aeb8";

  Transaction tx; 
  txf.Fetch (tx, TxID_hex, on_mainnet);

  PR_CHECK ("verify 1", Tx::Verify (tx, on_mainnet)); 
  //size_t txlen = ReadTransaction (tx, CreateReadMemStream (&tx_bin[0],  tx_bin.size()));


  printf ("    inputs(%zu)\n", tx.inputs.size ());
  printf ("    outputs(%zu)\n", tx.outputs.size ());


  for (auto i = 0; i < tx.outputs.size (); ++i)
    printf ( "    output[%zu] %s \n", i, script_ut::format_script (tx.outputs[i].script_pubkey).c_str());



  
  //PR_CHECK ("txlen == txbin.size", txlen == tx_bin.size ()); 


  return 0;
}


//
//
int flatbuffers_test (const std::vector<std::string>& args) {

  flatbuffers::FlatBufferBuilder build; 

  auto         burgername = build.CreateString ("oscar");

  rum::M101_Status stat;
  
  
  unsigned int burgerid   = 69; 
  std::string toppings_str[] = {"bacon", "cheez", "onion", "mayonaise"};
  
  std::vector<flatbuffers::Offset<flatbuffers::String>> toppingsvec;
  toppingsvec.push_back (build.CreateString ("bacon"));
  toppingsvec.push_back (build.CreateString ("cheez"));
  toppingsvec.push_back (build.CreateString ("onion"));
  toppingsvec.push_back (build.CreateString ("mayonaise")); 
  auto toppings = build.CreateVector (toppingsvec); 
  
  auto onionburger = rum::msg::CreateCheezburger (build, burgername, burgerid, toppings); 
  build.Finish (onionburger); 
  
  build.GetSize ();
  build.GetBufferPointer ();
  
  af::bytearray burgermem (build.GetSize()); {
    af::WriteStreamRef ws =
      af::CreateWriteMemStream(&burgermem[0], build.GetSize());

    ws->Write(build.GetBufferPointer(), burgermem.size());
  }

  auto cheezburger = rum::msg::GetCheezburger (&burgermem[0]);

  printf ("cheezburger: %s\n", cheezburger->name()->c_str());

  for (size_t i = 0; i < cheezburger->toppings()->size(); ++i) {
    printf ("toppings:\n");
    printf (" %s\n",  cheezburger->toppings()->GetAsString (i)->c_str());
  }
  
  return 0; 
}

