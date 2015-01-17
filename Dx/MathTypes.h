#ifndef include_MathTypes
#define include_MathTypes

#include <math.h>

// #ifdef __cplusplus
// extern "C" { 
// #endif

namespace Ma
{
	const double PI      = 3.1415926535897932384626433832795; 
	const double TWO_PI  = 6.283185307179586476925286766559; 
	const double HALF_PI = 1.5707963267948966192313216916398; 

	const double Pi      = 3.1415926535897932384626433832795; 
	const double TwoPi   = 6.283185307179586476925286766559; 
	const double HalfPi  = 1.5707963267948966192313216916398; 

	const int Degrees = 360; 

   const double kDegr2Rads = Ma::Pi / 180.0; 
   const double kRads2Degr = 180.0 / Ma::Pi ; 

   template<typename ElTy>
      inline ElTy Degr2Rads (ElTy degs) { 
      return degs * ElTy(kDegr2Rads); 
      }


   template<typename ElTy>
      inline ElTy Rads2Degr (ElTy rads) {
      return rads * ElTy(kRads2Degr); 
      }

	template<typename _Ty>
		inline _Ty& Clamp (_Ty& val, _Ty MinV, _Ty MaxV)
	{

		if (val < MinV)
			val = MinV;

		if (val > MaxV)
			val = MaxV; 

		return val; 
	}

	/** ************************************************************
	 * vector
	 **************************************************************/
	template<unsigned n, typename Ty_>
struct Vector { public: 

	// enum
	enum { 
		Degree = n 
		};

      // TYPEDEF 
      typedef Ty_ ElemType;

      // Constructor
      Vector () { 
	  }

      Vector(const Vector& rhs) { 
		for (int i = 0; i < Degree; i++) 
			e[i] = rhs[i]; 
		}
      // 
      // subscript 
            Ty_&  operator[] (size_t index) { return e[index]; }
      const Ty_&  operator[] (size_t index) const { return e[index]; }
		//
		// conversion 
		Ty_* ptr () { 
			return reinterpret_cast<Ty_*>(this); 
			} 

		const Ty_* ptr () const { 
			return reinterpret_cast<const Ty_*>(this); 
			} 


		Vector& operator= (const Vector& rhs) {

		  for (int i = 0; i < Degree; i++) 
			 e[i] = rhs[i]; 

		 return *this; 
      }

      Ty_ e[n]; 
	};

	//
	template<unsigned _N, typename _Ty>
		inline _Ty& X (Vector<_N, _Ty>& v) { 
			return v[0]; 
			}

	template<unsigned _N, typename _Ty>
		inline _Ty& Y (Vector<_N, _Ty>& v) { 
			return v[1]; 
			}

	template<unsigned _N, typename _Ty>
		inline _Ty& Z (Vector<_N, _Ty>& v) { 
			return v[2]; 
			}

	template<unsigned _N, typename _Ty>
		inline _Ty& W (Vector<_N, _Ty>& v) { 
			return v[3]; 
			}

		//
	template<unsigned _N, typename _Ty>
		inline const _Ty& X (const Vector<_N, _Ty>& v) { 
			return v[0]; 
			}

	template<unsigned _N, typename _Ty>
		inline const _Ty& Y (const Vector<_N, _Ty>& v) { 
			return v[1]; 
			}

	template<unsigned _N, typename _Ty>
		inline const _Ty& Z (const Vector<_N, _Ty>& v) { 
			return v[2]; 
			}

	template<unsigned _N, typename _Ty>
		inline const _Ty& W (const Vector<_N, _Ty>& v) { 
			return v[3]; 
			}


	//
	template<typename _Ty>
	struct EulerRot : public Vector<3, _Ty> { 
public:
		EulerRot (_Ty x, _Ty y, _Ty z) {
			e[0] = x; e[1] = y; e[2] = z;  
			}
		};

   /** ************************************************************
	 * matrix
	 **************************************************************/
   template<unsigned Rows, unsigned Cols, typename Ty_, typename VecTy_ = Vector<Cols, Ty_> >
struct Matrix  { 
    typedef Ty_ Elemtype;

	Matrix () { 
		}
    
   
    enum  { 
        NRows = Rows
		};
   
	enum { 
		NCols = Cols
		};
   
      // 
      // subscript 
            VecTy_& operator [] (size_t rindex) { return e[rindex]; }
      const VecTy_& operator [] (size_t rindex ) const { return e[rindex]; }
   
		// :::::::::::::::::::::::
		Ty_* ptr () { 
			return reinterpret_cast<Ty_*> (this); 
			}

		const Ty_* ptr () const { 
			return reinterpret_cast<const Ty_*> (this); 
		}

	  
	  //operator VecTy_* ( ) { return reinterpret_cast<VecTy_*>(this); }
      // multiply ::::::::::::::::::::
      VecTy_ e[Rows]; 
	};



	/** ************************************************************
	 * Plane 
	 **************************************************************/
	template<typename Ty_>
struct Plane : public Vector<4, Ty_>  {

	   enum { A = 0, B, C, D };

      Plane () {} 

      Plane (float a, float b , float c, float d)  {
         //FIXME: Set (a, b, c, d); 
      }
   }; 

	template<typename _Ty>
	inline _Ty& A (Plane<_Ty>& p) { 
		return p[0]; 
		}

	template<typename _Ty>
	inline _Ty& B (Plane<_Ty>& p) { 
		return p[1]; 
		}

	template<typename _Ty>
	inline _Ty& C (Plane<_Ty>& p) { 
		return p[2]; 
		}

	template<typename _Ty>
	inline _Ty& D (Plane<_Ty>& p) { 
		return p[3]; 
		}

	template<typename _Ty>
	inline const _Ty& A (const Plane<_Ty>& p) { 
		return p[0]; 
		}

	template<typename _Ty>
	inline const _Ty& B (const Plane<_Ty>& p) { 
		return p[1]; 
		}

	template<typename _Ty>
	inline const _Ty& C (const Plane<_Ty>& p) { 
		return p[2]; 
		}

	template<typename _Ty>
	inline const _Ty& D (const Plane<_Ty>& p) { 
		return p[3]; 
		}

	/** ************************************************************
	 * quaternion
	 **************************************************************/


	template<typename Ty_>
	struct Quaternion : public Vector<4, Ty_> { 

		Quaternion () : Vector<4, Ty_>() { 
			}

		};   
	//
	template<typename _Ty>
	inline _Ty& W (Quaternion<_Ty>& q) { 
		return q[0]; 
		} 

	template<typename _Ty>
	inline _Ty& X (Quaternion<_Ty>& q) { 
		return q[1]; 
		} 

	template<typename _Ty>
	inline _Ty& Y (Quaternion<_Ty>& q) { 
		return q[2]; 
		} 

	template<typename _Ty>
	inline _Ty& Z (Quaternion<_Ty>& q) { 
		return q[3]; 
		} 

	template<typename _Ty>
	inline const _Ty& W (const Quaternion<_Ty>& q) { 
		return q[0]; 
		} 

	template<typename _Ty>
	inline const _Ty& X (const Quaternion<_Ty>& q) { 
		return q[1]; 
		} 

	template<typename _Ty>
	inline const _Ty& Y (const Quaternion<_Ty>& q) { 
		return q[2]; 
		} 

	template<typename _Ty>
	inline const _Ty& Z (const Quaternion<_Ty>& q) { 
		return q[3]; 
		} 


	// 
	// Ray
	template<typename Ty_> 
	struct Ray  { 

		Ray () {
			}

		Ray (const Vector<3, Ty_>& orig, const Vector<3, Ty_>& dir) 
			: origin (orig),  direction (dir) { 
			}

		Vector<3, Ty_> origin; 
		Vector<3, Ty_> direction; 
	};

   //
   // Sphere 
   template<typename Ty_> 
struct Sphere { public: 
	Sphere () : posi (), radi (0) {
		}

    Vector<3, Ty_>	posi;  
    Ty_				radi; 
	}; 


	//
	template<typename Ty_> 
	inline Sphere<Ty_>& SetSphere (
		Sphere<Ty_>& sph, 
		const Vector<3, Ty_>& ctr, 
		Ty_ radius
		)
	{
		sph.posi = ctr; 
		sph.radi = radius; 
		return sph;
	}

   /** ************************************************************
	 * 
	 **************************************************************/
   template<typename Ty_>
   struct Frustum {
      // :::::::::::::::::::::::::::::::::::::::::::::::: 
      // enum
      // :::::::::::::::::::::::::::::::::::::::::::::::: 
	enum FrustumIntersect { 
		IntersectionInside = 0, 
		IntersectionOutside, 
		IntersectionStraddle 
		};
      
    enum FrustumPlane {
		BottomPlane = 0, 
		LeftPlane, 
		RightPlane, 
		TopPlane, 
		FrontPlane, 
		BackPlane, 
		_NumFrustumPlanes
		};

	enum FrustumEdge {
		TopRight = 0, 
		TopLeft, 
		BottomLeft, 
		BottomRight, 
		_NumFrustumEdges 
		};

      // :::::::::::::::::::::::::::::::::::::::::::::::: 
      Frustum (const Vector<3, Ty_>& origin, Ty_ fovX, Ty_ aspY, Ty_  zn, Ty_ zf)
         : m_zNear (zn), m_zFar (zf), m_FoVX (fovX), m_aspY (aspY) 
      {
         Copy (m_viewOrigin, origin); 
      }

		// :::::::::::::::::::::::::::::::::::::::::::::::: 
		// Member
		// :::::::::::::::::::::::::::::::::::::::::::::::: 
		Vector<3, Ty_> m_planeNormals[_NumFrustumPlanes];
		Vector<3, Ty_> m_edges[_NumFrustumPlanes];
		Vector<3, Ty_> m_viewDir;
		Vector<3, Ty_> m_viewOrigin; 

		Ty_   m_zNear; 
		Ty_   m_zFar;
		Ty_   m_FoVX;
		Ty_   m_aspY; 
	};




	/** ************************************************************
	 * 
	 **************************************************************/
	typedef Vector<3, unsigned char>  Vec3ub; 
	typedef Vector<4, unsigned char>  Vec4ub; 
	
	typedef Vector<2, int>     Vec2i;  
	typedef Vector<3, int>     Vec3i; 
	typedef Vector<4, int>     Vec4i; 
	
	typedef Vector<2, unsigned int>     Vec2ui;  
	typedef Vector<3, unsigned int>     Vec3ui; 
	typedef Vector<4, unsigned int>     Vec4ui; 

	typedef Vector<2, float>   Vec2f;  
	typedef Vector<3, float>   Vec3f; 
	typedef Vector<4, float>   Vec4f; 
	
	typedef Vector<2, double>  Vec2d; 
	typedef Vector<3, double>  Vec3d; 
	typedef Vector<4, double>  Vec4d; 

	typedef Vector<2, short>  Vec2s;    
	typedef Vector<3, short>  Vec3s;    
	typedef Vector<4, short>  Vec4s;    

	typedef Vector<2, unsigned short>  Vec2us;   
	typedef Vector<3, unsigned short>  Vec3us;   
	typedef Vector<4, unsigned short>  Vec4us;   

	typedef Vector<3, unsigned char>  Col3ub; 
	typedef Vector<4, unsigned char>  Col4ub; 
	typedef Vector<3, float>          Col3f; 
	typedef Vector<4, float>          Col4f; 

   //
   //
	struct V3f : public Vector<3, float> 
	{
		V3f() {
			X(*this) = 0.0f;
			Y(*this) = 0.0f;
			Z(*this) = 0.0f;
		}

		V3f (float x, float y, float z)
		{ 
			X(*this) = x; 
			Y(*this) = y; 
			Z(*this) = z; 
		}
	}; 

	/** 
	 *
	 */
	typedef Matrix<2, 2, float> Mat22f; 
	typedef Matrix<3, 3, float> Mat33f; 
	typedef Matrix<4, 4, float> Mat44f; 
	
	typedef Matrix<2, 2, double> Mat22d;  
	typedef Matrix<3, 3, double> Mat33d; 
	typedef Matrix<4, 4, double> Mat44d;  
	
	typedef Quaternion<float>    Quatf;
	typedef Quaternion<double>   Quatd; 
	
	typedef Ray<float> Rayf; 
	typedef Ray<double> Rayd; 
	
	typedef Sphere<float> Spheref; 
	typedef Sphere<double> Sphered; 
	
	typedef Plane<float> Planef; 
	typedef Plane<double> Planed; 
}

// #ifdef __cplusplus
// }
// #endif

#endif // M