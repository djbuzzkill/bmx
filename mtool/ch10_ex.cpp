

int test_read_network_envelope (const std::vector<std::string>& args);
int test_write_network_envelope (const std::vector<std::string>& args); 


//
//
int CH10_Ex (const std::vector<std::string>& args) {
  FN_SCOPE ();



  test_read_network_envelope (args);
  test_write_network_envelope (args); 
  


  return 0;
}
