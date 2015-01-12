#ifndef _PhysicsSys_
#define _PhysicsSys_

namespace Ph
{
	/**
	 * PhysicsSys
	 */
	class PhysicsSys
	{
	public:
		PhysicsSys ();
		~PhysicsSys();

		static const PhysicsSys& Instance ();

		void Update (float dt); 
	};



}

#endif