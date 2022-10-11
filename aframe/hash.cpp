// 
#include "hash.h"
#include "gcrypt.h"

#include "utility.h"






static void local_init_gcry() {

  //If you don’t have a need for secure memory, for example if your application does not use secret keys or other confidential data or it runs in a controlled environment where key material floating around in memory is not a problem, you should initialize Libgcrypt this way:

  /* Version check should be the very first call because it
     makes sure that important subsystems are initialized.
     #define NEED_LIBGCRYPT_VERSION to the minimum required version. */

  if (!gcry_check_version (GCRYPT_VERSION)) {
    fprintf (stderr, "libgcrypt is too old (need %s, have %s)\n", GCRYPT_VERSION , gcry_check_version (NULL));
    exit (2);
  }

  /* Disable secure memory.  */
  gcry_control (GCRYCTL_DISABLE_SECMEM, 0);

  /* ... If required, other initialization goes here.  */

  /* Tell Libgcrypt that initialization has completed. */
  gcry_control (GCRYCTL_INITIALIZATION_FINISHED, 0);

  
}




af::uint32 murmur3_x86_32 (const void* dat, af::uint32 len, af::uint32 seed) {
 http://stackoverflow.com/questions/13305290/is-there-a-pure-python-implementation-of-murmurhash
  using namespace af;

  const uint32 c1 = 0xcc9e2d51; 
  const uint32 c2 = 0x1b873593; 

  const uint8* u8d = reinterpret_cast<const uint8*>(dat); 
  
  uint32 h1 = seed; 

  uint32 k1, val; 
  
  uint32 rounded_end =  (len & 0xfffffffc);   // # round down to 4 byte block

  for (uint32 i = 0; i < rounded_end; i+=4) {
    // for i in range(0, roundedEnd, 4):
    //   # little endian load order
    k1 = (u8d[i] & 0xff) | ((u8d[i + 1] & 0xff) << 8) |  ((u8d[i + 2] & 0xff) << 16) | (u8d[i + 3] << 24); 
    k1 *= c1;
    k1 = (k1 << 15) | ((k1 & 0xffffffff) >> 17); // ROTL32(k1,15); 
    k1 *= c2; 
    
    h1 ^= k1; 
    h1 = (h1 << 13) | ((h1 & 0xffffffff) >> 19); // ROTL32(h1,13);
    h1 = h1 * 5 + 0xe6546b64; 
  }


  // tail
  k1  = 0;
  val = len & 0x03;

  // if val == 3:
  if (val == 3) {
    // k1 = (ord(data[roundedEnd + 2]) & 0xff) << 16
    k1 = (u8d[rounded_end + 2] & 0xff) << 16; 
  }
  // if val in [2, 3]:
  if (val == 2 || val == 3) {
    // k1 |= (ord(data[roundedEnd + 1]) & 0xff) << 8
    k1 |= (u8d[rounded_end + 1] & 0xff) << 8; 
  }
  // if val in [1, 2, 3]:
  if (in_bounds_incl<uint32> (val, 1, 3)) {
    k1 |= u8d[rounded_end] & 0xff; 
    k1 *= c1; 
    k1 = (k1 << 15) | ((k1 & 0xffffffff) >> 17); // ROTL32(k1,15)
    k1 *= c2; 
    h1 ^= k1; 
  }
  
  // finalization
  h1 ^= len; 

  // fmix(h1)
  h1 ^= ((h1 & 0xffffffff) >> 16); 
  h1 *= 0x85ebca6b; 
  h1 ^= ((h1 & 0xffffffff) >> 13); 
  h1 *= 0xc2b2ae35; 
  h1 ^= ((h1 & 0xffffffff) >> 16); 

  return (h1 & 0xffffffff); 
}



//
af::digest32& af::sha256 (digest32& out, const void* in, size_t lin) {
  //FN_SCOPE (); 
 
  // uint32 reqlen = gcry_md_get_algo_dlen (GCRY_MD_SHA256);
  // printf ("    required length [%u]\n", reqlen); 
    
  //Function: void gcry_md_hash_buffer (int algo, void *digest, const void *buffer, size_t length);
  //gcry_md_hash_buffer is a shortcut function to calculate a message digest of a buffer. This function does not require a context and immediately returns the message digest of the length bytes at buffer. digest must be allocated by the caller, large enough to hold the message digest yielded by the specified algorithm algo. This required size may be obtained by using the function gcry_md_get_algo_dlen.
  
  //Note that in contrast to gcry_md_hash_buffers this function will abort the process if an unavailable algorithm is used.
  // gcry_error_t errval =  gcry_md_open (std::data(msg), 0);   
  //auto hay = gcry_md_hash_buffer; 
  //SHA256Init (&sha_con);
  //gcry_md_hd_t crycon; 
  //auto hnd = gcry_md_open (&crycon, GCRY_MD_SHA256 , 0); 
  gcry_md_hash_buffer (GCRY_MD_SHA256, &out, in, lin);
  
  return out;  
}

//
af::digest32& af::hash256 (digest32& out, const void* in, size_t lin) {
  
  digest32 rnd1;
  sha256 (rnd1, in, lin);
  return sha256 (out, &rnd1[0], 32); 
}

//
af::digest20& af::ripemd160 (digest20& out, const void* in, size_t lin) {
  gcry_md_hash_buffer (GCRY_MD_RMD160, &out[0], in, lin);
  return out; 
}

  //
af::digest20& af::hash160 (digest20& out, const void* in, size_t lin) {
  digest32 dig32; 
  sha256 (dig32, in, lin);
  return ripemd160 (out, &dig32[0], 32); 
}

//
//
af::digest32& af::hmac_sha256 (digest32& omac, const fixnum32& key, const void* txt, size_t lentxt) {

  const size_t  expected_mac_len = 32;

  gcry_error_t  err; 
  gcry_ctx_t    ctx   = nullptr;
  gcry_mac_hd_t hdl   = nullptr;
  uint32        flags = 0; 
  if (GPG_ERR_NO_ERROR == gcry_mac_open (&hdl, GCRY_MAC_HMAC_SHA256, flags, ctx)) {

    assert (expected_mac_len  == gcry_mac_get_algo_maclen (gcry_mac_get_algo(hdl))); 
    //printf("algo name %s\n", gcry_mac_algo_name (gcry_mac_get_algo(hdl) )); 
    err = gcry_mac_setkey(hdl, &key[0], key.size () );
    assert(err == GPG_ERR_NO_ERROR);
    //printf ("maclen:%i | keylen:%i \n", gcry_mac_get_algo_maclen (gcry_mac_get_algo(hdl)), gcry_mac_get_algo_keylen (gcry_mac_get_algo(hdl))); 
    err = gcry_mac_write  (hdl, txt, lentxt);
    assert(err == GPG_ERR_NO_ERROR);

    size_t mac_len = gcry_mac_get_algo_maclen (gcry_mac_get_algo(hdl)); 
    err = gcry_mac_read (hdl, &omac[0], &mac_len); 
    assert(err == GPG_ERR_NO_ERROR);

    gcry_mac_close(hdl);
  }
  
  return omac; 
}
