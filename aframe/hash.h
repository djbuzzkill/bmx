//
//
#ifndef AF_HASH_H
#define AF_HASH_H

#include "common.h"
#include "shared_types.h"

namespace af {

  // ------------------------------------------------------------
  typedef byte32 digest32;
  typedef byte20 digest20;
  
  // ------------------------------------------------------------
  digest32& sha256    (digest32& out, const void* in, size_t lin);
  digest32& hash256   (digest32& out, const void* in, size_t lin);
  
  // ------------------------------------------------------------
  digest20& ripemd160 (digest20& out, const void* in, size_t lin);
  digest20& hash160   (digest20& out, const void* in, size_t lin);
  

}

#endif
