#ifndef _Render_
#define _Render_

#include "VecMath.h"

namespace Rn 
{
	class Renderer; 


	//
	// ProjectionState - load perspective GL projection 
	void ProjectionState (
		float fFoV, 
		float fAsp, 
		float fNear, 
		float fFar
		);
	
	//
	// ModelViewMatrix - build model view matrix
	Ma::Mat44f& ModelViewMatrix (
		Ma::Mat44f& out, 
		const Ma::Vec3f& mod_Pos, 
		const Ma::Quatf& mod_Rot, 
		const Ma::Vec3f& view_Pos, 
		const Ma::Quatf& view_Rot
		); 

   //
   // ModelViewMatrix - build model view matrix
	Ma::Mat44f& ModelViewMatrix (
		Ma::Mat44f& out, 
		const Ma::Vec3f& mod_Pos, 
		const Ma::Mat33f& mod_Rot, 
		const Ma::Vec3f& view_Pos, 
		const Ma::Quatf& view_Rot
		); 

	//
	// ModelViewState - load model GL model view matrix		
	void ModelViewState (
		const Ma::Vec3f& mod_Pos, 
		const Ma::Quatf& mod_Rot, 
		const Ma::Vec3f& view_Pos, 
		const Ma::Quatf& view_Rot
		);

	//
	// ModelViewState - load model GL model view matrix		
	void ModelViewState (
		const Ma::Vec3f& mod_Pos, 
		const Ma::Mat33f& mod_Rot, 
		const Ma::Vec3f& view_Pos, 
		const Ma::Quatf& view_Rot
		);

   //
   void Draw_AA_grid ( 
	   float		size, 
	   Ma::Quatf&	view_Rot, 
	   Ma::Vec3f&	view_Pos);


   void Draw_axes (
	   float size, 
      Ma::Matrix<4, 4, float>& mv);


   void Draw_line (
	   float             size, 
      const Ma::Vec3f&  col, 
      const Ma::Vec3f&  a, 
      const Ma::Vec3f&  b, 
	   Ma::Quatf&        view_Rot, 
      Ma::Vec3f&        view_Pos);


   void Draw_sphere (
      float size, 
      Ma::Vec3f& col3f, 
      Ma::Vec3f& model_Pos, 
	   Ma::Quatf& view_Rot, 
      Ma::Vec3f& view_Pos);


   void Draw_axes (
	   float size, 
	   Ma::Quatf& model_Rot, Ma::Vec3f& model_Pos, 
	   Ma::Quatf& view_Rot, Ma::Vec3f& view_Pos
	   ); 

   Renderer*   Create_renderer (); 
   void        Destroy_renderer (Renderer*); 

}

#endif