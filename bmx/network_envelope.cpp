
#include "network.h"
#include "aframe/hash.h"


using namespace af;
using namespace ffm; 
// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
uint64 bmx::Network::Envelope::Read (Struc& envel, ReadStreamRef rs, bool mainnet) {

  //FN_SCOPE ();

  uint64 readlen = 0;

  uint32 readmagic = 0;
  readlen += rs->Read (&readmagic, sizeof(uint32));

  // std::string b4str, aftstr;
  swap_endian<uint32> (&readmagic);
  
  if (mainnet) { 
    assert (readmagic == Network::kMAINNET_MAGIC);
  }
  else {
    assert (readmagic == Network::kTESTNET_MAGIC); 
  }

  envel.magic = readmagic; 
  
  envel.command.resize (12, byte(0));  
  readlen += rs->Read (&envel.command[0], 12); 
  while (envel.command.back () == byte{0x0}) envel.command.pop_back ();

  {
    std::string chars;
    for (auto e : envel.command)
      chars += std::to_integer<uint8>(e);
    printf("   command size(%zu) [%s]\n", envel.command.size (), chars.c_str());
  }

  //printf ( "    enve command size[%zu]\n",   envel.command.size ()); 
  
  uint32 payloadlen = 0;
  readlen += rs->Read (&payloadlen, sizeof(uint32));
  printf ("    payload length [%u] \n", payloadlen);

  uint32 rdchecksum = 0; 
  readlen += rs->Read (&rdchecksum, sizeof (uint32)); 

  envel.payload.resize (payloadlen, byte{0});
  readlen += rs->Read(&envel.payload[0], payloadlen); 

  union {
    uint32 checksum;
    byte bytes_cs_[4]; 
  };
  
  digest32 dig ;
  af::hash256 (dig, &envel.payload[0], payloadlen);
  std::copy (dig.begin (), dig.begin() + 4, bytes_cs_);

  if (rdchecksum != checksum)  {
    printf ("    [%s] CHECKSUM DOES NOT MATCH !!  \n", __FUNCTION__); 
    }
  
  assert (rdchecksum == checksum);

  return readlen; 
}

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
uint64 bmx::Network::Envelope::Write (WriteStreamRef ws, const Struc& nv) {
  uint64 writelen = 0;
  // magic
  uint32  magicbytes = nv.magic; 
  swap_endian<uint32>(&magicbytes); // comment about LE
  writelen += ws->Write (&magicbytes, sizeof(uint32)); 
  
  // command
  bytearray cmdpad = nv.command;
  while (cmdpad.size () < 12) cmdpad.push_back (byte (0)); 
  writelen += ws->Write (&cmdpad[0], 12); 
  
  // payload length
  uint32 payloadlen = nv.payload.size (); 
  writelen +=  ws->Write (&payloadlen, sizeof (uint32)); 
  
  // checksum bytes
  digest32 thegest; 
  writelen += ws->Write (&af::hash256(thegest, &nv.payload[0], nv.payload.size ())[0], 4);
  
  // payload 
  writelen += ws->Write (&nv.payload[0], nv.payload.size ()); 
  
  return writelen; 
}


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
int bmx::Network::Envelope::Send (af::conn_ref conn, const bmx::Network::Envelope::Struc& ne, int flags) {
  //FN_SCOPE (); 
  const int32 bufsize = 1024; 
  std::array<byte, bufsize> sendbuf;
  uint64    writelen_send = 0;
  //WriteStreamRef ws_ne = CreateWriteMemStream (&sendbuf[0], 1024); 
  writelen_send += Network::Envelope::Write (CreateWriteMemStream (&sendbuf, bufsize), ne);
  printf ("    serialized outgoing network envelope (%zu)\n", writelen_send); 
  return conn->Send (&sendbuf, writelen_send, flags);  
  }


// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
int bmx::Network::Envelope::Recv (bmx::netmessage_cb* const cb, af::conn_ref conn, bool mainnet, int flags) {

  enum recv_state {
    rs_recv_bytes,
    rs_read_env_prim,
    rs_read_env_final,
    rs_limbo
  };

  network_envelope ne;
  //
  uint64 ne_offset           = 0; // sum bytes of completed ne's
  int    accum_recv_len      = 0; // running count of bytes recvd
  uint64 accum_read_env_len  = 0; // byte progress of current envelope

  uint32 payloadlen          = 0; 
  uint32 rdchecksum          = 0; 
  
  const int32 bufsize = 1024;
  bytearray buf(bufsize, byte(0));
  ReadStreamRef rs = CreateReadMemStream(&buf[0], bufsize);

  recv_state cur_state = rs_recv_bytes;
  recv_state next_state = rs_read_env_prim;

  while (true) {
    //printf ("   cur state [%i]\n", cur_state);
    switch (cur_state) {
    //
    // rs_recv_bytes -------------------------------------
    case rs_recv_bytes: {
      
      int r = conn->Recv (&buf[accum_recv_len], bufsize - accum_recv_len , flags);
      //printf ("    rs_recv_bytes[%i]\n" , r);
      accum_recv_len += r; 

      cur_state = next_state;
      next_state = rs_limbo; 
      
    } break; // case
    //
    // rs_read_env_prim, read most of the ne, not payload
    case rs_read_env_prim: {

      uint32 readmagic = 0;
      accum_read_env_len = rs->Read (&readmagic, sizeof(uint32));

      swap_endian<uint32>(&readmagic);
      printf ("  readmagic[%x]\n", readmagic); 
      if (mainnet) {
        assert(readmagic == Network::kMAINNET_MAGIC);
      } else {
        assert(readmagic == Network::kTESTNET_MAGIC);
      }
      ne.magic = readmagic;

      ne.command.resize(12, byte(0));
      accum_read_env_len += rs->Read (&ne.command[0], 12);
      while (ne.command.back() == byte{0x0})
        ne.command.pop_back();
      //printf("   command (%zu) [%s]\n", ne.command.size(), af::to_string(ne.command).c_str());
 
      payloadlen = 0;
      accum_read_env_len += rs->Read(&payloadlen, sizeof(uint32));
      //printf("    payload length [%u] \n", payloadlen);

      rdchecksum = 0;
      accum_read_env_len += rs->Read(&rdchecksum, sizeof(uint32));

      // expected total network envelope length, more of an offset
      uint64 total_ne_length = accum_read_env_len + payloadlen + ne_offset;
      //printf ( "    accum_recv_len[%i], total_msg_length[%zu]\n", accum_recv_len, total_ne_length); 
      if (accum_recv_len < total_ne_length) {
	// we need more bytes
	cur_state  = rs_recv_bytes; 
	next_state = rs_read_env_final; 
      }
      else if (accum_recv_len == total_ne_length){
	// carry on 
	cur_state = rs_read_env_final;
	next_state = rs_limbo;
      }
      
    } break; // case 
   //
   // rs_read_env_final
   case rs_read_env_final: {

     ne.payload.resize (payloadlen, byte{0});

     accum_read_env_len  += rs->Read(&ne.payload[0], payloadlen);

     union {
       uint32 checksum;
       byte bytes_cs_[4];
     };

     digest32 dig;
     af::hash256(dig, &ne.payload[0], payloadlen);
     std::copy(dig.begin(), dig.begin() + 4, bytes_cs_);

     bool good_checksum = rdchecksum == checksum;
     if (rdchecksum != checksum) {
       printf("    [%s] CHECKSUM DOES NOT MATCH !!  \n", __FUNCTION__);
     }

     //
     // Dispatchmessage (cb, cmd_s, ne) someday
     if (cb) { // && good_checksum ??

       const std::string cmd_s = af::to_string (ne.command);
       //printf("   command (%zu) [%s]\n", cmd_s.size(), cmd_s.c_str());

       int           payload_readlen = 0; 
       ReadStreamRef payload_rs      = CreateReadMemStream(&ne.payload[0], payloadlen);

       if (cmd_s == "version") {
         Network::Message::Version vers;
         payload_readlen = Network::Message::Read(vers, payload_rs, mainnet);
         cb->OnVersion (vers, ne, mainnet);
       }
       else if (cmd_s == "verack") {
         // Network::Message::VerAck verack;
         cb->OnVerAck (ne, mainnet);
       }
       else if (cmd_s == "headers") {
	 // command = b'headers'
       }
       else if (cmd_s == "getheaders") {
	 // command = b'headers'
       }
       else if (cmd_s == "pong") {
	 // command = b'headers'
       }
       else if (cmd_s == "ping") {
	 // command = b'headers'
       }
       else if (cmd_s == "merkleblock") {
	 // command = b'headers'
       }
       else {
	 printf ( "UNKNOWN command[%s]\n", cmd_s.c_str()); 
       }
     } // cb

     //
     ne_offset += accum_read_env_len;
     assert (ne_offset <= accum_recv_len);
     //printf ("     ne_offset[%zu],  accum_recv_len[%i]\n", ne_offset, accum_recv_len);
     if (ne_offset < accum_recv_len) {
       // more bytes to process 
       cur_state = rs_read_env_prim;
       next_state = rs_limbo;
     }
     else {
       //??
       return accum_recv_len; 
     } 
     
   } break; // case

    } // switch 
  }
    
  return accum_recv_len; 
  
  }

// ---------------------------------------------------------------------
//
// ---------------------------------------------------------------------
std::string& bmx::Network::Envelope::Format (std::string& str, const bmx::Network::Envelope::Struc& enve) {
  //  FN_SCOPE (); 
  std::string s0, s1, s2;

  printf ( "    fmt command size[%zu]\n",   enve.command.size ()); 

  std::string cmd_s = af::to_string (enve.command); 
  
  str = std::string( "Envelope {\n")
  + "  command { \'" + cmd_s + "\' }\n"
  + "  payload {" + hex::encode (s1, &enve.payload[0], enve.payload.size()) + "}\n"
  + "  magic { 9x" +   hex::encode (s2, &enve.magic     , sizeof (uint32))     + "}\n"
  +  "}\n";

  return str; 
}

// ---------------------------------------------------------------------
// Payload Message::Version
// ---------------------------------------------------------------------
bmx::Network::Envelope::Struc& bmx::Network::Envelope::Payload (
   bmx::Network::Envelope::Struc&        ne,
   const bmx::Network::Message::Version& vers,
   bool                                  mainnet)
{ 
  // FN_SCOPE ();
  const std::string version_s             = "version";

  //SizeOf (const bmx::Network::Message::Version&  vers); 
  size_t sizeOf_MessageVersion = bmx::Network::Message::SizeOf (vers); 
  //printf ( "    sizeOf_MessageVersion [%zu]\n", sizeOf_MessageVersion); 
  
  //
  ne.magic = mainnet ? Network::kMAINNET_MAGIC : Network::kTESTNET_MAGIC; 

  // command
  ne.command.clear (); 
  to_bytes (ne.command, version_s);
  // {
  //   printf ("   command size [%zu]\n",  oenve.command.size ());
  //   std::string chars;
  //   for (auto e : oenve.command)
  //     chars += std::to_integer<uint8>(e);
  //   printf("   chars[%s]\n", chars.c_str());
  // }
  ne.payload.resize(sizeOf_MessageVersion); 
  auto writelen_vers = bmx::Network::Message::Write (CreateWriteMemStream (&ne.payload[0], sizeOf_MessageVersion), vers);
  assert (writelen_vers == sizeOf_MessageVersion); 

  return ne; 
}

// ---------------------------------------------------------------------
// Payload Message::Version
// ---------------------------------------------------------------------
bmx::Network::Envelope::Struc& bmx::Network::Envelope::PayloadVerAck (
   bmx::Network::Envelope::Struc& ne,
   bool                           mainnet)
{
  //FN_SCOPE ();
  const std::string verack_s = "verack";

  // magic
  ne.magic = mainnet ? Network::kMAINNET_MAGIC : Network::kTESTNET_MAGIC;
  // command
  ne.command.clear (); 
  to_bytes (ne.command, verack_s);

  // nada
  ne.payload.clear ();
  assert (ne.payload.size () == 0); 
  return ne;
}


// ---------------------------------------------------------------------
// Payload Message::GetHeaders
// ---------------------------------------------------------------------
bmx::Network::Envelope::Struc& bmx::Network::Envelope::Payload (
    bmx::Network::Envelope::Struc&           ne,
    const bmx::Network::Message::GetHeaders& msg,
    bool                                     mainnet) {

  // FN_SCOPE ();
  const std::string getheaders_s             = "getheaders";

  //
  ne.magic = mainnet ? Network::kMAINNET_MAGIC : Network::kTESTNET_MAGIC; 

  // command
  ne.command.clear (); 
  to_bytes (ne.command, getheaders_s);
  // {
  //   printf ("   command size [%zu]\n",  oenve.command.size ());
  //   std::string chars;
  //   for (auto e : oenve.command)
  //     chars += std::to_integer<uint8>(e);
  //   printf("   chars[%s]\n", chars.c_str());
  // }
  uint64 writelen_payload = 0; 
  // payload 
  ne.payload.resize (SizeOf (msg)); 
  WriteStreamRef payl_ws = CreateWriteMemStream (&ne.payload[0], ne.payload.size()); 
  
  writelen_payload += Network::Message::Write (payl_ws, msg,  mainnet);
  assert (writelen_payload == SizeOf(msg));
  // ne.payload.resize(sizeOf_MessageVersion); 
  return ne;
}


// ---------------------------------------------------------------------
// Payload Message::Headers
// ---------------------------------------------------------------------
bmx::Network::Envelope::Struc& bmx::Network::Envelope::PayloadHeaders (bmx::Network::Envelope::Struc& ne, const blockarray& blocks, bool mainnet) {

  // FN_SCOPE ();
  const std::string headers_s = "headers";

  //
  ne.magic = mainnet ? Network::kMAINNET_MAGIC : Network::kTESTNET_MAGIC; 

  // command
  ne.command.clear (); 
  to_bytes (ne.command, headers_s);

  // payload
  ne.payload.resize (sizeof(block) * blocks.size());
  WriteStreamRef payl_ws = CreateWriteMemStream (&ne.payload[0], ne.payload.size());
  uint64 writelen_payload = 0;
  writelen_payload += Network::Message::Write (payl_ws, blocks, mainnet); 
		      
  return ne;
}

// ---------------------------------------------------------------------
// Payload Message::Ping
// ---------------------------------------------------------------------
bmx::Network::Envelope::Struc& bmx::Network::Envelope::PayloadPing (bmx::Network::Envelope::Struc& ne, uint64 nonce, bool mainnet) {

  // FN_SCOPE ();
  const std::string ping_s = "ping";

  //
  ne.magic = mainnet ? Network::kMAINNET_MAGIC : Network::kTESTNET_MAGIC; 

  // command
  ne.command.clear (); 
  to_bytes (ne.command, ping_s);


  union {
    uint64 nonce_BE; 
    byte bytes[sizeof(uint64)]; 
  };
  
  nonce_BE = nonce;
  ne.payload.resize (sizeof(uint64)); 
  std::reverse_copy (&bytes[0], &bytes[7], &ne.payload[0]);

  return ne;
}

// ---------------------------------------------------------------------
// Payload Message::Pong
// ---------------------------------------------------------------------
bmx::Network::Envelope::Struc& bmx::Network::Envelope::PayloadPong (bmx::Network::Envelope::Struc& ne, uint64 nonce, bool mainnet) {
  // FN_SCOPE ();
  const std::string pong_s             = "pong";
  // 
  ne.magic = mainnet ? Network::kMAINNET_MAGIC : Network::kTESTNET_MAGIC; 
  // command
  ne.command.clear (); 
  to_bytes (ne.command, pong_s);

  union {
    uint64 nonce_BE; 
    byte bytes[sizeof(uint64)]; 
  };
  
  nonce_BE = nonce;
  ne.payload.resize (sizeof(uint64)); 
  std::reverse_copy (&bytes[0], &bytes[7], &ne.payload[0]); 
  
  return ne;
}

