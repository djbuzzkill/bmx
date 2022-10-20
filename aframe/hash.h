//
//
#ifndef AF_HASH_H
#define AF_HASH_H

#include "common.h"
#include "shared_types.h"

namespace af {

  // ------------------------------------------------------------

  //
  uint32 murmur3_x86_32 (const void* dat, uint32 len, uint32 seed = 0); 

  // ------------------------------------------------------------
  digest32& sha256    (digest32& out, const void* in, size_t lin);
  digest32& hash256   (digest32& out, const void* in, size_t lin);
  
  // ------------------------------------------------------------
  digest20& ripemd160 (digest20& out, const void* in, size_t lin);
  digest20& hash160   (digest20& out, const void* in, size_t lin);

  digest32& hmac_sha256 (digest32&, const fixnum32& key, const void* txt, size_t lentxt); 

}

#endif
