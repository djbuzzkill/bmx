
#include "connection.h"
#include "binary_IO.h"


#include "zmq.h"


using namespace af; 
//
struct _zmq_tcp_conn_00 : public af::connection {

  _zmq_tcp_conn_00 (const char* addr, int flags); 
  
  virtual ~_zmq_tcp_conn_00 (); 
  
  virtual int Send (const void* dat, int sendsize, int sflags); 
  virtual int Recv (void* dat, int rbufsize, int rflags); 

  virtual int GetOpt (); 
  virtual int SetOpt (); 

  void*     ctx; 
  void*     sock; 
  bytearray routingID;
  bytearray buf; 
}; 

_zmq_tcp_conn_00::_zmq_tcp_conn_00 (const char* addr, int flags) : ctx(zmq_ctx_new()), sock (zmq_socket(ctx, ZMQ_STREAM)), routingID(), buf(8 * 1024, byte(0)) {
  //FN_SCOPE();
  
  assert (sock); 
  int connres = zmq_connect (sock, addr); 
  assert (connres == 0);

  {
    // this should be routing ID
    zmq_msg_t msg_recv_ID;
    zmq_msg_init(&msg_recv_ID);
    int msg_recv_size = zmq_msg_recv(&msg_recv_ID, sock, 0);
    routingID.resize (msg_recv_size); 
    printf(" --> recv0  : msg_recv_size [%i]\n", msg_recv_size);
    size_t msg_size = zmq_msg_size(&msg_recv_ID);
    printf(" --> recv0 size [%zu]\n", msg_size);

    memcpy (&routingID[0], zmq_msg_data (&msg_recv_ID), msg_recv_size); 

    std::string msg_ID_hex; 
    hex::encode (msg_ID_hex, zmq_msg_data (&msg_recv_ID), msg_recv_size);
    printf ("msg_ID_hex [%s] \n", msg_ID_hex.c_str()); 
    auto close_res = zmq_msg_close(&msg_recv_ID);
    // this should be 0 length
    zmq_msg_t msg_0;
    zmq_msg_init(&msg_0);
    int recv_0_len = zmq_msg_recv(&msg_0, sock, 0);
    printf(" --> recv_len_0  [%i]\n", recv_0_len);
  }
  
  bytearray optbuf (256, byte(0)); 
  size_t  routinglen    = 256; 
  int routing_ID = zmq_getsockopt (sock, ZMQ_ROUTING_ID, &optbuf[0], &routinglen);

  assert (routingID.size () == routinglen);
  optbuf.resize (routinglen); 
  assert (eql  (routingID, optbuf)); // is this necessary
  
  std::string routingID_hex;
  hex::encode (routingID_hex, &routingID[0], routinglen); 
  printf ("    sockopt (%zu) [%s]  \n", routinglen, routingID_hex.c_str());


}

//
_zmq_tcp_conn_00::~_zmq_tcp_conn_00() {
  zmq_close (sock);
  zmq_ctx_shutdown (ctx);  
  }

int _zmq_tcp_conn_00::GetOpt (){ return 0; }
int _zmq_tcp_conn_00::SetOpt (){ return 0; }


int _zmq_tcp_conn_00::Send (const void *dat, int sendsize, int sflags) {

  int total_bytes_sent = 0; 
  int sendlen_data     = 0;
  int send_res         = 0;

  // send routing ID separately
  zmq_msg_t msgID; 
  zmq_msg_init_size (&msgID, routingID.size()); 
  memcpy(zmq_msg_data(&msgID), &routingID[0], routingID.size());

  send_res = zmq_msg_send (&msgID, sock, ZMQ_SNDMORE | sflags); 
  if (send_res < 0) {
    // some kind of error 
    puts (" (send ID res < 0)\n");
  }

  total_bytes_sent += send_res;
  
  // send the actual msg
  zmq_msg_t msg_enve; 
  zmq_msg_init_size (&msg_enve, sendsize); 
  memcpy (zmq_msg_data(&msg_enve), dat, sendsize);

  send_res = zmq_msg_send (&msg_enve, sock, sflags);
  if (send_res < 0) {
    // some kind of error 
    puts (" (send enve res < 0)\n");
  }

  sendlen_data     += send_res; 
  total_bytes_sent += sendlen_data; 
  
  return sendlen_data; 
}

//
int _zmq_tcp_conn_00::Recv (void* dat, int rbufsize, int rflags) {
  // FN_SCOPE ();

  int recv_msg_len = 0; 
  // the only flag for now
  int flags = (RF_dont_wait == rflags) ? ZMQ_DONTWAIT : 0; 

  WriteStreamRef ws  = CreateWriteMemStream (dat, rbufsize);

  //flags
  zmq_msg_t msg0;
  zmq_msg_init (&msg0); 
  int recv0len = zmq_msg_recv(&msg0, sock, flags);
  //printf ("     msg 0 len [%i] \n", recv0len);

  bool is_sz_eq = (recv0len == routingID.size ()); 
  bytearray dat0 (recv0len, byte(0));
  memcpy (&dat0[0], zmq_msg_data(&msg0), recv0len);
  
  if (eql(dat0, routingID)) { 
    //printf ("     good route\n");

    while (true) {
      
      zmq_msg_t msg; 
      zmq_msg_init_size (&msg, 1024);
      int recvlen = zmq_msg_recv (&msg, sock, flags);
      //printf ("    243 recvlen [%i] \n", recvlen);
      // this looks weird but ws->Write is correct 
      recv_msg_len += ws->Write (zmq_msg_data (&msg), recvlen);

      zmq_msg_close (&msg);
      
      if (zmq_msg_more (&msg) ) {
	puts ("    (loooop)" ); 
	continue;
      }

      break;
    }
    // assert (routingID == msg data); 
  }    

  zmq_msg_close (&msg0);
  
  return recv_msg_len;

}

//
//const std::string www
af::ConnectionRef af::Create_TCP_Connection (const std::string& addr, int flags) {

  return std::make_shared<_zmq_tcp_conn_00> (addr.c_str(), flags);

}
