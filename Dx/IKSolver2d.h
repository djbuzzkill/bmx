#ifndef include_IKSolver2D
#define include_IKSolver2D

// #ifdef __cplusplus
// extern "C" {
// #endif 


//#include "e9coretype.h"
#include "VecMath.h"


namespace Ik 
{ 
   float Angle_subtend (
      const Ma::Vec2f& p0, 
      const Ma::Vec2f& p1
      );  

   // 
   void Solve_2D (
      Ma::Vec2f*        pts, 
      float*            angles, 
      int               num_points, 
      const Ma::Vec2f&  targ
      ); 

}

// namespace Cr 
// {
// 	using namespace Ma; 
// 	//
// 	//
// 	struct Hmn_IK_Skeleton {
// 		}; 
// 
// 	// 
// 	struct SkeletonObj;
// 	struct BoneObj;
// 
// 	//
// 	//
// 	//
// 
// 	//void IKSolve2D (float* oTheta, int szChain, Vec2f* vTarg, Vec2f** pts);
// 
// 
// 	/****************************************************************
// 	 * IKChain -  
// 	 ***************************************************************/	
// 	struct IKBoneChain {
// 		BoneObj*	root;
// 		int			numSegs;
// 		BoneObj**	segs;
// 		};
// 
// 
// } //  c0


// #ifdef __cplusplus
// }
// #endif

#endif include_IKSolver2D
