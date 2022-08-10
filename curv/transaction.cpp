
#include "transaction.h"
#include "curl/curl.h"


using namespace af;



//
namespace curv { 

  //
  size_t  curl_write_cb(void* buffer, size_t size, size_t nmemb, void *userp) {

    af::write_stream* ws = reinterpret_cast<af::write_stream*> (userp);
    //char* cp = reinterpret_cast<char*> (buffer); 
    // std::string printstr (cp, cp + nmemb); 
    // printf ("\n%s\n", printstr.c_str()); 
    return ws->Write(buffer, nmemb); 
  }

  //
  std::string tx_lookup_uri (const std::string& txid, bool mainnet) {
    
    std::string ret = "https://";

    ret += mainnet ? "blockstream.info/api/tx/" : "blockstream.info/testnet/api/tx/";
    ret += txid;
    ret += "/hex"; 

    return ret; 
  }
}


//
//
bool curv::FetchTx (bytearray& out, bool mainnet, const std::string& txid_hex, TxMap& cache) {

  if (!cache.count (txid_hex)) {
    
    const size_t   buffsize = 8 * 1024; 
    bytearray      getbytes (buffsize);
    WriteStreamRef ws       = CreateWriteMemStream (&getbytes[0], buffsize);

    CURL* curl = curl_easy_init (); 
      
    std::string url = tx_lookup_uri (txid_hex, false);
    // printf ("url:%s\n", url.c_str ());
    curl_easy_setopt (curl, CURLOPT_URL,  url.c_str());
    curl_easy_setopt (curl, CURLOPT_HTTPGET, 1);
    curl_easy_setopt (curl, CURLOPT_WRITEFUNCTION, curl_write_cb);
    curl_easy_setopt (curl, CURLOPT_WRITEDATA, ws.get());

    CURLcode res  = curl_easy_perform (curl);

    if (size_t numbytes = ws->GetPos ()) {
      
      af::bytearray& val = cache[txid_hex];
      val.resize (numbytes); 
      std::copy (&getbytes[0], &getbytes[numbytes], val.begin()); 
    }
    
    curl_easy_cleanup (curl);
  }

  // 
  if (cache.count (txid_hex)) {

    const bytearray& val = cache[txid_hex]; 
    out.resize (val.size ()); 
    std::copy (val.begin (), val.end (), out.begin ()); 
    
    return true; 
  }

  return false; 
}


//
//
curv::TxFetcher::TxFetcher () : cache () {
}

//
curv::TxFetcher::~TxFetcher ()  {
}


//
bool curv::TxFetcher::Fetch (bytearray& out, const std::string& txid_hex, bool mainnet) {
  return FetchTx (out, mainnet, txid_hex, cache); 
}

//
curv::Transaction& curv::ReadTransaction (Transaction& out, af::ReadStreamRef rs) {
  
  size_t readlen = 0; 
 
  return out; 
} 
