
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
#include "utility.h"


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
bool bmx::FetchTx (std::string& out, bool mainnet, const std::string& txid_hex, TxMap& cache) {

  if (!cache.count (txid_hex)) {
    
    const size_t   buffsize = 128 * 1024; 
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
    out.resize (val.size (), 0); 
    std::copy (val.begin (), val.end (), reinterpret_cast<byte*>(&out[0])); 
    
    return true; 
  }

  return false; 
}

//
bool bmx::TxFetcher::Fetch (std::string& out, const std::string& txid_hex, bool mainnet) {
  return FetchTx (out, mainnet, txid_hex, cache); 
}

//
//
bool bmx::TxFetcher::Fetch (Transaction &otx, const std::string &txid_hex, bool mainnet) {

  af::bytearray txbin; 
  std::string   strhex; 
  if (Fetch (strhex, txid_hex, mainnet)) {
    printf ("--> %s: recv: %zu bytes \n", __FUNCTION__, strhex.size ());
    printf ("\n--> %s:strhex:%s\n", __FUNCTION__, strhex.c_str());
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
  FN_SCOPE (); 
  using namespace bmx;

  size_t readlen = 0;
  
  size_t script_size = 0; 

  readlen += rs->Read (&txin.prev_txid[0], 32);
  std::reverse (txin.prev_txid.begin (), txin.prev_txid.end()); 
  
  readlen += rs->Read (&txin.prev_index, sizeof(txin.prev_index));

  //size_t script_beg = rs->GetPos();
  readlen += ReadScript (txin.script_sig, rs); 
  //size_t script_len = rs->GetPos() - script_beg; 

  readlen += rs->Read (&txin.sequence, sizeof (txin.sequence)); 

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
  //FN_SCOPE (); 
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
  printf ("num_inputs:%zu\n", num_inputs); 
  tx.inputs.resize (num_inputs);
  for (auto i = 0; i < num_inputs; ++i) 
    readlen +=  read_and_parse_txin (tx.inputs[i], rs);

  // outputs
  readlen += util::read_varint (num_outputs, rs, 0); // __FUNCTION__);
  printf ("num_outputs:%zu\n", num_outputs); 
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
  FN_SCOPE(); 
  using namespace ffm;
  
  //const std::uint32_t ver      = 4;  // x86 LE
  const size_t        buf_size = 1024 * 128; 
  bytearray           workb    (buf_size); 
  size_t writetxlen = WriteTransaction (CreateWriteMemStream (&workb[0], buf_size) , tx );
  PR ("330\n");   

  printf  ("writetxlen:%zu\n", writetxlen);

  Transaction htx ; 
  size_t readtxlen =  ReadTransaction (htx, CreateReadMemStream (&workb[0], buf_size)); 
  PR ("334\n");   
  
  if (writetxlen != readtxlen) {
    printf("..writetxlen: %zu\n", writetxlen );
    printf("..readtxlen: %zu\n", readtxlen);
    assert (writetxlen == readtxlen); 
  }

  //
  for (size_t i = 0;  i < htx.inputs.size (); ++i)  {

    TxIn& cur_txin = htx.inputs[i];

    PR ("loop\n");   

    if (i == txind) {
      ScriptPubKey (cur_txin, on_mainnet); 
    }
    else {
      cur_txin.script_sig.clear (); 
    }
  }

  const std::uint32_t hashtype = SIGHASH_ALL; 
#ifdef ARCH_BIG_ENDIAN
  // HASHTYPE also LE 
#endif


  WriteStreamRef wsh = CreateWriteMemStream (&workb[0], buf_size);
  size_t tx_for_hashing_len = 0; 
  tx_for_hashing_len += WriteTransaction (wsh, htx);
  tx_for_hashing_len += wsh->Write (&hashtype , sizeof(SIGHASH_ALL));
  PR ("\n368");   

  //
  size_t streampos = wsh->GetPos (); 
  if (tx_for_hashing_len != streampos) {
    printf("<wsh|streamposition: %zu\n", streampos);
    printf("<wsh|txlen_to_hash: %zu\n", tx_for_hashing_len);
    assert (tx_for_hashing_len == streampos);
  }


  //
  return hash256 (osh, &workb[0], wsh->GetPos ());
}

//
//
bmx::TxIn& bmx::ScriptPubKey (TxIn &txin, bool mainnet) {
  FN_SCOPE ();
  TxFetcher txfr;
  Tx::Struc prev_tx;
  std::string txid_hex; 
  txfr.Fetch (prev_tx, hex::encode (txid_hex, &txin.prev_txid[0], txin.prev_txid.size ()), mainnet);
  // replace script_sig, replace w/script_pubkey
  txin.script_sig.clear ();
  append (txin.script_sig, prev_tx.outputs[txin.prev_index].script_pubkey);
  return txin; 
}

//
//
std::uint64_t bmx::Amount (const TxIn& txin, bool mainnet) {
  FN_SCOPE ();
 
  TxFetcher txf;
  Transaction prevtx;
  std::string txid_hex; 
  txf.Fetch  (prevtx, hex::encode (txid_hex, &txin.prev_txid[0], txin.prev_txid.size() ), mainnet);

  //
  std::uint64_t amount = prevtx.outputs[txin.prev_index].amount;
  if (amount & 0xfff0000000000000) {
    printf ("<%s:this amount is too damn high: %zu > \n", __FUNCTION__, amount); 
  }
  //

  return prevtx.outputs[txin.prev_index].amount; 
}

//
//
std::int64_t bmx::Tx::Fee (const bmx::Tx::Struc& tx, bool mainnet) {
  FN_SCOPE ();

  std::int64_t input_sum =  0;
  for (size_t i = 0; i < tx.inputs.size (); ++i) {
    input_sum += Amount (tx.inputs[i], mainnet);
  }

  std::int64_t output_sum = 0; 
  for (size_t i =0 ; i < tx.outputs.size (); ++i) {
    output_sum +=  tx.outputs[i].amount; 
  }
    
  return input_sum - output_sum; 
}

//
// 
bool bmx::Tx::VerifyInput (const Transaction& tx, size_t input_index, bool mainnet) {
  FN_SCOPE ();
  // make a copy 
  TxIn txin = tx.inputs[input_index]; 

  ScriptPubKey (txin, mainnet); 
  const command_list& script_pubkey = txin.script_sig; // is sigpubkey now

  PR ("433\n"); 
  script_env env;
  Init_secp256k1_Env (env.ffme); 
  //digest32 sighash;
  PR ("437\n"); 

  SignatureHash (env.z , tx, input_index, mainnet);

  
  append (append (env.cmds, tx.inputs[input_index].script_sig), script_pubkey); 
  PR ("441\n"); 

  return EvalScript (env); 
}

//
//
bool bmx::Tx::Verify (const Transaction& tx, bool mainnet) {
  FN_SCOPE ();
		     
  if (Tx::Fee (tx, mainnet) < 0) 
    return false; 

  for (size_t i = 0; i < tx.inputs.size (); ++i)
    {
    if ( !VerifyInput (tx, i, mainnet))
      return false; 
    }
  
   return true; 
}

//
//
bool bmx::Tx::SignInput (bmx::Transaction& otx, unsigned int input_index, const bmx::PrivateKey& p, bool mainnet) {

  digest32 zhash;
  SignatureHash (zhash, otx, input_index, mainnet) ; 

  printf ("privatekey:\n");
  for ( auto uc : p )
    printf ("%x ", uc); 
  printf ("\nprivatekey:end\n");
  
  Signature sig;
  if (SECP256k1_Sign (sig, p, zhash)) {

    bytearray memDER (8 * 1024, byte{0}) ;

    WriteStreamRef ws = CreateWriteMemStream (&memDER[0], memDER.size()); 
    
    const std::uint32_t hashtype = SIGHASH_ALL;

    size_t lensig  = 0;
    unsigned char one = 1; 
    lensig += WriteSignature_DER(ws, sig);
    lensig += ws->Write (&one, 1);
    bytearray elDER (&memDER[0], &memDER[lensig]); 
    printf ("lensig:%zu\n", lensig); 
    
    
    bytearray memSEC (8 * 1024, byte{0}) ; 
    PublicKey   pubkey;
    const bool  pubkey_compress = true; 
    size_t SEC_len = WritePublicKey_SEC (CreateWriteMemStream(&memSEC[0], memSEC.size()), MakePublicKey (pubkey, p), pubkey_compress); 
    bytearray elSEC (&memSEC[0], &memSEC[SEC_len]);
    printf ("SEC_len:%zu\n", SEC_len); 
    
    otx.inputs[input_index].script_sig = {
      // sig + sec; 
      sco (elDER),
      sco (elSEC)
    };

    return VerifyInput (otx, input_index, mainnet); 
  }



    // def sign_input(self, input_index, private_key):
  //     z = self.sig_hash(input_index)
  //     der = private_key.sign(z).der()
  //     sig = der + SIGHASH_ALL.to_bytes(1, 'big')
  //     sec = private_key.point.sec()
  //     self.tx_ins[input_index].script_sig = Script([sig, sec])
  //     return self.verify_input(input_index)

  return false; 
}

