
#ifndef AF_UTILITY_INCLUDE
#define AF_UTILITY_INCLUDE


#include "common.h"



#define CODE_ME(){printf("CODE_ME:%s|ln:%i\n", __FUNCTION__, __LINE__);} 


namespace af {
  
  typedef std::vector<unsigned char>         bytearray; 

  typedef std::array<unsigned char, 32>      byte32; // 
  typedef std::array<unsigned char, 20>      byte20; // mainly for ripemd160
  
  typedef std::map<std::string, std::string> propmap; 
 
  //
  //
  struct destructor {
    
    virtual ~destructor () = 0; 

  protected:
    destructor () {}
  }; 
 
  //
  //
  inline void checkres (const std::string& label, bool cond) {

    printf ("%s:%s\n", label.c_str(), (cond ? "TRUE" : "FALSE")); 
      
  }


  namespace hex {

    // uc -> hex
    std::string from_uc (unsigned char c); 

    // hex -> uc
    unsigned char to_uc (const std::string& chars); 
    // 
    inline unsigned char octet (const std::string& str) {
      return to_uc(str);
    }


    std::string& encode (std::string& out, const void* bin_BE, size_t len, bool BE_true);
    void*        decode (void* outBE, size_t len, const std::string& str, bool BE_true);
    

  }

  namespace base58 {
    std::string& encode (std::string& out, const void* inBE, size_t len, bool BE_true);
    void*        decode (void* outBE, size_t olen,  const std::string& in, bool BE_true ); 

  }
  
} // af


#endif
