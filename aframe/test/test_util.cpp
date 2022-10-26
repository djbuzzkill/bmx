


  
int test_base58_decode (const std::vector<std::string>& args) {
  FN_SCOPE (); 

  std::string test_str_hex = "3db68a2171756cfb0c7af980ac8780b4b5c892412f50cd8c4808182c7408aeb8";
  //std::string test_str_hex = "mwJn1YPMq7y5F8J3LkC5Hxg9PHyZ5K4cFv"; 
  printf ("   A) test input:[%s]\n", test_str_hex.c_str());

  bytearray testbin;
  hex::decode (testbin, test_str_hex);

  std::string enc58; 
  base58::encode_checksum (enc58, &testbin[0], testbin.size());
  printf ("   B) encoded bas58[%zu|%s]\n", enc58.size(), enc58.c_str()); 
  
  bytearray resbin; 
  base58::decode (resbin, enc58);

  std::string reshex;
  hex::encode(reshex, &resbin[0], resbin.size());
  printf("   C) decoded res[%s]\n", reshex.c_str());

  return 0; 
}
