#ifndef Include_VecFn
#define Include_VecFn


#include "MathTypes.h"

namespace Ma
{
   /* ***********************************************************************
    *                                                
    ************************************************************************/ 
   template<typename ElTy_>
inline Vector<2, ElTy_>& Set (Vector<2, ElTy_>& v, ElTy_ x, ElTy_ y) 
   {
      v[0] = x; v[1] = y; 
      return v;
   }

   /* ***********************************************************************
    *                                                
    ************************************************************************/ 
   template<typename T>
inline Vector<3, T>& Set (Vector<3, T>& v, T x, T y, T z) 
   {
      v[0] = x; v[1] = y; v[2] = z; 
      return v;
   }

   /* ***********************************************************************
    *                                                
    ************************************************************************/ 
   template<typename ElTy_>
inline Vector<4, ElTy_>& Set (Vector<4, ElTy_>& v, ElTy_ x, ElTy_ y, ElTy_ z, ElTy_ w) 
   {
      v[0] = x; v[1] = y; v[2] = z; v[3] = w; 
      return v;
   }

   template<int N, typename ElTy>
      inline Ma::Vector<N, ElTy>& 
      Set (Ma::Vector<N, ElTy>& v, const ElTy* p)
   {
      for (int i = 0; i < N; i++)
         i[v.ptr()] = p[i];

      return v;
   }

   /* ***********************************************************************
    *                                                
    ************************************************************************/ 
	template<int N, typename Ty_>
inline Vector<N, Ty_>& Add (Vector<N, Ty_>& out, const Vector<N, Ty_>& lhs, const Vector<N, Ty_>& rhs)
	{
		for (int i = 0; i < N; i++) { out[i] = lhs[i] + rhs[i]; }
		return out; 
	}

   /* ***********************************************************************
    *                                                
    ************************************************************************/ 
   template<int N, typename Ty_>
inline Vector<N, Ty_>& Sub (Vector<N, Ty_>& out, const Vector<N, Ty_>& lhs, const Vector<N, Ty_>& rhs)
   {
      for (int i = 0; i < N; i++) { out[i] = lhs[i] - rhs[i]; }
      return out; 
   }

   /* ***********************************************************************
    *                                                
    ************************************************************************/ 
   template<int N, typename Ty_>
inline Ty_ Distance (const Vector<N, Ty_>& lhs, const Vector<N, Ty_>& rhs)
   {
      Vector<N, Ty_> vTmp; 
      return Length (SubVec (vTmp, lhs, rhs));  
   }


	//
	//
   template<int N, typename Ty_, typename SclTy_>
inline Vector<N, Ty_>& Lerp (Vector<N, Ty_>& out, const Vector<N, Ty_>& a, const Vector<N, Ty_>& b, SclTy_ t)
   {
      for (int i = 0; i < N; i++) { 
         Ty_ interpRange = b[i] - a[i]; 
         out[i] = a[i] + (interpRange * t); 
      }
      return out; 
   }
      

	//
	//
   template<int N, typename Ty_>
	inline Vector<N, Ty_>& Incr (Vector<N, Ty_>& out, const Vector<N, Ty_>& incr)
   {
      for (size_t i = 0; i < N; i++)
		out[i] += incr[i];

	  return out; 
   }

	//
	//
	template<int N, typename Ty_>
   inline Vector<N, Ty_>& Scale (Vector<N, Ty_>& inout, Ty_ scale)
   {
      for (int i = 0; i < N; i++) { inout[i] = inout[i] * scale; }
      return inout; 
   }
   /* ***********************************************************************
    *                                                
    ************************************************************************/ 
   template<int N, typename Ty_>
   inline Vector<N, Ty_>& Scale (Vector<N, Ty_>& out, const Vector<N, Ty_>& vec, Ty_ scale)
   {
      for (int i = 0; i < N; i++) { out[i] = vec[i] * scale; }
      return out; 
   }
   /* ***********************************************************************
    *                                                
    ************************************************************************/ 
   template<int N, typename Ty_>
   inline Vector<N, Ty_>& Copy (Vector<N, Ty_>& dst, const Vector<N, Ty_>& src)
   {
      for (int i = 0; i < N; i++) { dst[i] = src[i];  }
      return dst; 
   }

   /* ***********************************************************************
    *                                                
    ************************************************************************/ 
   template<int N, typename Ty_>
   inline Vector<N, Ty_>& Negative (Vector<N, Ty_>& dst, const Vector<N, Ty_>& src)
   {
      for (int i = 0; i < N; i++) { dst[i] = -src[i]; }
      return dst; 
   }

   /* ***********************************************************************
    *                                                
    ************************************************************************/ 
   template<int N, typename Ty_>
   inline Vector<N, Ty_>& Negate (Vector<N, Ty_>& v)
   {
      for (int i = 0; i < N; i++) { v[i] = -v[i]; }
      return v; 
   }

   /** ************************************************************
	 * 
	 **************************************************************/
   template<int N_, typename Ty_>
	inline Ty_ Dot (
		const Vector<N_, Ty_>& lhs, 
		const Vector<N_, Ty_>& rhs
		) 
	{
		Ty_ res = Ty_(0); 
		for (int i = 0; i < N_; i++) { 
			res += lhs[i] * rhs[i]; 
			}  
		return res; 
	}

   template<typename _Ty>
	inline _Ty Dot (
		const Vector<3, _Ty>& lhs, 
		const Vector<3, _Ty>& rhs
		) 
	{
		return lhs[0] * rhs[0] +  lhs[1] * rhs[1] +  lhs[2] * rhs[2]; 
	}

   template<typename _Ty>
	inline _Ty Dot (
		const Vector<4, _Ty>& lhs, 
		const Vector<4, _Ty>& rhs
		) 
	{
		return lhs[0] * rhs[0] +  lhs[1] * rhs[1] +  lhs[2] * rhs[2] +  lhs[3] * rhs[3]; 
	}
	
   //
   // DotPlane
	template<typename Ty_>
	inline Ty_ DotPlane (const Plane<Ty_>& p, const Vector<3, Ty_>& v) {
		Ty_ res = Ty_(0); 
		for (int i = 0; i < 3; i++) { res += p[i] * v[i]; }  
		res -= p[Plane<Ty_>::D]; 
		return res; 
		}

   //
   // Length 
	template<int N, typename Ty_>
		inline Ty_ Length (const Vector<N, Ty_>& v) {
		return sqrt (Dot (v, v)); 
		}

   //
   // Normalize 
	template<int N, typename Ty_>
	inline Vector<N, Ty_>& Normalize (Vector<N, Ty_>& v) {
		Ty_ invLen = Ty_(1) / Length (v); 
		for (int i = 0; i < N; i++) { v[i] *= invLen; }  
		return v; 
	}

   //
   // Cross 
	template<int N, typename Ty_>
	inline Vector<N, Ty_>& Normal (
		Vector<N, Ty_>& out, 
		const Vector<N, Ty_>& v)
	{
		Ty_ invLen = Ty_(1) / Length (v); 
		for (int i = 0; i < N; i++) { out[i] = v[i] * invLen; }  
		return out; 
	}

   //
   // Cross 
	template<typename Ty_>
	inline Vector<3, Ty_>& Cross (
		Vector<3, Ty_>& out, 
		const Vector<3, Ty_>& lhs, 
		const Vector<3, Ty_>& rhs) 
   {
	   Vector<3, Ty_> vTmp;
	   vTmp[0] = (lhs[1] * rhs[2]) - (lhs[2] * rhs[1]);
	   vTmp[1] = (lhs[2] * rhs[0]) - (lhs[0] * rhs[2]);
	   vTmp[2] = (lhs[0] * rhs[1]) - (lhs[1] * rhs[0]);
      out = vTmp;
      return out;
   }

   // 
   // Cross
	template<typename Ty_>
	inline Vector<3, Ty_> Cross (
		const Vector<3, Ty_>& lhs, 
		const Vector<3, Ty_>& rhs) 
   {
	   Vector<3, Ty_> vTmp;
	   vTmp[0] = (lhs[1] * rhs[2]) - (lhs[2] * rhs[1]);
	   vTmp[1] = (lhs[2] * rhs[0]) - (lhs[0] * rhs[2]);
	   vTmp[2] = (lhs[0] * rhs[1]) - (lhs[1] * rhs[0]);
      return vTmp;
   }


   //
   // Spherical 
	template<typename _Ty>
	inline Vector<3, _Ty>& Spherical (Vector<3, _Ty>& out, _Ty theta, _Ty phi)
	{
		_Ty sin_Theta = sin (theta);
		_Ty cos_Theta = cos (theta);

		_Ty sin_Phi = sin (phi);
		_Ty cos_Phi = cos (phi);

		X (out) = cos_Phi * cos_Theta;
		Y (out) = sin_Phi;
		Z (out) = cos_Phi * sin_Theta;
		return out;
	}

   // Perp 
   template<typename ElTy>
      inline Vector<2, ElTy>& 
   Perp (Vector<2, ElTy>& v)
      {
   ElTy _ = X(v); 
   X(v) = -Y(v); 
   Y(v) = _; 
   return v;
      }


   //
   //
   template<typename ElTy>
      inline Vector<2, ElTy>& 
   Perp (Vector<2, ElTy>&  d, Vector<2, ElTy>&  s)
      {
   return Set(d, -Y(s), X(s)); 
      }
}
#endif
