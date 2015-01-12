#include <Xp/Xp.h>

#include <Dx/Horde3D_platform.h>
#include <Dx/Scene.h>


static char kContent_root[]      = "C:/Quarantine/awsum/humanoid_test"; 

static char kForward_pipeline[]  = "pipelines/forward.pipeline.xml"; 
static char kDeferred_pipeline[] = "pipelines/deferred.pipeline.xml"; 
static char kWhatsMyName[]       = "models/hmnd/hmnd_04.scene.xml"; 
//static char kWhatsMyName[]       = "models/watsmyname.geo.scene"; 
static char kTrenchcoatGuy[]     = "models/man/man.scene.xml"; 
static char kWalkAnim[]          = "animations/man.anim";
static char kStage[]             = "models/Underground.geo.scene";
//
// App
class Hmnd_04
   // components
   : H3D::Comp         
   //, xp::BlPhy_Comp           
   // interfaces
   , public sy::RT_window_listener
{
		std::map<int, H3DNode>		   bone_Map; 
		std::map<int, std::string>	   bone_Names; 
		std::map<int, H3D::Obj*>   joint_Objs; 
      bool init_ ; 
      std::map<std::string, Sc::Obj*> obj_Map;

public:

   //
   //
   Hmnd_04 (sy::System_context* sys) : windy (0), init_ (false), _dticker (0.0)

   {
      Initialize (sys); 
   }
   
  
   //
   //
   virtual ~Hmnd_04 ()
   {
      for (H3D::Comp::ObjectTable::iterator i = obj_Table.begin (); i != obj_Table.end (); i++)
         delete i->second;
   }


	// RealtimeTask 
	//
	// Initialize :: SystemContex -> int 
   

	virtual int Initialize (sy::System_context* sys) 
	{
      if (init_)
         return 1; 

      // HWND
      windy.reset (sys->Create_GraphicsWindow (this, "H M N D - 04", 800, 600, false)); 


		// control/input
		//xp::Init_input (*this, sys, windy.get()); 

      // physics

      // audio

      // 

      bool enable_trenchguy = true; 
      bool enable_whatsmyname = true; 
                     
		if (h3dInit ())
		{



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
            Sc::Add_resource (kTrenchcoatGuy, Sc::SceneGraph, 0 );
            Sc::Add_resource (kWalkAnim     , Sc::Animation,  0 );
      	   //res_Table["trench_guy"]   = h3dAddResource( H3DResTypes::SceneGraph, kTrenchcoatGuy, 0 );
	         //res_Table["walk_anim"]    = h3dAddResource( H3DResTypes::Animation,  kWalkAnim, 0 );
            }

            if (enable_whatsmyname)
            {
            // test humanoid
            Sc::Add_resource ( kWhatsMyName, Sc::SceneGraph, 0 );
            //res_Table["watsmyname"]    = h3dAddResource( H3DResTypes::SceneGraph, kWhatsMyName, 0 );
            }

            // environment model  
            Sc::Add_resource ( kStage, Sc::SceneGraph, 0);
			   //res_Table["stage"]	      = h3dAddResource (H3DResTypes::SceneGraph, kStage, 0);

			   // Load resources
			   h3dutLoadResourcesFromDisk (kContent_root);
         }


			// build scene ( build scene graph
         {
            Ma::Vec3f v_0, v_1; 

            {
            // the setting/stage whatever
            Sc::Create_scene_obj ("stage", kStage); 
			   //node_Table["stage"]        = h3dAddNodes (H3DRootNode, res_Table["stage"]); 
            }


            if (enable_trenchguy)
            { // get this working first

            obj_Map["trench_guy"] = Sc::Create_scene_obj ("trench_guy", kTrenchcoatGuy); 
            Sc::Set_pos (obj_Map["trench_guy"], Ma::Set (v_0, -100.0f, 6.0f, 140.0f));
            Sc::Set_scl (obj_Map["trench_guy"], Ma::Set (v_0, 60.0f, 60.0f, 60.0f)); 

            //node_Table["trench_guy"]      = h3dAddNodes (H3DRootNode, res_Table["trench_guy"]); 
            //obj_Table[ "trench_guy" ]     = new (H3D::Obj) (node_Table["trench_guy"]);
            //obj_Table[ "trench_guy" ]->Set_pos (Ma::Set (v_0, -100.0f, 6.0f, 140.0f)); 
            //obj_Table[ "trench_guy" ]->Set_scl (Ma::Set (v_0, 60.0f, 60.0f, 60.0f)); 
            }
            
            if (enable_whatsmyname)
            { // our test model
            
            obj_Map["watsmyname"] = Sc::Create_scene_obj ("watsmyname", kWhatsMyName); 

            Sc::Set_pos (obj_Map["watsmyname"], Ma::Set (v_0, 100.0f, 64.0f, 140.0f));
            Sc::Set_scl (obj_Map["watsmyname"], Ma::Set (v_0, 10.0f, 10.0f, 10.0f)); 

            // node_Table["watsmyname"]   = h3dAddNodes (H3DRootNode, res_Table["watsmyname"]); 
            // obj_Table[ "watsmyname" ]  = new (H3D::Obj) (node_Table["watsmyname"]);
            // obj_Table["watsmyname"]->Set_pos (Ma::Set (v_0, 100.0f, 64.0f, 140.0f)); 
            // obj_Table["watsmyname"]->Set_scl (Ma::Set (v_0, 10.0f, 10.0f, 10.0f)); 
            }

			   { // camera
            obj_Map["camera"] = Sc::Create_camera ("camera", Sc::Get_resource (Sc::Pipeline, kForward_pipeline)); 
            Sc::Set_pos (obj_Map["camera"] , Ma::Set (v_0, 0.0f, 80.0f, -10.0f)); 
            //node_Table["camera"]       = h3dAddCameraNode (H3DRootNode, "Camera", res_Table["forward_pipe"]); 
			   //obj_Table["camera"]        = new (H3D::Obj) (node_Table["camera"]);
            //obj_Table["camera"]->Set_pos (Ma::Set (v_0, 0.0f, 80.0f, -10.0f)); 
            }
            
            
            {
		      // Add light source
		      node_Table["light"]= h3dAddLightNode( H3DRootNode, "Light1", 0, "LIGHTING", "SHADOWMAP");

		      h3dSetNodeTransform (node_Table["light"], 0, 1200.0, 0, -90, 0, 0, 1, 1, 1);
		      h3dSetNodeParamF	(node_Table["light"], H3DLight::RadiusF, 0, 2000);
		      h3dSetNodeParamF	(node_Table["light"], H3DLight::FovF, 0, 90);
		      h3dSetNodeParamI	(node_Table["light"], H3DLight::ShadowMapCountI, 1);
		      h3dSetNodeParamF	(node_Table["light"], H3DLight::ShadowMapBiasF, 0, 0.01f);
		      h3dSetNodeParamF	(node_Table["light"], H3DLight::ColorF3, 0, 1.0f);
		      h3dSetNodeParamF	(node_Table["light"], H3DLight::ColorF3, 1, 0.8f);
		      h3dSetNodeParamF	(node_Table["light"], H3DLight::ColorF3, 2, 1.0f);
		      h3dSetNodeParamF	(node_Table["light"], H3DLight::ColorMultiplierF, 0, 1.0f);
		      obj_Table["light"] = new H3D::Obj (node_Table["light"]);

            }

         }


      //
		if (node_Table["watsmyname"]) 
		{

			Ma::Vec3f man_Min, man_Max; 

			h3dGetNodeAABB(node_Table["watsmyname"],
				&Ma::X (man_Min),
				&Ma::Y (man_Min),
				&Ma::Z (man_Min),
				&Ma::X (man_Max),
				&Ma::Y (man_Max),
				&Ma::Z (man_Max)
				); 

			//std::map<int, H3DNode>		   bone_Map; 
			//std::map<int, std::string>	   bone_Names; 
			//std::map<int, H3D::Obj*>   joint_Objs; 

			std::map<std::string, Ma::Vec3f> bone_Pos; 
			std::map<std::string, Ma::Vec3f> bone_Rot; 
			std::map<std::string, Ma::Vec3f> bone_Scl; 

			int num_Joints = h3dFindNodes (node_Table["watsmyname"], 0, H3DNodeTypes::Joint); 
		
         Ma::Vec3f obj_pos, obj_rot; 

         obj_Table["watsmyname"]->Get_pos (obj_pos); 
         obj_Table["watsmyname"]->Get_rot (obj_rot); 
         
			for (int iJoint = 0; iJoint < num_Joints; iJoint++) {
				bone_Map[iJoint]	= h3dGetNodeFindResult (iJoint); 
				bone_Names[iJoint]	= h3dGetNodeParamStr (bone_Map[iJoint] , H3DNodeParams::NameStr); 
				joint_Objs[iJoint]	= new H3D::Obj (bone_Map[iJoint]);

				joint_Objs[iJoint]->Get_pos (bone_Pos[bone_Names[iJoint]]); 
				joint_Objs[iJoint]->Get_rot (bone_Rot[bone_Names[iJoint]]); 
				joint_Objs[iJoint]->Get_scl (bone_Scl[bone_Names[iJoint]]); 
				}

			std::map<H3DNode, H3DNode>			   parent_Map; 
			std::map<std::string, std::string>	name_ChildParentMap; 
			std::map<std::string, int>			   joint_Index;

			{	// std::shared_ptr<FILE> f ( fopen ("c:/usr/man_dump.dat", "wb"), fclose ); 
				for (int iJoint = 0; iJoint < num_Joints; iJoint++)
				{
					parent_Map[ bone_Map[iJoint]] = h3dGetNodeParent (bone_Map[iJoint]); 
				 
					std::string child_Name	= h3dGetNodeParamStr (bone_Map[iJoint] , H3DNodeParams::NameStr); 
					std::string parent_Name = h3dGetNodeParamStr (parent_Map[ bone_Map[iJoint]] , H3DNodeParams::NameStr);

					name_ChildParentMap[child_Name] = parent_Name; 

					joint_Index[child_Name] = h3dGetNodeParamI (bone_Map[iJoint], H3DJoint::JointIndexI); 
               
					//fwrite (bone_Pos[bone_Names[iJoint]].ptr(), sizeof(float), 3, f.get()); 
					//fwrite (bone_Rot[bone_Names[iJoint]].ptr(), sizeof(float), 3, f.get()); 
					//fwrite (bone_Scl[bone_Names[iJoint]].ptr(), sizeof(float), 3, f.get()); 		
				}
			}

         Ma::Vec3f v_0; 
         
         //joint_Objs[joint_Index["head"]]->Set_rot (Ma::Set (v_0, 0.0f, 0.0f, 45.0f)); 
         //joint_Objs[joint_Index["arm_lower_left"]]->Set_rot (Ma::Set (v_0, 0.0f, 0.0f, 45.0f)); 
         //joint_Objs[joint_Index["arm_lower_right"]]->Set_rot (Ma::Set (v_0, 0.0f, 0.0f, 45.0f)); 


         bone_Map.size (); 
		   //h3dSetNodeParamF (node_Table["watsmyname"], H3DModel::LodDist1F, 0, 500.0f); 
		   //h3dSetNodeParamF (node_Table["watsmyname"], H3DModel::LodDist2F, 0, 5000.0f); 
		   //h3dSetNodeParamF (node_Table["watsmyname"], H3DModel::LodDist3F, 0, 10000.0f); 
		}

		
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
		}

      windy->Show (true); 
		init_ = true;
      return 0;
	}

	//
	// Exit :: SystemContext* -> int 
	virtual int Deinitialize (sy::System_context* sys) {
	   h3dRelease (); 
      init_ = false;
      return 0;
		}

	//
	// Update :: SystemContext* -> int
	virtual int Update (sy::System_context* sys) 
	{
		update_input (sys); 
		//
		update_dynamics (sys); 
		// 
		render (sys); 
		
		return 0;
   }

   // OnWindowResize :: wd -> ht -> void
   void OnWindowResize (int wd, int ht) 
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

      Sc::Resize_render_target (Sc::Get_resource (Sc::Pipeline, kForward_pipeline), wd, ht); 
		//if (res_Table.count ("forward_pipe"))
		//	h3dResizePipelineBuffers (res_Table.at ("forward_pipe"), wd , ht);
   }

	//
	// OnWindowClose :: void 
	virtual void OnWindowClose ()
	{

	}

	//
	// OnWindowActivate :: bool -> void 
	virtual void OnWindowActivate	(bool)
	{

	}

private: 


   //
	void update_input (sy::System_context* sys)
   {
      sy::Mouse_state ms; 
      sy::Keyboard_state ks; 
      sys->Poll_input (ms, ks); 

      //if (sy::Is_keydown (sy::Key_Q, ks)) {
      //   sys->SysReq (sy::System_context::Req_Exit); 
      //   return; 
      //   }   


	   {	// update view movement
	
		   Ma::Vec3f vPos, vRot; 
         // get current transform
		   obj_Table["camera"]->Get_pos (vPos);
		   obj_Table["camera"]->Get_rot (vRot); 

         // compute new transform
		   xp::Update_view_transform (vPos, vRot, 0.3f, ks, ms); 

         // set current transform
		   obj_Table["camera"]->Set_pos (vPos); 
		   obj_Table["camera"]->Set_rot (vRot); 
	   }
   }

   double _dticker; 
   //
	void update_dynamics (sy::System_context* sys)
   {
      _dticker += 0.1; 
      Ma::Vec3f v_0;
      {  // trench_guy
         obj_Table["trench_guy"]->Get_rot (v_0); 
         Ma::Y (v_0) += 1.0f; 
         obj_Table["trench_guy"]->Set_rot (v_0); 
      }

      {  // watsmyname 
         obj_Table["watsmyname"]->Get_rot (v_0); 
         Ma::Y (v_0) += 1.0f; 
         obj_Table["watsmyname"]->Set_rot (v_0); 

         float _rotz = 45.0f * cos (_dticker); 
         joint_Objs[6]->Set_rot (Ma::Set (v_0, 0.0f, 0.0f,  _rotz )); 
         joint_Objs[8]->Set_rot (Ma::Set (v_0, 0.0f, 0.0f,  _rotz )); 
         joint_Objs[12]->Set_rot (Ma::Set (v_0, 0.0f, 0.0f,  _rotz )); 
         //joint_Objs[joint_Index["arm_lower_left"]]->Set_rot (Ma::Set (v_0, 0.0f, 0.0f, 45.0f)); 
         //joint_Objs[joint_Index["arm_lower_right"]]->Set_rot (Ma::Set (v_0, 0.0f, 0.0f, 45.0f)); 

      }      


   } 

   //
   void render (sy::System_context* sys)
   {

  		// Render scene
		h3dRender(node_Table["camera"]);
		// Finish rendering of frame
		h3dFinalizeFrame();
      //	
		windy->Swap_buffers (); 
   }

   // 
	std::shared_ptr<sy::Graphics_window>	windy; 
};

static double _dticker; 


sy::RT_window_listener* Create_Hmnd_04_test  (sy::System_context* sys)
{
   sy::RT_window_listener* ret = new Hmnd_04 (sys) ; 
   return ret ; 
}
