// test_SDL_integration.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"


#include "Horde3D.h"
#include "Horde3DUtils.h"

#include <Dx/Dx.h>
#include <Xp/Xp.h>

#include <Dx/IKSolver2d.h> 
#include <Dx/Anim_biped.h> 

#include <Dx/Horde3D_platform.h>
 

namespace HO_armature_hmnd 
{
   const char* kJointNames[] = {
      "root", 
      "root.001", 
      "spine.001", 
      "spine.002", 
      "spine.003", 
      "neck", 
      "head", 
      "shoulder.left", 
      "arm.upper.left", 
      "arm.lower.left", 
      "hand.left", 
      "shoulder.right", 
      "arm.upper.right", 
      "arm.lower.right", 
      "hand.right", 
      "hip.left", 
      "leg.upper.left", 
      "leg.lower.left", 
      "foot.left", 
      "toe.left", 
      "hip.right", 
      "leg.upper.right", 
      "leg.lower.right", 
      "foot.right", 
      "toe.right", 
      }; 

   const size_t kNum_joints = El_count (kJointNames); 

   //StandIdle : public Movement_state 
   //WalkRelaxed: public Movement_state 
   //WalkStrong: public Movement_state 
   //RunLight: public Movement_state 
   //RunHard: public Movement_state 
   //CombatRight: public Movement_state 
   //Sneak: public Movement_state 
   //Skip : public Movement_state  


}; 



   typedef std::map<std::string, H3DNode> H3DNodeMap ;

//
class test_SDL_integration 
   : public sy::RT_window_listener
   //
   , H3D::Comp         
   // , xp::BlPhy_Comp 
{

	//std::map<int, H3DNode>		   bone_Map; 
	//std::map<int, std::string>	   bone_Names; 
	//std::map<int, H3D::Obj*>   joint_Objs; 
   Sc::Obj::Arr bone_arr; 		
   Sc::Obj::Map bone_map; 
   
   //Sc::Obj::Map               bone_Node; 
   //std::map<std::string, int> bone_Index; 
   //H3D::Comp::ObjectTable      bone_Objs; 
   std::map<std::string, Sc::Obj*> obj_Map;

   bool init_ ; 

   std::shared_ptr<sy::Graphics_window>    windo; 

   An::Biped wat; 

   Ma::Vec3f   fwd_pos_R, rear_pos_R,
               fwd_pos_L, rear_pos_L; 
   //
   //

   void init_scene ()
   {

      bool enable_trenchguy = true; 
      bool enable_whatsmyname = true; 

      // create the horde scene
      if (h3dInit ())
      {		// Set options

		   static char kContent_root[]      = "C:/Quarantine/awsum/humanoid_test"; 

         static char kForward_pipeline[]  = "pipelines/forward.pipeline.xml"; 
         static char kDeferred_pipeline[] = "pipelines/deferred.pipeline.xml"; 
         //static char kWhatsMyName[]       = "models/hmnd/hmnd_04.scene.xml"; 
         static char kWhatsMyName[]       = "models/watsmyname.geo.scene"; 
         static char kTrenchcoatGuy[]     = "models/man/man.scene.xml"; 
         static char kWalkAnim[]          = "animations/man.anim";
         static char kStage[]             = "models/Underground.geo.scene";

			// Set options
			h3dSetOption (H3DOptions::LoadTextures,   1);
			h3dSetOption (H3DOptions::TexCompression,	0);
			h3dSetOption (H3DOptions::FastAnimation,	1);
			h3dSetOption (H3DOptions::MaxAnisotropy,	4);
			h3dSetOption (H3DOptions::ShadowMapSize,  2048);

			// setup pipeline/resources
         {
            // a forward renderer
            Sc::Add_resource (kForward_pipeline , Sc::Pipeline, 0);
            //res_Table["forward_pipe"]  = h3dAddResource (H3DResTypes::Pipeline, kForward_pipeline , 0);

            if (enable_trenchguy)
            {
               // trench coat man
               Sc::Add_resource (kTrenchcoatGuy,  Sc::SceneGraph, 0 );
               Sc::Add_resource (kWalkAnim      , Sc::Animation,  0 );
      	      // res_Table["trench_guy"]   = h3dAddResource( H3DResTypes::SceneGraph, kTrenchcoatGuy, 0 );
	            // res_Table["walk_anim"]    = h3dAddResource( H3DResTypes::Animation,  kWalkAnim, 0 );
            }

            if (enable_whatsmyname)
            {
               // test humanoid
               Sc::Add_resource (kWhatsMyName, Sc::SceneGraph, 0 );
               //res_Table["watsmyname"]    = h3dAddResource( H3DResTypes::SceneGraph, kWhatsMyName, 0 );
            }

            // environment model  
            Sc::Add_resource (kStage, Sc::SceneGraph, 0);
			   //res_Table["stage"]	      = h3dAddResource (H3DResTypes::SceneGraph, kStage, 0);

			   // Load resources
			   h3dutLoadResourcesFromDisk (kContent_root);
         }


			// build scene ( build scene graph
         {
            Ma::Vec3f v_0, v_1; 

            {
            // the setting/stage whatever
            obj_Map["stage"] = Sc::Create_scene_obj ("stage", kStage);  
			   //node_Table["stage"]        = h3dAddNodes (H3DRootNode, res_Table["stage"]); 
            }


            if (enable_trenchguy)
            { // get this working first
            //node_Table["trench_guy"]      = h3dAddNodes (H3DRootNode, res_Table["trench_guy"]); 
            //obj_Table[ "trench_guy" ]     = new (H3D::Obj) (node_Table["trench_guy"]);
            obj_Map["trench_guy" ] = Sc::Create_scene_obj ("trench_guy", kTrenchcoatGuy);    
            
            Sc::Set_pos (obj_Map[ "trench_guy" ] , Ma::Set (v_0, -100.0f, 6.0f, 140.0f)); 
            Sc::Set_scl (obj_Map[ "trench_guy" ] , Ma::Set (v_0, 60.0f, 60.0f, 60.0f)); 
            }
            
            if (enable_whatsmyname)
            { // our test model
            const float scale_ = 10.0f; 

            obj_Map["watsmyname"] = Sc::Create_scene_obj ("watsmyname", kWhatsMyName); 
            //node_Table["watsmyname"]   = h3dAddNodes (H3DRootNode, res_Table["watsmyname"]); 
            //obj_Map[ "watsmyname" ]  = new (H3D::Obj) (node_Table["watsmyname"]);
            Sc::Set_pos (obj_Map["watsmyname"], Ma::Set (v_0, 100.0f, 64.0f, 140.0f)); 
            Sc::Set_scl (obj_Map["watsmyname"], Ma::Set (v_0, scale_, scale_, scale_)); 

            }

			   { // camera
            //node_Table["camera"]       = h3dAddCameraNode (H3DRootNode, "Camera", res_Table["forward_pipe"]); 
			   //obj_Table["camera"]        = new (H3D::Obj) (node_Table["camera"]);
            obj_Map["camera"] = Sc::Create_camera ("Camera", Sc::Get_resource (Sc::Pipeline, kForward_pipeline));             
            Sc::Set_pos (obj_Map["camera"], Ma::Set (v_0, 0.0f, 80.0f, -10.0f)); 
            }
            
            
            {
            Ma::Col3f col; 
            Ma::Set (col, 1.0f, 0.8f, 1.0f);
		      // Add light source
		      // node_Table["light"]= h3dAddLightNode( H3DRootNode, "Light1", 0, "LIGHTING", "SHADOWMAP");
		      // h3dSetNodeTransform (node_Table["light"], 0, 1200.0, 0, -90, 0, 0, 1, 1, 1);
		      // h3dSetNodeParamF	(node_Table["light"], H3DLight::RadiusF, 0, 2000);
		      // h3dSetNodeParamF	(node_Table["light"], H3DLight::FovF, 0, 90);
		      // h3dSetNodeParamI	(node_Table["light"], H3DLight::ShadowMapCountI, 1);
		      // h3dSetNodeParamF	(node_Table["light"], H3DLight::ShadowMapBiasF, 0, 0.01f);
		      // h3dSetNodeParamF	(node_Table["light"], H3DLight::ColorF3, 0, 1.0f);
		      // h3dSetNodeParamF	(node_Table["light"], H3DLight::ColorF3, 1, 0.8f);
		      // h3dSetNodeParamF	(node_Table["light"], H3DLight::ColorF3, 2, 1.0f);
		      // h3dSetNodeParamF	(node_Table["light"], H3DLight::ColorMultiplierF, 0, 1.0f);
		      // obj_Table["light"] = new H3D::Obj (node_Table["light"]);
            obj_Map["light"] = Sc::Create_light ("Light1", 0, "LIGHTING", "SHADOWMAP"); 
            Sc::Light_params (obj_Map["light"], 2000.0f, 90.0, 1, 0.01f, col, 1.0f); 


            }

         }


      

      // 
		if (node_Table["watsmyname"]) 
		{
			int num_Joints = h3dFindNodes (node_Table["watsmyname"], 0, H3DNodeTypes::Joint); 

         bone_arr.resize (num_Joints); 		

         Ma::Vec3f obj_pos, obj_rot; 

         obj_Table["watsmyname"]->Get_pos (obj_pos); 
         obj_Table["watsmyname"]->Get_rot (obj_rot); 
         
//Sc::Create_scene_obj (
         //   Sc::Get_skeleton_arr (
			for (int iJoint = 0; iJoint < num_Joints; iJoint++) {
            H3DNode     bone_node = h3dGetNodeFindResult (iJoint);
            std::string bone_name = h3dGetNodeParamStr (bone_node, H3DNodeParams::NameStr); 
            int         joint_ind = h3dGetNodeParamI (bone_node , H3DJoint::JointIndexI); 

            Sc::Bind_node (&bone_arr[joint_ind], bone_node); 
            bone_map[bone_name] = &bone_arr[joint_ind];

            //bone_Node [bone_name] = bone_node;
            //bone_Index[bone_name] = h3dGetNodeParamI (bone_node , H3DJoint::JointIndexI); 
            //bone_Objs [bone_name] = new H3D::Obj (bone_node);

				}

         An::Compute_leg_params (wat.L.bind , wat.R.bind , bone_map);

         for (int i = 0; i < 4; i++)
         {
            Ma::Set (wat.L.dyna.chain[i] , Ma::Z(wat.L.bind.points[i]), Ma::Y (wat.L.bind.points[i])); 
            Ma::Set (wat.R.dyna.chain[i] , Ma::Z(wat.R.bind.points[i]), Ma::Y (wat.R.bind.points[i])); 

            wat.L.dyna.deltas[i] = 0.0f; 
            wat.R.dyna.deltas[i] = 0.0f; 
         }

         // + 1, because we do not pass in the fixed origin, assumed to be at <0>
         // target same heel position to compute bind angles, 
         // Ik::Solve_2D (wat.L.dyna.chain + 1, wat.L.dyna.deltas, 2, wat.L.dyna.chain[2]); 
         // Ik::Solve_2D (wat.R.dyna.chain + 1, wat.R.dyna.deltas, 2, wat.R.dyna.chain[2]); 

         //for (int i = 0; i < 4; i++)
         //{
         //wat.L.bind.deltas[i] = wat.L.dyna.deltas[i];
         //wat.R.bind.deltas[i] = wat.R.dyna.deltas[i];
         //}
         //
         //// just set heel and foot at zero for now
         //wat.L.bind.deltas[2] = wat.L.bind.deltas[3] = 0.0f;
         //wat.R.bind.deltas[2] = wat.R.bind.deltas[3] = 0.0f;


         Ma::Vec3f hip_pos, heel_pos, leg_len, hgt_diff;  
         Ma::Vec3f fwd_dist, bak_dist;

         Ma::Set (fwd_dist, 0.0f, 0.0f,  4.0f); 
         Ma::Set (bak_dist, 0.0f, 0.0f, -20.0f); 

         //An::Abs_node_pos (hip_pos, "leg.upper.left", bone_Node); 
         //An::Abs_node_pos (heel_pos, "foot.left", bone_Node);      

         Ma::Sub (leg_len, heel_pos, hip_pos); 
         Ma::Scale (hgt_diff, leg_len, 0.98f); 
         Ma::Scale (hgt_diff, leg_len, 0.94f); 

         Ma::Add (fwd_pos_L, hgt_diff, fwd_dist); 
         Ma::Add (rear_pos_L, hgt_diff, bak_dist); 

         //An::Abs_node_pos (hip_pos, "leg.upper.right", bone_Node); 
         //An::Abs_node_pos (heel_pos, "foot.right", bone_Node); 

         Ma::Sub (leg_len, heel_pos, hip_pos); 
         Ma::Scale (hgt_diff, leg_len, 0.95f); 

         Ma::Add (fwd_pos_R, hgt_diff, fwd_dist); 
         Ma::Add (rear_pos_R, hgt_diff, bak_dist); 

		}

      resize_render_targets (800, 600); 

      }
   }

   //
   //
   void update_input (sy::System_context* sys)
   {
      // 
      sy::Mouse_state      ms; 
      sy::Keyboard_state   ks;    
      sys->Poll_input (ms, ks); 

      if (sy::Is_keydown (sy::SC_Q, ks))
      {
         sys->SysReq (sy::System_context::Req_Exit); 
         return; 
      }

		{	
         // update view movement
			Ma::Vec3f vPos, vRot; 
			obj_Table["camera"]->Get_pos (vPos);
			obj_Table["camera"]->Get_rot (vRot); 

         static float move_rate = 1.0f;

			xp::Update_view_transform (vPos, vRot, move_rate, ks,  ms); 

			obj_Table["camera"]->Set_pos (vPos); 
			obj_Table["camera"]->Set_rot (vRot); 
		}


   }

   //
   //
   double _dticker; 
   //
	void update_dynamics (sy::System_context* sys)
   {
      _dticker += 0.02; 
      Ma::Vec3f v_0;
      {  // trench_guy
         obj_Table["trench_guy"]->Get_rot (v_0); 
         Ma::Y (v_0) += 1.0f; 
         obj_Table["trench_guy"]->Set_rot (v_0); 
      }

      {  // watsmyname 
         // obj_Table["watsmyname"]->Get_rot (v_0); 
         // Ma::Y (v_0) += 0.2f; 
         // obj_Table["watsmyname"]->Set_rot (v_0); 

         float _rotza = 40.0f * cosf (3.0f * float(_dticker)) ; 
         float _rotzb = 40.0f * sinf (3.0f * float (_dticker + Ma::Pi)) ; 
         

         Sc::Set_rot (bone_map["neck"], Ma::Set (v_0, 0.1f * _rotza, 0.0f, 0.0f)); 
         Sc::Set_rot (bone_map["head"], Ma::Set (v_0, 0.2f * _rotza, 0.0f, 0.0f)); 
         // arms
         // fwd_pos_R, rear_pos_R,
         // fwd_pos_L, rear_pos_L;
         
         {  
            Ma::Vec3f v_, dir_;
            Ma::Vec2f ball_L, heel_L, v_foot; //, targ_R; 

   
            float t_ = 0.5 * cosf (2.0f * float(_dticker) + float(Ma::Pi)) + 0.5f; 
            //float foot_ang_ = 0.5 * cosf (1.0f * _dticker ) + 0.5; 
            //float t_ = 0.1 ; // * cosf (1.0f * _dticker + Ma::Pi) + 0.5f; 
            float foot_ang_ = 0.5f * cosf (5.0f * _dticker ) + 0.5; 
            float foot_ang_degr_ = Ma::Rads2Degr (foot_ang_); 

            foot_ang_ = 0.0f;


            Ma::Sub (dir_, fwd_pos_L, rear_pos_L); 
            Ma::Scale (v_, dir_, t_); 
            Ma::Incr(v_, rear_pos_L); 

            Ma:Set (ball_L, Ma::Z(v_), Ma::Y(v_)); 

            //An::Calc_heel_pos (v_foot, wat.L.bind, foot_ang_); 
            
            Ma::Add (heel_L, ball_L, v_foot); 

            {  // IK solved in yz-space 
               // theta negative because coords applied in xy-space
               Ik::Solve_2D (wat.L.dyna.chain + 1, wat.L.dyna.deltas, 2, heel_L); 
               
               float hip_angl = Ma::Rads2Degr (wat.L.dyna.deltas[0]); 
               Sc::Set_rot (bone_map["leg.upper.left"], Ma::Set (v_0, -hip_angl, 0.0f, 0.0f)); 


               float knee_angl = Ma::Rads2Degr (wat.L.dyna.deltas[1]);  
               Sc::Set_rot (bone_map["leg.lower.left"], Ma::Set (v_0, -knee_angl, 0.0f, 0.0f));

               //float foot_ang= // Ma::Rads2Degr (wat.L.dyna.deltas[1]);  
               
               float ankle_ang = hip_angl + knee_angl + Ma::Rads2Degr (foot_ang_);
               Sc::Set_rot (bone_map["foot.left"], Ma::Set (v_0, ankle_ang , 0.0f, 0.0f));

               float ball_ang = - Ma::Rads2Degr (foot_ang_);
                  //float knee_angl = Ma::Rads2Degr (wat.L.dyna.deltas[1]);  
               Sc::Set_rot (bone_map["toe.left"], Ma::Set (v_0, ball_ang , 0.0f, 0.0f));

            }
         }

         // joint_Objs[8]->Get_rot (v_0); 
         // joint_Objs[8]->Set_rot (Ma::Set (v_0, 0.0f, 0.0f, _rotza + 15.0f)); 
         // 
         // joint_Objs[12]->Get_rot(v_0); 
         // joint_Objs[12]->Set_rot(Ma::Set (v_0, 0.0f, 0.0f, _rotzb - 30.0f)); 
         // 
         // joint_Objs[16]->Set_rot (Ma::Set (v_0, _rotzb - 30.0f, 0.0f, 0.0f)); 
         // joint_Objs[17]->Set_rot (Ma::Set (v_0, 30.0f - _rotzb, 0.0f, 0.0f)); 
         // joint_Objs[18]->Set_rot (Ma::Set (v_0, 30.0f - _rotzb, 0.0f, 0.0f)); 
         // 
         // joint_Objs[21]->Set_rot (Ma::Set (v_0, -_rotzb - 30.0f, 0.0f, 0.0f));
         // joint_Objs[22]->Set_rot (Ma::Set (v_0, 30.0f - _rotzb, 0.0f, 0.0f));
         // joint_Objs[23]->Set_rot (Ma::Set (v_0, 30.0f - _rotzb, 0.0f, 0.0f));

         //joint_Objs[joint_Index["arm_lower_left"]]->Set_rot (Ma::Set (v_0, 0.0f, 0.0f, _rotz )); 
         //joint_Objs[joint_Index["arm_lower_right"]]->Set_rot (Ma::Set (v_0, 0.0f, 0.0f, _rotz )); 
      }      
   }

   //
   //
   void resize_render_targets (int wd, int ht)
   {
		// Resize viewport
		if (node_Table.count ("camera")) 
		{
			h3dSetNodeParamI(node_Table["camera"], H3DCamera::ViewportXI, 0 );
			h3dSetNodeParamI(node_Table["camera"], H3DCamera::ViewportYI, 0 );
			h3dSetNodeParamI(node_Table["camera"], H3DCamera::ViewportWidthI, wd );
			h3dSetNodeParamI(node_Table["camera"], H3DCamera::ViewportHeightI, ht);

			h3dSetupCameraView(node_Table["camera"], 60.0f, (float) wd / (float) ht, 1.0f, 5000.0f );
		}
    
      
      // resize render targets
		if ( Sc::Get_resource (Sc::Pipeline, "forward_pipe") )
			h3dResizePipelineBuffers (Sc::Get_resource (Sc::Pipeline, "forward_pipe"), wd , ht);
   }

   //
   //
   void render ()
   {
  		// Render scene
		h3dRender(node_Table["camera"]);
		// Finish rendering of frame
		h3dFinalizeFrame();
      //	
		windo->Swap_buffers ();    
   }

public: 

   test_SDL_integration () : init_ (false), _dticker(0.0) {
   } 

   virtual ~test_SDL_integration () {
   }

	virtual int Initialize (sy::System_context* sys) { 
   if (init_) 
      return 0; 
   windo.reset (sys->Create_GraphicsWindow (this, "test SDL2", 800, 600, false)); 
   init_scene (); 
   // xp::Init_Bullet (*this); 
   windo->Show (true); 
   init_ = true; 
   return 0; 
   }
    
   //
	virtual int Deinitialize (sy::System_context* sys) { 
   init_ = false; 
   h3dRelease (); 
   return 0; 
   }
	
   //
   virtual int Update (sy::System_context* sys) { 
   update_input (sys);
   update_dynamics (sys);
   render (); 
   return 0; 
   } 
   
   //
	virtual void OnWindowResize (int wd, int ht) {
   resize_render_targets (wd, ht); 
   }; 

   //
	virtual void OnWindowClose () {
   } 

   //
	virtual void OnWindowActivate	(bool activate)
   {
   }



}; 


//
//
int _tmain (int argc, _TCHAR* argv[])
{

   std::shared_ptr<sy::System_context>  sys (sy::Create_context ()); 
   std::shared_ptr<sy::RT_Task>   rtt (new test_SDL_integration); 
   
   sy::Run_realtime_task (sys.get(), rtt.get()); 

	return 0;
}

