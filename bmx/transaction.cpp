
// -----------------------------------------------------------
// 
// -----------------------------------------------------------
#include "transaction.h"
#include "script_types.h"
#include "script.h"
#include "secp256k1.h"
#include "utility.h"

#include "aframe/binary_IO.h"
#include "aframe/utility.h"
#include "aframe/hash.h"
#include "ffm/ffm.h"

#include "curl/curl.h"

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
  //FN_SCOPE ()
  if (!cache.count (txid_hex)) {
    
    const size_t   buffsize = 128 * 1024; 
    bytearray      getbytes (buffsize);
    WriteStreamRef ws       = CreateWriteMemStream (&getbytes[0], buffsize);

    CURL* curl = curl_easy_init (); 
      
    std::string url = tx_lookup_uri (txid_hex, mainnet);

    printf ("   getting.. %s\n", url.c_str () );
    
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
  size_t txbinlen = 0;
  if (Fetch (strhex, txid_hex, mainnet)) {
    // printf ("    --> recv: [%zu] bytes \n", strhex.size ()); // , strhex.c_str());
    //    printf ("\n--> %s:strhex:%s\n", __FUNCTION__, strhex.c_str());
    af::hex::decode (txbin, strhex);

    size_t binoffs = 0;
    // segwit check??
    if (std::to_integer<uint8>(txbin[4]) == 0) {
      txbin[5] = txbin[3];
      txbin[4] = txbin[2];
      txbin[3] = txbin[1];
      txbin[2] = txbin[0];
      txbinlen += ReadTransaction (otx, CreateReadMemStream (&txbin[2], txbin.size() - 2));
      otx.locktime = *reinterpret_cast<uint32*> (&txbin[txbin.size() - 4]); 
    }
    else {
      txbinlen += ReadTransaction (otx, CreateReadMemStream (&txbin[0], txbin.size()));
    }

    return (txbinlen == txbin.size ()); 
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
  //FN_SCOPE (); 
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
    printf (" Tx [version.%u]\n \n", tx.version);
    //return readlen; 
  }

  // inputs
  readlen += util::read_varint (num_inputs, rs, "reading num inputs"); // __FUNCTION__ );
  //printf ("[num_inputs:%zu\n", num_inputs); 
  tx.inputs.resize (num_inputs);
  for (auto i = 0; i < num_inputs; ++i) {
    //printf  ( "input no.%zu\n", i);
    readlen +=  read_and_parse_txin (tx.inputs[i], rs);
  }
  

  // outputs
  readlen += util::read_varint (num_outputs, rs, "reading num ouputs"); // __FUNCTION__);
  //printf ("[num_outputs:%zu\n", num_outputs); 
  tx.outputs.resize (num_outputs);
  for (auto i = 0; i < num_outputs; ++i) {
    //printf ( "ouput no.%zu\n", i);
    readlen += read_and_parse_txout (tx.outputs[i], rs);
  }

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
digest32& bmx::Tx::SignatureHash (digest32& osh, const Transaction& tx, size_t txind, bool on_mainnet, const command_list& redeemscript) {
  //FN_SCOPE(); 
  using namespace ffm;
  const size_t bufsize = 8 * 1024; 
  bytearray buf (bufsize); 
  WriteStreamRef ws = CreateWriteMemStream (&buf[0], bufsize); 

  uint64 writelen = 0; 
  // begin write 
  writelen += ws->Write (&tx.version, sizeof(uint32)); 
  //
  TxInputs htx_ins (tx.inputs.size()); ; 
  for (size_t i = 0;  i < tx.inputs.size (); ++i)  {
    htx_ins[i] = tx.inputs[i]; 

    if (i == txind) {
      if (redeemscript.size ())  {
	htx_ins[i].script_sig = redeemscript; 
	printf ("%s have Redeem Script\n", __FUNCTION__); 
      }
      else {
	command_list script_pubkey;
	htx_ins[i].script_sig = ScriptPubKey (script_pubkey, tx.inputs[i], on_mainnet);
      }
    }
    else {
      htx_ins[i].script_sig.clear ();
    }
  }
  // tx ins
  writelen += write_tx_inputs  (ws, htx_ins) ;
  // tx outs
  writelen += write_tx_outputs (ws, tx.outputs); 

  const uint32 hashtype = SIGHASH_ALL; 
#ifdef ARCH_BIG_ENDIAN
  swap_endian (&hash_type); // arch is BE
#endif
  writelen += ws->Write (&tx.locktime, sizeof(uint32)); 
  writelen += ws->Write (&hashtype, sizeof(uint32)); 

  printf("<wsh|writelen: %zu\n", writelen);
  //
  hash256 (osh, &buf[0], writelen);
  return osh;
}



//
bmx::command_list& bmx::ScriptPubKey (bmx::command_list& oscr, const TxIn &txin, bool mainnet) {
  //FN_SCOPE ();
  TxFetcher txfr;
  Tx::Struc prev_tx;
  std::string txid_hex;

  oscr.clear ();

  //printf ("%s -> txin.prev_index:%i", __FUNCTION__, txin.prev_index); 
    
  txfr.Fetch (prev_tx, hex::encode (txid_hex, &txin.prev_txid[0], txin.prev_txid.size ()), mainnet);
  //printf ("%s -> TxIn { prev_index:%i, prev_tx:%s } \n", __FUNCTION__, txin.prev_index, txid_hex.c_str()); 
  return append (oscr, prev_tx.outputs[txin.prev_index].script_pubkey); 
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
    printf ("<%s:THIS AMOUNT IS TOO DAMN HIGH: %zu > \n", __FUNCTION__, amount); 
  }
  //

  return prevtx.outputs[txin.prev_index].amount; 
}

//
//
int64 bmx::Tx::Fee (const bmx::Tx::Struc& tx, bool mainnet) {
  //FN_SCOPE ();

  int64 input_sum =  0;
  for (size_t i = 0; i < tx.inputs.size (); ++i) {
    input_sum += Amount (tx.inputs[i], mainnet);
  }

  int64 output_sum = 0; 
  for (size_t i =0 ; i < tx.outputs.size (); ++i) {
    output_sum +=  tx.outputs[i].amount; 
  }
    
  return input_sum - output_sum; 
}

//
//
bool bmx::Tx::VerifyInput (const Transaction& tx, size_t input_index, bool mainnet) {
  //FN_SCOPE ();
  script_env env;
  Init_secp256k1_Env (env.ffme);

  // make a copy 
  command_list script_pubkey; // txin.script_sig; // is sigpubkey now
  //printf ("<%s -> BEGIN ScriptPubKey  \n", __FUNCTION__);
  ScriptPubKey (script_pubkey, tx.inputs[input_index], mainnet);

  // BIP0016
  command_list redeem_script;
  if (script_ut::is_p2sh_script_pubkey (script_pubkey)) {
    
    const bmx::script_command& cmd = tx.inputs[input_index].script_sig.back ();

    auto buf_size = arr(cmd).size () + 10;
    bytearray scriptbytes (buf_size, byte(0));

    auto writelen = 0; 
    WriteStreamRef ws = CreateWriteMemStream (&scriptbytes[0], buf_size); 
    writelen += util::write_varint (ws, arr(cmd).size ());
    writelen += ws->Write (&arr(cmd)[0], arr(cmd).size ()); 
    printf ("   redeem_script len(%zu)\n", writelen); 
    auto readlen = ReadScript (redeem_script, CreateReadMemStream (&scriptbytes[0], writelen));
  }

  SignatureHash (env.z , tx, input_index, mainnet, redeem_script);
  
  append (append (env.cmds, tx.inputs[input_index].script_sig), script_pubkey);

  return EvalScript (env);
}

//
//
bool bmx::Tx::Verify (const Transaction& tx, bool mainnet) {
  FN_SCOPE ();
		     
  if (Tx::Fee (tx, mainnet) < 0) {
    printf (" BAD -----------------------------> fee is < 0 !!\n"); 
    return false;
  }

  for (size_t i = 0; i < tx.inputs.size (); ++i) {

    if ( !VerifyInput (tx, i, mainnet) ) {
      printf(" <<<<<<<<< %s >>>>>>>>> fail verify input (%zu)\n", __FUNCTION__, i);
      return false;
    }
  }

  return true; 
}

//
//
bool bmx::Tx::SignInput (bmx::Transaction& otx, unsigned int input_index, const bmx::PrivateKey& p, bool mainnet) {
  //FN_SCOPE ();
  digest32 zhash;
  SignatureHash (zhash, otx, input_index, mainnet) ;

  const int kBUFF_SIZE = 1024; 

  Signature sig;
  if (SECP256k1_Sign (sig, p, zhash)) {
    //printf ("\n%s + sig(r) [%s]\n", __FUNCTION__, fmthex(sig.r).c_str()); 
    //printf ("%s + sig(s) [%s]\n", __FUNCTION__, fmthex(sig.s).c_str());
    const uint8 hashtype = SIGHASH_ALL;
    //
    bytearray memDER (kBUFF_SIZE, byte{0}) ;
    WriteStreamRef ws = CreateWriteMemStream (&memDER[0], kBUFF_SIZE); 
    size_t lensig = 0;
    lensig += WriteSignature_DER(ws, sig);
    lensig += ws->Write (&hashtype, sizeof(hashtype));
    bytearray elDER (&memDER[0], &memDER[lensig]);
    //
    bytearray memSEC (kBUFF_SIZE, byte{0}) ; 
    PublicKey   pubkey;
    const bool  pubkey_compress = true; 
    size_t SEC_len = WritePublicKey_SEC (CreateWriteMemStream(&memSEC[0], kBUFF_SIZE), MakePublicKey (pubkey, p), pubkey_compress);
    bytearray elSEC (&memSEC[0], &memSEC[SEC_len]);

    // sig + sec; 
    otx.inputs[input_index].script_sig = {
      sco (elDER), sco (elSEC)
    };
    //
    return VerifyInput (otx, input_index, mainnet);
  }

  return false; 
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



