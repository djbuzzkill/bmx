
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
   
  };

  // -----------------------------------------------------------------
  // 
  // Input 
  // -----------------------------------------------------------------
  struct TxIn { 
    af::byte32         prev_txid;   // 32b LE
    unsigned int       prev_index;  // int LE
    bmx::command_list  script_sig;  // af::bytearray script_sig; mebe bytes?
    unsigned int       sequence;    // 
  }; 

  typedef std::vector<TxIn> TxInputs; 
  // -----------------------------------------------------------------
  // 
  // output transaction
  // -----------------------------------------------------------------
  struct TxOut {
    size_t             amount; 
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
      unsigned int  version;    
      TxInputs      inputs;
      TxOutputs     outputs; 
      unsigned int  locktime; //; 
      
    };
  
    digest32& SignatureHash   (digest32& ohash,  const Struc& tx); 
    bool      VerifyInput     (const Struc& tx); 
    Struc&    SignInput       (Struc& tx, unsigned int tx_ind, const PrivateKey&); 
    bool      Verify          (const Struc& tx) ; 
  }
  
  //
  typedef Tx::Struc Transaction; 
  // -----------------------------------------------------------------
  // 
  // -----------------------------------------------------------------
  void       print_txin       (const TxIn& txin, size_t len); 
  void       print_txo        (const TxOut& txo, size_t len);
  
  // 
  size_t     ReadTransaction  (Transaction& out, af::ReadStreamRef rs);
  size_t     WriteTransaction (af::WriteStreamRef ws, const Transaction& out); 

  
} // bmx
#endif
