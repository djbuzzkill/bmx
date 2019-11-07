#ifndef SPUX_RUNNABLETASK_INCLUDE
#define SPUX_RUNNABLETASK_INCLUDE

#include "Shared.h"

namespace spux
{
	int dumb (); 

   struct System; 


	// 
	// abstract realtime loop
	struct Runnable 
      {
   public: 

	virtual int Initialize    (Ref<System>::Type)			= 0; 
   virtual int Run           (Ref<System>::Type, double) = 0; 
   virtual int Deinitialize  (Ref<System>::Type)         = 0; 
	   //
   protected: 
		Runnable () { }
	   }; 


} // Sy

#endif
