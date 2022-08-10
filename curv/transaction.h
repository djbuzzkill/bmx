#ifndef AF_TRANSACTION_H
#define AF_TRANSACTION_H

#include "common.h"
#include "aframe/shared_types.h"
#include "aframe/binary_IO.h"



// 
namespace curv {

  // 
  typedef std::map<std::string, af::bytearray> TxMap; 
  // 
  bool FetchTx (af::bytearray& out, bool mainnet, const std::string& txid_hex, TxMap& cache);

  // 
  struct TxFetcher  {
    //
    TxFetcher (); 
    ~TxFetcher(); 

    TxMap cache; 

    bool Fetch (af::bytearray& out, const std::string& txid_hex, bool mainnet);
   
  };


  // 
  // we dont know yet
  struct Transaction { 

    af::bytearray bin; 
  };
  

  //
  //
  Transaction& ReadTransaction (Transaction& out, af::ReadStreamRef rs);
  
} // curv
#endif
