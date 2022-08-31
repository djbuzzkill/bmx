
#ifndef AF_UTILITY_INCLUDE
#define AF_UTILITY_INCLUDE

#include "common.h"
#include "shared_types.h"
#include "binary_IO.h"

#define CODE_ME(){printf("CODE_ME:%s|ln:%i\n", __FUNCTION__, __LINE__);} 
#define POUT(s) {printf("%s|ln:%i\n", (s), __LINE__);}
#define PR(s) printf("%s", (s));


namespace af
{
  // 
  typedef std::map<std::string, std::string> propmap; 

  //
  inline void checkres (const std::string& label, bool cond) {

    printf ("%s:%s\n", label.c_str(), (cond ? "TRUE" : "FALSE")); 
 
  }

  //
  size_t SizeOf_file (const std::string& fq);

  //
  //
  inline size_t SizeOf_file (const std::string& fn) {
    //printf  ( "%s[%s]\n", __FUNCTION__, fn.c_str());    
    if (std::FILE* f = std::fopen (fn.c_str(), "r")) {
      //printf  ( "[f is good]\n" );    
      std::shared_ptr<std::FILE> sp (f, std::fclose); 
      if (0 == std::fseek (sp.get(), 0, SEEK_END)) 
	return  std::ftell (sp.get()); 
    }
    
    return 0; 
  }

  //
  // all the bytes
  template<typename Seq>
  inline Seq& From_file (Seq& out, const std::string& fname) {

    if (auto sizeOf_file = SizeOf_file (fname)) {

      out.resize (sizeOf_file); 

      ReadStreamRef rs = CreateReadFileStream (fname);

      if (rs) {
	rs->Read (&out[0], sizeOf_file); 
      }
    }
    return out; 
  } 
  
  //
  template<typename Ty> 
  inline bool in_bounds_incl (Ty val , Ty minval , Ty maxval) {
    return (val >= minval && val <= maxval);
  }

  //
  template<typename Ty> 
  inline bool inb_in (Ty val , Ty minval , Ty maxval) {
    return (val >= minval && val <= maxval);
  }
  // 
  template<typename Ty> 
  inline bool in_bounds_excl (Ty val, Ty minval, Ty maxval) { 
    return (val > minval && val < maxval);
  }    
     
  //
  template<typename Ty> 
  inline bool inb_ex (Ty val, Ty minval, Ty maxval) { 
    return (val > minval && val < maxval);
  }    

  //
  //
  inline bool eql (const bytearray& lhs, const bytearray& rhs) {

    if (lhs.size () != rhs.size())
      return false;

    for (auto i = 0; i < lhs.size(); ++i) {
      if (lhs[i] != rhs[i])
	return false; 
    }

    return true;
  }


  
  //
  // hexadecimal --------------------------- 
  namespace hex
  {
    //
    // uc -> hex 
    std::string from_uc (unsigned char c); 

    //
    // hex -> uc
    unsigned char to_uc (const std::string& chars); 
    // 
    inline unsigned char octet (const std::string& str) {
      return to_uc(str);
    }

    //
    //
    std::string& encode (std::string& out, const void*, size_t len);

    //
    //
    bytearray&   decode (bytearray& out_bin, const std::string& str);

  }

  // base58 --------------------------------
  namespace base58
  {
    std::string& encode (std::string& out, const void* inBE, size_t len);
    void*        decode (void* outBE, size_t olen,  const std::string& in); 


    std::string& encode_checksum (std::string& out, const void* inBE, size_t len); 
  }
  
} // af


#endif
