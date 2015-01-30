// The fantastic, decomposing Darth Sion, held together by pain and anger and the Dark Side of the Force.

#ifndef include_VecMath
#define include_VecMath




#include "VecFn.h"
#include "MatFn.h"
#include "QuatFn.h"

#include "Utility.h"
namespace Ma 
{ 
	//
	//
	template<typename Ty_>
	inline Ray<Ty_>& Set (
		Ray<Ty_>& out, 
		const Vector<3, Ty_>& orig, 
		const Vector<3, Ty_>& dir)
	{
	   Copy (out.vOrigin, orig); 
	   Copy (out.vDirection, dir); 
	   return out; 
	}


   /** ************************************************************
	 * 
	 **************************************************************/
   template<typename Ty_> 
	inline Plane<Ty_>* BuildFrustumPlanes (Plane<Ty_>* planes, const Frustum<Ty_>& frus)
   {
      DX_Assert (planes, "TODO:BuildFrustumPlanes"); 
   }


   /** ************************************************************
	 * 
	 **************************************************************/
	template<typename Ty_>
	inline Plane<Ty_>& Set (Plane<Ty_>& out, Ty_ a, Ty_ b, Ty_ c, Ty_ d) {
		A (out) = a;
		B (out) = b;
		C (out) = c;
		D (out) = d;
		return out; 
		}

	/** ************************************************************
	 * 
	 **************************************************************/
	template<typename Ty_>
	inline Plane<Ty_>& SetPlane (
		Plane<Ty_>& out, 
		const Vector<3, Ty_>& nrm, 
		const Vector<3, Ty_>& pnt
		)
	{
	return Set (out, nrm[0], nrm[1], nrm[2], Dot (nrm, pnt));	
	}


   /** 
    * 
	*/
	template<typename Ty_>
	inline Plane<Ty_>& SetPlane (
		Plane<Ty_>& out, 
		const Vector<3, Ty_>& p0, 
		const Vector<3, Ty_>& p1, 
		const Vector<3, Ty_>& p2
		)
   {
      Vector<3, Ty_> vA, vB, vRes; 
      SubVec (vA, p1, p0); 
      SubVec (vB, p2, p1); 
      return SetPlane (out, Cross (vRes, vA, vB), p0); 
   }

	//
	// ISectRaySphere - should go with intersection functions
	template<typename Ty_>
	inline bool ISectRaySphere (
		const Ray<Ty_>&      ray, 
		const Sphere<Ty_>&   sph, 
		Vector<3, Ty_>*      vHit = reinterpret_cast<Vector<3, Ty_>*>(0), 
		Ty_*                 closestDist = reinterpret_cast<Ty_*>(0)
		)
	{
      Vector<3, Ty_> nrmDir, projVec, distVec; 
      // from ray start to sphere center
      SubVec (distVec, sph.m_position, ray.vOrigin); 
      // the direction the ray is travelling
      Normal (nrmDir, ray.vDirection);  
      // distVec projected onto ray direction
      Ty_ projLen = Dot (nrmDir, distVec); 
      // shoot ray out to location of sphere
      AddVec (projVec, Scale (projVec, nrmDir, projLen), ray.vOrigin);
      // find distance from sphere center orthogonal to the ray dir      
      Ty_ absDist = fabs (Distance (projVec, sph.m_position)); 
      // 
      if (sph.m_radius < absDist)
      {
         // outside of radius; no intersection
         return false;
      }
      else 
      {
         // we hit, want intersection point too?
         if (vHit)
         {
            Ty_ hitFrac = sph.m_radius * cos (asin (absDist / sph.m_radius)); 
            Scale (*vHit, nrmDir, (projLen - hitFrac));
         }
         if (closestDist)
         {
            *closestDist = absDist; 
         }

         return true; 
      }
   }



} // M



#endif

 