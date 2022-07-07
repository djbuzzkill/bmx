#ifndef AF_HASH_H
#define AF_HASH_H

#include "common.h"
#include "utility.h"

namespace af
{



  typedef af::byte32 digest32;
  
  digest32& sha256 (digest32& out, const void* in, size_t lin);
  digest32& hash256 (digest32& out, const void* in, size_t lin);
  
}

#endif
