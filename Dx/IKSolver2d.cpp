#include "IKSolver2d.h"
//#include "e9utility.h"
//#include "core/e9terrain.h"
//#include "graphics/e9renderobj.h"
//#include "core/e9armature.h"



// ---------------------------------------------------------------
//  IKSolve2D () -  IK root at <0>       
// 
//                           szChain: 3:
//  
//                           <0>-------->p0----------->p1----------->p2 
//                           r(0)-------->r(1)--------->r(2)----------->
//                           <0>---len0---> ----len1-----> -----len2----->
//  
//  coord convention
//  
//    scene space.<_, y, z>  =>  IK.<z, y>, 
//                                     
//                                           where z = right, y = up
// 
// ---------------------------------------------------------------
const int kMaxIterations = 5;
const int kMaxChainSize = 4;

float Cross2 (const Ma::Vec2f& lhs, const Ma::Vec2f& rhs) 
{
	//return (lhs->x * rhs->y) - (lhs->y * rhs->x);
   return Ma::X(lhs) * Ma::Y(rhs) - Ma::Y(lhs) * Ma::X(rhs); 
}

//
//
float Ik::Angle_subtend (const Ma::Vec2f& p0, const Ma::Vec2f& p1) 
{
	Ma::Vec2f v0, v1, vDir;

	Ma::Normalize (Ma::Copy (v0, p0));
	Ma::Normalize (Ma::Copy (v1, p1));
	
	Ma::Sub (vDir, v1, v0);

	float dotProd = Ma::Dot (v0, v1);
	if (dotProd > 1.0f)
	{
		dotProd = 1.0f;
	}
	if (dotProd < -1.0f)
	{
		dotProd = -1.0f;
	}
	float res = acosf (dotProd);
	// negate
	return (Cross2 (Ma::Negate (v1), v0) > 0.0f) ? res : -res;
}


//
void Ik::Solve_2D (

   Ma::Vec2f*        pts_,       // in/out,  in curr locs, out new loc's
   float*            angles_,    // out,     get back rel angles, 1st rel to <1, 0>

   int               num_points, 
   const Ma::Vec2f&  targ_)
{
   
   // find current rel angles
   Ma::Vec2f v_; 

   //Ma::Vec2f x_basis; 
   //Ma::Set (x_basis, 1.0f, 0.0f)); 
   //for (int ipnt = 0; ipnt < num_points; ipnt++)
   //{
   //   if (ipnt == 0)
   //      angles_[ipnt] = Angle_subtend (x_basis, pts_[ipnt]); 
   //   else
   //      angles_[ipnt] = Angle_subtend (pts_[ipnt-1], pts_[ipnt]); 
   //}


   //
   for (size_t iter = 0; iter < kMaxIterations ; iter++)
   {
      //
      // collect all current angles wrt <1, 0>
      // collect all current vec mag; 


      // float abs_angs[kMaxChainSize]; // rel to <1, 0>            
      // float rel_mags[kMaxChainSize]; // rel to mag[x-1]

      //float rel_angs[kMaxChainSize]; 

      // CCD
      int endp_ = num_points - 1;

      for (int ipnt = num_points - 1; ipnt >=  0; ipnt--)
      {
         // endp_
         

         // pnts_[ipnt]
         Ma::Vec2f v_targ, v_curr;

         Ma::Sub (v_targ, targ_, pts_[ipnt -1]); 
         Ma::Sub (v_curr, pts_[endp_], pts_[ipnt -1]); 

         float angl_ = Angle_subtend (v_curr, v_targ); 
         
         angles_[ipnt] += angl_ ; 


         Ma::Mat22f m_;
         Ma::SetMatRotateZ (m_, angl_); 

         for (int jpnt = ipnt; jpnt < num_points; jpnt++)
         {
            Ma::Vec2f v_, v__; 

            Ma::Sub (v_, pts_[jpnt], pts_[ipnt - 1] ); 

            Ma::Mult (v__, m_, v_); 

            Ma::Add (pts_[jpnt], pts_[ipnt - 1], v__);  

            // now revise all the points below (children)
         }
      }


      // Ma::Vec2f v_;
      // std::vector<float> DBG_angs_;
      // for (int ipt = 0; ipt < num_points; ipt++)
      // {
      //    if (ipt == 0)
      //    {
      //    DBG_angs_.push_back ( Angle_subtend (pts_[0], x_basis) ); 
      //    
      //    }
      //    else
      //    {
      //       //                   Ma::Sub (v_, pts_[ipt], pts_[ipt-1]); 
      //       //       DBG_angs_.push_back ( Angle_subtend (pts_[ipt], 
      // 
      // 
      //    // X--->0--->1--->2
      //    // q--->q--->q--->
      //    }
      // }

      // turn towards target
      
   }
} 



void test_Angle_subtend ()
{
   Ma::Vec2f test_V[10]; 
   Ma::Vec2f v_; 


   float    angs_[10]; 
   Ma::Set (v_, 0.34f, 81.0f);

   Ma::Normalize (v_); 

   float base_len    = Ma::Length (v_); 
   float base_acos   = acosf ( Ma::X (v_)); 
   float base_asin   = asinf ( Ma::Y (v_)); 

   for (int i = 0; i < 10; i++)
   {
      float ang_ = float(Ma::HalfPi) * i * 0.1f; 
      Ma::Set (test_V[i], cosf (ang_ ) , sinf (ang_ )); 
      angs_[i] = Ma::Rads2Degr (Ik::Angle_subtend (v_, test_V[i])) ; 
   }
}


#ifdef _SOMETHING_

namespace Cr
{



/**
 * 
 */	
   void IKSolve2D (float* oTheta, int szChain, Vec2f* vTarg, Vec2f** pts) 
   {

	   Mat22f mTmp;
	   int lastInd = szChain -1;

	   Vec2f vCurPos[kMaxChainSize]; 	// absolute position in IK chain space
	   Vec2f vRelDispl[kMaxChainSize]; 	// relative to parent
	   float dTheta[kMaxChainSize];			// angle change for current iteration

	   // initialized data
	   for (int i = 0; i < szChain; ++i) {
		   Copy (vCurPos[i], *pts[i]);
		   if (i > 0) {
			   SubVec (vRelDispl[i], vCurPos[i], vCurPos[i-1]); 
		   }
		   else {
			   Copy (vRelDispl[i], vCurPos[i]); 
		   }
		   oTheta[i] = 0.0f;  
		   dTheta[i] = 0.0f;
	   }

	   // iteration loop
	   Vec2f vTargDir, vCurDir, vCurEndPnt; 
	   for (int nIter = 0; nIter < kMaxIterations; ++nIter) {

		   // CCD loop
		   Copy (vCurEndPnt, vCurPos[lastInd]); 
		   for (int iSeg = lastInd; iSeg >= 0; --iSeg) {
			   if (iSeg > 0) {
				   SubVec (vCurDir, vCurEndPnt, vCurPos[iSeg-1]);
				   SubVec (vTargDir, *vTarg, vCurPos[iSeg-1] );
			   }
			   else {
				   Copy (vCurDir, vCurEndPnt);
				   Copy (vTargDir, *vTarg);
			   }

			   dTheta[iSeg] = Angle_subtend (vCurDir, vTargDir);
			   MultMat (&vCurDir, SetMat22RotateZ (&mTmp, dTheta[iSeg]), &vCurDir);
			   oTheta[iSeg] += dTheta[iSeg];
				
			   if (iSeg > 0) {
				   AddVec (vCurEndPnt ,vCurPos[iSeg-1], vCurDir);
			   }
			   else {
				   Copy (vCurEndPnt, vCurDir);
			   }
		   }

		   // Update current & relative positions
		   float acumTheta = 0.0f;
		   for (int iSeg = 0; iSeg < szChain; ++iSeg) {
			   acumTheta += dTheta[iSeg];
			   if (iSeg > 0) {
				   MultMatVec (vCurPos[iSeg], SetMat22RotateZ (&mTmp, acumTheta), vRelDispl[iSeg]);
				   AddVec (vCurPos[iSeg], vCurPos[iSeg], vCurPos[iSeg-1]);
				   SubVec (vRelDispl[iSeg], vCurPos[iSeg], vCurPos[iSeg-1]);
			   }
			   else {
				   MultMatVec (vCurPos[iSeg], SetMat22RotateZ (&mTmp, acumTheta), &vRelDispl[iSeg]);
				   Copy (vRelDispl[iSeg], vCurPos[iSeg]);
			   }
		   }


	   } 	// iteration loop


   }


///****************************************************************
// * Humanoid LocoMotion     
// ***************************************************************/	
//void HumLocoMoInit (HumLocoMo* hum, int nSegsL, int nSegsR) 
//{
//	hum->legStateL = BipedStandIdle;
//	hum->legStateR = BipedStandIdle;
//	hum->fullExt = 0.0f;
//
//	hum->ballRadius = 0.0f;
//	hum->ankleRadius = 0.0f;
//
//	hum->lt.numSegs = nSegsL;
//	hum->lt.segs = new BoneObj*[nSegsL];
//	hum->rt.numSegs = nSegsR;
//	hum->rt.segs = new BoneObj*[nSegsR];
//
//}



} // c0


#endif