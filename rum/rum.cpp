#include "rum.h"
#include "rum_msg_generated.h"


using namespace af;



namespace rum {
  // 
  struct MessageBase {

    enum {
      ID_M101_Status = 0,
      ID_M102_ = 0,
      ID_M103 = 0,
      ID_M104 = 0,
    };

    //
    unsigned int id; 
    char sender[32] ; 
  }; 

  // 
  //
  struct M101_Status : public MessageBase {
    enum { ID = ID_M101_Status };
    unsigned count;
    unsigned flags;
  };
 
}


int Wat (const std::vector<std::string>& args) {

  flatbuffers::FlatBufferBuilder build; 

  auto         burgername = build.CreateString ("oscar");

  rum::M101_Status stat;
  
  
  unsigned int burgerid   = 69; 
  std::string toppings_str[] = {"bacon", "cheez", "onion", "mayonaise"};
  
  std::vector<flatbuffers::Offset<flatbuffers::String>> toppingsvec;
  toppingsvec.push_back (build.CreateString ("bacon"));
  toppingsvec.push_back (build.CreateString ("cheez"));
  toppingsvec.push_back (build.CreateString ("onion"));
  toppingsvec.push_back (build.CreateString ("mayonaise")); 
  auto toppings = build.CreateVector (toppingsvec); 
  
  auto onionburger = rum::msg::CreateCheezburger (build, burgername, burgerid, toppings); 
  build.Finish (onionburger); 
  
  build.GetSize ();
  build.GetBufferPointer ();
  
  af::bytearray burgermem (build.GetSize()); {
    af::WriteStreamRef ws =
      af::CreateWriteMemStream(&burgermem[0], build.GetSize());

    ws->Write(build.GetBufferPointer(), burgermem.size());
  }

  auto cheezburger = rum::msg::GetCheezburger (&burgermem[0]);

  printf ("cheezburger: %s\n", cheezburger->name()->c_str());

  for (size_t i = 0; i < cheezburger->toppings()->size(); ++i) {
    printf ("toppings:\n");
    printf (" %s\n",  cheezburger->toppings()->GetAsString (i)->c_str());
  }
  
  return 0; 
}



// 
int test_curl (const std::vector<std::string>& args) {

  const std::string tx_id = "3db68a2171756cfb0c7af980ac8780b4b5c892412f50cd8c4808182c7408aeb8"; 

  af::bytearray txbytes;
  curv::TxFetcher txfetch;

  if (txfetch.Fetch (txbytes, tx_id, false)) {

    std::string printstr  (&txbytes[0], &txbytes[txbytes.size()]);
   printf ("\n\n\n\n\n\n",printstr.c_str()); 
   printf ("contents: %s\n",printstr.c_str()); 

  }
    
  
  // std::string test_url = url; // "https://curl.se" ;

  // curl_easy_setopt  (hcurl, CURLOPT_URL,  test_url.c_str());
  // curl_easy_setopt  (hcurl, CURLOPT_HTTPGET, 1);
  // curl_easy_setopt  (hcurl, CURLOPT_WRITEFUNCTION, curl_write_cb);

  
  // size_t buffsize = 500 * 1024; 
  // bytearray getbytes (buffsize);
  // WriteStreamRef ws = CreateWriteMemStream (&getbytes[0], buffsize);

  // write_struct cbstruc =  { ws.get(), std::vector<int> () }; 
     
  // curl_easy_setopt(hcurl, CURLOPT_WRITEDATA, &cbstruc);

  // CURLcode res  = curl_easy_perform (hcurl);

  // printf ("[%s]totalbytes: %zu\n", __FUNCTION__, ws->GetPos ()); 
  // printf ("   numwrites: %zu\n", cbstruc.writes.size ());

  // for (size_t i = 0; i < cbstruc.writes.size (); ++i)  {
  //   printf ("%i: %i\n", i, cbstruc.writes[i] );

  //   }


  
  // std::string printstr  (&getbytes[0], &getbytes[ws->GetPos ()]);
  // printf ("\n\n\n\n\n\n",printstr.c_str()); 
  // printf ("contents: %s\n",printstr.c_str()); 
  
  

  return 0;
}

 




// -----------------------------------------
int main (int argv, char** argc)
{
  std::vector<std::string> args (argc, argc + argv);

  //int watres = Wat (args);
  int watres = test_curl (args); 
  return watres;
}
