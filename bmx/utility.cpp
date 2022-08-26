


#include "utility.h"


using namespace af; 


  //
  // 
  size_t bmx::util::read_varint (size_t& out, af::ReadStreamRef rs, const char* trace) {

    if (trace)
      printf ( "%s:%s()\n", trace, __FUNCTION__); 
    
    size_t readlen = 0; 
    unsigned char leaderbyte = 0;
    
    readlen += rs->Read (&leaderbyte,  1); 

    //    printf ("%s:leaderbyte:%i\n", __FUNCTION__, leaderbyte);
    
    switch (leaderbyte)  {
    case 253: readlen += rs->Read (&out, 2); break;
    case 254: readlen += rs->Read (&out, 4); break;
    case 255: readlen += rs->Read (&out, 8); break;
    default: out = leaderbyte; break;
    }
    
    return readlen; 
    
  }
  
  
  //
size_t bmx::util::write_varint (af::WriteStreamRef ws, size_t v) {
    
    size_t writelen = 0; 
    
    if (v < 253) {
      writelen +=  ws->Write (&v, 1);
    }
    if (v < 0x10000) { // revise ranges
      const unsigned char leader = 0xfd;
      writelen += ws->Write  (&leader, 1); 
      writelen += ws->Write (&v, 2); 
      // 2 bytes
    }
    else if (v < 0x100000000) {
      const unsigned char leader = 0xfe; 
      // 4 bytes
      writelen += ws->Write (&leader, 1); 
      writelen += ws->Write (&v, 4); 
    }
    else {
      // must be big if u got this far
      const unsigned char leader = 0xff;
      writelen += ws->Write (&leader, 1); 
      writelen += ws->Write (&v, 1);
      
    }
    return writelen; 
    
  }
