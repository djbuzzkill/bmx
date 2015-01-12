
#include "stdafx.h"
#include "resource.h"

#include <Dx/Dx.h>
#include <Xp/Xp.h>

#include <stdio.h>


// Global Variables:
// YIINUT9R
sy::RT_window_listener* Create_TestRunState	(sy::System_context* sys); 
sy::RT_window_listener* Create_Ungr_00_task	(sy::System_context* sys); 
sy::RT_window_listener* Create_Cave_test		(sy::System_context* sys); 
sy::RT_window_listener* Create_Hmnd_04_test  (sy::System_context* sys);


int _tmain (int argc, _TCHAR* argv[])
{

// 
// main
// 
//int APIENTRY _tWinMain (
//   HINSTANCE hInstance, 
//   HINSTANCE hPrevInstance, 
//	LPTSTR lpCmdLine, 
//   int nCmdShow)
//{
//   UNREFERENCED_PARAMETER (hPrevInstance);	
//   UNREFERENCED_PARAMETER (lpCmdLine);		
   
   // TODO: Place code here.
   //MSG		msg;
   //HACCEL	hAccelTable;
   // std::shared_ptr<sy::System_context> sysContext (xp::Create_Win32_system (hInstance)); 


   std::shared_ptr<sy::System_context>  sys_ctx  (sy::Create_context ());    


   //
   std::shared_ptr<sy::RT_window_listener> rtt;  
   size_t task_Ind = 0; 
   switch (task_Ind) {
      case 0: rtt.reset (Create_Hmnd_04_test   (sys_ctx.get())); break;
      case 1: rtt.reset (Create_Ungr_00_task	(sys_ctx.get())); break;
      case 2: rtt.reset (Create_TestRunState	(sys_ctx.get())); break;
      case 3: rtt.reset (Create_Cave_test		(sys_ctx.get())); break;
   }

   //
   // switch 
   //int run_state  = 0; 
   //int num_states = 3; 
      
   sy::Run_realtime_task (sys_ctx.get(), rtt.get());
   //do switch (run_state) 
   //{

   //   case 0:  rt_Task->Initialize (sysContext.get()); 
   //            run_state++; 
   //            break; 

   //   case 1:  while (sysContext->DispatchEvents ()) {
   //               rt_Task->Update(sysContext.get()); 
   //               }
   //            run_state++; 
   //            break; 

   //   case 2:  rt_Task->Deinitialize (sysContext.get()); 
   //            run_state++; 
   //            break; 

   //} while (run_state < 3); 

   //

   rtt.reset (); 
   
   return 0;
}


//
//
//void TransformState (
//	const Ma::Vec3f& mod_Pos, 
//	const Ma::Quatf& mod_Rot, 
//	const Ma::Vec3f& view_Pos, 
//	const Ma::Quatf& view_Rot)
//{
//	//
//	Ma::Vec3f	mv_Pos, v_Tmp; 
//	Ma::Mat33f	inv_Mat; 
//	Ma::Quatf	mv_Rot, view_InvRot; 
//
//	Ma::Sub (v_Tmp, mod_Pos, view_Pos); 
//	
//	
//	Ma::QuatInverse (view_InvRot, view_Rot);
//	Ma::SetMat33Quat (inv_Mat, view_InvRot); 
//
//	Ma::Mult (mv_Pos, inv_Mat, v_Tmp); 
//	
//	Ma::Mult (mv_Rot, mod_Rot, view_InvRot); 
//
//	Ma::Mat44f mv_Mat; 
//	Ma::SetMat44Quat (mv_Mat, mv_Rot); 
//	Ma::AddMat44Transl (mv_Mat, mv_Pos); 
//	
//	Ma::Transpose (mv_Mat); 
//
//	glMatrixMode (GL_MODELVIEW); 
//	glLoadMatrixf (mv_Mat.ptr()); 
//
//
//}
//
