#include "stdafx.h"
//#include "Ungr_00.h"
#include <string>

#include <Dx/Dx.h>
#include <Xp/Xp.h>

#include <Dx/Horde3D_platform.h>

const char kForward_pipeline[] = "pipeline/forward.pipeline.xml"; 



// NAMESPACE BEGIN
namespace Ungr_00  {


	class Runnable
      // isa 
		//: xp::OIS_Comp
		: H3D::Comp
		//, xp::BlPhy_Comp
      // interfaces
      , public sy::RT_window_listener
	{
   std::shared_ptr<sy::Graphics_window>	windo; 
   bool                                   init_; 


public: 

	// constr
	Runnable (sy::System_context* sys) 
      : windo ()
      , init_ (false)
   {
      Initialize (sys); 
	}

	// ~Runnable 
	virtual ~Runnable ()
	{
	}

	// RealtimeTask 
	//
	// METHOD :: Enter -> SystemContex -> int 
	int Runnable::Initialize (sy::System_context* sys) 
	{
      if (init_)
         return 1; 
      // BEGIN 
		// display/graphics

      windo.reset (sys->Create_GraphicsWindow (this, "test Ungr_00", 800, 600, false)); 
		//platf_Wnd.reset (sys->Create_window (this, "test Ungr_00")); 
		//displ_Buf.reset (sys->Create_display(platf_Wnd.get())); 

		// control/input
		//xp::Init_input (*this, sys, platf_Wnd.get()); 


		if (h3dInit ())
		{
		   const char content_dir[] = "C:/Quarantine/awsum/Underground";

			// Set options
			h3dSetOption (H3DOptions::LoadTextures,		1);
			h3dSetOption (H3DOptions::TexCompression,	0);
			h3dSetOption (H3DOptions::FastAnimation,	0);
			h3dSetOption (H3DOptions::MaxAnisotropy,	4);
			h3dSetOption (H3DOptions::ShadowMapSize, 2048);

			// Pipelines/resources
         Sc::Add_resource (kForward_pipeline, Sc::Pipeline, 0); 

			//res_Table["pipeline"]	= h3dAddResource (H3DResTypes::Pipeline, kForward_pipeline , 0);

			// scene
			//res_Table["Undgr"]	   = h3dAddResource (H3DResTypes::SceneGraph, "model/ung04x04.geo.scene", 0);
         Sc::Add_resource ("model/ung04x04.geo.scene", Sc::SceneGraph, 0);

			// Load resources
			h3dutLoadResourcesFromDisk (content_dir);

			// camera
			node_Table["camera"] = h3dAddCameraNode (H3DRootNode, "Camera", Sc::Get_resource (Sc::Pipeline, kForward_pipeline)); 

         //
			h3dSetNodeTransform (
				node_Table.at("camera"), 
				0, 10.0, -5.0, // pos
				0, 0, 0, 		  // rot
				1.0f, 1.0f, 1.0f  // scl
				);
		   
			obj_Table   [ "camera" ] 
            = 
            new (H3D::Obj) (node_Table["camera"]);

			// add scene
			node_Table["Undgr"] = h3dAddNodes (H3DRootNode, Sc::Get_resource (Sc::SceneGraph, "model/ung04x04.geo.scene")); 


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

	
		windo->Show (true); 
      return 0;
	}

	//
	// METHOD Exit	:: SystemContext* -> int 
	int Runnable::Deinitialize	(sy::System_context* sys) {
	   h3dRelease (); 
		return 0;
   }

	//
	// METHOD Update :: SystemContext* -> int
	int Runnable::Update (sy::System_context* sys) 
	{
	
		// animation/dynamics
		{
		}


		// Render scene
		// h3dRender(node_Table["camera"]);
		// 
		// // Finish rendering of frame
		// h3dFinalizeFrame();
		// 
		// displ_Buf->Swap_buffers (); 
		// 
		// return 0;  

	#if 0
		+ Input 
		+ Dynamics 
			- Animations	- play animation
			- IK			- apply faux-physics on momentum frame, calc IK chains
			- Physics		- 
			- wat 
		+ Render 
	#endif
		//
		Update_Input (sys); 
		//
		Update_Dynamics (sys); 
		// 
		Render (sys); 
		//
		return 0;
   }

	// 
	// METHOD OnWindowResize :: int -> int -> void 
	void Runnable::OnWindowResize (int wd, int ht) 
	{
		//if (myMouse)
		//{
		//	const OIS::MouseState& mouse = myMouse->getMouseState (); 
		//	mouse.width = wd; 
		//	mouse.height = ht; 
		//}


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
		//if (res_Table.count ("pipeline"))
		//	h3dResizePipelineBuffers (res_Table.at ("pipeline"), wd , ht);

	}

	//
	// METHOD OnWindowClose :: void 
	void Runnable::OnWindowClose ()
	{

	}

	//
	// METHOD OnWindowActivate	:: bool -> void 
	void Runnable::OnWindowActivate	(bool)
	{

	}


private: 

   //
	// METHOD Update_Input -> void 
	void Runnable::Update_Input (sy::System_context* sys)
	{
      sy::Mouse_state      mouse_state;
      sy::Keyboard_state   kbd_state;

      sys->Poll_input (mouse_state, kbd_state); 

		{	// update view movement
			// myMouse->capture (); 
			// myKeyboard->capture (); 
	
			Ma::Vec3f vPos, vRot; 
			obj_Table["camera"]->Get_pos (vPos);
			obj_Table["camera"]->Get_rot (vRot); 

			xp::Update_view_transform (vPos, vRot, 0.1f, kbd_state, mouse_state); 

			obj_Table["camera"]->Set_pos (vPos); 
			obj_Table["camera"]->Set_rot (vRot); 
		}

	}

	//
	// METHOD Update_Dynamics -> void 
	void Runnable::Update_Dynamics (sy::System_context* sys)
	{
	
	}

	//
	// METHOD Render -> void 
	void Runnable::Render (sy::System_context* sys)
	{
		// Render scene
		h3dRender(node_Table["camera"]);

		// Finish rendering of frame
		h3dFinalizeFrame();

	
		windo->Swap_buffers (); 
	}

	}; // CLASS END 






} // NAMESPACE END 




//
//
sy::RT_window_listener* Create_Ungr_00_task (sy::System_context* sys)
{
	return new Ungr_00::Runnable (sys); 
}


