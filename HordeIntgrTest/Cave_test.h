#ifndef _Cave_test_
#define _Cave_test_

#include "TestCommon.h"




//
// H3D_Obj
class H3D_Obj
	{
public: 

	H3D_Obj (); 
	H3D_Obj (H3DNode node); 

	void Bind (H3DNode node); 
	void Set_pos (const Ma::Vec3f& pos); 
	void Set_rot (const Ma::Vec3f& rot); 
	void Set_scl (const Ma::Vec3f& scl); 

	Ma::Vec3f& Get_pos (Ma::Vec3f& pos); 
	Ma::Vec3f& Get_rot (Ma::Vec3f& rot); 
	Ma::Vec3f& Get_scl (Ma::Vec3f& scl); 


private: 

	void update_xf();

	H3DNode		node; 
	Ma::Vec3f	pos, rot, scl; 

	}; 

//
// Cave_test 
class Cave_test 
	: public Sy::RealtimeTask 
	, public Sy::WindowListener
{
public:

	Cave_test(Sy::SystemContext* sys);
	virtual ~Cave_test(); 

	// RealtimeTask 
	virtual int Enter	(Sy::SystemContext* sys); 
	virtual int Exit	(Sy::SystemContext* sys); 
	virtual int Update	(Sy::SystemContext* sys); 

	// 
	virtual void	OnWindowResize		(int wd, int ht) ; 
	virtual void	OnWindowClose		(); 
	virtual void	OnWindowActivate	(bool); 



private: 
	std::shared_ptr<Sy::Window>		renderWindow; 
	void init_input (Sy::SystemContext* sys, Sy::Window* rw);

	// IO 
	OIS::InputManager*		myInputMgr;   // OIS input manager
	OIS::Keyboard*			myKeyboard;   // keyboard device
	OIS::Mouse*				myMouse;      // mouse device

	std::map<std::string, std::string>	prop_Table;
	std::map<std::string, H3DRes>		res_Table;
	std::map<std::string, H3DNode>		node_Table;
	std::map<std::string, H3D_Obj*>		obj_Table;
	

	std::shared_ptr<btDefaultCollisionConfiguration>	mCollisionConfiguration;	
	std::shared_ptr<btCollisionDispatcher>				mCollisionDispatcher;
	std::shared_ptr<btAxisSweep3>						mBroadPhaseInterface;
	std::shared_ptr<btCollisionWorld>					mCollisionWorld;


};

#endif // _Cave_test_






