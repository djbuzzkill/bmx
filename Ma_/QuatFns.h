#ifndef _QuatFns_
#define _QuatFns_

#include "MathTypes.h"

namespace Ma 
{ 
//
//
	template<typename _Ty>
	Quaternion<_Ty>& Set (Quaternion<_Ty>& out, _Ty w, _Ty x, _Ty y, _Ty z)
	{
		W(out) = w;
		X(out) = x;
		Y(out) = y;
		Z(out) = z;
		return out;
	}


	//
	// Quaternion Functions
	template<typename _Ty>
	inline Quaternion<_Ty>& Copy (
		Quaternion<_Ty>& out, 
		const Quaternion<_Ty>& src) 
	{ 
		W (out) = W (src);
		X (out) = X (src);
		Y (out) = Y (src);
		Z (out) = Z (src);
		return out;
	}
	//
	template<typename _Ty>
	inline Quaternion<_Ty>& Mult (
		Quaternion<_Ty>& out, 
		const Quaternion<_Ty>& lhs, 
		const Quaternion<_Ty>& rhs) 
	{ 
		Quaternion<_Ty> qTmp;

		W(qTmp)=  (W(lhs) * W(rhs)) - (X(lhs) * X(rhs)) - (Y (lhs) * Y(rhs)) - (Z (lhs) * Z(rhs));
		X(qTmp)=  (W(lhs) * X(rhs)) + (X(lhs) * W(rhs)) + (Y (lhs) * Z(rhs)) - (Z (lhs) * Y(rhs));
		Y(qTmp)=  (W(lhs) * Y(rhs)) - (X(lhs) * Z(rhs)) + (Y (lhs) * W(rhs)) + (Z (lhs) * X(rhs));
		Z(qTmp)=  (W(lhs) * Z(rhs)) + (X(lhs) * Y(rhs)) - (Y (lhs) * X(rhs)) + (Z (lhs) * W(rhs));
		return Copy (out, qTmp); 
	}

	template<typename _Ty>
	Quaternion<_Ty>& QuatRotXAxis (Quaternion<_Ty>& out, _Ty rad) 
	{ 
		Quaternion<_Ty> q_Tmp;
		float half_Theta;
		half_Theta = _Ty(rad * 0.5);
		W(q_Tmp) = cos (half_Theta);
		X(q_Tmp) = sin (half_Theta);
		Y(q_Tmp) = _Ty(0.0);
		Z(q_Tmp) = _Ty(0.0);
		return Copy (out, q_Tmp);
	}


	template<typename _Ty>
	Quaternion<_Ty>& QuatRotYAxis (Quaternion<_Ty>& out, _Ty rad) { 
		Quaternion<_Ty>	qTmp;
		_Ty				half_Theta;
		half_Theta		= _Ty (rad*0.5);
		W (qTmp)		= cosf (half_Theta);
		X (qTmp)		= _Ty(0.0);
		Y (qTmp)		= sinf (half_Theta);
		Z (qTmp)		= _Ty(0.0);
		return Copy (out, qTmp);
	}

	template<typename _Ty>
	Quaternion<_Ty>& Conj (Quaternion<_Ty>& q) {
		X(q) = -X(q);
		Y(q) = -Y(q);
		Z(q) = -Z(q);
		return q;
	}

	template<typename _Ty>
	Quaternion<_Ty>& Conj (Quaternion<_Ty>& out, const Quaternion<_Ty>& q) {

		W(out) =  W(q);
		X(out) = -X(q);
		Y(out) = -Y(q);
		Z(out) = -Z(q);
		return out;
	}



	//float DotQuat (quaternionf* lhs, quaternionf* rhs) { 
	//	return (lhs->w * rhs->w) + (lhs->x * rhs->x) + (lhs->y * rhs->y) + (lhs->z * rhs->z);
	//}
	template<typename _Ty>
	Quaternion<_Ty>& QuatInvert (Quaternion<_Ty>& q) { 

		_Ty dot_Inv = _Ty(1) / Dot (q, q);
		Conj (q);
		W (q) *= dot_Inv;
		X (q) *= dot_Inv;
		Y (q) *= dot_Inv;
		Z (q) *= dot_Inv;
		return q;
	}

	template<typename _Ty>
	Quaternion<_Ty>& QuatInverse (Quaternion<_Ty>& out, const Quaternion<_Ty>& q) { 
	
		Quaternion<_Ty> q_Tmp;
		_Ty dot_Inv = _Ty(1) / Dot (q, q);
		Conj (Copy (q_Tmp, q));

		W(out) =  W(q_Tmp) * dot_Inv;
		X(out) =  X(q_Tmp) * dot_Inv;
		Y(out) =  Y(q_Tmp) * dot_Inv;
		Z(out) =  Z(q_Tmp) * dot_Inv;
		return out;
	}

	template<typename _Ty>
	inline Quaternion<_Ty>& SetIdent (Quaternion<_Ty>& q) {
		//q->w = 1.0f, q->x = 0.0f, q->y = 0.0f, q->z = 0.0f;
		W (q) = _Ty (1); 
		X (q) = Y (q) = Z (q) = _Ty (0); 
		return q;
	}


}
//
//
//quaternionf* CopyQuat (quaternionf* out, quaternionf* src) { 
//	out->w = src->w;
//	out->x = src->x;
//	out->y = src->y;
//	out->z = src->z;
//	return out;
//}
//
//quaternionf* MultQuat (quaternionf* out, quaternionf* lhs, quaternionf* rhs) { 
//
//	quaternionf qTmp;
//	float* o = (float*) out;
//	float* l = (float*) lhs;
//	float* r = (float*) rhs;
//
//	qTmp.w =  (l[0] * r[0]) - (l[1] * r[1]) - (l[2] * r[2]) - (l[3] * r[3]);
//	qTmp.x =  (l[0] * r[1]) + (l[1] * r[0]) + (l[2] * r[3]) - (l[3] * r[2]);
//	qTmp.y  =  (l[0] * r[2]) - (l[1] * r[3]) + (l[2] * r[0]) + (l[3] * r[1]);
//	qTmp.z =  (l[0] * r[3]) + (l[1] * r[2]) - (l[2] * r[1]) + (l[3] * r[0]);
//	return CopyQuat (out, &qTmp); 
//}
//
//quaternionf* QuatAxisRot (quaternionf* out, vector3f* axis, float rad) { 
//
//	float halftheta;
//	float halfsinetheta;
//	halftheta = rad*0.5f;
//	halfsinetheta = sinf (halftheta);
//	float* o = (float*) out;
//	float* x = (float*) axis;
//	o[0] = cosf (halftheta);
//	o[1] = x[0] * halfsinetheta;
//	o[2] = x[1] * halfsinetheta;
//	o[3] = x[2] * halfsinetheta;
//	return out;
//}
//
//
//float DotQuat (quaternionf* lhs, quaternionf* rhs) { 
//
//	return (lhs->w * rhs->w) + (lhs->x * rhs->x) + (lhs->y * rhs->y) + (lhs->z * rhs->z);
//}
//
//quaternionf* ConjQuat (quaternionf* q) {
//	q->x = -q->x;
//	q->y = -q->y;
//	q->z = -q->z;
//	return q;
//}
//
//quaternionf* QuatConj (quaternionf* out, quaternionf* q) {
//
//	out->w = q->w;
//	out->x = -q->x;
//	out->y = -q->y;
//	out->z = -q->z;
//	return out;
//}
//
//
//quaternionf* QuatNormalize (quaternionf* q) 
//{
//	//quaternionf qTmp;
//	float invmag =  1.0f / sqrtf ((q->w * q->w) + (q->x * q->x) + (q->y * q->y) + (q->z * q->z));
//	q->w *= invmag;
//	q->x *= invmag;
//	q->y *= invmag;
//	q->z *= invmag;
//	return q;
//}
//
//quaternionf* QuatInvert (quaternionf* q) { 
//
//	float dotinv = 1.0f / DotQuat (q, q);
//	ConjQuat (q);
//	q->w *= dotinv;
//	q->x *= dotinv;
//	q->y *= dotinv;
//	q->z *= dotinv;
//	return q;
//}
//
//quaternionf* QuatInverse (quaternionf* out, quaternionf* q) { 
//	
//	quaternionf qTmp;
//	float dotinv = 1.0f / DotQuat (q, q);
//	ConjQuat (CopyQuat (&qTmp, q));
//
//	out->w =  qTmp.w * dotinv;
//	out->x =  qTmp.x * dotinv;
//	out->y =  qTmp.y * dotinv;
//	out->z =  qTmp.z * dotinv;
//	return out;
//}
//
//
//
//quaternionf* QuatRotXAxis (quaternionf* out, float rad) 
//{ 
//	quaternionf qTmp;
//	float halftheta;
//	halftheta = rad*0.5f;
//	qTmp.w = cosf (halftheta);
//	qTmp.x = sinf (halftheta);
//	qTmp.y = 0.0f;
//	qTmp.z = 0.0f;
//	return CopyQuat (out, &qTmp);
//}
//
//
//quaternionf* QuatRotZAxis (quaternionf* out, float rad) 
//{ 
//	quaternionf qTmp;
//	float halftheta;
//	halftheta = rad*0.5f;
//	qTmp.w = cosf (halftheta);
//	qTmp.x = 0.0f;
//	qTmp.y = 0.0f;
//	qTmp.z = sinf (halftheta);
//	return CopyQuat (out, &qTmp);
//}
//
//quaternionf* QuatYawPitch (quaternionf* out, float yaw, float pitch) {
//	quaternionf qX, qY; 
//	return MultQuat (out, QuatRotYAxis (&qY, yaw), QuatRotXAxis (&qX, pitch));
//}


#endif // _QuatFns_
