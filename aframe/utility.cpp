//
//s
#include "utility.h"

#include "binary_IO.h"
#include "hash.h"

#include "ffm/ffm.h"



namespace af
{
  namespace base58
  {
    
    //
    const char base58_enc[] =
      "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"; 
    
    // 58 + 1 
    static_assert (sizeof(base58_enc) == 59, "base58 alpabet is 58 bytes long"); 
    
  }
  
} // af

//const char af::base58::base58_enc_[] = "plkj"; 

// 
using namespace ffm;
using namespace af;






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

// turn  binary number into string
std::string& af::base58::encode (std::string& out, const void* inBE, size_t len) {
  
  const char secp256k1_p_sz[] = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f";
  
  ffm::FEConRef F = ffm::Create_FE_context (secp256k1_p_sz);
  ffm::ScopeDeleter dr (F); 
  // def encode_base58(s):
  //     count = 0
  //     for c in s:  # <1>
  //         if c == 0:
  //             count += 1
  //         else:
  //             break
  //     num = int.from_bytes(s, 'big')
  //     prefix = '1' * count
  //     result = ''
  //     while num > 0:  # <2>
  //         num, mod = divmod(num, 58)
  //         result = BASE58_ALPHABET[mod] + result
  //     return prefix + result  # <3>
  
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




// base58 string into BE binary
void* af::base58::decode (void* outBE, size_t olen,  const std::string& ) {
  
  const char secp256k1_p_sz[] = "0xfffffffffffffffffffffffffffffffffffffffffffffffffffffffefffffc2f";
  
  ffm::FEConRef F = ffm::Create_FE_context (secp256k1_p_sz);
  ffm::ScopeDeleter dr (F); 

  CODE_ME();  
  
  unsigned char* out = reinterpret_cast<unsigned char*>(outBE);  

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
    out.push_back (to_uc ( str.substr(str_ind, 2))); 
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

