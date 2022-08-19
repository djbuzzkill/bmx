
// -----------------------------------------------------------
// 
// -----------------------------------------------------------
#include "transaction.h"
#include "curl/curl.h"
#include "aframe/binary_IO.h"
#include "ffm/ffm.h"


using namespace af;
//
//
//

namespace {
  //
  // 
  size_t read_varint (size_t& out, af::ReadStreamRef rs, const char* trace = 0) {

    if      (trace)
      printf ( "from:%s\n", trace); 
    
    size_t readlen = 0; 
    unsigned char leaderbyte = 0;
    
    readlen += rs->Read (&leaderbyte,  1); 

    printf ("%s:leaderbyte:%i\n", __FUNCTION__, leaderbyte);
    
    switch (leaderbyte)  {
    case 253: readlen += rs->Read (&out, 2); break;
    case 254: readlen += rs->Read (&out, 4); break;
    case 255: readlen += rs->Read (&out, 8); break;
    default: out = leaderbyte; break;
    }
    
    return readlen; 
    
  }
  
  
  //
  size_t write_varint (af::WriteStreamRef ws, size_t v) {
    
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

  // --------------------------------------------------------------
  //
  // --------------------------------------------------------------
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

} // anon





// --------------------------------------------------------------
// 
// --------------------------------------------------------------
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
// reading 
//

// --------------------------------------------------------------
   //
// --------------------------------------------------------------
size_t read_and_parse_txin (curv::TxIn& txin, af::ReadStreamRef rs) {

  size_t readlen = 0;
  
  size_t script_size = 0; 

  readlen += rs->Read (&txin.prev_txid[0], 32);  
  readlen += rs->Read (&txin.prev_index, 4);

  readlen += read_varint (script_size, rs, "ln188");
  txin.script_sig.resize (script_size); 
  readlen += rs->Read (&txin.script_sig[0], script_size);
  readlen += rs->Read (&txin.sequence, sizeof (unsigned int)); 

  return readlen;
}


// --------------------------------------------------------------
//
// --------------------------------------------------------------
size_t read_and_parse_txout (curv::TxOut& txout, af::ReadStreamRef rs) {
  using namespace curv;
  size_t readlen     = 0;
  size_t script_size = 0;
  
  readlen += rs->Read (&txout.amount, 8);     // amount
  readlen += read_varint (script_size, rs, "ln250"); // varint :sizeof(ScriptKey)
  
  printf ("script_size:%zu\n", script_size); 
  
  txout.script_bin.resize (script_size);   
  readlen += rs->Read (&txout.script_bin[0], script_size); // ScriptKey

  return readlen; 
}




// --------------------------------------------------------------
//
// --------------------------------------------------------------
size_t curv::ReadTransaction (Transaction& tx, af::ReadStreamRef rs) {

  size_t readlen     = 0; 
  size_t num_inputs  = 0; 
  size_t num_outputs = 0; 

  // version
  readlen += rs->Read (&tx.version, sizeof(unsigned int));

  // inputs
  readlen += read_varint (num_inputs, rs, "num_inputs");
  tx.inputs.resize (num_inputs);
  for (auto i = 0; i < num_inputs; ++i) 
    readlen +=  read_and_parse_txin (tx.inputs[i], rs); 
  
  // outputs
  readlen += read_varint (num_outputs, rs, "num_outputs");
  tx.outputs.resize (num_outputs);
  for (auto i = 0; i < num_outputs; ++i) 
    readlen += read_and_parse_txout (tx.outputs[i], rs);

  printf ("ln250\n"); 

  // locktime
  readlen += rs->Read (&tx.locktime, sizeof(unsigned int)); 
  printf ("locktime:%i\n", tx.locktime); 
  //
  return readlen; 
} 


// 
// writing 
//

// --------------------------------------------------------------
//
// --------------------------------------------------------------
size_t write_tx_inputs (curv::TxInputs& tx, af::ReadStreamRef ws) {
  // CODE_ME ();
  return 0 ;
}

//
//
size_t write_tx_outputs (curv::TxOutputs& tx, af::WriteStreamRef ws) {

  size_t writelen = 0;
  //
  for (auto i = 0; i < tx.size(); ++i) {

    curv::TxOut& out = tx[i]; 
    writelen += ws->Write    (&out.amount, sizeof(size_t)); 
    writelen += write_varint (ws, out.script_bin.size());
    writelen += ws->Write    (&out.script_bin[0], out.script_bin.size());

  }

  return writelen; 
} 


//
size_t curv::WriteTransaction (af::WriteStreamRef ws, const Transaction& out) {
  write_tx_inputs;  
    
  write_tx_outputs;
  return 0; 
}
