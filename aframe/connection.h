
#ifndef AF_CONNECTION_H
#define AF_CONNECTION_H


#include "common.h"
#include "utility.h"
#include "shared_types.h"


namespace af {

  //
  //
  struct connection : public destructor {

    // enum Rflags : int {
    enum { RF_dont_wait = 0x01 };
    // const int32 RF_dont_wait = 0x02;
    // const int32 RF_dont_wait = 0x04;
    // const int32 RF_dont_wait = 0x08;
    // const int32 RF_dont_wait = 0x10;
    // //    _RF_max_bit_= (0x1 << 31) };

    // enum Sflags {

    virtual int Send(const void *dat, int sendsize, int sendflags) = 0;
    virtual int Recv(void *dat, int rbufsize, int recvflags) = 0;

    virtual int GetOpt() = 0;
    virtual int SetOpt() = 0;

  protected:
    connection() = default;
  };

  //
  typedef std::shared_ptr<connection> ConnectionRef;
  typedef ConnectionRef conn_ref;
  //
  ConnectionRef Create_TCP_Connection (const std::string &addr, int flags); 


} // namespace af

#endif
