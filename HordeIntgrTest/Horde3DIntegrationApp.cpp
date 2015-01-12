#include "StdAfx.h"
#include "Horde3DIntegrationApp.h"


template<typename _Ty>
void DontCare (_Ty& x)
{
	(void) x; 
}


void TestRunState::init_input (SystemContext* sys, Window* rw)
{
	if (myInputMgr = sys->CreateInputSystem (rw))
	{
		myKeyboard	= reinterpret_cast<OIS::Keyboard*>	(myInputMgr->createInputObject(OIS::OISKeyboard, true));
		myMouse		= reinterpret_cast<OIS::Mouse*>		(myInputMgr->createInputObject(OIS::OISMouse, true));


		myKeyboard->setBuffered (false); 
		myMouse->setBuffered	(false); 

		myKeyboard->capture (); 
		myMouse->capture (); 

		const OIS::MouseState& mouse = myMouse->getMouseState (); 


		mouse.width = 640; 
		mouse.height = 480; 

		const OIS::Axis& xAxis = mouse.X; 
		const OIS::Axis& yAxis = mouse.Y;
		const OIS::Axis& zAxis = mouse.Z;

		//OIS::MouseButtonID
		//MB_Left = 0, MB_Right, MB_Middle, MB_Button3, MB_Button4,	MB_Button5, MB_Button6,	MB_Button7

		//myKeyboard->setEventCallback(this);
		//myMouse->setEventCallback	(this);
	}
}


/**
 * 
 */
TestRunState::TestRunState  ()
	: renderWindow (0)
{
	for( unsigned int i = 0; i < 320; ++i )
	{	
		_keys[i] = false;
		_prevKeys[i] = false;
	}

	_x = 0; _y = 3; _z = 19; 

	_rx = 7; 
	_ry = 0; 

	_velocity = 10.0f;
	_curFPS = 30;

	_statMode = 0;
	_freezeMode = 0; _debugViewMode = false; _wireframeMode = false;
	_animTime = 0; _weight = 1.0f;
	m_camera = 0;

	_contentDir = "./Content";

}

/**
 * 
 */
TestRunState::~TestRunState  ()
{

}
static int wat = 0; 
/**
 * TestRunState
 */
int TestRunState::Enter	(SystemContext* sys)
{
	renderWindow = sys->CreateOpengGLWindow ();

	init_input (sys, renderWindow); 

	if (h3dInit ())
	{

		// Set options
		h3dSetOption( H3DOptions::LoadTextures, 1 );
		h3dSetOption( H3DOptions::TexCompression, 0 );
		h3dSetOption( H3DOptions::FastAnimation, 0 );
		h3dSetOption( H3DOptions::MaxAnisotropy, 4 );
		h3dSetOption( H3DOptions::ShadowMapSize, 2048 );

		// Add resources
		// Pipelines
		_hdrPipeRes		= h3dAddResource( H3DResTypes::Pipeline, "pipelines/hdr.pipeline.xml", 0 );
		_forwardPipeRes = h3dAddResource( H3DResTypes::Pipeline, "pipelines/forward.pipeline.xml", 0 );

		// Overlays
		_fontMatRes		= h3dAddResource( H3DResTypes::Material, "overlays/font.material.xml", 0 );
		_panelMatRes	= h3dAddResource( H3DResTypes::Material, "overlays/panel.material.xml", 0 );
		_logoMatRes		= h3dAddResource( H3DResTypes::Material, "overlays/logo.material.xml", 0 );

		// Environment
		H3DRes envRes = h3dAddResource( H3DResTypes::SceneGraph, "models/sphere/sphere.scene.xml", 0 );
		// Knight
		//H3DRes knightRes = h3dAddResource( H3DResTypes::SceneGraph, "models/knight/knight.scene.xml", 0 );
		//H3DRes knightAnim1Res = h3dAddResource( H3DResTypes::Animation, "animations/knight_order.anim", 0 );
		//H3DRes knightAnim2Res = h3dAddResource( H3DResTypes::Animation, "animations/knight_attack.anim", 0 );
		// Particle system
		//H3DRes particleSysRes = h3dAddResource( H3DResTypes::SceneGraph, "particles/particleSys1/particleSys1.scene.xml", 0 );


		//H3DRes test_level_res = h3dAddResource (H3DResTypes::SceneGraph, "test_level/lvl.scene.xml", 0x0); 
		H3DRes terrain_res = h3dAddResource (H3DResTypes::SceneGraph, "test_level/test_terrain_00.scene.xml", 0x0); 
		

		// Load resources
		h3dutLoadResourcesFromDisk( _contentDir.c_str() );

		// Add scene nodes
		// Add camera
		//_cam = h3dAddCameraNode( H3DRootNode, "Camera", _hdrPipeRes );
 		m_camera = h3dAddCameraNode (H3DRootNode, "Camera", _forwardPipeRes);

		//h3dSetNodeParamI( _cam, H3DCamera::OccCullingI, 1 );
		// Add environment
		//H3DNode env = h3dAddNodes (H3DRootNode, envRes);
		//h3dSetNodeTransform( env, 0, -20, 0, 0, 0, 0, 20, 20, 20 );

		// Add knight
		//_knight = h3dAddNodes( H3DRootNode, knightRes );
		//h3dSetNodeTransform( _knight, 0, 0, 0, 0, 180, 0, 0.1f, 0.1f, 0.1f );
		//h3dSetupModelAnimStage( _knight, 0, knightAnim1Res, 0, "", false );
		//h3dSetupModelAnimStage( _knight, 1, knightAnim2Res, 0, "", false );

		// Attach particle system to hand joint
		//h3dFindNodes( _knight, "Bip01_R_Hand", H3DNodeTypes::Joint );
		//H3DNode hand = h3dGetNodeFindResult( 0 );
		//_particleSys = h3dAddNodes( hand, particleSysRes );
		//h3dSetNodeTransform( _particleSys, 0, 40, 0, 90, 0, 0, 1, 1, 1 );

		// Add light source
		m_light = h3dAddLightNode( H3DRootNode, "Light1", 0, "LIGHTING", "SHADOWMAP" );
		h3dSetNodeTransform (m_light, 0, 100, 0, -90, 0, 0, 1, 1, 1 );
		h3dSetNodeParamF	(m_light, H3DLight::RadiusF, 0, 2000 );
		h3dSetNodeParamF	(m_light, H3DLight::FovF, 0, 90);
		h3dSetNodeParamI	(m_light, H3DLight::ShadowMapCountI, 1 );
		h3dSetNodeParamF	(m_light, H3DLight::ShadowMapBiasF, 0, 0.01f );
		h3dSetNodeParamF	(m_light, H3DLight::ColorF3, 0, 1.0f );
		h3dSetNodeParamF	(m_light, H3DLight::ColorF3, 1, 0.8f );
		h3dSetNodeParamF	(m_light, H3DLight::ColorF3, 2, 1.0f );
		h3dSetNodeParamF	(m_light, H3DLight::ColorMultiplierF, 0, 1.0f );

		// add scene
		//m_cave = h3dAddNodes (H3DRootNode, test_level_res); 
		m_terrain = h3dAddNodes (H3DRootNode, terrain_res); 
		h3dSetNodeTransform (m_terrain, 0, 0, 0, 0, 0, 0, 1.0f, 1.0f, 1.0f );

		//Set (_lit_pos, 1000.0, -430.0, 402.0); 
		Set (_lit_pos, 100.0, 200.0, -100.0); 
		Set (_lit_rot, 0, 0, 0); 
		Set (_lit_scl, 1.0, 1.0, 1.0); 

		// camera
		Set (_cam_pos, 0.0, 100.0, 0.0);
		Set (_cam_rot, 0, 0, 0); 
		Set (_cam_scl, 1.0, 1.0, 1.0);

		// terrain
		Set (_terrain_pos, 0.0, 0.0, 0.0); 
		Set (_terrain_rot, 0.0, 0.0, 0.0); 
		Set (_terrain_scl, 1.0, 1.0, 1.0); 

		// Customize post processing effects
		//H3DNode matRes = h3dFindResource( H3DResTypes::Material, "pipelines/postHDR.material.xml" );
		//h3dSetMaterialUniform( matRes, "hdrExposure", 2.5f, 0, 0, 0 );
		//h3dSetMaterialUniform( matRes, "hdrBrightThres", 0.5f, 0, 0, 0 );
		//h3dSetMaterialUniform( matRes, "hdrBrightOffset", 0.08f, 0, 0, 0 );
	}


	{	// collision
		btVector3 worldAabbMin (-5000.0,-5000.0,-5000.0);
		btVector3 worldAabbMax ( 5000.0, 5000.0, 5000.0);

		mCollisionConfiguration.reset(new btDefaultCollisionConfiguration ()); 
		mCollisionDispatcher.reset(new btCollisionDispatcher (mCollisionConfiguration.get())); 
		mBroadPhaseInterface.reset(new btAxisSweep3 (worldAabbMin, worldAabbMax)); 

		mCollisionWorld.reset ( 
			new btCollisionWorld (
				mCollisionDispatcher.get(), 
				mBroadPhaseInterface.get(), 
				mCollisionConfiguration.get()
				)
			); 
	
	}


	renderWindow->SetListener (this); 
	renderWindow->Show (true); 

	return 0; 
}

int TestRunState::Exit (SystemContext* sys)
{
	h3dRelease (); 
	return 0; 
}

int TestRunState::Update(SystemContext* sys)
{
	float fps = 30.0; 

	_curFPS = fps;



	// update collision
	// if isect, do not move in that direction


	{	myMouse->capture (); 

		const OIS::MouseState& mouse = myMouse->getMouseState (); 

		mouse.width		= h3dGetNodeParamI (m_camera, H3DCamera::ViewportWidthI ); 
		mouse.height	= h3dGetNodeParamI (m_camera, H3DCamera::ViewportHeightI); 

		const OIS::Axis& xAxis = mouse.X; 
		const OIS::Axis& yAxis = mouse.Y; 
		const OIS::Axis& zAxis = mouse.Z; 
		
		float xModifier = 0.2; 
		float yModifier = 0.2; 
		float zModifier = 0.2; 

		_cam_rot.y -= xModifier * xAxis.rel;
		_cam_rot.x += yModifier * yAxis.rel;
	}

	{	myKeyboard->capture (); 
	
		//if (myKeyboard->isKeyDown (OIS::KeyCode::KC_F))
		//	_cam_rot.y += 0.2; 

		//if (myKeyboard->isKeyDown (OIS::KeyCode::KC_RIGHT))
		//	_cam_rot.y -= 0.2; 


		//if (myKeyboard->isKeyDown (OIS::KeyCode::KC_UP))
		//	_cam_rot.x -= 0.2; 
	
		//if (myKeyboard->isKeyDown (OIS::KeyCode::KC_DOWN))
		//	_cam_rot.x += 0.2; 

		//myKeyboard->isKeyDown (OIS::KeyCode::KC_0); 
		//myKeyboard->isModifierDown (OIS::Keyboard::Modifier::Alt); 
	}


	// update input

	// update 

	h3dSetOption( H3DOptions::DebugViewMode, _debugViewMode ? 1.0f : 0.0f );
	h3dSetOption( H3DOptions::WireframeMode, _wireframeMode ? 1.0f : 0.0f );
	
	if( !_freezeMode )
	{
		_animTime += sys->GetDt (); 


		//h3dSetNodeTransform (
		//	_knight, 
		//	2.5 * ::cosf (2 * _animTime), 
		//	4.0 + ::cosf (4 * _animTime), 
		//	1.5 * ::sinf (3 * _animTime), 
		//	10 * _animTime, 
		//	0, 
		//	100 * _animTime, 
		//	0.05f, 0.05f, 0.05f );

		
		// Do animation blending
		//h3dSetModelAnimParams (_knight, 0, _animTime * 60.0f, _weight );
		//h3dSetModelAnimParams (_knight, 1, _animTime * 60.0f, 1.0f - _weight );

		// Animate particle systems (several emitters in a group node)

		//unsigned int cnt = h3dFindNodes( _particleSys, "", H3DNodeTypes::Emitter );
		//for( unsigned int i = 0; i < cnt; ++i )
		//	h3dAdvanceEmitterTime( h3dGetNodeFindResult( i ), 1.0f / _curFPS );
	}
	
	// Set camera parameters


	// camera
	h3dSetNodeTransform (m_camera, 
		_cam_pos.x, _cam_pos.y, _cam_pos.z, 
		_cam_rot.x, _cam_rot.y, _cam_rot.z, 
		_cam_scl.x, _cam_scl.y, _cam_scl.z);
	
	// light
	h3dSetNodeTransform (m_light, 
		_lit_pos.x, _lit_pos.y, _lit_pos.z,
		_lit_rot.x, _lit_rot.y, _lit_rot.z, 
		_lit_scl.x, _lit_scl.y, _lit_scl.z);

	// terrain
	h3dSetNodeTransform (m_terrain, 
		_terrain_pos.x, _terrain_pos.y, _terrain_pos.z,
		_terrain_rot.x, _terrain_rot.y, _terrain_rot.z,
		_terrain_scl.x, _terrain_scl.y, _terrain_scl.z);

	// Show stats
	h3dutShowFrameStats (_fontMatRes, _panelMatRes, _statMode);
	if( _statMode > 0 )
	{	
		// Display weight
		_text.str( "" );
		_text << std::fixed << std::setprecision( 2 ) << "Weight: " << _weight;
		h3dutShowText( _text.str().c_str(), 0.03f, 0.24f, 0.026f, 1, 1, 1, _fontMatRes );
	}

	// Show logo
	const float ww = (float)h3dGetNodeParamI (m_camera, H3DCamera::ViewportWidthI ) /
	                 (float)h3dGetNodeParamI (m_camera, H3DCamera::ViewportHeightI );
	const float ovLogo[] = { ww-0.4f, 0.8f, 0, 1,  ww-0.4f, 1, 0, 0,  ww, 1, 1, 0,  ww, 0.8f, 1, 1 };

	h3dShowOverlays (ovLogo, 4, 1.f, 1.f, 1.f, 1.f, _logoMatRes, 0 );
	
	// Render scene
	h3dRender( m_camera );

	// Finish rendering of frame
	h3dFinalizeFrame();

	// Remove all overlays
	h3dClearOverlays();

	// Write all messages to log file
	h3dutDumpMessages();	

	renderWindow->SwapBuffers (); 


	return 0; 
}

void TestRunState::OnWindowResize (int wd, int ht)
{
	// Resize viewport
	h3dSetNodeParamI(m_camera, H3DCamera::ViewportXI, 0 );
	h3dSetNodeParamI(m_camera, H3DCamera::ViewportYI, 0 );
	h3dSetNodeParamI(m_camera, H3DCamera::ViewportWidthI, wd );
	h3dSetNodeParamI(m_camera, H3DCamera::ViewportHeightI, ht);
	
	// Set virtual camera parameters
	h3dSetupCameraView(m_camera, 45.0f, (float) wd / ht, 1.0f, 1000.0f );
	h3dResizePipelineBuffers( _hdrPipeRes, wd, ht);
	h3dResizePipelineBuffers( _forwardPipeRes, wd , ht);
}

void TestRunState::OnWindowClose ()
{
	wat++; 
}

void TestRunState::OnWindowActivate (bool activate)
{
	wat++; 
}
