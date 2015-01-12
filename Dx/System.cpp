#include "System.h"

namespace sy
{

   int Run_realtime_task (sy::System_context* sys, sy::RT_Task* rtt)
   {
      int run_state =0 ; 
      do switch (run_state)
      {
      case 0: 
         rtt->Initialize (sys); 
         run_state++; 
         break; 

      case 1: 
         while (sys->DispatchEvents ())
         {
            rtt->Update (sys); 
         }
         run_state++; 
         break;
      case 2: 
         rtt->Deinitialize (sys); 
         run_state++; 
         break;

      } while (run_state < 3); 

      return 0; 
   }
}