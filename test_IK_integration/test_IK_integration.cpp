// test_IK_integration.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"



#include "stdafx.h"

#include <map>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <sstream>

#include <Dx/Dx.h>
#include <Xp/Xp.h> 
#include <Dx/Horde3D_platform.h> 


#include <Dx/IKSolver2d.h>
#include <Dx/Anim_biped.h>

#include <Horde3D.h>
#include <Horde3DUtils.h>

#include <SDL.h>

#include <SDL_opengl.h>

//
//
const int k_num_frames = 360;


//
//
class IK_test : public sy::RT_window_listener
{

   //
   double                                 Dt; 
   double                                 accum_time;
   sy::Time_tracker                       tracker;

   int                                    viewport_wd;
   int                                    viewport_ht;

   std::shared_ptr<sy::Graphics_window>    windo; 
   Ma::Vec3f                              view_Rot,  
                                          view_Pos;

   An::Humanoid_artic                     wat; 
   An::Stride_extents                     strexts; 


      
   Ma::Vec2f                              ground_interp[k_num_frames];
   Ma::Vec2f                              recoil_interp[k_num_frames];
   //Ma::Vec2f                              interp_support[k_num_frames];

   int frame_counter; 


   void init_scene (sy::System_context* sys) 
   {
      frame_counter = 0; 

      Ma::Set (view_Pos, 0.0f, 8.0f, 0.0f);
      Ma::Set (view_Rot, 0.0f, 0.0f, 0.0f);

      An::Stride_extents exts;

      // for this testing data, assume leg coords are relative to root
      const float root_coord[] = {0.0f, 0.0f, 0.0f }; 
      const float seg_coords_l[][3] = 
      {
         { 0.405f, -0.08,   0.0 },
         { 0.0f,   -2.4f,  0.02 },
         { 0.0f,   -2.6f, -0.06 },
         { 0.0f,   -0.01f, 0.9f },
      };

      const float seg_coords_r[][3] = 
      {
         {-0.405f, -0.08,    0.0 },
         { 0.0f,   -2.4f,   0.02 },
         { 0.0f,   -2.6f,  -0.06 },
         { 0.0f,   -0.01f,  0.9f },
      };

      const float joint_rads[] = 
      {
         2.0f, 1.0f, 0.1f, 0.7f
      }; 

      
      // root_coord + hip/upperleg joints will form the fixed triangle. 
      // IK will be computed with the hip joints as the origin, but step
      // target (height, radius) will be based off the root point. 
      Set(wat.legs.root_pos, root_coord);  

      Ma::Vec3f v, v_acc; 
      Set (v_acc, seg_coords_l[0]);

      // An::Compute_leg_params (
      for (int i = 0; i < 4; i++)
      {
         Set (v, seg_coords_l[i]); 
         Ma::Incr (v_acc, v); 

         Set (wat.legs.L.bind.points[i], v_acc.ptr());
         Set (wat.legs.R.bind.points[i], v_acc.ptr());
         wat.legs.L.bind.radius[i] = joint_rads[i]; 
         wat.legs.R.bind.radius[i] = joint_rads[i]; 

         Set (wat.legs.L.dyna.chain[i], Ma::Z(wat.legs.L.bind.points[i]), Ma::Y(wat.legs.L.bind.points[i])); 
         wat.legs.L.dyna.deltas[i] = 0.0f;

         Set (wat.legs.R.dyna.chain[i], Ma::Z(wat.legs.R.bind.points[i]), Ma::Y(wat.legs.R.bind.points[i])); 
         wat.legs.R.dyna.deltas[i] = 0.0f;
      }

      // wat.legs.L.bind.toe_dist, 
      Ma::Vec2f L_targ; 
      Ma::Vec2f R_targ; 

      Ma::Set (L_targ, 0.0f, -100.0f); 
      Ma::Set (R_targ, 0.0f, -100.0f); 
      // wat.legs.L.bind.points[i]
      // wat.legs.R.bind.points[i]
      // Ik::Solve_2D (wat.legs.L.dyna.chain + 1, wat.legs.L.dyna.deltas, 2, L_targ); 
      // Ik::Solve_2D (wat.legs.R.dyna.chain + 1, wat.legs.R.dyna.deltas, 2, R_targ);  

      
      180 / 12.0f; 

      float leg_len = seg_coords_l[1][1] + seg_coords_l[2][1]; 

      {
         Ma::Vec2f a, b, v; 
         Ma::Mat22f M; 
         // 15 degr.
         float arc_angle = float(Ma::Pi) / 16.0f; 
         float degr_     = arc_angle * Ma::kRads2Degr; 

         // here's the ankle joint location. 
         Ma::Scale (a, wat.legs.L.dyna.chain[2], 1.0f); 
         //Ma::Scale (a, wat.legs.L.dyna.chain[2], An::Biped::kMin_compression); 
      

         // compute testing points 
         Ma::Vec2f fwd_pt, rear_pt;  
         Ma::SetMatRotateZ (M, arc_angle); 
         Ma::Mult (fwd_pt, M, a); // <- fwd

         Ma::SetMatRotateZ (M, -arc_angle); 
         Ma::Mult (rear_pt, M, a); // bkwd
         float interp_step_   = 1.0/k_num_frames;

         {  // interpolated ground points 
            Ma::Vec2f  f2r_dir;  
         
            Ma::Sub (f2r_dir, rear_pt, fwd_pt);  


            //
            float angle_step     = Ma::Pi / (k_num_frames * 3.0f); 
            float start_offs_    = 2.0f * Ma::Pi / 3.0f; 

            for (int i = 0; i < k_num_frames; i++)
            {
               Ma::Scale (ground_interp[i],  f2r_dir, interp_step_ * i);
               Ma::Incr (ground_interp[i], fwd_pt); 
                
            }
         }  

         {  // interpolated recoil points 
            Ma::Vec2f  r2f_dir;  
            Ma::Sub (r2f_dir, fwd_pt, rear_pt); 

            float start_angle_ = Ma::Pi / 6.0f; 
            float stop_angle_ = Ma::Pi - start_angle_; 
            float arc_ = stop_angle_ - start_angle_ ; 

            float dAngle =  arc_ / k_num_frames; 

            float arc_hgt[k_num_frames]; 
            for (int i = 0; i < k_num_frames; i++) 
               arc_hgt[i] = sinf ( i * dAngle + start_angle_) - sinf (start_angle_); 

            
            for (int i = 0; i < k_num_frames; i++)
            {
               float x_ =  i * interp_step_ * X(r2f_dir); 
               float y_ = (i * interp_step_ * Y(r2f_dir)) + arc_hgt[i]; 

               Ma::Set (recoil_interp[i], x_, y_);
               Ma::Incr (recoil_interp[i], rear_pt); 
            }
         }

      }

     	// important state
	   glEnable (GL_DEPTH_TEST);  
	   glEnable (GL_CULL_FACE );
   }

   //
   void update_input (sy::System_context* sys)
   {
      sy::Keyboard_state   kb; 
      sy::Mouse_state      ms;
      
      sys->Poll_input (ms, kb); 

      if (sy::Is_keydown (sy::SC_Q, kb))
         sys->SysReq (sy::System_context::Req_Exit); 

      xp::Update_view_transform (view_Pos, view_Rot, Dt * 10.0, kb, ms); 
   } 

   //
   void update_anim (sy::System_context* sys)
   {
   } 

   //
   void render (sy::System_context* sys)
   {
      Ma::Quatf q, q_x, q_y; 

      const float kDegr2Rad = Ma::Pi / 180.0;

      Ma::RotXAxis (q_x, kDegr2Rad * Ma::X (view_Rot)); 
      Ma::RotYAxis (q_y, kDegr2Rad * Ma::Y (view_Rot)); 
      Ma::Mult (q, q_y, q_x); 

	   //
	   Rn::ProjectionState (
		   float (Ma::Pi / 3.0), 
		   float (viewport_wd) / float (viewport_ht), 
		   1.0f, 
		   5000.0f
		   ); 

	   //
	   //
	   glClearColor (0.0f, 0.0f, 0.0f, 1.0f); 
	   glClearDepth (1.0); 
	   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
	   // Render_Cave_test (sg_view_Rot, sg_view_Pos); 

      // reference floor
      Rn::Draw_AA_grid (128.0, q, view_Pos); 


      Ma::Vec3f target_point;

      int counter_mod = 2 * k_num_frames ;
      int half_num_frames = k_num_frames / 2; 
      float half_fram_step = 1.0f / half_num_frames;
      frame_counter++; 
      frame_counter = frame_counter % counter_mod; 
      

      float foot_lift_angle = 0.0f; 

      if (frame_counter < k_num_frames)
      {
         // recoil, contact aim 
         Ma::Set(target_point, 0.0f, Y(recoil_interp[frame_counter]), X(recoil_interp[frame_counter])); 
         foot_lift_angle = 1.0; 
      }
      else
      {

         
         int single_phase = frame_counter - k_num_frames; 
         Ma::Set(target_point, 0.0f, Y(ground_interp[single_phase ]), X(ground_interp[single_phase])); 

         
         if (single_phase < half_num_frames)
         {
            // flat/support   
            foot_lift_angle = 0.0f; 
         }
         else 
         {
            // power/push 
            foot_lift_angle = 1.0f * (single_phase - half_num_frames) * half_fram_step;
         }
      }

      
      
      Ma::Vec3f tgtcol; 
      Ma::Set (tgtcol, 0.4f, 0.4f, 1.0f); 
      Rn::Draw_sphere (0.2, tgtcol, target_point, q, view_Pos); 


      Ma::Quatf mod_Rot;
      Ma::Vec3f mod_Pos;

      Ma::SetIdent(mod_Rot); 


      Ma::Vec2f tgt_2d;
      Ma::Set(tgt_2d, Ma::Z(target_point), Ma::Y(target_point)); 


      //Ma::Vec2f& An::Calc_heel_pos (Ma::Vec2f& out, const An::Leg_bind& leg, float theta)
      static float osc_ank = 0.0f;
      osc_ank += 2.0f * Dt;
      //float foot_lift_angle = 0.3f * (cosf (osc_ank) + 1.0f); 

      Ma::Vec2f vfoot; 
      {
         // Rot_foot - we're computing the ankle pos by ball pos

         Ma::Vec2f a; 
         An::Rot_foot (vfoot, An::Vec_ball_2_heel (a, wat.legs.L.bind), tgt_2d, -foot_lift_angle);  
      }
      Ik::Solve_2D (wat.legs.L.dyna.chain + 1, wat.legs.L.dyna.deltas, 2, vfoot); 
      
      Ma::Vec3f col; 
      Ma::Set (col, 1.0f, 0.75f, 0.5f);
      float accum_delta = 0.0;
      {            
         Ma::Vec3f  foot3;
         Ma::Vec2f foot2;
         Ma::Mat22f M;   

         // we have to manually compute the foot pos
         // pt3 - pt2 = length of the foot
         Ma::Sub (foot3, wat.legs.L.bind.points[3], wat.legs.L.bind.points[2]); 
         Ma::Set (foot2, Ma::Z(foot3), Ma::Y(foot3)); 

         wat.legs.L.dyna.deltas[2] = wat.legs.L.dyna.deltas[0] + wat.legs.L.dyna.deltas[1]; //  + wat.legs.L.dyna.deltas[2];
         wat.legs.L.dyna.deltas[2] = -foot_lift_angle - wat.legs.L.dyna.deltas[2] ;

         // accumulate from hip, knee, and ankle (but ankle is always 0 here)
         float foot_angle = wat.legs.L.dyna.deltas[0] + wat.legs.L.dyna.deltas[1] + wat.legs.L.dyna.deltas[2];
         //Ma::SetMatRotateZ (foot_mat, -foot_angle); 
         Ma::SetMatRotateZ (M, foot_angle); 

         // angle/heel
         Ma::Mult (wat.legs.L.dyna.chain[3], M, foot2); 
         Ma::Incr (wat.legs.L.dyna.chain[3], wat.legs.L.dyna.chain[2]);
         //
      }

      {
         // ball angle, assume ground is always at 0 incline
         wat.legs.L.dyna.deltas[3] = wat.legs.L.dyna.deltas[2] + wat.legs.L.dyna.deltas[1] + wat.legs.L.dyna.deltas[0]; 
         wat.legs.L.dyna.deltas[3] = -wat.legs.L.dyna.deltas[3];
      }



      // draw debug shapes
      for (int i = 0; i < 4; i++)
      {
         Ma::Vec3f a, b, v;

         Ma::Set (v, 0.0f, Ma::Y (wat.legs.L.dyna.chain[i]), Ma::X (wat.legs.L.dyna.chain[i])); 

         accum_delta +=  wat.legs.L.dyna.deltas[i]; 
         Ma::RotXAxis (mod_Rot, -accum_delta); 

         // draw sphere
         Rn::Draw_sphere(0.1f, col, v, q, view_Pos); 
         // draw axis
         Rn::Draw_axes  (0.4f, mod_Rot, v, q, view_Pos); 

         if (i) 
         { 
            Ma::Set (a, 0.0f, Ma::Y (wat.legs.L.dyna.chain[i-1]), Ma::X (wat.legs.L.dyna.chain[i-1]) );
            Ma::Set (b, 0.0f, Ma::Y (wat.legs.L.dyna.chain[i]),   Ma::X (wat.legs.L.dyna.chain[i])   );
            Rn::Draw_line (1.0f, col, a, b, q, view_Pos); 
         }
      }

      //
      windo->Swap_buffers (); 
   }

public: 

   IK_test (sy::System_context* sys)
   {}

   virtual ~IK_test ()
   {}

   //
   //
   virtual int	Initialize (sy::System_context* sys) 
   { 
      windo.reset (sys->Create_GraphicsWindow (this, "IK test", 800,600,false)); 
      accum_time = 0.0;
      Dt = 0.0; 
      tracker.Reset (sys); 

      init_scene (sys); 
      windo->Show (true); 
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

      Dt = tracker.GetDt (sys); 
      accum_time += Dt;
      update_input (sys); 
      update_anim (sys); 
      render (sys); 

      return 0; } 

   //
   //
   virtual void OnWindowResize	(int wd, int ht)  { 
      viewport_wd = wd; 
      viewport_ht = ht;
      glViewport (0, 0, viewport_wd ,viewport_ht); 
      }; 

   //
   //
   virtual void OnWindowClose () { }; 
   //
   //
   virtual void OnWindowActivate	(bool activate) { }; 

}; 

sy::RT_window_listener* Create_IK_test (sy::System_context* sys)
{
   return new IK_test (sys); 
}

//
//
int _tmain (int argv, char** argc)
   {
std::shared_ptr<sy::System_context>     sys (sy::Create_context ()); 
std::shared_ptr<sy::RT_window_listener>  rtt (Create_IK_test (sys.get())); 
sy::Run_realtime_task (sys.get(), rtt.get()); 
return 0; 
   }


