#pragma once
#include "Horde3D.h"
#include "Horde3DUtils.h"

#include <memory>
#include <sstream>
#include <iomanip>
#include "OIS.h"

#include "btBulletCollisionCommon.h"

class Window ; 
class RealtimeTask ; 
class SystemContext; 



//
// SystemContext 
	class SystemContext 
	{ 
public: 

	enum Req {
		ReqExit = 0,  
	}; 

	virtual ~SystemContext () {
	}

	virtual Window*				CreateOpengGLWindow	() = 0; 
	virtual OIS::InputManager*	CreateInputSystem	(Window*) = 0;   // OIS input manager


	virtual bool	DispatchEvents			() = 0; 

	virtual	void	ResetTime		() = 0; 
	virtual	double  GetTimeElapsed	() = 0; 
	virtual	double  GetDt			() = 0; 

	virtual void	SysReq (Req req) = 0; 

protected: 
	SystemContext  () { }
	}; 


// oo is ill defined 
// oo is an artificially devised a idea that is over empasized

class WindowListener
{
public: 
	virtual ~WindowListener () {}

	virtual void OnWindowResize		(int wd, int ht) = 0; 
	virtual void OnWindowClose		() = 0; 
	virtual void OnWindowActivate	(bool activate) = 0; 
protected: 
	WindowListener () {
	}
}; 

//
// AppWindow 
class Window 
	{
public: 
	
	virtual ~Window () {
	}

	//
	// funcs
	virtual void SetListener (WindowListener* listener) = 0;

	virtual void Show (bool show) = 0; 
	virtual void SetPos (int x, int y) = 0; 
	virtual void SwapBuffers () = 0; 

protected: 
	Window  () { 
	}
}; 


//
// RealtimeApp 
class RealtimeTask 
	{
public: 

	enum State 
	{
		Initializing,
		Running,
		Exiting, 
	}; 

	virtual ~RealtimeTask() {
	}

	// 
	virtual int		Enter	(SystemContext*) = 0; 
	virtual int		Exit	(SystemContext*) = 0; 
	virtual int		Update	(SystemContext*) = 0; 
	protected: 
	RealtimeTask() {}
	}; 



struct vec3f_ { float x, y, z; }; 


template<typename OutTy_, typename Ty_>
OutTy_& Set (OutTy_& out, Ty_ x, Ty_ y, Ty_ z) { 
	out.x =x, out.y = y, out.z = z; 
	return out; 
	}

/**
 * 
 */
class TestRunState : public RealtimeTask , public WindowListener
{
public:
	TestRunState ();
	virtual ~TestRunState (); 

	// RealtimeTask 
	virtual int Enter	(SystemContext* sys); 
	virtual int Exit	(SystemContext* sys); 
	virtual int Update	(SystemContext* sys); 

	// 
	virtual void	OnWindowResize		(int wd, int ht) ; 
	virtual void	OnWindowClose		(); 
	virtual void	OnWindowActivate	(bool); 



protected: 

	Window*				renderWindow; 


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

	vec3f_				_cam_pos, 
						_cam_rot, 
						_cam_scl;


	vec3f_				_lit_pos, 
						_lit_rot, 
						_lit_scl; 


	vec3f_				_terrain_pos, 
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



	void init_input (SystemContext* sys, Window* ); 

};

