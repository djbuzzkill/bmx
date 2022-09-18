
#ifndef AF_UTILITY_INCLUDE
#define AF_UTILITY_INCLUDE

#include "common.h"
#include "shared_types.h"
#include "binary_IO.h"

#define CODE_ME(){printf("CODE_ME:%s|ln:%i\n", __FUNCTION__, __LINE__);} 
#define POUT(s) {printf("%s|ln:%i\n", (s), __LINE__);}
#define PR(s) printf("%s", (s));

#define PR_CHECK(m,c) if((c)){printf("SUCCEEDED >> %s\n", m);}else{printf("!! %s << FAILED\n", m);}
#define FN_SCOPE() fnscope ____FNSCOPE(__FUNCTION__); 

namespace af
{
  // 
  typedef std::map<std::string, std::string> propmap; 

  //
  inline void checkres (const std::string& label, bool cond) {

    printf ("%s:%s\n", label.c_str(), (cond ? "TRUE" : "FALSE")); 
 
  }


  //
  //
  template<typename Seq> 
  inline void pr_a(const Seq& a, const char* msg = 0) {
    printf ("%s -> ", (msg ? msg : __FUNCTION__));

    for (auto e : a) printf ("%02x", e);

    printf ("\n "); 
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

    printf ("%s -> lhs(%zu), rhs(%zu) \n", __FUNCTION__, lhs.size(), rhs.size());

    if (lhs.size () != rhs.size()) {
      return false;
    }


    for (auto i = 0; i < lhs.size(); ++i) {

      if (lhs[i] != rhs[i]) {
	printf ("%s -> lhs[%zu]:%x, rhs[%zu]:%x\n", __FUNCTION__, i, lhs[i], i , rhs[i]); 	
	return false;
      }

    }

    return true;
  }


  //
  //
  template<typename Seq>
  inline void print_bytes (const Seq& seq) {
    for (auto e : seq)   
      printf ("%x ", e);  
  }







  //
  //
  template<typename Seq>
  inline std::string  fmthex(const Seq& seq) {

    uint8 stride = 3;
    
    std::vector<char> buf (stride * seq.size(), ' ');
    size_t counter = 0;
    bool firsttime = true;
    for (auto e : seq ) {

      if (firsttime) 
	sprintf (&buf[counter], "%02x", e);
      else
	sprintf (&buf[counter], " %02x", e);


      if (firsttime) { 
	counter += stride - 1;
	firsttime = false;
      }
      else {
	counter += stride;
      }
	
      
    }

    return std::string (&buf[0]); 
  }    
  
  //
  // print scope 
  class fnscope {

    const char*        f;
    
  public: 
    fnscope (const char* fn) :f(fn) {
      printf (" <ENTER:%s> \n" , f ); 
    }
    
    ~fnscope () {
      printf (" <EXIT:%s>\n" , f ); 
    } 
    
  };

  
  //
  // hexadecimal --------------------------- 
  namespace hex
  {

    //
    // uc -> hex 
    std::string from_uc (unsigned char c);

    //
    // byte -> string
    std::string  from_byte (byte b); 

    //
    // hex -> uc
    unsigned char to_uc (const std::string& chars);


    inline
    uint8 to_u8 (const std::string& chars) { return to_uc(chars);}

    //
    // string -> byte
    byte  to_byte (const std::string& hexv); 


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
