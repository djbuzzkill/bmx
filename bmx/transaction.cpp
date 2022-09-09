
// -----------------------------------------------------------
// 
// -----------------------------------------------------------
#include "transaction.h"
#include "script.h"
#include "curl/curl.h"
#include "aframe/binary_IO.h"
#include "aframe/utility.h"
#include "aframe/hash.h"
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
      
    std::string url = tx_lookup_uri (txid_hex, mainnet);

    printf ("%s from: %s\n", __FUNCTION__, url.c_str () );
    
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
//
bool bmx::TxFetcher::Fetch (Transaction &otx, const std::string &txid_hex, bool mainnet) {

  af::bytearray txbin; 
  af::bytearray txhex;
  std::string   strhex; 
  if (Fetch (txhex, txid_hex, mainnet)) {

    printf ("\n--> %s: recv > %zu bytes \n", __FUNCTION__, txhex.size ());

    strhex.assign (txhex.begin(), txhex.end ());
    // printf ("\n--> %s:strhex:%s\n", __FUNCTION__, strhex.c_str());
    af::hex::decode (txbin, strhex);
    
    size_t txlen  = ReadTransaction (otx, CreateReadMemStream (&txbin[0], txbin.size()));
    return (txlen == txbin.size ()); 
  }

  return false; 
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
  std::reverse (txin.prev_txid.begin (), txin.prev_txid.end()); 
  
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
  //printf ("%s:ENTER\n", __FUNCTION__);
  size_t readlen     = 0; 
  size_t num_inputs  = 0; 
  size_t num_outputs = 0; 

  // version
  readlen += rs->Read (&tx.version, sizeof(tx.version));

  // only ver1 for now
  if (tx.version != 0x0001) {
    printf ("%s:unsupported version.%u | ln:%i \n", __FUNCTION__, tx.version, __LINE__);
    return readlen; 
  }

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
  readlen += rs->Read (&tx.locktime, sizeof(tx.locktime)); 
  //printf ("%s:EXIT\n", __FUNCTION__);
  return readlen; 
} 


// 
// writing 
//

// --------------------------------------------------------------
//
// --------------------------------------------------------------
size_t write_tx_inputs (af::WriteStreamRef ws, const bmx::TxInputs& txis) {

  using namespace bmx;
  size_t writelen = 0;

  writelen += util::write_varint (ws, txis.size ());

  for (auto i = 0; i < txis.size(); ++i) {

    const bmx::TxIn& txi = txis[i];

    byte32 txid_LE = txi.prev_txid;
    std::reverse (txid_LE.begin(), txid_LE.end());
    writelen += ws->Write   (&txid_LE[0], 32);

    writelen += ws->Write   (&txi.prev_index, sizeof(txi.prev_index));
    writelen += WriteScript (ws, txi.script_sig);
    writelen += ws->Write   (&txi.sequence, sizeof (txi.sequence)); 
  }

  return writelen ;
}

//
//
size_t write_tx_outputs (af::WriteStreamRef ws, const bmx::TxOutputs& txos) {
  using namespace bmx;
  size_t writelen = 0;

  writelen += util::write_varint (ws, txos.size ());

  for (auto i = 0; i < txos.size(); ++i) {

    const bmx::TxOut& txo = txos[i]; 
    writelen += ws->Write (&txo.amount, sizeof(txo.amount));
    writelen += WriteScript (ws, txo.script_pubkey);
  }

  return writelen; 
} 

//
//
size_t bmx::WriteTransaction (af::WriteStreamRef ws, const Transaction& tx) {
  //printf ("%s:%i streampos (%zu) \n",   __FUNCTION__, __LINE__,   ws->GetPos ());       
  size_t writelen = 0;
  // version
  writelen += ws->Write (&tx.version, sizeof(tx.version));
  // inputs
  writelen += write_tx_inputs  (ws, tx.inputs) ;
  // outputs
  writelen += write_tx_outputs (ws, tx.outputs);
  // locktime
  writelen += ws->Write (&tx.locktime, sizeof (tx.locktime));
  //printf ("%s:%i streampos (%zu) \n",   __FUNCTION__, __LINE__,   ws->GetPos ());       
  return writelen; 
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
digest32& bmx::Tx::SignatureHash (digest32& osh, const Transaction& tx, size_t txind, bool on_mainnet) {
  FN_SCOPE("sig"); 
  
  //    printf ("%s:ENTER \n", __FUNCTION__); 

  using namespace ffm;
  
  const std::uint32_t ver      = 4;  // x86 LE
  const size_t        buf_size = 1024 * 4; 
  bytearray           workb    (buf_size); 
  
  size_t writetxlen = WriteTransaction (CreateWriteMemStream (&workb[0], buf_size) , tx );

  Transaction htx ; 
  size_t readtxlen =  ReadTransaction (htx, CreateReadMemStream (&workb[0], buf_size)); 
  
  PR ("b4 assert\n");
  assert (writetxlen == readtxlen); 
  PR ("a4 assert\n");

  
#ifdef ARCH_BIG_ENDIAN
  // make sure ver is written out LE
  // swap_endian (ver); 
#endif

  for (size_t i = 0;  i < htx.inputs.size (); ++i)  {

    TxIn& cur_txin = htx.inputs[i];

    if (i == txind) {
      PR ("(cur_txin.script_sig = script_pubkey)"); 
      bytearray txbin; 
      TxFetcher txfr;
      std::string txid_hex; 
      {
	Tx::Struc prev_tx;
	txfr.Fetch (prev_tx, hex::encode (txid_hex, &cur_txin.prev_txid[0], cur_txin.prev_txid.size ()), on_mainnet);
	// replade script_sig, replace w/script_pubkey
	cur_txin.script_sig.clear ();
	append (cur_txin.script_sig, prev_tx.outputs[cur_txin.prev_index].script_pubkey);
      }
    }
    else {
      PR ("(else clear)"); 
      cur_txin.script_sig.clear (); 
      }
  } // inputs


  const std::uint32_t hashtype = SIGHASH_ALL; 
  
#ifdef ARCH_BIG_ENDIAN
  // HASHTYPE also LE 
#endif

  WriteStreamRef wsh = CreateWriteMemStream (&workb[0], buf_size);
  
  size_t txlen_to_hash = 0; 
  txlen_to_hash += WriteTransaction (wsh, htx);
  txlen_to_hash += wsh->Write (&hashtype , sizeof(SIGHASH_ALL));

  size_t streampos = wsh->GetPos (); 
  
  // printf ("%s:EXIT \n", __FUNCTION__);
  if (txlen_to_hash != streampos) {
    printf("<wsh|streamposition: %zu\n", streampos);
    printf("<wsh|txlen_to_hash: %zu\n", txlen_to_hash);
    assert (txlen_to_hash == streampos);
  }

  return hash256 (osh, &workb[0], wsh->GetPos ());
  
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


//
// bmx::command_list& bmx::ScriptPubKey (command_list& outpubkey, const af::byte32 txid, size_t txind, bool mainnet)  {


//   outpubkey.clear (); 
//   TxFetcher fetch;
//   std::string txid_hex;
  
//   af::bytearray tx_bin; 
  
//   if (fetch.Fetch (bx_bin, hex::encode (txid_hex, &txid[0], txid.size ()), mainnet))  {
//     Transaction tx;
//     size_t readlen = ReadTransaction (tx, CreateReadMemStream (&tx_bin[0], tx_bin.size()) ); 
//     fetchassert (readlen == txid.size ());
//   }

//   return opubk; 
// }
