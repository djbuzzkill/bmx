#include "rum.h"


#include "curl/curl.h"
#include "zmq.h"





int Wat (std::vector<std::string>& args) {

  (void) args; 
  
  return 0; 
}



int main (int argv, char** argc)
{
  std::vector<std::string> args (argc, argc + argv);

  int watres = Wat (args);



  
  return watres;
}
