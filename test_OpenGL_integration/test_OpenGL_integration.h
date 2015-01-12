#ifndef _test_OpenGL_integration_
#define _test_OpenGL_integration_

#include "resource.h"

#include "Dx/VecMath.h"
#include "Dx/Render.h"

#include "Dx/Data.h"


//
//
struct Bl_bone {

	typedef std::vector<Bl_bone> Array; 

	char name[64]; 
	char parent_name[64]; 

	Ma::Vec3f 
		head, 
		head_local, 
		tail, 
		tail_local; 


	Ma::Vec3f 
		x_axis, 
		y_axis, 
		z_axis; 

	Ma::Mat33f matrix; 
	Ma::Mat44f matrix_local; 

	Ma::Vec3f local_pos; 

	}; 

//
//
struct Bl_edit_bone {

	typedef std::vector<Bl_edit_bone> Array; 
	char name[64];
	char parent_name[64];
	float fRoll;
	Ma::Vec3f head_Pos; 
	Ma::Mat33f axes; 
	Ma::Mat44f matrix; 
	}; 

//
//
void Draw_bones (
	Bl_bone::Array& bones, 
	Ma::Quatf&	view_Rot, 
	Ma::Vec3f&	view_Pos
	); 



//
//
void Draw_axes (
	float size, 
	Ma::Quatf& model_Rot, Ma::Vec3f& model_Pos, 
	Ma::Quatf& view_Rot, Ma::Vec3f& view_Pos
	); 

//
//
void Draw_axes (
	float size, 
	const Ma::Mat33f& model_Rot, const Ma::Vec3f& model_Pos, 
	const Ma::Quatf& view_Rot, const Ma::Vec3f& view_Pos
	); 

//
//
void Draw_AA_grid (
	float		size, 
	Ma::Quatf&	view_Rot, 
	Ma::Vec3f&	view_Pos
	); 


//
//
void Draw_edit_bones (
	Bl_edit_bone::Array&	bones, 
	Ma::Quatf&				view_Rot, 
	Ma::Vec3f&				view_Pos
	); 


#endif _test_OpenGL_integration_