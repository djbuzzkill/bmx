#ifndef _TestRunState_
#define _TestRunState_

#include "TestCommon.h"
		//CreateInputSystem	(Window*) = 0;   // OIS input manager



/**
 * encapsulate RT Task & Window Listener
 */
class TestRunState : public Sy::RealtimeTask , public Sy::WindowListener
{
public:
	TestRunState (Sy::SystemContext* sys);
	virtual ~TestRunState (); 

	// RealtimeTask 
	virtual int Enter	(Sy::SystemContext* sys); 
	virtual int Exit	(Sy::SystemContext* sys); 
	virtual int Update	(Sy::SystemContext* sys); 

	// 
	virtual void	OnWindowResize		(int wd, int ht) ; 
	virtual void	OnWindowClose		(); 
	virtual void	OnWindowActivate	(bool); 



protected: 

	std::shared_ptr<Sy::Window>		renderWindow; 


	bool				_keys[320], _prevKeys[320];

	float				_x, _y, _z, _rx, _ry;  // Viewer position and orientation
	float				_velocity;  // Velocity for movement
	float				_curFPS;
	std::stringstream	_text;

	int					_statMode;
	int					_freezeMode;
	bool				_debugViewMode, _wireframeMode;
	float				_animTime, _weight;
	
	// Engine objects
	H3DRes				_fontMatRes, _panelMatRes;
	H3DRes				_pipeRes, _logoMatRes, _hdrPipeRes, _forwardPipeRes;

	Ma::Vec3f				_cam_pos, 
						_cam_rot, 
						_cam_scl;


	Ma::Vec3f				_lit_pos, 
						_lit_rot, 
						_lit_scl; 


	Ma::Vec3f				_terrain_pos, 
						_terrain_rot, 
						_terrain_scl; 

	//H3DRes				
	H3DNode				m_camera; 
	H3DNode				m_light; 
	H3DNode				m_terrain, m_ledge, m_column, m_cave, m_inset; 


	// IO 
	OIS::InputManager*		myInputMgr;   // OIS input manager
	OIS::Keyboard*			myKeyboard;   // keyboard device
	OIS::Mouse*				myMouse;      // mouse device


	std::shared_ptr<btDefaultCollisionConfiguration>	mCollisionConfiguration;	
	std::shared_ptr<btCollisionDispatcher>				mCollisionDispatcher;
	std::shared_ptr<btAxisSweep3>						mBroadPhaseInterface;
	std::shared_ptr<btCollisionWorld>					mCollisionWorld;


	LARGE_INTEGER		prev_time; 

	std::string        _contentDir;



	void init_input (Sy::SystemContext* sys, Sy::Window* ); 

};

#endif