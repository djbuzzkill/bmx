#ifndef include_Goby
#define include_Goby

#include <map>
#include <string>

#include "GlobalTypes.h"
#include "Collision.h"

#include "Utility.h"
#include "MathTypes.h"
#include "Anim_biped.h"

namespace Dx 
{
 
	struct Transform { 
		Ma::Vec3f	posi; 
		Ma::Mat33f	orie; 
		}; 

	/** 
	 * 
	 */
	struct Scene;
	struct SceneMgr;

	struct MeshObj;
	struct GobyRes;
	struct ResourceObject;

	struct Transform; 
	struct Component;
	struct Goby;

	/** 
	 * 
	 */

	//struct Component 
	//{
	//	Component (Goby* obj) : goby (obj) 
	//	{
	//	}

	//	std::size_t	typeID;
	//	Goby*		goby;

	//	Goby* GetObj ()	 
	//	{ 
	//		return goby; 
	//	}

	//	std::size_t GetID ()  
	//	{ 
	//		return typeID; 
	//	}	

	//	// --------------------------------------
	//	// Create () - external references 
	//	// limited to resource 
	//	// --------------------------------------
	//	virtual void Create (ResourceObject* res) = 0;

	//	// --------------------------------------
	//	// Initialize () - initialize assumes all
	//	// inter object components are present 
	//	// in Goby 
	//	// --------------------------------------
	//	virtual void Initialize (void*) = 0;

	//};


	/** ************************************************************
	 * 
	 **************************************************************/
	// template<typename CmpTy_> 
	// struct CompType 
	// {
	// 	CompType () {
	// 	}
   // 
	// 	template<typename IndTy_>
	// 	static CmpTy_* Get (IndTy_ ind) 
	// 	{
	// 		std::vector<CmpTy_*>& compArr= CompType::Instance()->compArr;
   // 
	// 		if (ind < compArr.size()) 
	// 		{
	// 			return compArr[ind];
	// 		}
	// 		return 0;
	// 	}
   // 
	// 	static CmpTy_* Get (Goby* cntr) 
	// 	{
	// 		std::vector<CmpTy_*>& compArr= CompType::Instance()->compArr;
	// 		for (unsigned i = 0; i < compArr.size(); i++) 
	// 		{
	// 			if (compArr[i]->GetObj() == ctr) 
	// 			{
	// 				return compArr[i];
	// 			}
	// 		}
	// 		return 0;
	// 	}
   // 
	// 	static CompType* Instance() 
	// 	{
	// 		static CompType* thisType = 0;
	// 		if (thisType  == 0) 
	// 		{
	// 			thisType = new CompType;
	// 		}
	// 		return thisType;
	// 	}
   // 
	// 	static CmpTy_* New (Goby* ctr) 
	// 	{
	// 		std::vector<CmpTy_*>& compArr= CompType::Instance()->compArr;
	// 		CmpTy_* newComp = new CmpTy_(ctr);
   // 
	// 		newComp->typeID = (unsigned) CompType::Instance();
	// 		compArr.push_back (newComp);
	// 		ctr->Add (newComp);
	// 		return newComp;
	// 	} 
   // 
	// 	static void Free (CmpTy_* cmp) 
	// 	{
	// 		std::vector<CmpTy_*>& compArr = CompType::Instance()->compArr;
	// 		//std::list<CmpTy_*>::iterator iter = compList.find (cmp);
	// 		for (std::vector<CmpTy_*>::size_type i = 0; i < compArr.size(); i++) 
	// 		{
	// 			if (compArr[i] = cmp) 
	// 			{
	// 				delete cmp;
	// 				compArr[i] = compArr.back ();
	// 				compArr.pop_back ();
	// 				break;
	// 			}
	// 		}
	// 	}
   // 
	// 	//unsigned regID;	// dont use for anything; this changes
	// 	std::vector<CmpTy_*> compArr;
	// };

	/** ************************************************************
	 * 
	 **************************************************************/
	struct Goby : public Destructor  
	{


		unsigned	         obj_flags; 
      //An::Biped*        biped; 
      Sc::Obj*          sob; 
      Co::Collider*     coll; 
      An::Articulator*  arti; 
      //
		// 
      Goby () : obj_flags (0), sob (0), coll (0), arti(0)
      {
		}

      //
      // 
		virtual ~Goby () {
		}
	};

   //inline An::Articulator*&   Ar (Goby* g) { return g->biped; } 
   inline Sc::Obj*&           So (Goby& g) { return g.sob; } 
   inline Co::Collider*&      Cl (Goby& g) { return g.coll; } 

   
   typedef std::map<std::string, Goby> GobyMap; 
}

#endif


