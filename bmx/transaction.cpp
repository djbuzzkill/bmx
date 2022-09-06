
// -----------------------------------------------------------
// 
// -----------------------------------------------------------
#include "transaction.h"
#include "script.h"
#include "curl/curl.h"
#include "aframe/binary_IO.h"
#include "aframe/utility.h"
#include "ffm/ffm.h"
#include "secp256k1.h"


using namespace af;
//
//
//

namespace {

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
bool bmx::FetchTx (bytearray& out, bool mainnet, const std::string& txid_hex, TxMap& cache) {

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

    const af::bytearray& val = cache[txid_hex]; 
    out.resize (val.size ()); 
    std::copy (val.begin (), val.end (), out.begin ()); 
    
    return true; 
  }

  return false; 
}


//
bool bmx::TxFetcher::Fetch (af::bytearray& out, const std::string& txid_hex, bool mainnet) {
  return FetchTx (out, mainnet, txid_hex, cache); 
}


// 
// reading 
//

// --------------------------------------------------------------
   //
// --------------------------------------------------------------
size_t read_and_parse_txin (bmx::TxIn& txin, af::ReadStreamRef rs) {

  using namespace bmx;

  size_t readlen = 0;
  
  size_t script_size = 0; 

  readlen += rs->Read (&txin.prev_txid[0], 32);  
  readlen += rs->Read (&txin.prev_index, 4);

  //size_t script_beg = rs->GetPos();
  readlen += ReadScript (txin.script_sig, rs); 
  //size_t script_len = rs->GetPos() - script_beg; 

  readlen += rs->Read (&txin.sequence, sizeof (unsigned int)); 

  return readlen;
}


// --------------------------------------------------------------
//
// --------------------------------------------------------------

size_t read_and_parse_txout (bmx::TxOut& txout, af::ReadStreamRef rs) {
  using namespace bmx;

  size_t readlen     = 0;
  //size_t script_size = 0;
  readlen += rs->Read (&txout.amount, 8);     // amount
  // readlen += util::read_varint (script_size, rs, 0); // __FUNCTION__); // varint :sizeof(ScriptKey)
  // txout.script_pubkey.resize (script_size);   
  // readlen += rs->Read (&txout.script_pubkey[0], script_size); // ScriptKey
  //printf ("script_size:%zu\n", script_size); 
  readlen += ReadScript (txout.script_pubkey, rs); 

  
  return readlen; 
}




// --------------------------------------------------------------
//
// --------------------------------------------------------------
size_t bmx::ReadTransaction (Transaction& tx, af::ReadStreamRef rs) {

  size_t readlen     = 0; 
  size_t num_inputs  = 0; 
  size_t num_outputs = 0; 

  // version
  readlen += rs->Read (&tx.version, sizeof(unsigned int));

  // inputs
  readlen += util::read_varint (num_inputs, rs, 0); // __FUNCTION__ );
  tx.inputs.resize (num_inputs);
  for (auto i = 0; i < num_inputs; ++i) 
    readlen +=  read_and_parse_txin (tx.inputs[i], rs); 
  
  // outputs
  readlen += util::read_varint (num_outputs, rs, 0); // __FUNCTION__);
  tx.outputs.resize (num_outputs);
  for (auto i = 0; i < num_outputs; ++i) 
    readlen += read_and_parse_txout (tx.outputs[i], rs);


  // locktime
  readlen += rs->Read (&tx.locktime, sizeof(unsigned int)); 

  //
  return readlen; 
} 


// 
// writing 
//

// --------------------------------------------------------------
//
// --------------------------------------------------------------
size_t write_tx_inputs (bmx::TxInputs& tx, af::ReadStreamRef ws) {
  // CODE_ME ();
  return 0 ;
}

//
//
size_t write_tx_outputs (bmx::TxOutputs& tx, af::WriteStreamRef ws) {
  using namespace bmx;

  size_t writelen = 0;
  //
  for (auto i = 0; i < tx.size(); ++i) {

    bmx::TxOut& out = tx[i]; 
    writelen += ws->Write          (&out.amount, sizeof(size_t));

    writelen += WriteScript (ws, out.script_pubkey);
    // writelen += util::write_varint (ws, out.script_pubkey.size());
    // writelen += ws->Write          (&out.script_pubkey[0], out.script_pubkey.size());
  }

  return writelen; 
} 


//
size_t bmx::WriteTransaction (af::WriteStreamRef ws, const Transaction& out) {
  write_tx_inputs;  







  CODE_ME (); 



  
  write_tx_outputs;
  return 0; 
}

//
//
void bmx::print_txin (const TxIn& txin, size_t indent) {

  std::string space (indent, ' '); 
  std::string stmp;  
  printf ( "%sTxIn {\n",  space.c_str());

  hex::encode (stmp, &txin.prev_txid[0], txin.prev_txid.size()); 
  printf ("%s  prev_txid:0x%s\n",   space.c_str(), stmp.c_str());
  
  printf ("%s  prev_ind:%u\n",    space.c_str(), txin.prev_index);        // int LE

  printf ("%s  script_sig(%zu)\n", space.c_str(), txin.script_sig.size ());

  hex::encode (stmp, &txin.sequence, sizeof(txin.sequence)); 
  printf ("%s  sequence:0x%s\n",    space.c_str(), stmp.c_str());   //
  printf ("%s}\n", space.c_str());   //
}



//
//
void bmx::print_txo (const TxOut& txo, size_t indent) {

  std::string space (indent, ' '); 
  std::string stmp;  

  printf ( "%sTxOut {\n", space.c_str());
  printf ( "%s  amount:%zu\n",   space.c_str(), txo.amount);

  printf ( "%s  script_pubkey(%zu)\n", space.c_str() , txo.script_pubkey.size ()); 
      //hex::encode (stmp, &txo.script_pubkey[0], txo.script_pubkey.size()); 
  //printf ( "%s  scriptbin:0x%s\n",   space.c_str(), stmp.c_str()); 
  printf ( "%s}\n",       space.c_str());
  
}


//
//
digest32& bmx::Tx::SignatureHash (digest32& osh,  const Transaction& tx) {

  using namespace ffm;
  
  const size_t   buf_size = 1024 * 1024; 
  bytearray      workb       (buf_size, 0); 
  WriteStreamRef ws          = CreateWriteMemStream (&workb[0], buf_size);

  FFM_Env env; 
  Init_secp256k1_Env (env);
  FEConRef& F      = env.F;
  ECConRef& EC     = env.EC;
  pt::map&  points = env.pm; 
  el::map&  elems  = env.em; 

  ScopeDeleter dr (F);


  
  

  
  const unsigned int ver = 4;  // x86 LE

  
  


    // def sig_hash(self, input_index):
    //     '''Returns the integer representation of the hash that needs to get
    //     signed for index input_index'''
    //     # start the serialization with version
    //     # use int_to_little_endian in 4 bytes
    //     # add how many inputs there are using encode_varint
    //     # loop through each input using enumerate, so we have the input index
    //         # if the input index is the one we're signing
    //         # the previous tx's ScriptPubkey is the ScriptSig
    //         # Otherwise, the ScriptSig is empty
    //         # add the serialization of the input with the ScriptSig we want
    //     # add how many outputs there are using encode_varint
    //     # add the serialization of each output
    //     # add the locktime using int_to_little_endian in 4 bytes
    //     # add SIGHASH_ALL using int_to_little_endian in 4 bytes
    //     # hash256 the serialization
    //     # convert the result to an integer using int.from_bytes(x, 'big')
    //     raise NotImplementedError
  
 

  return osh; 
}


//
// 
bool bmx::Tx::VerifyInput  (const Transaction& tx) {

    // def verify_input(self, input_index):
    //     '''Returns whether the input has a valid signature'''
    //     # get the relevant input
    //     # grab the previous ScriptPubKey
    //     # get the signature hash (z)
    //     # combine the current ScriptSig and the previous ScriptPubKey
    //     # evaluate the combined script
    //     raise NotImplementedError

		     
  CODE_ME(); 
  return false; 
}

//
//
bool bmx::Tx::Verify (const Transaction& tx) {
		     
  CODE_ME(); 

  //   # tag::source2[]
  //   def verify(self):
  //       '''Verify this transaction'''
  //       if self.fee() < 0:  # <1>
  //           return False
  //       for i in range(len(self.tx_ins)):
  //           if not self.verify_input(i):  # <2>
  //               return False
  //       return True
  //   # end::source2[]

   return false; 
}

//
//
bmx::Transaction& bmx::Tx::SignInput (bmx::Transaction& otx, unsigned int index, const bmx::PrivateKey& p) {

    // def sign_input(self, input_index, private_key):
    //     # get the signature hash (z)
    //     # get der signature of z from private key
    //     # append the SIGHASH_ALL to der (use SIGHASH_ALL.to_bytes(1, 'big'))
    //     # calculate the sec
    //     # initialize a new script with [sig, sec] as the cmds
    //     # change input's script_sig to new script
    //     # return whether sig is valid using self.verify_input
    //     raise NotImplementedError

  return otx; 
}
