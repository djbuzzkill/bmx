
//
int test_merkle_tree_populate_1(const std::vector<std::string> &args) {
  FN_SCOPE (); 
  const char* hex_hashes[] = {
    "9745f7173ef14ee4155722d1cbf13304339fd00d900b759c6f9d58579b5765fb",
    "5573c8ede34936c29cdfdfe743f7f5fdfbd4f54ba0705259e62f39917065cb9b",
    "82a02ecbb6623b4274dfcab82b336dc017a27136e08521091e443e62582e8f05",
    "507ccae5ed9b340363a0e6d765af148be9cb1c8766ccc922f83e4ae681658308",
    "a7a4aec28e7162e1e9ef33dfa30f0bc0526e6cf4b11a576f6c5de58593898330",
    "bb6267664bd833fd9fc82582853ab144fece26b7a8a5bf328f8a059445b59add",
    "ea6d7ac1ee77fbacee58fc717b990c4fcccf1b19af43103c090f601677fd8836",
    "457743861de496c429912558a106b810b0507975a49773228aa788df40730d41",
    "7688029288efc9e9a0011c960a6ed9e5466581abf3e3a6c26ee317461add619a",
    "b1ae7f15836cb2286cdd4e2c37bf9bb7da0a2846d06867a429f654b2e7f383c9",
    "9b74f89fa3f93e71ff2c241f32945d877281a6a50a6bf94adac002980aafe5ab",
    "b3a92b5b255019bdaf754875633c2de9fec2ab03e6b8ce669d07cb5b18804638",
    "b5c0b915312b9bdaedd2b86aa2d0f8feffc73a2d37668fd9010179261e25e263",
    "c9d52c5cb1e557b92c84c52e7c4bfbce859408bedffc8a5560fd6e35e10b8800",
    "c555bc5fc3bc096df0a0c9532f07640bfb76bfe4fc1ace214b8b228a1297a4c2",
    "f9dbfafc3af3400954975da24eb325e326960a25b87fffe23eef3e7ed2fb610e",
  }; 

  int32 num_hashes = sizeof(hex_hashes)/ sizeof(char*);// sizeof(hex_hashes) / sizeof(std::string); 
  printf ("   num_hashes: %i\n", num_hashes);

  hasharray hashes (num_hashes);
  bytearray bits (31, byte{1});
  //
  for (int32 i = 0; i < num_hashes; ++i) {
    bytearray bytes;
    std::string hexs = hex_hashes[i];

    printf ("   hexs(%i):%s\n", i, hexs.c_str()); 
    hex::decode (bytes, hexs); 
  
    copy_BE (hashes[i], hex::decode (bytes, hexs));
  }
  printf (" wat \n");


  merkletree mt;

  Init (mt, hashes.size ());
  printf ( "    merkle:\n%s\n",   MerkleTree::Format (mt).c_str ());

  Populate (mt, bits, hashes);
  printf (" wat \n");

  std::string want_hex = "597c4bafe3832b17cbbabe56f878f4fc2ad0f6a402cee7fa851a9cb205f87ed1";

  bytearray rootbin;
  hex::decode (rootbin, want_hex);

  digest32 wantroot;
  copy_BE (wantroot, rootbin);

  // get hash
  digest32 roothash; 
  PR_CHECK("root hash calc'd", MerkleTree::Node::Hash (roothash, mt, MerkleTree::Node::Root())); 
  PR_CHECK("root hash eql",  roothash == wantroot); 

  
  //   def test_populate_tree_1(self):
  //       hex_hashes = [
  //           "9745f7173ef14ee4155722d1cbf13304339fd00d900b759c6f9d58579b5765fb",
  //           "5573c8ede34936c29cdfdfe743f7f5fdfbd4f54ba0705259e62f39917065cb9b",
  //           "82a02ecbb6623b4274dfcab82b336dc017a27136e08521091e443e62582e8f05",
  //           "507ccae5ed9b340363a0e6d765af148be9cb1c8766ccc922f83e4ae681658308",
  //           "a7a4aec28e7162e1e9ef33dfa30f0bc0526e6cf4b11a576f6c5de58593898330",
  //           "bb6267664bd833fd9fc82582853ab144fece26b7a8a5bf328f8a059445b59add",
  //           "ea6d7ac1ee77fbacee58fc717b990c4fcccf1b19af43103c090f601677fd8836",
  //           "457743861de496c429912558a106b810b0507975a49773228aa788df40730d41",
  //           "7688029288efc9e9a0011c960a6ed9e5466581abf3e3a6c26ee317461add619a",
  //           "b1ae7f15836cb2286cdd4e2c37bf9bb7da0a2846d06867a429f654b2e7f383c9",
  //           "9b74f89fa3f93e71ff2c241f32945d877281a6a50a6bf94adac002980aafe5ab",
  //           "b3a92b5b255019bdaf754875633c2de9fec2ab03e6b8ce669d07cb5b18804638",
  //           "b5c0b915312b9bdaedd2b86aa2d0f8feffc73a2d37668fd9010179261e25e263",
  //           "c9d52c5cb1e557b92c84c52e7c4bfbce859408bedffc8a5560fd6e35e10b8800",
  //           "c555bc5fc3bc096df0a0c9532f07640bfb76bfe4fc1ace214b8b228a1297a4c2",
  //           "f9dbfafc3af3400954975da24eb325e326960a25b87fffe23eef3e7ed2fb610e",
  //       ]
  //       tree = MerkleTree(len(hex_hashes))
  //       hashes = [bytes.fromhex(h) for h in hex_hashes]
  //       tree.populate_tree([1] * 31, hashes)
  //       root = '597c4bafe3832b17cbbabe56f878f4fc2ad0f6a402cee7fa851a9cb205f87ed1'
  //       self.assertEqual(tree.root().hex(), root)
  // return 0;

  return 0; 
}

//
int test_merkle_tree_populate_2(const std::vector<std::string> &args) {

  const char*  hex_hashes[] = {
    "42f6f52f17620653dcc909e58bb352e0bd4bd1381e2955d19c00959a22122b2e",
    "94c3af34b9667bf787e1c6a0a009201589755d01d02fe2877cc69b929d2418d4",
    "959428d7c48113cb9149d0566bde3d46e98cf028053c522b8fa8f735241aa953",
    "a9f27b99d5d108dede755710d4a1ffa2c74af70b4ca71726fa57d68454e609a2",
    "62af110031e29de1efcad103b3ad4bec7bdcf6cb9c9f4afdd586981795516577",
  }; 

  const size_t num_hashes = sizeof(hex_hashes) / sizeof(char*); 

  merkletree mt;
  Init (mt, num_hashes);

  bytearray flag_bits (11, byte{1}); 
  hasharray hashes    (num_hashes);
  //
  for (int32 i = 0; i < num_hashes; ++i) {
    bytearray bytes;
    std::string hexs = hex_hashes[i];

    printf ("   hexs(%i):%s\n", i, hexs.c_str()); 
    hex::decode (bytes, hexs); 
  
    copy_BE (hashes[i], hex::decode (bytes, hexs));
  }

  Populate (mt, flag_bits, hashes);
  
  const std::string want_root_hex = "a8e8bd023169b81bc56854137a135b97ef47a6a7237f4c6e037baed16285a5ab"; 
  bytearray want_root_hash;
  hex::decode (want_root_hash, want_root_hex); 

  digest32 want_hash; 
  copy_BE(want_hash, want_root_hash); 
  

  digest32 root_hash; 
  PR_CHECK("root hash calc'd", MerkleTree::Node::Hash (root_hash, mt, MerkleTree::Node::Root())); 
  PR_CHECK("pop2 root eql", want_hash == root_hash); 






  return 0;   
}

//

//
int test_merkle_block_parse (const std::vector<std::string> &args) {

    // def test_parse(self):
    //     hex_merkle_block = '00000020df3b053dc46f162a9b00c7f0d5124e2676d47bbe7c5d0793a500000000000000ef445fef2ed495c275892206ca533e7411907971013ab83e3b47bd0d692d14d4dc7c835b67d8001ac157e670bf0d00000aba412a0d1480e370173072c9562becffe87aa661c1e4a6dbc305d38ec5dc088a7cf92e6458aca7b32edae818f9c2c98c37e06bf72ae0ce80649a38655ee1e27d34d9421d940b16732f24b94023e9d572a7f9ab8023434a4feb532d2adfc8c2c2158785d1bd04eb99df2e86c54bc13e139862897217400def5d72c280222c4cbaee7261831e1550dbb8fa82853e9fe506fc5fda3f7b919d8fe74b6282f92763cef8e625f977af7c8619c32a369b832bc2d051ecd9c73c51e76370ceabd4f25097c256597fa898d404ed53425de608ac6bfe426f6e2bb457f1c554866eb69dcb8d6bf6f880e9a59b3cd053e6c7060eeacaacf4dac6697dac20e4bd3f38a2ea2543d1ab7953e3430790a9f81e1c67f5b58c825acf46bd02848384eebe9af917274cdfbb1a28a5d58a23a17977def0de10d644258d9c54f886d47d293a411cb6226103b55635'
    //     mb = MerkleBlock.parse(BytesIO(bytes.fromhex(hex_merkle_block)))
    //     version = 0x20000000
    //     self.assertEqual(mb.version, version)
    //     merkle_root_hex = 'ef445fef2ed495c275892206ca533e7411907971013ab83e3b47bd0d692d14d4'
    //     merkle_root = bytes.fromhex(merkle_root_hex)[::-1]
    //     self.assertEqual(mb.merkle_root, merkle_root)
    //     prev_block_hex = 'df3b053dc46f162a9b00c7f0d5124e2676d47bbe7c5d0793a500000000000000'
    //     prev_block = bytes.fromhex(prev_block_hex)[::-1]
    //     self.assertEqual(mb.prev_block, prev_block)
    //     timestamp = little_endian_to_int(bytes.fromhex('dc7c835b'))
    //     self.assertEqual(mb.timestamp, timestamp)
    //     bits = bytes.fromhex('67d8001a')
    //     self.assertEqual(mb.bits, bits)
    //     nonce = bytes.fromhex('c157e670')
    //     self.assertEqual(mb.nonce, nonce)
    //     total = little_endian_to_int(bytes.fromhex('bf0d0000'))
    //     self.assertEqual(mb.total, total)
    //     hex_hashes = [
    //         'ba412a0d1480e370173072c9562becffe87aa661c1e4a6dbc305d38ec5dc088a',
    //         '7cf92e6458aca7b32edae818f9c2c98c37e06bf72ae0ce80649a38655ee1e27d',
    //         '34d9421d940b16732f24b94023e9d572a7f9ab8023434a4feb532d2adfc8c2c2',
    //         '158785d1bd04eb99df2e86c54bc13e139862897217400def5d72c280222c4cba',
    //         'ee7261831e1550dbb8fa82853e9fe506fc5fda3f7b919d8fe74b6282f92763ce',
    //         'f8e625f977af7c8619c32a369b832bc2d051ecd9c73c51e76370ceabd4f25097',
    //         'c256597fa898d404ed53425de608ac6bfe426f6e2bb457f1c554866eb69dcb8d',
    //         '6bf6f880e9a59b3cd053e6c7060eeacaacf4dac6697dac20e4bd3f38a2ea2543',
    //         'd1ab7953e3430790a9f81e1c67f5b58c825acf46bd02848384eebe9af917274c',
    //         'dfbb1a28a5d58a23a17977def0de10d644258d9c54f886d47d293a411cb62261',
    //     ]
    //     hashes = [bytes.fromhex(h)[::-1] for h in hex_hashes]
    //     self.assertEqual(mb.hashes, hashes)
    //     flags = bytes.fromhex('b55635')
    //     self.assertEqual(mb.flags, flags)

  
  return 0;
}

//
//
int test_valid_merkle_block (const std::vector<std::string> &args) {

  // hex_merkle_block =
  // '00000020df3b053dc46f162a9b00c7f0d5124e2676d47bbe7c5d0793a500000000000000ef445fef2ed495c275892206ca533e7411907971013ab83e3b47bd0d692d14d4dc7c835b67d8001ac157e670bf0d00000aba412a0d1480e370173072c9562becffe87aa661c1e4a6dbc305d38ec5dc088a7cf92e6458aca7b32edae818f9c2c98c37e06bf72ae0ce80649a38655ee1e27d34d9421d940b16732f24b94023e9d572a7f9ab8023434a4feb532d2adfc8c2c2158785d1bd04eb99df2e86c54bc13e139862897217400def5d72c280222c4cbaee7261831e1550dbb8fa82853e9fe506fc5fda3f7b919d8fe74b6282f92763cef8e625f977af7c8619c32a369b832bc2d051ecd9c73c51e76370ceabd4f25097c256597fa898d404ed53425de608ac6bfe426f6e2bb457f1c554866eb69dcb8d6bf6f880e9a59b3cd053e6c7060eeacaacf4dac6697dac20e4bd3f38a2ea2543d1ab7953e3430790a9f81e1c67f5b58c825acf46bd02848384eebe9af917274cdfbb1a28a5d58a23a17977def0de10d644258d9c54f886d47d293a411cb6226103b55635'
  // mb = MerkleBlock.parse(BytesIO(bytes.fromhex(hex_merkle_block)))
  // self.assertTrue(mb.is_valid())

  return 0;
}

//
//
int test_merkle_tree_structure (const std::vector<std::string> &args) {
  
  FN_SCOPE (); 
  // for (int i = 1; i < 1024; ++i) {
  //   int x = std::log2 ( i ); 
  //   printf ( "   log2(%i) = %i \n", i, x); 
  // }
  {
    int32 total = 9;
    int32 max_depth = std::ceil(std::log2(total));

    for (int32 ind = 1; ind < 20; ++ind) {
      int32 depth = std::log2(ind);
      int32 num_nodes = std::ceil(total / std::pow(2, max_depth - depth));
      printf(" ind(%i), max_depth(%i), curdepth(%i), num_nodes(%i)\n", ind, max_depth, depth, num_nodes);
    }
  }

  merkletree mt;
  Init (mt, 9);
  printf ( "    merkle:\n%s\n",   MerkleTree::Format (mt).c_str ());

  // for (int32 ind = 1; ind < 30; ++ind) { 
  //   printf ( " RightChildExists (%i) = %s\n", ind, (MerkleTree::Node::RightChildExists (mt, ind) ? "TRUE" : "FALSE")); 
  // }
  
  return 0; 
}

int test_init (const std::vector<std::string> &args) {
  FN_SCOPE ();

  merkletree mt;
  Init (mt, 9);
  printf ( "    merkle:\n%s\n",   MerkleTree::Format (mt).c_str ());

  PR_CHECK ("num_nodes(0) == 1", MerkleTree::Node::num_nodes (mt, 0) == 1) ; 
  PR_CHECK ("num_nodes(1) == 2", MerkleTree::Node::num_nodes (mt, 1) == 2) ; 
  PR_CHECK ("num_nodes(2) == 3", MerkleTree::Node::num_nodes (mt, 2) == 3) ; 
  PR_CHECK ("num_nodes(3) == 5", MerkleTree::Node::num_nodes (mt, 3) == 5) ; 
  PR_CHECK ("num_nodes(4) == 9", MerkleTree::Node::num_nodes (mt, 4) == 9) ; 
  return 0;
}    
//
int test_merkle (const std::vector<std::string> &args) {

  test_merkle_tree_structure (args) ;
  
  test_init (args) ;
  test_merkle_tree_populate_1(args) ;
  test_merkle_tree_populate_2(args) ;
  test_merkle_block_parse    (args) ;
  test_valid_merkle_block    (args) ;
  return 0; 
}
