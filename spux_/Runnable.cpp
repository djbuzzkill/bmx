#include "Runnable.h"
#include "System.h"

namespace spux
{
	static int wat = 0; 



   int Run_realtime_task (Ref<Runnable>::Type task, Ref<System>::Type sys); 
}

int spux::dumb () 
{ 
   return wat++;  
} 




//
int spux::Run_realtime_task (Ref<Runnable>::Type task, Ref<System>::Type sys)
{
   int run_state = 0; 
   do switch (run_state)
   {
   case 0: 

      task->Initialize (sys); 
      run_state++; 
      break; 

   case 1: 
      while (sys->DispatchEvents ())
      {
         task->Run (sys, 0.0); 
      }
      run_state++; 
      break;
   case 2: 
      task->Deinitialize (sys); 
      run_state++; 
      break;

   } while (run_state < 3); 

   return 0; 
}

