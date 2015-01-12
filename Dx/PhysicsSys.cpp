#include "PhysicsSys.h"


namespace Ph
{
	PhysicsSys::PhysicsSys()
	{
	}

	PhysicsSys::~PhysicsSys()
	{
	}

	void PhysicsSys::Update (float dt) 
	{
	}

	const PhysicsSys& Instance ()
	{
		static PhysicsSys s_Instance; 

		return s_Instance;
	}


}