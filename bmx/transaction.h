
#ifndef BMX_TRANSACTION_H
#define BMX_TRANSACTION_H

#include "common.h"
#include "utility.h"
#include "script_types.h"

#include "aframe/shared_types.h"
#include "aframe/binary_IO.h"



// 
namespace bmx {


  // -----------------------------------------------------------------
  // 
  // Input contains script_sig's
  // -----------------------------------------------------------------
  struct TxIn { 
    af::byte32        prev_txid;   // 32b LE
    uint32            prev_index;  // int LE
    bmx::command_list script_sig;  // 
    uint32            sequence;    // 
  }; 

  command_list&   ScriptPubKey (command_list& oscr, const TxIn &inout, bool mainnet);
  std::uint64_t   Amount       (const TxIn& txin, bool mainnet); 

  typedef std::vector<TxIn> TxInputs;

  //

  // -----------------------------------------------------------------
  // 
  // output transaction pubkey in here
  // -----------------------------------------------------------------
  struct TxOut {
    uint64            amount; 
    bmx::command_list script_pubkey;
  }; 

  typedef std::vector<TxOut>  TxOutputs;

  //

  // -----------------------------------------------------------------
  //  
  // -----------------------------------------------------------------
  namespace Tx {

    struct Struc {
      //
      Struc () : version (0), inputs (), locktime (0), outputs (0) {
      }
      // 
      uint32    version;    
      TxInputs  inputs;
      TxOutputs outputs; 
      uint32    locktime; //; 
      //
    };

    Struc&       Init   (Struc& tx, uint32 ver, uint32 num_in, uint32 num_out, uint32 locktime); 
    Struc&       Input  (Struc& tx, uint32 index, const byte32& txid, uint32 prev_ind, const command_list& script_sig, uint32 sequence = 0xffffffff); 
    Struc&       Output (Struc& tx, uint32 index, uint64 amount, const command_list& script_pubkey); 

    digest32&    SignatureHash   (digest32& ohash, const Struc& tx,  size_t indx, bool mainnet, const command_list& redeemscript = command_list()); 
    bool         VerifyInput     (const Struc& tx, size_t input_index, bool mainnet); 
    int64        Fee             (const Struc& tx, bool mainnet); 
    
    bool         SignInput       (Struc& tx, unsigned int tx_ind, const PrivateKey&, bool mainnet); 
    bool         Verify          (const Struc& tx, bool mainnet); 

  }

  typedef Tx::Struc Transaction; 
  // -----------------------------------------------------------------
  // 
  // -----------------------------------------------------------------
  void          print_txin       (const TxIn& txin, size_t len); 
  void          print_txo        (const TxOut& txo, size_t len);
  
  // 
  size_t        ReadTransaction  (Transaction& out, af::ReadStreamRef rs);
  size_t        WriteTransaction (af::WriteStreamRef ws, const Transaction& tx); 

  //  command_list& ScriptPubKey (command_list& outpubkey, const af::byte32 txid, size_t txind, bool mainnet_is_true = false); 
  //command_list& ScriptPubKey (command_list& outpubkey, const Transaction& tx, size_t txind); 

  // -----------------------------------------------------------------
  // 
  // -----------------------------------------------------------------
  typedef std::map<std::string, af::bytearray> TxMap; 

  // 
  bool FetchTx (std::string& out, bool mainnet, const std::string& txid_hex, TxMap& cache);
  // -----------------------------------------------------------------
  // 
  // -----------------------------------------------------------------
  struct TxFetcher
  {
    TxMap cache; 
    //
    TxFetcher () : cache() {
    }
      
    ~TxFetcher() {
    } 

    bool Fetch (std::string& out, const std::string& txid_hex, bool mainnet);
    bool Fetch (Transaction& out, const std::string& txid_hex, bool mainnet);
   
  };
  
  
} // bmx
#endif
