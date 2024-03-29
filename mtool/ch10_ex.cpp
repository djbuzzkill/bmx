

int test_read_network_envelope (const std::vector<std::string>& args);
int test_write_network_envelope (const std::vector<std::string>& args); 
int test_version_message (const std::vector<std::string>& args); 

int test_handshake  (const std::vector<std::string>& args) ; 
int test_getdata_serialize (const std::vector<std::string>& args); 

//
//
int CH10_Ex (const std::vector<std::string>& args) {
  FN_SCOPE ();

  test_read_network_envelope (args);
  test_write_network_envelope (args); 
  test_version_message (args); 
  test_handshake  (args); 
  test_getdata_serialize (args); 

  return 0;
}
