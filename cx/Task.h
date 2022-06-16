#ifndef _Task_incl_
#define _Task_incl_


namespace sy
{
	int dumb (); 

   struct SystemContext; 
	// 
	// abstract OS/Sys


   //struct Mouse 
   //   {
   //   enum Button
   //      {
   //      MS_Left  = (0x1 << 0), 
   //      MS_Middle= (0x1 << 1), 
   //      MS_Right = (0x1 << 2),
   //      };  
   //
   //   virtual unsigned  GetButtonState () = 0; 
   //   virtual void      GetAbsPos      (int& x, int& y, int z) = 0; 
   //   virtual void      GetRelPos      (int& x, int& y, int z) = 0; 
   //
   //protected:    
   //
   //   Mouse () {}
   //   };       
   //
   //
   //struct Keyboard 
   //   {
   //   
   //protected: 
   //
   //   Keyboard () {}
   //   }; 



      
	// 
	// abstract realtime loop
	class RT_Task 
      {
   public: 

	virtual void Initialize    (SystemContext*)				= 0; 
   virtual void Update		   (double , SystemContext*)  = 0; 
   virtual void Deinitialize  (SystemContext*)           = 0; 
	   //
   protected: 
		RT_Task () { }
	   }; 

} // Sy

#endif
