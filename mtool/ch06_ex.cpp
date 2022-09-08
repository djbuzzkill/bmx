











int Ex6_1 (const std::vector<std::string>& args) {
  // using namespace af;
  // using namespace ffm; 
  // using namespace bmx;
    
  //  ScopeDeleter dr(F);
  
  PR ("CH 6.1.2\n"); 
  // from script import Script
  bytearray z_bin;
  af::digest32 z; 
  // z                       = 0x7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d
  const std::string z_xstr  = "7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d";
  //z                      = 0x7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d
  
  bytearray sec_bin;  
  // sec       = bytes.fromhex('04887387e452b8eacc4acfde10d9aaf7f6d9a0f975aabb10d006e4da568744d06c61de6d95231cd89026e286df3b6ae4a894a3378e393e93a0f45b666329a0ae34')
  const std::string sec_xstr = "04887387e452b8eacc4acfde10d9aaf7f6d9a0f975aabb10d006e4da568744d06c61de6d95231cd89026e286df3b6ae4a894a3378e393e93a0f45b666329a0ae34";
  hex::decode (sec_bin, sec_xstr); 
  
  
  PublicKey pubk;
  size_t read_pubkey_size = ReadPoint (pubk, CreateReadMemStream (&sec_bin[0], sec_bin.size())); 
  print_point ("from SECbin", pubk); 
  
  bytearray sig_bin; 
  // sig       = bytes.fromhex('3045022000eff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c022100c7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab601')
  const std::string sig_xstr = "3045022000eff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c022100c7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab601";
  hex::decode (sig_bin, sig_xstr);

  bmx::Signature sig;
  size_t sig_size = sig_bin.size () - 1;

  bmx::ReadSignature_DER (sig, sig_size, CreateReadMemStream (&sig_bin[0], sig_size));
  print_sig ("sig_bin", sig); 

    // }
  bmx::command_list script_sig = {
    sco  (sig_bin), 
  };
    
  bmx::command_list script_pubkey = {
    sco (sec_bin), 
    sco (OP_CHECKSIG), // checksig
  };


  
  script_env env;
  Init_secp256k1_Env (env.ffme);
  copy_BE (env.z, hex::decode (z_bin, z_xstr));
  append (append (env.cmds, script_sig), script_pubkey);
    
  
  if (bmx::EvalScript (env)) {
    PR ("bmx::EvalScript (comb_script) !! SUCCESS !!\n"); 
  }
  else {
    PR ("!! FAILED !! bmx::EvalScript (comb_script)\n ");
  }



  return 0;
  
}

    
//
//
 int Ex6_3 (const std::vector<std::string>& args) {

  printf ("%s:ENTER\n", __FUNCTION__); 
   // Create a ScriptSig that can unlock this ScriptPubKey. Note OP_MUL multiplies the top two elements of the stack.

   // 767695935687

   command_list script_sig = {
     sco (OP_2), 
     };


   command_list script_pubkey = {
     sco(OP_DUP),
     sco(OP_DUP),
     sco(OP_MUL),
     sco(OP_ADD),
     sco(OP_6),
     sco(OP_EQUAL)
     }; 


   script_env env;
   Init_secp256k1_Env (env.ffme);
   //copy_BE (env.z, hex::decode (z_bin, z_xstr));
   append (append (env.cmds, script_sig), script_pubkey); 

   if (EvalScript (env)) {
     PR ("bmx::EvalScript (comb_script) !! SUCCESS !!\n"); 
   }
   else {
     PR ("!! FAILED !! bmx::EvalScript (comb_script)\n ");
   }
   

   printf ("%s:EXIT\n", __FUNCTION__); 
   
   return 0; 
 }

//
//
 int Ex6_4 (const std::vector<std::string>& args) {

   // Figure out what this Script is doing :

   // `6e879169a77ca787`
   
   // * `69 = OP_VERIFY`
   // * `6e = OP_2DUP`
   // * `7c = OP_SWAP`
   // * `87 = OP_EQUAL`
   // * `91 = OP_NOT`
   // * `a7 = OP_SHA1`
   
   command_list script_pubkey = {
     sco (OP_2DUP),  // 6e
     sco (OP_EQUAL), // 87
     sco (OP_NOT),   // 91
     sco (OP_VERIFY),// 69
     sco (OP_SHA1),  // a7
     sco (OP_SWAP),  // 7c
     sco (OP_SHA1),  // a7
     sco (OP_EQUAL), //87
   }; 

   // Use the `Script.parse` method and look up what various opcodes do at
   // https://en.bitcoin.it/wiki/Script
   
   return 0;
 }

// 
int CH6_Ex (std::vector<std::string>& args) {
    //
  printf ("%s:ENTER\n", __FUNCTION__);

  int res_6_1 = Ex6_1 (args); 
  int res_6_3 = Ex6_3 (args); 
  int res_6_4 = Ex6_4 (args);

  printf ("%s:EXIT\n", __FUNCTION__); 
  return 0;
}

