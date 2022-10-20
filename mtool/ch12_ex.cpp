

int test_bloom_filter_add (const std::vector<std::string> &args); 
int test_getdata_serialize  (const std::vector<std::string>& args);
int test_bloom_filter_filterload (const std::vector<std::string> &args); 


int test_bloom_filter (const std::vector<std::string> &args);

//
//
int CH12_Ex (const std::vector<std::string>& args) {
  FN_SCOPE ();

  test_bloom_filter_add (args) ;
  test_bloom_filter_filterload (args); 
  test_getdata_serialize  (args); 


  return 0;
}
