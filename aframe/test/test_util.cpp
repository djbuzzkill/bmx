


  
int test_base58_decode (const std::vector<std::string>& args) {
  FN_SCOPE (); 
  std::string test_str_hex = "3db68a2171756cfb0c7af980ac8780b4b5c892412f50cd8c4808182c7408aeb8";

  printf ("   A) test input:[%s]\n", test_str_hex.c_str());

  bytearray testbin;
  hex::decode (testbin, test_str_hex);

  std::string enc58; 
  base58::encode_checksum (enc58, &testbin[0], testbin.size());
  printf ("   B) encoded bas58[%zu|%s]\n", enc58.size(), enc58.c_str()); 
  
  bytearray resbin, res2;

  std::string test2_enc = "mmGVDymUuKBqF25GJh791MpoBGqqw7JGjR"; 
  base58::decode (res2, test2_enc );
  printf (".............res2.size[%zu]\n", res2.size()); 
  
  base58::decode (resbin, enc58);

  std::string reshex;
  hex::encode(reshex, &resbin[0], resbin.size());
  printf("   C) decoded res[%s]\n", reshex.c_str());

  return 0; 
}



int Ex_4_4 (const std::vector<std::string>& args) {

  std::string hex1 = "7c076ff316692a3d7eb3c3bb0f8b1488cf72e1afcd929e29307032997a838a3d"; 
  std::string hex2 = "eff69ef2b1bd93a66ed5219add4fb51e11a840f404876325a1e8ffe0529a2c";
  std::string hex3 = "c7207fee197d27c618aea621406f6bf5ef6fca38681d82b2f06fddbdce6feab6"; 

  bytearray dec1; 
  bytearray dec2; 
  bytearray dec3; 
  hex::decode (dec1, hex1);  
  hex::decode (dec2, hex2);  
  hex::decode (dec3, hex3);


  std::string enc1, enc2, enc3; 
  base58::encode (enc1, &dec1[0], dec1.size());  
  base58::encode (enc2, &dec2[0], dec2.size());  
  base58::encode (enc3, &dec3[0], dec3.size());  

  printf ("[%zu|%s]\n[%zu|%s]\n[%zu|%s]\n ", enc1.size(), enc1.c_str(), enc2.size(), enc2.c_str(), enc3.size(), enc3.c_str() ); 
  
  
  return 0; 
  }

