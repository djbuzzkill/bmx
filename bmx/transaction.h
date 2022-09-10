
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
    af::byte32         prev_txid;   // 32b LE
    std::uint32_t      prev_index;  // int LE
    bmx::command_list  script_sig;  // af::bytearray script_sig; mebe bytes?
    std::uint32_t      sequence;    // 
  }; 

  TxIn&         ScriptPubKey (TxIn &inout, bool mainnet);
  std::uint64_t Amount       (const TxIn& txin, bool mainnet); 

  typedef std::vector<TxIn> TxInputs;


  // -----------------------------------------------------------------
  // 
  // output transaction pubkey in here
  // -----------------------------------------------------------------
  struct TxOut {
    std::uint64_t      amount; 
    bmx::command_list  script_pubkey;
  }; 

  typedef std::vector<TxOut>  TxOutputs;
  // -----------------------------------------------------------------
  //  
  // -----------------------------------------------------------------
  namespace Tx {

    struct Struc {
      //
      Struc () : version (0), inputs (), locktime (0), outputs (0) {
      }
      // 
      std::uint32_t version;    
      TxInputs      inputs;
      TxOutputs     outputs; 
      std::uint32_t locktime; //; 
      //
    };
  
    digest32&    SignatureHash   (digest32& ohash, const Struc& tx, size_t indx, bool mainnet); 
    bool         VerifyInput     (const Struc& tx, size_t input_index, bool mainnet); 
    std::int64_t Fee             (const Struc& tx, bool mainnet); 
    
    Struc&       SignInput       (Struc& tx, unsigned int tx_ind, const PrivateKey&, bool mainnet); 
    bool         Verify          (const Struc& tx, bool mainnet); 
  }
  
  //
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
  bool FetchTx (af::bytearray& out, bool mainnet, const std::string& txid_hex, TxMap& cache);
  // 
  // -----------------------------------------------------------------
  // 
  // -----------------------------------------------------------------
  struct TxFetcher  {

    TxMap cache; 
    //
    TxFetcher () : cache() {} 
    ~TxFetcher() {} 

    bool Fetch (af::bytearray& out, const std::string& txid_hex, bool mainnet);
    bool Fetch (Transaction& out, const std::string& txid_hex, bool mainnet);
   
  };
  
  
} // bmx
#endif
