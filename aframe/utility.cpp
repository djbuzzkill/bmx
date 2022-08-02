#include "utility.h"

#include "ffm/ffm.h"


namespace af {

  namespace hex {


    // 
    //  u8 value into hex 'XX' 
    std::string from_uc (unsigned char c) {
      
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
    unsigned char to_uc (const std::string& chars) {
      
      std::map<char, unsigned char> hex_enc = {
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
      
      if (chars.size () != 2) {
	return 0; 
      }
      
      if (0 == hex_enc.count (chars[hi])) {
	return 0;
      }
      
      if (0 == hex_enc.count (chars[lo])) {
	return 0;
      }
      
      unsigned char out = hex_enc[chars[hi]] << 4; 
      out += hex_enc[chars[lo]];
      return out; 
      
    }


  } // str

  namespace base58
  {
    // 
    using namespace ffm;

    //
    const char base58_enc[] =
      "123456789ABCDEFGHJKLMNPQRSTUVWXYZabcdefghijkmnopqrstuvwxyz"; 

    // 58 + 1 
    static_assert (sizeof(base58_enc) == 59, "base58 alpabet is 58 bytes long"); 

  }

} // af




    // turn  binary number into string
std::string& af::base58::encode (std::string& out, const void* inBE, size_t len)  {
  
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

// base58 string into BE binary
void* af::base58::decode (void* outBE, size_t olen,  const std::string& ) {
  
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
std::string& af::hex::encode (std::string& out, const af::bytearray& bin_in ) {

  out = "";
 
  //      const unsigned char* ucbin = reinterpret_cast<const unsigned char*>(bin_in); 
  
  
  for (auto c : bin_in) {
    out += from_uc ( c );
  }
  
  
  return out;
}

