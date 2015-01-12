// test_Locomotion.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include <map>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <algorithm>

#include <Dx/Dx.h>
#include <Xp/Xp.h> 
#include <Dx/Horde3D_platform.h> 


#include <Dx/IKSolver2d.h>

#include <Horde3D.h>
#include <Horde3DUtils.h>

#include <SDL.h>

#include <SDL_opengl.h>


//
//
class Locomotion_test  : public sy::RT_window_listener
{

   std::shared_ptr<sy::Graphics_window> windo; 

public: 
   Locomotion_test (sy::System_context* sys) {}
   virtual ~Locomotion_test () {}


   //
   //
   virtual int	Initialize (sy::System_context* sys) 
   { 
      //windo.reset (sys->Create_GraphicsWindow (this, "IK test", 800,600,false)); 
      //accum_time = 0.0;
      //Dt = 0.0; 
      //tracker.Reset (sys); 
      //
      //init_scene (sys); 
      //windo->Show (true); 
      return 0; 
   } 

   //
   //
   virtual int	Deinitialize (sy::System_context* sys) 
   { 
      return 0; 
   } 

   //
   //
   virtual int	Update (sy::System_context* sys) { 
   return 0; 
   }




   //
   //
   virtual void OnWindowResize	(int wd, int ht)  { 
      //viewport_wd = wd; 
      //viewport_ht = ht;
      //glViewport (0, 0, viewport_wd ,viewport_ht); 
      }; 

   //
   //
   virtual void OnWindowClose () { }; 
   //
   //
   virtual void OnWindowActivate	(bool activate) { }; 
}; 


Locomotion_test* Create_test_locomotion(sy::System_context* sys)
{
   return new Locomotion_test (sys); 
}



//
// main entry
int _tmain(int argc, _TCHAR* argv[])
   {

std::shared_ptr<sy::System_context> sys (sy::Create_context ()); 
std::shared_ptr<sy::RT_window_listener>   rtt (Create_test_locomotion(sys.get()), cx::Destruct); 
sy::Run_realtime_task (sys.get(), rtt.get()); 

return 0;
   }

