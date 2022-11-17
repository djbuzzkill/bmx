//
//s
#include "utility.h"

#include "binary_IO.h"
#include "hash.h"

#include <ffm/ffm.h>




namespace af {

  namespace base58 {
    //
    const char base58_enc[] =
      "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"; 
    
    // 58 + 1 
    static_assert (sizeof(base58_enc) == 59, "base58 alpabet is 58 bytes long"); 

    const std::map<char, uint8> char_to_int_map = {
      {'1',0 },{'2',1 },{'3', 2},{'4', 3},{'5', 4},{'6',5 },{'7', 6},{'8',7}, {'9',8}, {'A', 9}, 
      {'B',10},{'C',11},{'D',12},{'E',13},{'F',14},{'G',15},{'H',16},{'J',17},{'K',18},{'L',19}, 
      {'M',20},{'N',21},{'P',22},{'Q',23},{'R',24},{'S',25},{'T',26},{'U',27},{'V',28},{'W',29}, 
      {'X',30},{'Y',31},{'Z',32},{'a',33},{'b',34},{'c',35},{'d',36},{'e',37},{'f',38},{'g',39}, 
      {'h',40},{'i',41},{'j',42},{'k',43},{'m',44},{'n',45},{'o',46},{'p',47},{'q',48},{'r',49}, 
      {'s',50},{'t',51},{'u',52},{'v',53},{'w',54},{'x',55},{'y',56},{'z',57},
    };

    // 
    uint8 ch_2_int (char c) {
      assert (char_to_int_map.count (c)); 
      return char_to_int_map.at(c); 
    }
    
  } 	   
} // af

//const char af::base58::base58_enc_[] = "plkj"; 

// 
using namespace ffm;
using namespace af;

std::string af::hex::from_byte(byte b) {

  const std::string hexdigits = "0123456789abcdef";
  std::string ret = "";
  
  byte lo = byte{0x0f} & b;
  byte hi = b >> 4;
  
  //printf ("%s[hi:%i|lo:%i|c:%i]\n", __FUNCTION__, hi, lo, c);
  
  ret += hexdigits[std::to_integer<uint8>(hi)] ;
  ret += hexdigits[std::to_integer<uint8>(lo)] ; 
  
  return ret; 
  
  
}

// 
//  u8 value into hex 'XX' 
std::string hex::from_uc (unsigned char c) {
  
  const std::string hexdigits = "0123456789abcdef";
  std::string ret = "";
  
  unsigned char lo = 0x0f & c;
  unsigned char hi = c >> 4;
  
  //printf ("%s[hi:%i|lo:%i|c:%i]\n", __FUNCTION__, hi, lo, c);
  
  ret += hexdigits[hi] ;
  ret += hexdigits[lo] ; 
  
  return ret; 
}

// 
    //  hex digit 'XX' into u8
unsigned char af::hex::to_uc (const std::string& chars) {
  
  static std::map<char, unsigned char> hex_enc = {
    {'0', 0 },
    {'1', 1 }, 
    {'2', 2 },
    {'3', 3 }, 
    {'4', 4 },
    {'5', 5 }, 
    {'6', 6 },
    {'7', 7 }, 
    {'8', 8 },
    {'9', 9 }, 
    {'a', 10},
    {'b', 11}, 
    {'c', 12},
    {'d', 13}, 
    {'e', 14}, 
    {'f', 15}, 
    {'A', 10},
    {'B', 11}, 
    {'C', 12},
    {'D', 13}, 
    {'E', 14}, 
    {'F', 15}, 
  };  
  
  const unsigned char hi = 0;
  const unsigned char lo = 1;
  
  if (0 == hex_enc.count (chars[hi])) {
    return 0;
  }
  
  if (0 == hex_enc.count (chars[lo])) {
    return 0;
  }

  if (chars.size() < 2)
    return hex_enc[chars[lo]];

  unsigned char out = hex_enc[chars[hi]] << 4; 
  out += hex_enc[chars[lo]];
  return out;
}

byte af::hex::to_byte(const std::string& chars) {

  
  // static std::map<char, byte> hex_enc = {
  //   {'0', 0 },
  //   {'1', 1 }, 
  //   {'2', 2 },
  //   {'3', 3 }, 
  //   {'4', 4 },
  //   {'5', 5 }, 
  //   {'6', 6 },
  //   {'7', 7 }, 
  //   {'8', 8 },
  //   {'9', 9 }, 
  //   {'a', 10},
  //   {'b', 11}, 
  //   {'c', 12},
  //   {'d', 13}, 
  //   {'e', 14}, 
  //   {'f', 15}, 
  //   {'A', 10},
  //   {'B', 11}, 
  //   {'C', 12},
  //   {'D', 13}, 
  //   {'E', 14}, 
  //   {'F', 15}, 
  // };

  static std::map<char, byte> hex_encb = {
    {'0', byte{0 }},
    {'1', byte{1 }}, 
    {'2', byte{2 }},
    {'3', byte{3 }}, 
    {'4', byte{4 }},
    {'5', byte{5 }}, 
    {'6', byte{6 }},
    {'7', byte{7 }}, 
    {'8', byte{8 }},
    {'9', byte{9 }}, 
    {'a', byte{10}},
    {'b', byte{11}}, 
    {'c', byte{12}},
    {'d', byte{13}}, 
    {'e', byte{14}}, 
    {'f', byte{15}}, 
    {'A', byte{10}},
    {'B', byte{11}}, 
    {'C', byte{12}},
    {'D', byte{13}}, 
    {'E', byte{14}}, 
    {'F', byte{15}}, 
  };  
  
  const uint8 hi = 0;
  const uint8 lo = 1;
  
  if (0 == hex_encb.count (chars[hi])) {
    return byte{0};
  }
  
  if (0 == hex_encb.count (chars[lo])) {
    return byte{0};
  }

  if (chars.size() < 2)
    return hex_encb[chars[lo]];

  byte out = hex_encb[chars[hi]] << 4; 
  out |= hex_encb[chars[lo]];
  return out; 
}

// turn binary number into string
std::string& af::base58::encode (std::string& out, const void* inBE, size_t len) {
  
  const char secp256k1_p_sz[] = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f";
  
  ffm::FEConRef F = ffm::Create_FE_context (secp256k1_p_sz);
  ffm::ScopeDeleter dr (F); 
  
  size_t count = 0;  {
    const unsigned char* in = reinterpret_cast<const unsigned char*>(inBE);  
    while (in[count] == 0) count++; 
  }
  std::string prefix (count, base58_enc[0]); 
  std::string result = ""; 
  
  FE_t num = dr (F->New_bin(inBE, len, false));
  FE_t den = dr (F->New_ui(58)); 
  FE_t q   = dr (F->New());
  FE_t r   = dr (F->New()); 
  // Compare op1 and op2. Return a positive value if op1 > op2,
  // zero if op1 = op2, or a negative value if op1 < op2.
  while (F->Cmp_ui(num, 0)) {
    
    size_t rem = F->DivMod_ui (q, r, num, 58); 
    F->Set (num, q); 
    result +=  base58_enc[rem];
  }
  std::reverse (result.begin(), result.end()); 
  
  out = prefix + result; 
  return out;
  
}



//
//
std::string& base58::encode_checksum (std::string& out, const void* inBE, size_t len) {

  af::digest32 dig; 
  af::hash256 (dig, inBE, len);

  size_t         memsize  = len + 4; 
  bytearray      mem      (memsize); 
  af::WriteStreamRef ws       = CreateWriteMemStream (&mem[0], memsize); 
  size_t         writelen = 0; 

  writelen += ws->Write (inBE, len);
  writelen += ws->Write (&dig[0], 4);

  return base58::encode (out, &mem[0], writelen); 
  
}


//  should this b called decode_checksum. ?
af::bytearray &base58::decode (af::bytearray& outBE, const std::string &instr) {

  const char secp256k1_p_sz[] = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f";
  ffm::FEConRef F = ffm::Create_FE_context (secp256k1_p_sz);
  ffm::ScopeDeleter dr (F);

  FE_t n = dr(F->New_si(0));
  for (auto c : instr) {
    //printf ("      --> add char[%c| %u]\n", c, ch_2_int (c)); 
    F->SMul_ui (n, n, 58);
    F->SAdd_ui (n, n, ch_2_int (c));
  }

  int n_sign = 0;  //
  bytearray n_bin; //  
  F->Raw (n_bin, n_sign, n, false); 

  bytearray checksum_in;
  checksum_in.push_back(n_bin.back()); 
  n_bin.pop_back ();
  checksum_in.push_back(n_bin.back()); 
  n_bin.pop_back (); 
  checksum_in.push_back(n_bin.back()); 
  n_bin.pop_back (); 
  checksum_in.push_back(n_bin.back()); 
  n_bin.pop_back ();

  digest32 h256; 
  hash256(h256, &n_bin[0], n_bin.size());

  bytearray checksum_test (4); 
  std::reverse_copy (&h256[0], &h256[4], &checksum_test[0]);
  //std::copy (&h256[0], &h256[3], &checksum_test[0]);
  if (!eql (checksum_in, checksum_test)) {
    printf (" [ FAILED CHECKSUM] ---> inpput[%s] != check[%s]\n", fmthex(checksum_in).c_str(), fmthex(checksum_test).c_str()); 
    // bad checksum; 
  }

  // remove net byte, y do this? here?
  outBE.resize (n_bin.size()-1); //??
  std::copy (&n_bin[1], &n_bin[n_bin.size()], &outBE[0]);
  
  return outBE;
} 



//
// hex::decode () - turn hex string into bin string
//    
af::bytearray& af::hex::decode (af::bytearray& out, const std::string& str ) {
  
  out.clear (); 
  
  // hex strings should always be even
  if (str.size () % 2 != 0) {
    return out; 
  }
  
  size_t num_out_bytes = str.size () / 2;
  
  for (auto ich = 0; ich < num_out_bytes; ++ich) {
    
    auto str_ind = 2 * ich ;
    out.push_back (to_byte ( str.substr(str_ind, 2))); 
  }
  
  return out;
}


//
// hex::encode() - binary into hex string
//
std::string& af::hex::encode (std::string& out, const void* bin, size_t len ) {

  out = "";
 
  const unsigned char* ucbin = reinterpret_cast<const unsigned char*>(bin); 
 
    
  for (auto i = 0; i < len; ++i) {
    auto c = ucbin[i];
    
    out += from_uc ( c );
  }
  
  
  return out;
}





// TEST TEST TEST TEST TEST TEST TEST TEST 
void test_encoding_base58_match () {
  FN_SCOPE (); 
  std::string base_encoding_s = base58::base58_enc; 
  for (uint8 i = 0; i < base_encoding_s.size(); ++i) {
    assert (i == base58::char_to_int_map.at (base58::base58_enc[i])); 
  }
}
// TEST TEST TEST TEST TEST TEST TEST TEST


#include "test/test_util.cpp"
