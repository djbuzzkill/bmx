

int test_bloom_filter_add (const std::vector<std::string> &args) {

  bloomfilter bf;
  BloomFilter::Init (bf, 10, 5, 99);

  BloomFilter::Add (bf, af::to_bytes (std::string ("Hello World")));

  bytearray bits1 (bf.size, byte(0)); 
  bitfield_to_bytes  (bits1, bf.bitfield);
  
  std::string expected1_hex = "0000000a080000000140"; 
  bytearray expected1;
  hex::decode (expected1, expected1_hex);
  
  PR_CHECK("bitfield 1 matches", eql(bits1, expected1));

  BloomFilter::Add (bf, af::to_bytes (std::string("Goodbye!"))); 
  std::string expected2_hex = "4000600a080000010940";
  bytearray expected2;
  hex::decode (expected2, expected2_hex);

  bytearray bits2 (bf.size, byte(0)); 
  bitfield_to_bytes  (bits2, bf.bitfield);
  
  PR_CHECK("bitsfield 2 matches", eql(bits2, expected2));

  return 0; 
  
}

int test_bloom_filter_filterload (const std::vector<std::string> &args) {
  bloomfilter bf;
  BloomFilter::Init (bf, 10, 5, 99);

  BloomFilter::Add (bf, af::to_bytes (std::string ("Hello World")));
  BloomFilter::Add (bf, af::to_bytes (std::string ("Goodbye!"))); 

  bytearray bits1 (bf.size, byte(0)); 
  bitfield_to_bytes  (bits1, bf.bitfield);


  std::string expected_hex = "0a4000600a080000010940050000006300000001";

  // BloomFilter::SizeOf (bf); 
  bytearray buf (1024, byte(0)); 
  uint64 writelen_bf = BloomFilter::Write (CreateWriteMemStream(&buf[0], 1024), bf);

  std::string bfhex ; 
  hex::encode (bfhex, &buf[0], writelen_bf); 

  printf ("    bf hex[%s]\n    ex hex[%s]\n", bfhex.c_str(), expected_hex.c_str());
  
  PR_CHECK("serialize bloom filter", bfhex == expected_hex); 
  return 0; 

}

  
int test_bloom_filter (const std::vector<std::string> &args) {
  FN_SCOPE(); 
  test_bloom_filter_add (args) ;
  test_bloom_filter_filterload (args); 
  return 0; 
}
