#include <Xp/Xp.h>
#include <Dx/Horde3D_platform.h>
#include <Dx/Scene.h>


const char* kForward_pipeline = "pipeline/forward.pipeline.xml";
const char* kUnderground = "model/Underground.geo.scene";
const char* kTrenchguy = "model/man/man.scene.xml";
//
// Cave_test 
class Cave_test 
	: H3D::Comp
	//, xp::BlPhy_Comp
	, public sy::RT_window_listener
{

	std::shared_ptr<sy::Graphics_window>   windo; 
   bool init_; 
public:

	Cave_test(sy::System_context* sys);
	virtual ~Cave_test(); 

	// RealtimeTask 
	virtual int		Initialize	(sy::System_context* sys); 
	virtual int		Deinitialize	(sy::System_context* sys); 
	virtual int		Update (sy::System_context* sys); 
	// Windows Listener 
	virtual void	OnWindowResize		(int wd, int ht) ; 
	virtual void	OnWindowClose		(); 
	virtual void	OnWindowActivate	(bool); 

	};

//
//
sy::RT_window_listener* Create_Cave_test (sy::System_context* sys)
{
	return new Cave_test (sys); 
}

//
//
Cave_test::~Cave_test()
{
}

//
void Cave_test::OnWindowResize (int wd, int ht) 
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
	//if (res_Table.count ("forwardPipe"))
	//{
	//	h3dResizePipelineBuffers( res_Table["forwardPipe"], wd , ht);
	//}

	// Set virtual camera parameters
	//h3dResizePipelineBuffers( _hdrPipeRes, wd, ht);
}

//
//
void Cave_test::OnWindowClose ()
{
}

//
//
void Cave_test::OnWindowActivate (bool)
{
}

//
//
Cave_test::Cave_test(sy::System_context* sys)
   : windo (), init_ (false)
{
   Initialize (sys); 
}

//
//
int Cave_test::Initialize (sy::System_context* sys)
{
   if (init_)
      return 1; 

      windo.reset (sys->Create_GraphicsWindow (this, "Cave_test", 800, 600, false)); 
	//platf_Wnd.reset (sys->Create_window (this, "Cave_test"));
	//displ_buff.reset (sys->Create_display (platf_Wnd.get())); 

	//platf_Wnd.reset (sys->Create_window ()); 
	//displ_buff.reset (sys->Create_displ_buff()); 

	// xp::Init_input (*this, sys, platf_Wnd.get()); 

	//	init_input (sys, displ_Wnd.get()); 

	const char* content_Dir  = "C:/Quarantine/awsum/Underground";

	if (h3dInit ())
	{
		// Set options
		h3dSetOption (H3DOptions::LoadTextures,		1);
		h3dSetOption (H3DOptions::TexCompression,	0);
		h3dSetOption (H3DOptions::FastAnimation,	0);
		h3dSetOption (H3DOptions::MaxAnisotropy,	4);
		h3dSetOption (H3DOptions::ShadowMapSize, 2048);

		// Add resources
		// Pipelines
      Sc::Add_resource( kForward_pipeline, Sc::Pipeline, 0);
		//res_Table["forwardPipe"]	= h3dAddResource (H3DResTypes::Pipeline, "pipeline/forward.pipeline.xml", 0);
		// scene
      Sc::Add_resource (kUnderground, Sc::SceneGraph, 0);
		//res_Table["Underground"]	= h3dAddResource (H3DResTypes::SceneGraph, kUnderground, 0);

      Sc::Add_resource (kTrenchguy, Sc::SceneGraph, 0);
		//res_Table["Man"]			= h3dAddResource (H3DResTypes::SceneGraph, kTrenchguy      , 0);

		// Load resources
		h3dutLoadResourcesFromDisk (content_Dir);

		// camera
      Sc::Create_camera ("camera", Sc::Get_resource (Sc::Pipeline, kForward_pipeline)); 

		//node_Table["camera"] = h3dAddCameraNode (H3DRootNode, "Camera", res_Table["forwardPipe"]); 

		h3dSetNodeTransform (
			node_Table["camera"], 
			320, 50.0, -305.0, // pos
			0, 0, 0, 		  // rot
			1.0f, 1.0f, 1.0f  // scl
			);

		obj_Table["camera"] = new H3D::Obj(node_Table["camera"]);
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

		// add scene
      Sc::Create_scene_obj( "Underground", kUnderground); 
		//node_Table["Underground"] = h3dAddNodes (H3DRootNode, res_Table["Underground"]); 
            
		// h3dSetNodeTransform (node_Table["Underground"], 0, 0, 0, 0, 0, 0, 1.0f, 1.0f, 1.0f );

      Sc::Create_scene_obj ("TrenchGuy", kTrenchguy); 
		//node_Table["Man"]		= h3dAddNodes (H3DRootNode, res_Table["Man"]);


		if (node_Table["Man"]) 
		{

			obj_Table["Man"] = new H3D::Obj (node_Table["Man"]);
			Ma::Vec3f man_Pos, man_Scl; 
			obj_Table["Man"]->Set_pos (Ma::Set (man_Pos, 46.f, 10.0f, -95.0f) ); 
			obj_Table["Man"]->Set_scl (Ma::Set (man_Scl, 60.f, 60.0f, 60.0f) ); 

			h3dSetNodeParamF (node_Table["Man"], H3DModel::LodDist1F, 0, 200.0f); 
			h3dSetNodeParamF (node_Table["Man"], H3DModel::LodDist2F, 0, 500.0f); 
			h3dSetNodeParamF (node_Table["Man"], H3DModel::LodDist3F, 0, 1000.0f); 
			Ma::Vec3f man_Min, man_Max; 

			h3dGetNodeAABB(node_Table["Man"],
				&Ma::X (man_Min),
				&Ma::Y (man_Min),
				&Ma::Z (man_Min),
				&Ma::X (man_Max),
				&Ma::Y (man_Max),
				&Ma::Z (man_Max)
				); 

			std::map<int, H3DNode>		bone_Map; 
			std::map<int, std::string>	bone_Names; 
			std::map<int, H3D::Obj*>		joint_Objs; 

			std::map<std::string, Ma::Vec3f> bone_Pos; 
			std::map<std::string, Ma::Vec3f> bone_Rot; 
			std::map<std::string, Ma::Vec3f> bone_Scl; 

			int num_Joints = h3dFindNodes (node_Table["Man"], 0, H3DNodeTypes::Joint); 
		
			for (int iJoint = 0; iJoint < num_Joints; iJoint++) {
				bone_Map[iJoint]	= h3dGetNodeFindResult (iJoint); 
				bone_Names[iJoint]	= h3dGetNodeParamStr (bone_Map[iJoint] , H3DNodeParams::NameStr); 
				joint_Objs[iJoint]	= new H3D::Obj (bone_Map[iJoint]);

				joint_Objs[iJoint]->Get_pos (bone_Pos[bone_Names[iJoint]]); 
				joint_Objs[iJoint]->Get_rot (bone_Rot[bone_Names[iJoint]]); 
				joint_Objs[iJoint]->Get_scl (bone_Scl[bone_Names[iJoint]]); 
				}

			std::map<H3DNode, H3DNode>			parent_Map; 
			std::map<std::string, std::string>	name_ChildParentMap; 
			std::map<std::string, int>			joint_Index;

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
		}
		


	}
	windo->Show (true); 
	// FLIP Blender Y & Z coords?? 
   init_ = true; 
	return 0; 
} 

//
//
int Cave_test::Deinitialize (sy::System_context* sys)
{
   init_ = false;
	return 0; 
}


//
//
int Cave_test::Update (sy::System_context* sys)
{
   sy::Mouse_state ms;
   sy::Keyboard_state ks; 
	{	// update view movement
      sys->Poll_input  (ms, ks); 
       
	
		Ma::Vec3f vPos, vRot; 

		obj_Table["camera"]->Get_pos (vPos);
		obj_Table["camera"]->Get_rot (vRot); 

		xp::Update_view_transform (vPos, vRot, 0.3f, ks, ms); 

		obj_Table["camera"]->Set_pos (vPos); 
		obj_Table["camera"]->Set_rot (vRot); 
	}

	// Render scene
	h3dRender(node_Table["camera"]);

	// Finish rendering of frame
	h3dFinalizeFrame();

	windo->Swap_buffers (); 
	
	return 0; 
} 


//C:\Quarantine\Hardcore;C:\usr\glew-1.9.0_x86\include;C:\usr\Horde3D\Horde3D\Bindings\C++;C:\usr\Horde3D_SDK\Horde3D\Source\Horde3DUtils;C:\usr\Horde3D\Horde3D\Source\Horde3DEngine;C:\usr\Horde3D_SDK\Horde3D\Source\Shared;C:\usr\DevIL-SDK_x86\include;$(OIS_ROOT)\includes;$(BULLET_SDK_ROOT)\include;$(VCInstallDir)include;$(VCInstallDir)atlmfc\include;$(WindowsSDK_IncludePath);$(IncludePath)

//C:\Quarantine\Hardcore;
//C:\usr\glew-1.9.0_x86\include;
//C:\usr\Horde3D_SDK\Horde3D\Bindings\C++;
//C:\usr\Horde3D_SDK\Horde3D\Source\Horde3DUtils;
//C:\usr\Horde3D_SDK\Horde3D\Source\Horde3DEngine;
//C:\usr\Horde3D_SDK\Horde3D\Source\Shared;$(IncludePath)