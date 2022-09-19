// 
#include "hash.h"
#include "gcrypt.h"








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









//
af::digest32& af::sha256 (digest32& out, const void* in, size_t lin) {
 
  //Function: void gcry_md_hash_buffer (int algo, void *digest, const void *buffer, size_t length);
  //gcry_md_hash_buffer is a shortcut function to calculate a message digest of a buffer. This function does not require a context and immediately returns the message digest of the length bytes at buffer. digest must be allocated by the caller, large enough to hold the message digest yielded by the specified algorithm algo. This required size may be obtained by using the function gcry_md_get_algo_dlen.
  
  //Note that in contrast to gcry_md_hash_buffers this function will abort the process if an unavailable algorithm is used.
  // gcry_error_t errval =  gcry_md_open (std::data(msg), 0);   
  //auto hay = gcry_md_hash_buffer; 
  //SHA256Init (&sha_con);
  //gcry_md_hd_t crycon; 
  //auto hnd = gcry_md_open (&crycon, GCRY_MD_SHA256 , 0); 
  gcry_md_hash_buffer (GCRY_MD_SHA256, std::data(out), in, lin);
  
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

  gcry_error_t err; 

  gcry_ctx_t    ctx = nullptr;
  gcry_mac_hd_t hdl = nullptr;
  uint32 flags = 0; 
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
