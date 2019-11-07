#ifndef SPUX_RUNNABLETASK_INCLUDE
#define SPUX_RUNNABLETASK_INCLUDE

#include "Shared.h"

namespace spux
{
	int dumb (); 

   struct System; 

	// 
	//  
	struct Runnable 
      {
   public: 

	virtual void Initialize    (System*)				= 0; 
   virtual void Run           (System*, double)   = 0; 
   virtual void Deinitialize  (System*)           = 0; 
	   //
   protected: 
		Runnable () { }
	   }; 

   int Run_realtime_task       (Runnable* rtt, System* sys); 

} // Sy

#endif
