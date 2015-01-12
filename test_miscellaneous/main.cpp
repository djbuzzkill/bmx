//

//#include "stdafx.h"

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>


#include <memory>
#include <Dx/Dx.h>

sy::RT_window_listener* Create_test (sy::System_context*); 
//



int _tmain (int argc, _TCHAR* argv[])
{
   std::shared_ptr<sy::System_context>     sys (sy::Create_context ()); 
   std::shared_ptr<sy::RT_window_listener>   rtt (Create_test (sys.get())); 
   
   return sy::Run_realtime_task (sys.get(), rtt.get()); 

}

