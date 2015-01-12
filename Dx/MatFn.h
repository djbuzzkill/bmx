#ifndef include_MatrixFn
#define include_MatrixFn



#include "MathTypes.h"

namespace Ma
{ 


	//
	//
	template<unsigned R, unsigned C, typename _Ty>
	inline Matrix<R, C, _Ty>& SetIdent (Matrix<R, C, _Ty>& m) {
		for (unsigned r = 0; r < R; r++) {
			for (unsigned c = 0; c < C; c++) {
				if (r == c) 
					m[r][c] = _Ty(1);
				else 
					m[r][c] = _Ty(0); 
				}
			}
		return m;
		}	
	//
	//

	//
	// MultMat33Vec3
	template<typename _Ty>
	Vector<3, _Ty>& Mult (
		Vector<3, _Ty>& out, 
		const Matrix<3, 3, _Ty>& m, 
		const Vector<3, _Ty>& v) 
	{
		Vector<3, _Ty> v_Tmp;
		X(v_Tmp) = Dot (m[0], v);
		Y(v_Tmp) = Dot (m[1], v);
		Z(v_Tmp) = Dot (m[2], v);
		return Copy (out, v_Tmp);	
	}
	
	// MultMat44Vec4 
	template<typename _Ty>
	Vector<4, _Ty>& Mult (
		Vector<4, _Ty>& out, 
		const Matrix<4, 4, _Ty>& mat, 
		const Vector<4, _Ty>& v) 
	{
		Vector<4, _Ty>& v_Tmp;

		X (v_Tmp) = Dot (mat[0], v);
		Y (v_Tmp) = Dot (mat[1], v);
		Z (v_Tmp) = Dot (mat[2], v);
		W (v_Tmp) = Dot (mat[3], v);

		return Copy (out, v_Tmp);	
	}


	/* *******************************************************
	 * 
	 *******************************************************/ 
	template<typename _Ty>
	Matrix<4, 4, _Ty>& Mat44Ortho (Matrix<4, 4, _Ty>& out, _Ty lt, _Ty rt, _Ty top, _Ty bot, _Ty zn, _Ty zf) 
	{
		_Ty rl_inv = 1.0f / (rt - lt);
		_Ty tb_inv = 1.0f / (top - bot);
		_Ty fn_inv = 1.0f / (zf - zn);

		out[0][0] = 2.0f*rl_inv,	out[0][1] = 0.0f,			out[0][2] = 0.0f,			out[0][3] = -(rt + lt) * rl_inv, 
		out[1][0] = 0.0f,		out[1][1] = 2.0f*tb_inv,	out[1][2] = 0.0f,			out[1][3] = -(top + bot) * tb_inv, 
		out[2][0] = 0.0f,		out[2][1] = 0.0f,			out[2][2] = -2.0f*fn_inv,	out[2][3] = -(zf + zn) * fn_inv, 
		out[3][0] = 0.0f,		out[3][1] = 0.0f,			out[3][2] = 0.0f,			out[3][3] = 1.0f; 
		return out;
	}

	//
	//
	template<typename _Ty>
	inline Matrix<3, 3, _Ty>& SetMat33Quat (
		Matrix<3, 3, _Ty>& out, 
		const Quaternion<_Ty>& q
		)
	{
		float	s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;
		s = 2.0f;  

		xs = X(q) * s;
		ys = Y(q) * s;
		zs = Z(q) * s;

		wx = W(q) * xs;
		wy = W(q) * ys;
		wz = W(q) * zs;

		xx = X(q) * xs;
		xy = X(q) * ys;
		xz = X(q) * zs;

		yy = Y(q) * ys;
		yz = Y(q) * zs;
		zz = Z(q) * zs;

		out[0][0] = _Ty(1)-(yy + zz);
		out[0][1] = xy-wz;
		out[0][2] = xz+wy;

		out[1][0] = xy+wz;
		out[1][1] = _Ty(1) - (xx + zz);
		out[1][2] = yz-wx;

		out[2][0] = xz-wy;
		out[2][1] = yz + wx;
		out[2][2] = _Ty(1) - (xx + yy);

		return out;
	}
	
	/* *******************************************************
	 * 
	 *******************************************************/ 

	template<typename _Ty>	
	inline Matrix<4, 4, _Ty>& SetMat44Quat (
		Matrix<4, 4, _Ty>&		out, 
		const Quaternion<_Ty>&	q
		) 
	{
		_Ty	s, xs, ys, zs, wx, wy, wz, xx, xy, xz, yy, yz, zz;
		s = 2.0f; // / ((q->w*q->w) +  (q->x*q->x) + (q->y*q->y) + (q->z*q->z));
		xs = X (q) * s;
		ys = Y (q) * s;
		zs = Z (q) * s;
		wx = W (q) * xs;
		wy = W (q) * ys;
		wz = W (q) * zs;
		xx = X (q) * xs;
		xy = X (q) * ys;
		xz = X (q) * zs;
		yy = Y (q) * ys;
		yz = Y (q) * zs;
		zz = Z (q) * zs;

		out[0][0] = _Ty(1)-(yy+zz);
		out[0][1] = xy-wz;
		out[0][2] = xz+wy;
		out[0][3] = _Ty(0);

		out[1][0] = xy+wz;
		out[1][1] = _Ty(1) - (xx + zz);
		out[1][2] = yz - wx;
		out[1][3] = _Ty(0);

		out[2][0] = xz - wy;
		out[2][1] = yz + wx;
		out[2][2] = _Ty(1) - (xx + yy);
		out[2][3] = _Ty(0);

		out[3][0] = _Ty(0);
		out[3][1] = _Ty(0);
		out[3][2] = _Ty(0);
		out[3][3] = _Ty(1);
		return out;
	}

	

   /* ***********************************************************************
    *                                                
    ************************************************************************/ 
   template<int R, int C, typename Ty_>
   inline Matrix<R, C, Ty_>& Copy (Matrix<R, C, Ty_>& dst, const Matrix<R, C, Ty_>& src)
   {
      for (int iR = 0; iR < R; iR++) 
      {
         for (int iC = 0; iC < C; iC++) 
         {
            dst[iR][iC] = src[iR][iC];             
         }
      }
      return dst; 
   }


	//
	template<typename _Ty>
	inline Matrix<4, 4, _Ty>& SetMat44 (
		Matrix<4, 4, _Ty>& out, 
		_Ty a, _Ty b, _Ty c, _Ty d, 
		_Ty e, _Ty f, _Ty g, _Ty h, 
		_Ty i, _Ty j, _Ty k, _Ty l, 
		_Ty m, _Ty n, _Ty o, _Ty p) 
	{
		_Ty* elem = out.ptr (); 
		elem[0] =  a,	elem[1] =  b,	elem[2] =  c,	elem[3] =  d, 
		elem[4] =  e,	elem[5] =  f,	elem[6] =  g,	elem[7] =  h, 
		elem[8] =  i,	elem[9] =  j,	elem[10] =  k,	elem[11] =  l,
		elem[12] =  m,	elem[13] =  n,	elem[14] =  o,	elem[15] =  p;
		return out;
	}



	//
	//
	template<unsigned R, unsigned C, typename _Ty>
	inline Matrix<R, C, _Ty>& SetMatIdent (Matrix<R, C, _Ty>& m) {
		for (unsigned r = 0; r < R; r++) {
			for (unsigned c = 0; c < C; c++) {
				if (r == c) 
					m[r][c] = _Ty(1);
				else 
					m[r][c] = _Ty(0); 
				}
			}
		return m;
		}	
	//
	//

	template<typename _Ty>
	inline Matrix<4, 4, _Ty>& PerspectiveFoV_RHS (
		Matrix<4, 4, _Ty>& out, 
		_Ty fov, 
		_Ty asp,
		_Ty zn, 
		_Ty zf
		) 
	{
		_Ty hfov	= _Ty(0.5) * fov;
		_Ty ht		= _Ty(1.0) / tan (hfov);

		_Ty wd		= ht / asp;
		_Ty Q		= zn - zf;
		_Ty fovrQ	= zf / Q;

		return SetMat44 (
			out, 
			wd,		_Ty(0),	_Ty(0),		_Ty(0), 
			_Ty(0),	ht,		_Ty(0),		_Ty(0), 
			_Ty(0),	_Ty(0),	fovrQ,		(zn * fovrQ), 
			_Ty(0),	_Ty(0),	_Ty(-1),	_Ty(0)
		);
	}



	/* *******************************************************
	 * 
	 *******************************************************/ 
	template<typename _Ty>
	inline Matrix<4, 4, _Ty>& MultMat44 (
		Matrix<4, 4, _Ty>&			out, 
		const Matrix<4, 4, _Ty>&	lhs, 
		const Matrix<4, 4, _Ty>&	rhs) 
	{ 

		const _Ty* l = lhs.ptr();
		const _Ty* r = rhs.ptr();
		_Ty* o = out.ptr ();

		float tmp[16];
		unsigned balls, nuts;
		for ( balls = 0; balls < 4; balls++ ) {
			for ( nuts = 0; nuts < 4; nuts++ ) {
				tmp[(balls*4)+nuts] =	l[(balls*4)+0] * r[0+nuts] + l[(balls*4)+1] * r[4+nuts] + 
												l[(balls*4)+2] * r[8+nuts] + l[(balls*4)+3] * r[12+nuts];
			}
		}

		return Copy<4, 4, _Ty> (out, *(Matrix<4, 4, _Ty>*)tmp );
	} 

   /* ***********************************************************************
    *                                                
    ************************************************************************/ 
   template<int R, int C, typename Ty_> 
   inline Matrix<C, R, Ty_>& Transpose (Matrix<C, R, Ty_>& dst, const Matrix<R, C, Ty_>& src) 
   {
      for (int iR = 0; iR < R; iR++) 
      {
         for (int iC= 0; iC < C; iC++) 
         {
            dst[iC][iR] = src[iR][iC];             
         }
      }
      return dst; 
   }

   /* ***********************************************************************
    *                                                
    ************************************************************************/ 
   template<int R, int C, typename Ty_> 
   inline Matrix<C, R, Ty_>& Transpose (Matrix<C, R, Ty_>& mat) 
   {
      //ASSERT (R == C);
      for (int iR = 0; iR < R; iR++) 
      {
         for (int iC = iR; iC < C; iC++) 
         {
            Ty_ tmp = mat[iC][iR]; 
            mat[iC][iR] = mat[iR][iC];
            mat[iR][iC] = tmp;
         }
      }
      return mat; 
   }

   /** *********************************************************************
    *                                                
    ************************************************************************/ 
   template<int R, int C, typename Ty_> 
   inline Matrix<R, C, Ty_>& MultMat 
   (
      Matrix<R, C, Ty_>& out, 
      const Matrix<R, C, Ty_>& lhs, 
      const Matrix<R, C, Ty_>& rhs
   ) 
   { 
		Matrix<R, C, Ty_> mTmp; 
		for (int iR = 0; iR < R; iR++ ) 
		{
			for (int iC = 0; iC < C; iC++ ) 
			{
				mTmp[iR][iC]	= lhs[iR][0] * rhs[0][iC]  
								+ lhs[iR][1] * rhs[1][iC]  
								+ lhs[iR][2] * rhs[2][iC];  
			}
	   }
	   return Copy (out, mTmp);
   } 



   /** 
    * The version above breaks the compiler, 
    */
   template<unsigned _R, unsigned _C, typename _ElTy> 
   inline Vector<_C, _ElTy>& Mult (
	   Vector<_C, _ElTy>& out, 
	   const Matrix<_R, _C, _ElTy>& mat, 
	   const Vector<_C, _ElTy>& vec) 
   {
	   Vector<_C, _ElTy> vTmp;
		for (int i = 0; i < _C; i++) 
			vTmp[i] = Dot (mat[i], vec); 
		return Copy (out, vTmp);	
   }

	//
	template<unsigned _R, unsigned _C, typename _Ty>
	inline Matrix<_R, _C, _Ty>& SetMatRotateX (Matrix<_R, _C, _Ty>& out, _Ty rads) {
		_Ty t_Cos;
		_Ty t_Sin;

		SetMatIdent (out);
		t_Cos = (_Ty) cos (rads);
		t_Sin = (_Ty) sin (rads);

		out[1][1] = t_Cos;
		out[1][2] = -t_Sin;
		out[2][1] = t_Sin;
		out[2][2] = t_Cos;
		return out; 
		}


template<unsigned _R, unsigned _C, typename _Ty>
   inline Matrix<_R, _C, _Ty>& SetMatRotateY (
      Matrix<_R, _C, _Ty>& out, 
      _Ty rads) 
{

	_Ty t_Cos;
	_Ty t_Sin;

   SetMatIdent (out);
   t_Cos = (float)(cos (rads)),
   t_Sin = (float)(sin (rads));

   out[0][0] = t_Cos;
   out[0][2] = t_Sin;
   out[2][0] = -t_Sin;
   out[2][2] = t_Cos;

   return out; 
}

template<unsigned _R, unsigned _C, typename _Ty>

   inline Matrix<_R, _C, _Ty>& SetMatRotateZ (
      Matrix<_R, _C, _Ty>& out, 
      _Ty rads) 
{
	_Ty t_Cos;
	_Ty t_Sin;

   SetMatIdent (out);
   t_Cos = (float)(cos (rads)),
   t_Sin = (float)(sin (rads));


   out[0][0] = t_Cos;
   out[0][1] = -t_Sin;
   out[1][0] = t_Sin;
   out[1][1] = t_Cos;
   //m[0] = fcos;
   //m[1] = -fsin;
   //m[4] = fsin;
   //m[5] = fcos;
   return out; 
}


// 0 1 2 3
// 4 5 6 7
// 8 9 A B
// C D E F
// 
// 0 1 2
// 3 4 5
// 6 7 8


 
//
//matrix44f* SetMat44RotateY (matrix44f* out, float rads) {
//
//	float fcos;
//   float fsin;
//
//   SetMat44Ident (out);
//   fcos = (float)(cos (rads)),
//   fsin = (float)(sin (rads));
//   out->e[0] = fcos;
//   out->e[2] = fsin;
//   out->e[8] = -fsin;
//   out->e[10] = fcos;
//   return out; 
//
//}
//
//matrix44f* SetMat44RotateZ (matrix44f* out, float rads) {
//
//   float fsin;
//   float fcos;
//   float* m = out->e;
//
//   SetMat44Ident (out);
//   fsin = (float)(sin(rads));
//   fcos = (float)(cos(rads));
//
//   m[0] = fcos;
//   m[1] = -fsin;
//   m[4] = fsin;
//   m[5] = fcos;
//   return out; 
//}
	template<typename _Ty>
	inline Matrix<4, 4, _Ty>& AddMat44Transl (Matrix<4, 4, _Ty>& out, const Vector<3, _Ty>& v) {
		// tested
		out[0][3] += X (v);
		out[1][3] += Y (v);
		out[2][3] += Z (v);
		return out;
		}




} 
#endif // _MatrixFns_

