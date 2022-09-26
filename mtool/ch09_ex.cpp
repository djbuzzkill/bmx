


int test_read_block   (std::vector<std::string>& args);
int test_write_block  (std::vector<std::string>& args);
int test_bip9         (std::vector<std::string>& args);
int test_bip91        (std::vector<std::string>& args);
int test_bip141       (std::vector<std::string>& args);
int test_hash_block   (std::vector<std::string>& args);
int test_block_target (std::vector<std::string>& args);
int test_block_difficulty(std::vector<std::string>& args);
int test_check_pow    (std::vector<std::string>& args);
int test_calc_new_bits(std::vector<std::string>& args); 


//
int CH9_Ex (std::vector<std::string>& args) {

  FN_SCOPE();

  test_read_block(args);
  test_write_block(args);
  test_bip9(args);
  test_bip91(args);
  test_bip141(args);
  test_hash_block(args);
  test_block_difficulty(args);
  test_block_target(args);
  test_calc_new_bits (args) ;
  test_check_pow(args);

  return 0;
}

