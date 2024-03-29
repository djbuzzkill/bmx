


int test_merkle_tree_populate_1(const std::vector<std::string>& args) ;
int test_merkle_tree_populate_2(const std::vector<std::string>& args) ;
int test_merkle_block_parse    (const std::vector<std::string>& args) ;
int test_valid_merkle_block    (const std::vector<std::string>& args) ;

int test_headers_message_read (const std::vector<std::string>& args) ;
int test_getheaders_message_write (const std::vector<std::string>& args);
int test_ping_message_parse (const std::vector<std::string>& args);
int test_pong_message_parse(const std::vector<std::string> &args);
int test_merkleblock_message_parse (const std::vector<std::string>& args);
//
//
int CH11_Ex (const std::vector<std::string>& args) {
  FN_SCOPE ();

  test_merkle_tree_populate_1(args) ;
  test_merkle_tree_populate_2(args) ;
  test_merkle_block_parse    (args) ;
  test_valid_merkle_block    (args) ;

  test_headers_message_read  (args) ;
  test_getheaders_message_write(args);
  test_ping_message_parse    (args);
  test_pong_message_parse    (args);
  test_merkleblock_message_parse(args);

  return 0;
}
