// experimental_01.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "test_OpenGL_integration.h"

#include <map>
#include <list>
#include <vector>
#include <string>
#include <memory>
#include <sstream>
#include <algorithm>

#include <Dx/Dx.h>
#include <Xp/Xp.h> 
#include <Dx/Horde3D_platform.h> 


#include <Dx/IKSolver2d.h>

#include <Horde3D.h>
#include <Horde3DUtils.h>

#include <SDL.h>

#include <SDL_opengl.h>

#include <tinyxml2.h>

// #include "wingdi.h"
// #include "GL/glew.h"
// #include "GL/wglew.h"



//#include <WinUser.h> ??


#if defined (USING_DevIL)
   #include <IL/il.h>
#endif


#define MAX_LOADSTRING 100



Ma::Quatf axes_Rot; 
Ma::Vec3f axes_Pos; 


static Bl_edit_bone::Array	sg_edit_bones; 
static Bl_bone::Array		sg_bones; 

// Global Variables:
// HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// Forward declarations of functions included in this code module:
//ATOM				MyRegisterClass(HINSTANCE hInstance);
//LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
//
//static Ma::Quatf					model_Rot, view_Rot; 
//static Ma::Vec3f					model_Pos, view_Pos; 
static std::map<WPARAM, bool>	sg_keyState; 
static int						sg_Wd_window = 0;
static int						sg_Ht_window = 0; 
static int						sg_X_mouse = 0;
static int						sg_Y_mouse = 0;
//
//
//




namespace Rs
{
   typedef unsigned short Index;

   struct Vertex
   {
      Ma::Vec3f pos; 
      Ma::Vec3f norm; 
      Ma::Vec3f tan; 
      Ma::Vec3f bitan; 
      Ma::Vec3f txco[2]; 
   }; 

   
   struct Anim; 
}



template<typename ElTy>
  inline 
  Ma::Matrix<3, 3, ElTy>&  
  EulerMatrix_YXZ (
     Ma::Matrix<3, 3, ElTy>& m, 
     ElTy x, 
     ElTy y, 
     ElTy z)
{
  Ma::Matrix<3, 3, ElTy> mX, mY, mZ, t; 
  Ma::SetMatRotateX (mX, x); 
  Ma::SetMatRotateY (mY, y); 
  Ma::SetMatRotateZ (mZ, z); 

  Ma::MultMat (t, mY, mZ); 
  Ma::MultMat (m, mX, t); 

  return m;
}


template<typename ElTy>
  inline Ma::Quaternion<ElTy>& EulerQuat_YXZ (
      Ma::Quaternion<ElTy>& q, 
      ElTy x, ElTy y, ElTy z)
{

  Ma::Quaternion<ElTy> qX, qY, qZ, t; 
  Ma::RotXAxis (qX, x); 
  Ma::RotXAxis (qY, y); 
  Ma::RotXAxis (qZ, z); 

  Ma::MultMat (t, qY, qZ); 
  Ma::MultMat (q, qX, t); 

  return q;
}

template<typename ElTy>
   inline Ma::Matrix<4, 4, ElTy>& 
   SetMat (Ma::Matrix<4, 4, ElTy>& m, const Ma::Quaternion<ElTy>& q)
{
   ElTy qw = Ma::W (q);
   ElTy qx = Ma::X (q);
   ElTy qy = Ma::Y (q);
   ElTy qz = Ma::Z (q);

   const ElTy n = ElTy(1)/sqrt(qx*qx+qy*qy+qz*qz+qw*qw);
   qx *= n;
   qy *= n;
   qz *= n;
   qw *= n;

	m[0][0] = 1.0 - 2.0*qy*qy - 2.0*qz*qz; m[0][1] = 2.0*qx*qy - 2.0*qz*qw;       m[0][2] = 2.0*qx*qz + 2.0*qy*qw;          m[0][3] = 0.0;
	m[1][0] = 2.0*qx*qy + 2.0*qz*qw;       m[1][1] = 1.0 - 2.0*qx*qx - 2.0*qz*qz; m[1][2] = 2.0*qy*qz - 2.0*qx*qw;          m[1][3] = 0.0;
	m[2][0] = 2.0*qx*qz - 2.0*qy*qw;       m[2][1] = 2.0*qy*qz + 2.0*qx*qw;       m[2][2] = 1.0 - 2.0*qx*qx - 2.0 *qy*qy;   m[2][3] = 0.0;
	m[3][0] = 0.0;                         m[3][1] = 0.0;                         m[3][2] = 0.0,                            m[3][3] = 1.0;

   return m;
}

template<typename ElTy>
   inline Ma::Matrix<3, 3, ElTy>& 
   SetMat (Ma::Matrix<3, 3, ElTy>& m, const Ma::Quaternion<ElTy>& q)
{
   ElTy qw = Ma::W (q);
   ElTy qx = Ma::X (q);
   ElTy qy = Ma::Y (q);
   ElTy qz = Ma::Z (q);

   const ElTy n = ElTy(1)/sqrt(qx*qx+qy*qy+qz*qz+qw*qw);
   qx *= n;
   qy *= n;
   qz *= n;
   qw *= n;

	m[0][0] = 1.0 - 2.0*qy*qy - 2.0*qz*qz; m[0][1] = 2.0*qx*qy - 2.0*qz*qw;       m[0][2] = 2.0*qx*qz + 2.0*qy*qw;          
	m[1][0] = 2.0*qx*qy + 2.0*qz*qw;       m[1][1] = 1.0 - 2.0*qx*qx - 2.0*qz*qz; m[1][2] = 2.0*qy*qz - 2.0*qx*qw;          
	m[2][0] = 2.0*qx*qz - 2.0*qy*qw;       m[2][1] = 2.0*qy*qz + 2.0*qx*qw;       m[2][2] = 1.0 - 2.0*qx*qx - 2.0 *qy*qy;   

   return m;
}

//
class JointVisitor : public tinyxml2::XMLVisitor
  {

public: 

   struct bone 
   {
      std::string name; 
      int         joint_index;
   
      const bone*             parent; 
      std::list<const bone*>  children; 
   
      float       tx, ty, tz, 
                  rx, ry, rz, 
                  sx, sy, sz; 
   }; 

   typedef std::map<std::string, bone> BoneMap; 
   BoneMap bone_map;

   JointVisitor () {}
   virtual ~JointVisitor () {}

   /// Visit an element.
   virtual bool VisitEnter ( const tinyxml2::XMLElement& elem, const tinyxml2::XMLAttribute* first_attr)	
   {
      const std::string type = elem.Name (); 
      if (type == "Joint")
      {
         const char* name = elem.Attribute ("name"); 
         bone& b = bone_map[name] ; 
         const tinyxml2::XMLElement* p = dynamic_cast<const tinyxml2::XMLElement*> (elem.Parent()); 

         b.name = name; 

         std::string p_name = p->Attribute("name");   
         b.parent = 0;
         b.joint_index = 0;

         if (bone_map.count (p_name)) 
         {
            bone_map[p_name].children.push_back (&b); 
            b.parent = &bone_map[p_name]; // ->Attribute ("name"); 
         }

         b.joint_index = elem.IntAttribute ("jointIndex"); 

         b.tx = b.ty = b.tz = 0.0f;
         b.tx = elem.QueryFloatAttribute ("tx", &b.tx); 
         b.ty = elem.QueryFloatAttribute ("ty", &b.ty); 
         b.tz = elem.QueryFloatAttribute ("tz", &b.tz); 
         
         b.rx = b.ry = b.rz = 0.0f;
         elem.QueryFloatAttribute ("rx", &b.rx ); 
         elem.QueryFloatAttribute ("ry", &b.ry ); 
         elem.QueryFloatAttribute ("rz", &b.rz ); 

         b.sx = b.sy = b.sz = 1.0f;
         elem.QueryFloatAttribute ("sx", &b.sx); 
         elem.QueryFloatAttribute ("sy", &b.sy); 
         elem.QueryFloatAttribute ("sz", &b.sz); 
      }
      return true; 
   }

  // std::string scene_xml = "C:/Quarantine/awsum/humanoid_test/models/watsmyname.geo.scene"; 
  // tinyxml2::XMLDocument doc;
  // doc.LoadFile( scene_xml.c_str() );
  // tinyxml2::XMLElement* root_elem = doc.RootElement (); 
  // JointVisitor jvisitor;
  // root_elem->Accept (&jvisitor); 

  };

//
//
// template<typename ElTy>
//    inline Ma::Matrix<4, 4, ElTy>&
// Get_transform (Ma::Matrix<4, 4, ElTy>& m, JointVisitor::bone* b)
// {
//    if (b->parent)
//    {
//       Ma::Matrix<4, 4, ElTy> p, t_;
//       Get_transform (p, b->parent); 
//       return Ma::Mult (m, EulerMatrix_YXZ (t_, b->rx, b->ry, b->rz), p); 
//    }
//    else
//    {
//       return EulerMatrix_YXZ (m, b->rx, b->ry, b->rz); 
//    }
// 
// }

//static PIXELFORMATDESCRIPTOR sg_Default_PixelFormatDescriptor = 
//{
//	sizeof (PIXELFORMATDESCRIPTOR), // WORD  nSize;
//	0x0001,		// WORD  nVersion;
//	PFD_DRAW_TO_WINDOW | PFD_DOUBLEBUFFER | PFD_SUPPORT_OPENGL, // DWORD dwFlags;
//	PFD_TYPE_RGBA, //BYTE  iPixelType;
//	32,			// BYTE  cColorBits;
//
//	0,			// BYTE  cRedBits;
//	0,			// BYTE  cRedShift;
//	0,			// BYTE  cGreenBits;
//	0,			// BYTE  cGreenShift;
//	0,			// BYTE  cBlueBits;
//	0,			// BYTE  cBlueShift;
//				   
//	0,			// BYTE  cAlphaBits;
//	0,			// BYTE  cAlphaShift;
//	0,			// BYTE  cAccumBits;
//	0,			// BYTE  cAccumRedBits;
//	0,			// BYTE  cAccumGreenBits;
//	0,			// BYTE  cAccumBlueBits;
//	0,			// BYTE  cAccumAlphaBits;
//				   
//	24,			// BYTE  cDepthBits;
//	8,			// BYTE  cStencilBits;
//	0,			// BYTE  cAuxBuffers;
//				   
//	0,			// BYTE  iLayerType;
//	0,			// BYTE  bReserved;
//	0,			// DWORD dwLayerMask;
//	0,			// DWORD dwVisibleMask;
//	0,			// DWORD dwDamageMask;	
//}; 




//
//
template<typename Ty>
Ma::Vector<3, Ty>& Interp_bezier_quadratic (
	Ma::Vector<3, Ty>& out, 
	const Ma::Vector<3, Ty>& p0, 
	const Ma::Vector<3, Ty>& p1, 
	const Ma::Vector<3, Ty>& p2, 
	Ty                        t)
{
	Ma::Vector<3, _Ty> p_A, p_B; 

	Ma::Lerp (p_A, p0, p1, t);
	Ma::Lerp (p_B, p1, p2, t);

	return 	Ma::Lerp (out, p_A, p_B, t); 
}


namespace Lg // level geometry
{
	struct GeomObj;
	struct GeomCollection; 
	struct PolygonTesselator; 
	struct TriSurf;
	struct Vertex;
	struct QuadSurf;
	struct Edge;

   // topology like mountain range
   struct Spine_geom
   {
      std::vector<Rs::Index>  inds;
      std::vector<Rs::Vertex> verts;   
   };


// 
// 
// 
// 
   struct Spine_params 
   {
      std::vector<Ma::Vec3f>  pos;
      std::vector<Ma::Vec3f>  dirs;
      int                     num_sub_divs;

      // left/right, 2 per point
      float*            horz_len;   

      // left to right, (num_sub_divs + 1) per seg
      float*            vert_heights;
   };

   //
   // generate geometry for spine described
   bool Gen_spine_geom (
      Spine_geom&          out,
      const Spine_params&  params, 
      bool                 separate_facets, 
      bool                 closedgeom) 
   {
      return false;
   }

   //
   struct BendGeom
   {
      const unsigned short*   inds;
      const Ma::Vec3f*        verts;

      int                     num_sub_divs; 
      int                     num_spokes; 
      float                   arc_start;
      float                   arc_stop;
      Ma::Vec3f               inner_height; 
   };    
      
   //
   //
   bool Gen_bend_geom (float* overts, unsigned short* oinds, const BendGeom& params)
   {
      return  false;
   } 

   //
   // 
   struct VeinGeom
   {
      int                     num_points;
      const unsigned short*   inds; 
      const Ma::Vec3f*        verts;       
   };

   //
   bool Gen_vein_geom (Ma::Vec3f* overts, unsigned short* oinds, const VeinGeom& params )
   {
      return false; 
   }

	//
	struct GeomCollection {
		// actual rendering geometry
		size_t Add_render_op (size_t start, size_t count, int primitive_type); 
		size_t Add_vertex (const Ma::Vec3f& pos, const Ma::Vec3f& nrm, const Ma::Vec2f& txco); 
		// pos ,normal, txcrd, 
		}; 

	//
	//
	struct PolygonTesselator {
		virtual ~PolygonTesselator() { 
			}

		virtual size_t GenPolys (GeomCollection* collector, const GeomObj& geom_Obj) = 0;
		};

	// scene geometry vertex
	struct Vertex : public PolygonTesselator {

		Ma::Vec3f	v_Pos; 
		float		v_Rad; 
		
		// emits triangle fan 
		virtual size_t GenPolys (GeomCollection* collector, const GeomObj& geom_Obj);

		// with each adjoining vertex forms an edge
		std::list<size_t>	adj_Verts; 
		}; 

	//
	struct TriSurf : public PolygonTesselator {
		TriSurf () : shortest_Edge (-1) { 
		}
		// vert indices
		size_t	v_Inds[3]; 
		// 3 adjacent neihgbors
		size_t	adj_Surfs[4];
		// 
		int		shortest_Edge;

		// emits subdiveded triangle
		virtual size_t GenPolys (GeomCollection* collector, const GeomObj& geom_Obj);


	private:  
		// 
		Ma::Vec3f& InterPoint (Ma::Vec3f& out, const std::vector<Vertex>& vert_Arr, size_t iEdge, float t); 

	}; 

	//
	struct QuadSurf : public PolygonTesselator {

		// emits subdiveded quad 
		virtual size_t GenPolys (GeomCollection* collector, const GeomObj& geom_Obj);

		// 
		size_t v_Inds[4]; 
		// 4 adjacent neighbors 
		size_t adj_Surfs[4];
		}; 
	
	//
	struct Edge : public PolygonTesselator 
      {
		// emits subdiveded connecting strip
		virtual size_t GenPolys (GeomCollection* collector, const GeomObj& geom_Obj);

		// each edge is defined by one bezier curve:  a.vert_Inds[0], b.contr_Pt, c.vert_Inds[1]
		Ma::Vec3f	contr_Pt; 
		size_t		vert_Inds[2];
		size_t		surf_Inds[2];
		}; 

	//
	struct GeomObj {
		// contains geometry consisting of Vertex's, TriSurf's, Edge's, etc...
		// emits rendering geometry
		size_t	subd_Level; 
		float	vert_Radius; 

		std::vector<Vertex>		vert_Arr; 
		std::vector<Edge>		edge_Arr; 
		std::vector<TriSurf>	tri_Surfs; 
		std::vector<QuadSurf>	quad_Surfs; 

		GeomCollection			geom_Coll; 
		}; 



	Ma::Vec3f& TriSurf::InterPoint (Ma::Vec3f& out, const std::vector<Vertex>& vert_Arr, size_t iEdge, float t)
	{
		Ma::Vec3f v_Tmp; 

		float edge_Lengths[3]; 
		 
		if (shortest_Edge == -1)
		{
			// interpolate control point of shortest length with opposite vert
			edge_Lengths[0] = Ma::Length (Ma::Sub (v_Tmp, vert_Arr[v_Inds[1]].v_Pos, vert_Arr[v_Inds[0]].v_Pos)); 
			edge_Lengths[1] = Ma::Length (Ma::Sub (v_Tmp, vert_Arr[v_Inds[2]].v_Pos, vert_Arr[v_Inds[1]].v_Pos)); 
			edge_Lengths[2] = Ma::Length (Ma::Sub (v_Tmp, vert_Arr[v_Inds[0]].v_Pos, vert_Arr[v_Inds[2]].v_Pos)); 


			if (edge_Lengths[0] < edge_Lengths[1])
			{
				if (edge_Lengths[0] < edge_Lengths[2])
					shortest_Edge = 0; 
				else
					shortest_Edge = 2; 
			}
			else
			{
				if (edge_Lengths[1] < edge_Lengths[2])
					shortest_Edge = 1; 
				else
					shortest_Edge = 2; 
			}

		}
		
		switch (iEdge)
		{
		case 0: break; 
		case 1: break; 
		case 2: break; 

		}

		// vert_Arr
		return out;	
	}
	//
	size_t TriSurf::GenPolys (GeomCollection* collector, const GeomObj& geom_Obj)
	{
		// gen vertex list (rendering data)
		if (geom_Obj.vert_Radius > 0.0f)
		{
			// find reduced area tri
		}
		else
		{
			
		}

		//geom_Obj.vert_Arr[]
		//v_Inds[]
		return 0;
	}

	size_t Vertex::GenPolys (GeomCollection* collector, const GeomObj& geom_Obj)
	{
		// gen vertex list (rendering data)
		

		//geom_Obj.vert_Arr[]
		//v_Inds[]
		return 0;
	}

	size_t QuadSurf::GenPolys (GeomCollection* collector, const GeomObj& geom_Obj)
	{
		// gen vertex list (rendering data)
		

		//geom_Obj.vert_Arr[]
		//v_Inds[]
		return 0;
	}

	size_t Edge::GenPolys (GeomCollection* collector, const GeomObj& geom_Obj)
	{
		// gen vertex list (rendering data)
		

		//geom_Obj.vert_Arr[]
		//v_Inds[]
		return 0;
	}


}




//
//
void Render_triangle2 ()
{
	glBegin (GL_LINES); 
	
	glColor3f (1.0f, 0.0f, 1.0f); 
	


	glVertex3f (0.0f, 1.0f, -1.0f); // front
	glVertex3f (0.0f, -1.0f, 0.0f); 

	glVertex3f (0.0f, 1.0f, 1.0f); // front
	glVertex3f (0.0f, -1.0f, 0.0f); 


	glEnd (); 
}


void Render_test_triangle ()
{
	glBegin (GL_LINES); 
	
	glColor3f (1.0f, 0.0f, 1.0f); 
	
	// bottom
	
	glVertex3f (-1.0f, -1.0f, 0.0f); // front
	glVertex3f (0.0f,  1.0f, 0.0f); 

	glVertex3f (1.0f, -1.0f, 0.0f); // front
	glVertex3f (0.0f, 1.0f, 0.0f); 


	glEnd (); 
	
}

struct v3f : public Ma::Vector<3, float> 
{
	v3f (float x, float y, float z)
	{
		e[0] = x;
		e[1] = y;
		e[2] = z;
	}
};



void Render_tile ()
{
	static Ma::Vec3f sg_Floor_Tile[] = {
	
		v3f (-0.95f, -2.0f, -0.95f), 
		v3f (-0.95f, -2.0f,  0.95f),  
		v3f ( 0.95f, -2.0f, -0.95f), 
		v3f ( 0.95f, -2.0f,  0.95f),  
		v3f (-0.95f, -2.0f, -0.95f), 
		v3f ( 0.95f, -2.0f, -0.95f),  
		v3f (-0.95f, -2.0f,  0.95f), 
		v3f ( 0.95f, -2.0f,  0.95f),  

	};
	glBegin (GL_LINES); 
	
	glColor3f (0.0f, 0.5f, 1.0f); 

	for (size_t i = 0; i < 8; i++)
		glVertex3fv (sg_Floor_Tile[i].ptr ()); 

	glEnd (); 

}

void Render_test_cube ()
{
	glBegin (GL_LINES); 
	
	glColor3f (1.0f, 0.0f, 1.0f); 
	
	// bottom
	
	glVertex3f (-1.0f, -1.0f, -1.0f); // front
	glVertex3f (1.0f,  -1.0f, -1.0f); 
	glVertex3f (-1.0f, -1.0f, 1.0f); // rear
	glVertex3f (1.0f, -1.0f,  1.0f); 
	glVertex3f (-1.0f, -1.0f, 1.0f); // left
	glVertex3f (-1.0f, -1.0f, -1.0f); 
	glVertex3f (1.0f, -1.0f, 1.0f); // right
	glVertex3f (1.0f, -1.0f, -1.0f); 

	// top
	glVertex3f (-1.0f, 1.0f, -1.0f); // front
	glVertex3f (1.0f,  1.0f, -1.0f); 
	glVertex3f (-1.0f, 1.0f, 1.0f); // rear
	glVertex3f (1.0f, 1.0f,  1.0f); 
	glVertex3f (-1.0f, 1.0f, 1.0f); // left
	glVertex3f (-1.0f, 1.0f, -1.0f); 
	glVertex3f (1.0f, 1.0f, 1.0f); // right
	glVertex3f (1.0f, 1.0f, -1.0f); 

	// sides
	glVertex3f (-1.0, -1.0, 1.0); // fr lt
	glVertex3f (-1.0, 1.0, 1.0); 

	glVertex3f (1.0, -1.0, 1.0); // fr rt
	glVertex3f (1.0,  1.0, 1.0); 

	glVertex3f (-1.0, -1.0, -1.0); // r lt
	glVertex3f (-1.0,  1.0, -1.0); 
	glVertex3f (1.0, -1.0, -1.0); // r rt
	glVertex3f (1.0,  1.0, -1.0); 

	glEnd (); 

}

void Render_test_obj (
	const Ma::Vec3f& obj_Pos, 
	const Ma::Quatf& obj_Rot, 
	const Ma::Vec3f& view_Pos, 
	const Ma::Quatf& view_Rot
	)
{
	Rn::ModelViewState (obj_Pos, obj_Rot, view_Pos, view_Rot); 
	Render_test_triangle();
	Render_triangle2	();
	Render_test_cube	(); 
}

void Render_floor (
	const Ma::Vec3f& obj_Pos, 
	const Ma::Quatf& obj_Rot, 
	const Ma::Vec3f& view_Pos, 
	const Ma::Quatf& view_Rot
	) 
{
	Ma::Vec3f abs_Pos, tile_Pos; 

	for (size_t iZ = 0; iZ < 10; iZ++)
		for (size_t iX = 0; iX < 10; iX++)
	{
		float tile_X = (iX * 2.0f) - 10.0f + 1.0f;
		float tile_Z = (iZ * 2.0f) - 10.0f + 1.0f;
		Ma::Set (tile_Pos, tile_X, 0.0f, tile_Z), 
		Ma::Add (abs_Pos, obj_Pos, tile_Pos); 
		Rn::ModelViewState (abs_Pos, obj_Rot, view_Pos,  view_Rot); 

		Render_tile (); 
	}
}



void update_view_transform (float move_Rate, Ma::Quatf& view_Rot, Ma::Vec3f& view_Pos)
{
	Ma::Quatf q_Y,  q_X; 

	static float dYdx = 0.01f; 
	static float dXdy = 0.01f; 

int wat = 0;


	Ma::Vec3f dir_Fwd, dir_Right; 

	float view_X_rot = (sg_Y_mouse * dXdy) - (0.9f * Ma::Pi);
	Ma::Clamp (view_X_rot, -float (0.4f * Ma::Pi), float (0.20f * Ma::Pi)); 

	float view_Y_rot = sg_X_mouse * -dYdx;
	
	Ma::Spherical (dir_Fwd,  view_Y_rot - float(Ma::HalfPi), 0.0f); 
	Ma::Spherical (dir_Right,view_Y_rot, 0.0f);

	// view movement
	Ma::Vec3f incr_Vec;
	if (sg_keyState['F']) {
		Ma::Scale (incr_Vec, dir_Fwd, move_Rate ); 
		X (incr_Vec) = -X (incr_Vec);
		Ma::Incr (view_Pos, incr_Vec);
		}
	else if (sg_keyState['V']) {
		Ma::Scale (incr_Vec, dir_Fwd, -move_Rate); 
		X (incr_Vec) = -X (incr_Vec);
		Ma::Incr (view_Pos, incr_Vec);
		}
	//
	if (sg_keyState['D']) {
		Ma::Scale (incr_Vec, dir_Right, -move_Rate); 
		Z (incr_Vec) = 	-Z (incr_Vec); 
		Ma::Incr (view_Pos, incr_Vec);
		}
	if (sg_keyState['G']) {
		Ma::Scale (incr_Vec, dir_Right, move_Rate); 
		Z (incr_Vec) = 	-Z (incr_Vec); 
		Ma::Incr (view_Pos, incr_Vec);
		}
	// view orientation
	Ma::Set (view_Rot, 1.0f, 0.0f, 0.0f, 0.0f);
	if (sg_X_mouse > 0 && sg_Y_mouse > 0 && sg_X_mouse < sg_Wd_window && sg_Y_mouse < sg_Ht_window)
		Ma::Mult (view_Rot, Ma::RotYAxis (q_Y, view_Y_rot), Ma::RotXAxis (q_X, view_X_rot)); 

}

static Ma::Quatf	sg_model_Rot, sg_view_Rot; 
static Ma::Vec3f	sg_model_Pos, sg_view_Pos; 

void Init_main ()
{
	Ma::Set (sg_view_Pos, 0.0f, 10.0f, 0.0f);
	Ma::Set (sg_view_Rot, 1.0f, 0.0f, 0.0f, 0.0f);
}



//
void Update_main (HDC hdc)
{

	//if (true) 
	//{
	//	std::ostringstream oss;
	//	oss << "dX." << view_X_rot<< " | " << "dY." << view_Y_rot << std::endl;
	//	::OutputDebugStringA (oss.str ().c_str ()); 
	//}

	update_view_transform (0.04f, sg_view_Rot, sg_view_Pos); 

	static float rot_Y = 0.0; 
	rot_Y += float(Ma::Pi) / 30.0f; 
	//
	Rn::ProjectionState (
		float (Ma::Pi / 3.0), 
		float (sg_Wd_window) / float (sg_Ht_window) , 
		1.0f, 
		1000.0f
		); 

	//
	//
	glClearColor (0.1f, 0.1f, 0.1f, 1.0f); 
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 

	Render_test_obj (
		Ma::Set(sg_model_Pos, 0.0f, 0.0f, -6.0f), 
		Ma::RotYAxis(sg_model_Rot, rot_Y), 
		sg_view_Pos, 
		sg_view_Rot		
		); 
	

	Ma::Vec3f floor_Pos;
	Ma::Quatf ident_Quat;
	
	Render_floor (
		Ma::Set (floor_Pos, 0.0f, 0.0f, 0.0f), 
		Ma::SetIdent (ident_Quat), 
		sg_view_Pos, 
		sg_view_Rot		
		); 

	::SwapBuffers (hdc); 
}



Bl_edit_bone::Array& Load_edit_bones (Bl_edit_bone::Array& out, size_t num_Bones, const std::string& fname)
{
	out.resize (num_Bones); 
	std::shared_ptr<FILE> f (fopen (fname.c_str(), "rb"), fclose); 
	for (size_t i = 0; i < num_Bones; i++)
	{
		fread (out[i].name, sizeof(char), 64, f.get()); 	
		fread (out[i].parent_name, sizeof(char), 64, f.get()); 	
		fread (&out[i].fRoll, sizeof (float), 1, f.get()); 

		fread (out[i].head_Pos.ptr (), sizeof (float), 3, f.get()); 
		
		fread (out[i].axes[0].ptr(), sizeof(float), 3, f.get()); 
		fread (out[i].axes[1].ptr(), sizeof(float), 3, f.get()); 
		fread (out[i].axes[2].ptr(), sizeof(float), 3, f.get()); 

		fread (out[i].matrix.ptr (), sizeof(float), 16, f.get()); 
	}

	return out; 
}


Bl_bone::Array& Load_bone_dump (Bl_bone::Array& barr, size_t bone_Count, const std::string& fname)
{
	barr.resize (bone_Count); 
	std::shared_ptr<FILE> f (fopen (fname.c_str(), "rb"), fclose); 

	for (size_t i = 0; i < bone_Count; i++)
	{
		fread (barr[i].name, 1, 64, f.get()); 
		fread (barr[i].parent_name, 1, 64, f.get()); 

		fread (barr[i].head.ptr(), sizeof(float), 3, f.get()); 
		fread (barr[i].head_local.ptr(), sizeof(float), 3, f.get()); 

		fread (barr[i].tail.ptr(), sizeof(float), 3, f.get()); 
		fread (barr[i].tail_local.ptr(), sizeof(float), 3, f.get()); 

		fread (barr[i].x_axis.ptr(), sizeof(float), 3, f.get()); 
		fread (barr[i].y_axis.ptr(), sizeof(float), 3, f.get()); 
		fread (barr[i].z_axis.ptr(), sizeof(float), 3, f.get()); 

		fread (barr[i].matrix.ptr(), sizeof(float), 9, f.get()); 

		fread (barr[i].matrix_local.ptr(), sizeof(float), 16, f.get()); 

		Ma::X (barr[i].local_pos) = barr[i].matrix_local[0][3]; 
		Ma::Y (barr[i].local_pos) = barr[i].matrix_local[1][3]; 
		Ma::Z (barr[i].local_pos) = barr[i].matrix_local[2][3]; 

		// .geo   : inverse bind matrices 
		// .geo   : bone inds, bone wgts
		// .scene : skeleton hierarchy	


	}

	return barr; 
}

struct H3D_anim 
{

	char magic[4]; 
	int version; 
	int num_Anims; 
	int num_Frames;

	struct frame {	// frame ? 
		float rotation[4]; 
		float translation[3]; 
		float scale[3]; 
		}; 


	struct animation {
		char node_Name[256]; 
		char compressed; 
		std::vector<frame> frames; 
		}; 

	std::vector<animation> animations; 
};


H3D_anim& Load_anim (H3D_anim& out, const std::string& fname)
{
	std::shared_ptr<FILE> f (fopen (fname.c_str(), "rb"), fclose); 
	if (f) 
	{
		fread (&out.magic, 1, 4, f.get()); 
		fread (&out.version, 1, sizeof (int), f.get()); 
		fread (&out.num_Anims, 1, sizeof (int), f.get()); 
		fread (&out.num_Frames, 1, sizeof (int), f.get()); 

		
		out.animations.resize (out.num_Anims); 

		for (size_t iAnim = 0; iAnim < out.num_Anims; iAnim++)
		{
			fread (out.animations[iAnim].node_Name, 1, 256, f.get()); 
			fread (&out.animations[iAnim].compressed, 1, 1, f.get()); 

			out.animations[iAnim].frames.resize (out.animations[iAnim].compressed ? 1 : out.num_Frames); 
			for (size_t iFr = 0; iFr < (out.animations[iAnim].compressed ? 1 : out.num_Frames); iFr++) 
			{
				fread (out.animations[iAnim].frames[iFr].rotation, sizeof (float), 4, f.get()); 
				fread (out.animations[iAnim].frames[iFr].translation, sizeof (float), 3, f.get()); 
				fread (out.animations[iAnim].frames[iFr].scale, sizeof (float), 3, f.get()); 
			}

		}
	}

	
	return out; 
}

//<Model name = "Underground_Cave" geometry = "model/Underground.geo">
//  <Mesh name = "UpperProtrusion" material = "material/Underground_Cave.mtrl" batchStart = "0" batchCount = "1216" vertRStart = "0" vertREnd = "224" tx = "-430.8789978027344" ty = "402.2383728027344" tz = "1114.75" />
//  <Mesh name = "Cave_inset" material = "material/Underground_Cave.mtrl" batchStart = "1216" batchCount = "1266" vertRStart = "224" vertREnd = "468" tx = "5.39378023147583" ty = "13.776209831237793" tz = "450.11810302734375" />
//  <Mesh name = "Ledge_0" material = "material/Underground_Cave.mtrl" batchStart = "2482" batchCount = "192" vertRStart = "468" vertREnd = "519" tx = "-176.75660705566406" ty = "722.0390625" tz = "329.8165283203125" />
//  <Mesh name = "Spiral_Column" material = "material/Underground_Cave.mtrl" batchStart = "2674" batchCount = "1008" vertRStart = "519" vertREnd = "695" tx = "-44.801456451416016" ty = "95.11747741699219" tz = "471.62298583984375" />
//  <Mesh name = "Cave_main" material = "material/Underground_Cave.mtrl" batchStart = "3682" batchCount = "7958" vertRStart = "695" vertREnd = "2090" tx = "5.39378023147583" ty = "13.776209831237793" tz = "450.11810302734375" />
//</Model>

struct Primitive_params
{
	int indexStart; 
	int indexCount; 
	int vertFirst;
	int vertLast; 
}; 

Primitive_params sg_Cave_primitives[] = 
{
	{ 0 	, 1242  , 0 	, 224  }, 
	{ 1242  , 1266  , 224 	, 468  }, 
	{ 2508  , 192 	, 468 	, 519  }, 
	{ 2700  , 1008  , 519 	, 695  }, 
	{ 3708  , 7998  , 695 	, 2090 }, 
}; 

float sg_Primitive_locations[][3] = 
{
	{ 402.2383728027344	,	1114.75				,	-430.8789978027344	},
	{ 13.776209831237793,	450.11810302734375	,	 5.39378023147583	},
	{ 722.0390625		,	329.8165283203125	,	-176.75660705566406 }, 
	{ 95.11747741699219	,	471.62298583984375	,	-44.801456451416016 }, 
	{ 13.776209831237793,	450.11810302734375	,	 5.39378023147583	},
}; 


std::string		   geo_fname = "C:/Quarantine/awsum/Underground/model/Underground.geo"; 
static H3D::Geo   sg_geo; 
	



void Accum_extents (Ma::Vec3f& vMin, Ma::Vec3f& vMax, const Ma::Vec3f& v)
{
	Ma::X (vMin) = (std::min) ( Ma::X (vMin), Ma::X (v) ); 
	Ma::Y (vMin) = (std::min) ( Ma::Y (vMin), Ma::Y (v) ); 
	Ma::Z (vMin) = (std::min) ( Ma::Z (vMin), Ma::Z (v) ); 

	Ma::X (vMax) = (std::max) ( Ma::X (vMax), Ma::X (v) ); 
	Ma::Y (vMax) = (std::max) ( Ma::Y (vMax), Ma::Y (v) ); 
	Ma::Z (vMax) = (std::max) ( Ma::Z (vMax), Ma::Z (v) ); 
}


struct transform_frame
{
	typedef std::vector<transform_frame> Array; 
	Ma::Vec3f pos, rot, scl; 
}; 


void Init_Cave_test ()
{
	// H3D example of skel hierarchy
	std::shared_ptr<FILE> f (fopen ("c:/usr/man_dump.dat", "rb"), fclose); 

	transform_frame::Array bone_Frames (22); 
	for (size_t i = 0; i < 22; i++)
	{
		fread (bone_Frames[i].pos.ptr(), sizeof (float), 3, f.get()); 
		fread (bone_Frames[i].rot.ptr(), sizeof (float), 3, f.get()); 
		fread (bone_Frames[i].scl.ptr(), sizeof (float), 3, f.get()); 
	}



	// EditBones, different from regular bones
	Load_edit_bones (sg_edit_bones, 21, "C:/usr/dump/edit_bone.dat"); 

	
	// Blender dump of armature
	Load_bone_dump (sg_bones, 21, "c:/usr/dump/bones.dat"); 


	// important state
	glEnable (GL_DEPTH_TEST);  
	glEnable (GL_CULL_FACE); 

#if defined (USING_DevIL)

	ilInit (); 

	ILuint imgID =  ilGenImage (); 

	GLuint txrID; 
	ilBindImage (imgID); 
	std::string img_fname = "C:/Quarantine/awsum/Underground/texture/organic.png"; 

	if (ilLoadImage (img_fname.c_str ()))
	{
		size_t img_Width	= ilGetInteger (IL_IMAGE_WIDTH); 
		size_t img_Height	= ilGetInteger (IL_IMAGE_HEIGHT); 

		size_t img_BPP				= ilGetInteger (IL_IMAGE_BPP); 
		size_t img_BytesPerPixel	= ilGetInteger (IL_IMAGE_BYTES_PER_PIXEL); 

		size_t img_Format	= ilGetInteger (IL_IMAGE_FORMAT); 
		size_t img_Type		= ilGetInteger (IL_IMAGE_TYPE); 

		size_t		num_Bytes	= img_BytesPerPixel	* img_Width * img_Height;
		ILubyte*	bytes		=  ilGetData (); 

		std::vector<unsigned char> img_Bytes (num_Bytes); 
		std::copy (bytes, bytes + num_Bytes, img_Bytes.begin ()); 
		
		glGenTextures (1, &txrID); 		
		glEnable (GL_TEXTURE_2D); 
		glBindTexture (GL_TEXTURE_2D, txrID);

		glTexImage2D (
			GL_TEXTURE_2D, 
			0, 
			GL_RGBA8, 
			img_Width, 
			img_Height, 
			0, 
			GL_RGB,  
			GL_UNSIGNED_BYTE, 
			img_Bytes.data ()
			); 

		glTexEnvi (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_DECAL); 

		glActiveTexture (GL_TEXTURE0); 
		glClientActiveTexture (GL_TEXTURE0); 

		glTexParameteri (GL_TEXTURE_2D,   GL_TEXTURE_MIN_FILTER, GL_LINEAR); 
		glTexParameteri (GL_TEXTURE_2D,   GL_TEXTURE_MAG_FILTER, GL_LINEAR); 

		int wat = 0; 

	}
	else
	{
		ILenum errorID = ilGetError (); 
		int i = 0; i ++; 

	}

#endif 

	Ma::SetIdent	(axes_Rot); 
	Ma::Set			(axes_Pos, 280.0f, 120.0f, -280.0f);


	// set viewer position
	Ma::Set (sg_view_Pos, 320.0f, 150.0f, -305.0f);
	Ma::Set (sg_view_Rot, 1.0f, 0.0f, 0.0f, 0.0f);

	// load model
	H3D::Load (sg_geo, geo_fname); 




	// 
	// finding bounding extents of model
	size_t num_Primitives = El_count (sg_Cave_primitives); 
	int wat = 0; 
	for (size_t i = 0; i < num_Primitives ; i++)
	{
		Ma::Vec3f vMin, vMax, abs_Min, abs_Max, vA, vB; 
		Ma::Copy (vMin, sg_geo.vPos[sg_Cave_primitives[i].vertFirst]); 
		Ma::Copy (vMax, sg_geo.vPos[sg_Cave_primitives[i].vertFirst]); 

		size_t num_Verts = 
				sg_Cave_primitives[i].vertLast - sg_Cave_primitives[i].vertFirst;  

		for (size_t a = 0; a < num_Verts; a++)
			Accum_extents (vMin, vMax, sg_geo.vPos[ sg_Cave_primitives[i].vertFirst + a]); 
		

		Ma::Set (
			vA, 
			sg_Primitive_locations[i][0], 
			sg_Primitive_locations[i][1], 
			sg_Primitive_locations[i][2]
			); 

		Ma::Add (abs_Min, vMin, vA); 
		Ma::Add (abs_Max, vMax, vA); 

		wat++; 
	}


}

void Render_Cave_test (Ma::Quatf& view_Rot, Ma::Vec3f& view_Pos)
{
	glEnableClientState (GL_VERTEX_ARRAY); 
	glEnableClientState (GL_TEXTURE_COORD_ARRAY); 
	glEnable (GL_TEXTURE_2D); 


	//glEnableClientState (GL_NORMAL_ARRAY); 



	glPolygonMode(GL_FRONT_AND_BACK , GL_FILL); 
	glColor3f (1.0f, 1.0f, 1.0f); 

	for (size_t i = 0; i < 5; i++)
	{
		glVertexPointer  (3, GL_FLOAT, 0, sg_geo.vPos.data ()); 
		glTexCoordPointer(2, GL_FLOAT, 0, sg_geo.vTxc0.data ()); 
		//glNormalPointer  (GL_FLOAT, 0, sg_geo.vNrm.data ()); 
		Ma::Vec3f model_Pos;
		Ma::Quatf model_Rot;
		
		Ma::Set (
			model_Pos, 
			sg_Primitive_locations[i][0], 
			sg_Primitive_locations[i][1], 
			sg_Primitive_locations[i][2]
		);
		Ma::SetIdent (model_Rot); 

		Rn::ModelViewState (
			model_Pos,
			model_Rot, 
			view_Pos, 
			view_Rot); 

		glDrawElements (
			GL_TRIANGLES, 
			sg_Cave_primitives[i].indexCount, 
			GL_UNSIGNED_INT, 
			(&sg_geo.vInds[0] + sg_Cave_primitives[i].indexStart)
			); 
	}

	glPolygonMode(GL_FRONT_AND_BACK , GL_FILL); 

	glDisable (GL_TEXTURE_2D); 
	glDisableClientState (GL_VERTEX_ARRAY); 
	glDisableClientState (GL_NORMAL_ARRAY); 
	glDisableClientState (GL_TEXTURE_COORD_ARRAY); 
}



//
void Draw_axes (
	float size, 
	const Ma::Mat33f& model_Rot, const Ma::Vec3f& model_Pos, 
	const Ma::Quatf& view_Rot, const Ma::Vec3f& view_Pos
	)
{

	glDisable (GL_TEXTURE_2D); 
	glDisableClientState (GL_VERTEX_ARRAY); 
	glDisableClientState (GL_NORMAL_ARRAY); 
	glDisableClientState (GL_TEXTURE_COORD_ARRAY); 

	glEnable (GL_LINE_WIDTH); 
	glLineWidth (1.0f); 

	Rn::ModelViewState (
		model_Pos,
		model_Rot, 
		view_Pos, 
		view_Rot); 

	glBegin (GL_LINES); 

	glColor3f (1.0f, 0.2f, 0.2f); 
	glVertex3f (0.0, 0.0f, 0.0f); 
	glVertex3f (size, 0.0f, 0.0f); 


	glColor3f (0.2f, 1.0f, 0.2f); 
	glVertex3f (0.0, 0.0f, 0.0f); 
	glVertex3f (0.0, size, 0.0f); 

	glColor3f (0.2f, 0.2f, 1.0f); 
	glVertex3f (0.0, 0.0f, 0.0f); 
	glVertex3f (0.0, 0.0f, size); 

	glEnd (); 

	glLineWidth (1.0f); 
	glDisable (GL_LINE_WIDTH); 


}

//
//
void Draw_bones (
	Bl_bone::Array& bones, 
	Ma::Quatf&		view_Rot, 
	Ma::Vec3f&		view_Pos
	)
{

	Ma::Quatf rot_Ident; 
	Ma::SetIdent (rot_Ident); 

	for (size_t i = 0; i < bones.size (); i++)
	{

		const Bl_bone& b = bones[i]; 
		Ma::Vec3f scl_Pos; 
		Ma::Scale (scl_Pos, b.head_local, 100.0f);  

		Draw_axes (
			20.0f, 
			b.matrix, // _local 
			scl_Pos, 
			view_Rot, 
			view_Pos
			); 
	}
}

//
//
void Draw_edit_bones (
	Bl_edit_bone::Array&	bones, 
	Ma::Quatf&				view_Rot, 
	Ma::Vec3f&				view_Pos
	)
{
	bones.size (); 

	for (size_t i = 0; i < bones.size (); i++)
	{
		const Bl_edit_bone& b = bones[i]; 
		Ma::Vec3f scl_Pos; 
		Ma::Scale (scl_Pos, b.head_Pos, 200.0f);  

		Draw_axes (
			50.0f, 
			b.axes, 
			scl_Pos, 
			view_Rot, 
			view_Pos
			); 
	}
}


void Update_Cave_test (HDC hdc)
{

	update_view_transform (5.0f, sg_view_Rot, sg_view_Pos); 

	static float rot_Y = 0.0; 
	rot_Y += float(Ma::Pi / 30.0); 
	//
	Rn::ProjectionState (
		float (Ma::Pi / 3.0), 
		float (sg_Wd_window) / float (sg_Ht_window) , 
		1.0f, 
		5000.0f
		); 

	//
	//
	glClearColor (0.1f, 0.1f, 0.1f, 1.0f); 
	glClearDepth (1.0); 
	glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 


	// Render_Cave_test (sg_view_Rot, sg_view_Pos); 

	Ma::Quatf q_X, q_Y, q_Z; 

	Ma::RotZAxis (q_Z, Ma::Degr2Rads (-95.0f)); 
	Ma::RotXAxis (q_X, Ma::Degr2Rads (-1.45f)); 
	Ma::RotYAxis (q_Y, Ma::Degr2Rads (-89.9f)); 

	Ma::Mult (axes_Rot, q_X, q_Z); 
	Ma::Mult (axes_Rot, q_Y, axes_Rot); 


	Rn::Draw_axes (
		20.0f, 
		axes_Rot, axes_Pos, 
		sg_view_Rot, sg_view_Pos
		); 

	Rn::Draw_AA_grid (		
		5000.0f, 
		sg_view_Rot, sg_view_Pos
		); 

	//Draw_edit_bones (
	//	sg_edit_bones, 
	//	sg_view_Rot, sg_view_Pos
	//	); 

	Draw_bones (
		sg_bones, 		
		sg_view_Rot, sg_view_Pos
		); 


	::SwapBuffers (hdc); 
}


//
void Test_Db_Write () 
{
	Db::Data_file_header	hdr; 
	Db::Data_entry_header	entry_hdr[3]; 
	
	// fake data
	float		weights[69]; 
	Ma::Vec3f	vTangents[10]; 
	unsigned	indices[1212]; 

	hdr.ID = 0; 
	hdr.version = 1; 
	hdr.entry_Count = 3; 


	entry_hdr[0].elem_Count = 69; 
	sprintf (entry_hdr[0].label, "Weights"); 
	entry_hdr[0].type = Db::F32; 

	for (size_t i (0); i != 69; i++) 
		weights[i] = i * 0.01f;  


	entry_hdr[1].elem_Count = 3 * 10; // everything is made of primitives
	sprintf (entry_hdr[1].label, "Tangents"); 
	entry_hdr[1].type = Db::F32; 
	for (size_t i (0); i != 10; i++) 
		Ma::Set (vTangents[i], 1.0f, 0.0f, 0.0f); 


	entry_hdr[2].elem_Count = 1212; 
	entry_hdr[2].type = Db::U32; 
	sprintf (entry_hdr[2].label, "Indices"); 

	for (size_t i = 0; i < 1212; i++) 
		indices[i] = i * 3; 

	int flips_Bits = ~0x0; 

	std::string				test_Outf = "C:/usr/tmp/fake_data_test.bin"; 
	std::shared_ptr<FILE>	fout (fopen (test_Outf.c_str (), "wb"), fclose); 


	long pos_fout = ftell (fout.get()); 

	size_t entry_Start_offs = sizeof (Db::Data_file_header) + 3 * 2 * sizeof (size_t); 

	size_t entry_Size[3]; 
	size_t entry_Offs[3];

	entry_Offs[0] = entry_Start_offs; 
	entry_Size[0] = sizeof (Db::Data_entry_header) + 69 * sizeof (float); 

	entry_Offs[1] = entry_Offs[0] + entry_Size[0] ; 
	entry_Size[1] = 10 * sizeof (Ma::Vec3f); 

	entry_Offs[2] = entry_Offs[1] + entry_Size[1];
	entry_Size[2] = 1212 * sizeof (size_t);


	fwrite (&hdr, 1, sizeof (Db::Data_file_header), fout.get()); 

	for (size_t i = 0; i < 3; i++) {
		fwrite (entry_Offs + i , 1, sizeof (size_t), fout.get()); 
		fwrite (entry_Size + i , 1, sizeof (size_t), fout.get()); 
		}

	// entry 0
	fwrite (entry_hdr + 0, sizeof (Db::Data_entry_header), 1, fout.get()); 
	fwrite (weights , sizeof (float), 69, fout.get()); 

	// entry 1
	fwrite (entry_hdr + 1, sizeof (Db::Data_entry_header), 1, fout.get()); 
	fwrite (vTangents, sizeof (Ma::Vec3f), 10, fout.get()); 

	// entry 2
	fwrite (entry_hdr + 2, sizeof (Db::Data_entry_header), 1, fout.get()); 
	fwrite (indices, sizeof (size_t), 1212, fout.get());  



	Ut::Byte_array DBG_file; 

	Ut::Load_data_file (DBG_file, test_Outf); 
	const Db::Data_entry_header& hdr0 = Db::Entry_header (DBG_file.data(), 0);
	const Db::Data_entry_header& hdr1 = Db::Entry_header (DBG_file.data(), 1);
	
	
	float*		fptr	= (float*) Db::Entry_data (DBG_file.data(), 0); 
	Ma::Vec3f*	tans	= (Ma::Vec3f*) Db::Entry_data (DBG_file.data(), 1); 
	 
	size_t		num_File_entries 
						= Db::Num_entries (DBG_file.data()); 


	 

	DBG_file.size (); 
}

void Test_geo_files ()
{
//  <Mesh name = "UpperProtrusion" material = "material/Underground_Cave.mtrl" batchStart = "0" batchCount = "1216" vertRStart = "0" vertREnd = "224" tx = "-430.8789978027344" ty = "402.2383728027344" tz = "1114.75" />
//  <Mesh name = "Cave_inset" material = "material/Underground_Cave.mtrl" batchStart = "1216" batchCount = "1266" vertRStart = "224" vertREnd = "468" tx = "5.39378023147583" ty = "13.776209831237793" tz = "450.11810302734375" />
//  <Mesh name = "Ledge_0" material = "material/Underground_Cave.mtrl" batchStart = "2482" batchCount = "192" vertRStart = "468" vertREnd = "519" tx = "-176.75660705566406" ty = "722.0390625" tz = "329.8165283203125" />
//  <Mesh name = "Spiral_Column" material = "material/Underground_Cave.mtrl" batchStart = "2674" batchCount = "1008" vertRStart = "519" vertREnd = "695" tx = "-44.801456451416016" ty = "95.11747741699219" tz = "471.62298583984375" />
//  <Mesh name = "Cave_main" material = "material/Underground_Cave.mtrl" batchStart = "3682" batchCount = "7958" vertRStart = "695" vertREnd = "2090" tx = "5.39378023147583" ty = "13.776209831237793" tz = "450.11810302734375" />

   std::string k_H3D_test_anim      = "C:/usr/humanoid_0x5.anim"; 

	H3D::Anim humanoid_anim; 
	H3D::Load (humanoid_anim, k_H3D_test_anim ); 

	std::string k_Horde3D_content    = "C:/usr/Horde3D_SDK/Horde3D/Binaries/Content";
	std::string k_Hmnd_test_content  = "C:/Quarantine/awsum/humanoid_test/models";

	std::string	man_fname = k_Horde3D_content + "/models/man/man.geo";

	H3D::Geo man_geo; 
	H3D::Load (man_geo, man_fname); 
   
   H3D::Geo watsmyname_geo; 

   std::string untitled_fname = k_Hmnd_test_content + "/watsmyname.geo"; 
   H3D::Load (watsmyname_geo, untitled_fname);

	//std::string	anim_file = k_Hmnd_test_content  + "/animations/man.anim"; 
	//H3D_anim	anim; 
	//Load_anim (anim, anim_file); 

	
	std::string k_awsum_content = "C:/Quarantine/awsum"; 

	std::string geo_file = k_awsum_content + "/Underground/model/Underground.geo"; 
	
   H3D::Geo geo; 
	H3D::Load(geo, geo_file); 

	geo.vPos.size (); 
}


//
//
class test_Hord3D_bones          
   : public sy::RT_window_listener
{
   std::shared_ptr<sy::Graphics_window>    windo; 
   Ma::Vec3f                              view_Rot, 
                                          view_Pos; 

   int viewport_wd;
   int viewport_ht;

   double Dt; 
   sy::Time_tracker tracker;

   JointVisitor jvisitor;

   
   Ma::Vec2f IK_points[4]; 
   Ma::Vec2f target_point; 

   //void IKSolve2D (float* oTheta, int szChain, Vec2f* vTarg, Vec2f** pts) 
   struct 
      {
   Lg::Spine_geom geom;
      } spine; 

   void init_level_geom ()
   {
      //std::vextor<Ma::Vec3f>      spine_norms;

      Lg::Spine_params  params = {
         // int                     num_points;             
         // const unsigned short*   inds;                
         // const Ma::Vec3f*        verts;               
         // 
         // Ma::Vec3f         dir_height;                
         // int               num_sub_divs;                 
         //    
         // // left/right, 2 per point                   
         // float*            horz_len;                  
         //                                                 
         // // left to right, (num_sub_divs + 1) per seg 
         // float*            vert_heights;              
         }; 

      //
      Lg::Gen_spine_geom (spine.geom, params, false, true); 
   }

   void draw_level_geom ()
   {
      spine.geom;

   }
   //
   void init_scene (sy::System_context* sys)
   {

      init_level_geom (); 
   
      Ma::Set (target_point, 0.0f, 6.0f); 
      Ma::Set (IK_points[0], 0.0f, 8.0f); 
      Ma::Set (IK_points[1], 2.0f, 8.0f); 
      Ma::Set (IK_points[2], 4.0f, 8.0f); 
      Ma::Set (IK_points[3], 6.0f, 8.0f); 

      
   	std::string	scene_xml = "C:/Quarantine/awsum/humanoid_test/models/man/man.scene.xml";
      //std::string scene_xml = "C:/Quarantine/awsum/humanoid_test/models/watsmyname.geo.scene"; 
      tinyxml2::XMLDocument doc;
      doc.LoadFile( scene_xml.c_str() );
      tinyxml2::XMLElement* root_elem = doc.RootElement (); 
      root_elem->Accept (&jvisitor); 

      viewport_wd = 800;
      viewport_ht = 600;


      Dt = 0.0; 
      tracker.Reset (sys);



      Ma::Set (view_Pos, 0.0f, 8.0f, 0.0f);
      Ma::Set (view_Rot, 0.0f, 0.0f, 0.0f);

     	// important state
	   glEnable (GL_DEPTH_TEST);  
	   glEnable (GL_CULL_FACE);
   } 
   
   //
   void update_input (sy::System_context* sys)
   {
      sy::Keyboard_state   kb; 
      sy::Mouse_state      ms;
      
      sys->Poll_input (ms, kb); 

      if (sy::Is_keydown (sy::SC_Q, kb))
         sys->SysReq (sy::System_context::Req_Exit); 

      xp::Update_view_transform (view_Pos, view_Rot, float(Dt * 10.0), kb, ms); 
   } 

   //
   void update_dynamics (sy::System_context* sys)
   {
   }

   //
   void render (sy::System_context* sys)
   {
      Ma::Quatf q, q_x, q_y; 

      const float kDegr2Rad = Ma::Pi / 180.0;

      Ma::RotXAxis (q_x, kDegr2Rad * Ma::X (view_Rot)); 
      Ma::RotYAxis (q_y, kDegr2Rad * Ma::Y (view_Rot)); 
      Ma::Mult (q, q_y, q_x); 

	   //
	   Rn::ProjectionState (
		   float (Ma::Pi / 3.0), 
		   float (viewport_wd) / float (viewport_ht), 
		   1.0f, 
		   5000.0f
		   ); 

	   //
	   //
	   glClearColor (0.0f, 0.0f, 0.0f, 1.0f); 
	   glClearDepth (1.0); 
	   glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT); 
	   // Render_Cave_test (sg_view_Rot, sg_view_Pos); 

      // reference floor
      Rn::Draw_AA_grid (128.0, q, view_Pos); 

      {  // draw bones...
         Ma::Quatf bonrot; 
         Ma::Vec3f bonpos; 

         //Ma::AddMat44Transl (
         JointVisitor::BoneMap& bonez = jvisitor.bone_map;

         for (JointVisitor::BoneMap::iterator it = bonez.begin(); it != bonez.end (); it++)
         {
            Ma::Mat44f m, mv, v; 

            // view_Rot are eulerz
            //Ma::SetMat44Quat (v, view_Rot); 
            Ma::AddMat44Transl (v, view_Pos); 

            //Get_transform (m, &it->second); 

            Ma::MultMat (mv, v, m); 

            // Draw_axes (1.0f, mv); 
         }




         //Ma::SetIdent (bonrot);
         //Ma::Quatf qx, qy, qz, q_; 
         static float x_rot = 0.0; 

         x_rot += (Dt * Ma::HalfPi);

         //Ma::RotXAxis (bonrot, x_rot); 
         //Ma::RotYAxis (bonrot, x_rot); 
         Ma::RotZAxis (bonrot, x_rot); 

         // Ma::Mult (q_, qx, qz); 
         // Ma::Mult (
         // EulerMatrix_YXZ
         
         //Draw_axes (1.0f, bonrot, Ma::Set (bonpos, 8.0f, 5.0f, 2.0f), q, view_Pos); 

         
         Ma::Vec3f co; 
         Ma::Set (co, 0.0f, 0.25f, 1.0f); 


         Ma::Set (
            target_point, 
            cosf (4 * x_rot + Ma::HalfPi) +  5.0f,
            2.0f * sinf (x_rot) + 8.0f
            ); 
        
         
         Rn::Draw_sphere (0.2, co, Ma::Set (bonpos, Ma::X(target_point), Ma::Y(target_point), 0.0f), q, view_Pos); 


         
         float thetas_[4];
         Ik::Solve_2D (IK_points + 1, thetas_, 3, target_point) ; 

         for (int i = 0; i < 4; i++)
         {
            Ma::Set (co, 1.0f, 0.75f, 0.5f);
            Rn::Draw_sphere (0.2, co, Ma::Set (bonpos, Ma::X(IK_points[i]), Ma::Y(IK_points[i]), 0.0f), q, view_Pos); 

            if (i) 
            {
               Ma::Vec3f a, b; 
               Ma::Set (a, Ma::X(IK_points[i-1]), Ma::Y(IK_points[i-1]), 0.0f);
               Ma::Set (b, Ma::X(IK_points[i]), Ma::Y(IK_points[i]), 0.0f);

               Rn::Draw_line (1.0f, co, a, b, q, view_Pos); 

            }
         }


         draw_level_geom ();



         //`sphere (1.0, Ma::Set (bonpos, 8.0f, 5.0f, 2.0f), q, view_Pos); 

      }



      windo->Swap_buffers (); 
   }


public: 

   //
   test_Hord3D_bones (sy::System_context* sys) {
   }

   //
   virtual ~test_Hord3D_bones () {
   }
   //
	virtual void OnWindowResize (int wd, int ht) {
      viewport_wd = wd; 
      viewport_ht = ht;
      glViewport (0, 0, viewport_wd ,viewport_ht); 
      }

	virtual void OnWindowClose (){}
	virtual void OnWindowActivate	(bool activate) {} 
	
   //
   //
   virtual int Initialize (sy::System_context* sys) { 
      windo.reset (sys->Create_GraphicsWindow (this, "test Horde bones", 800,600,false)); 
      init_scene (sys); 
      windo->Show (true); 
      return 0; 
      } 

   //
   //
	virtual int Deinitialize (sy::System_context*) { 

      h3dRelease (); 
      return 0; 
      } 


	virtual int Update (sy::System_context* sys) { 
      Dt = tracker.GetDt (sys); 
      update_input (sys); 
      update_dynamics (sys); 
      render (sys); 
      return 0; 
      } 
}; 


//
//
sy::RT_window_listener* Create_test_bones (sy::System_context* sys)
{
   return new test_Hord3D_bones (sys); 
}




//
//
// 
typedef void (*Fn) (void); 
Fn sg_TestProgs[] = {
	Test_Db_Write , 
	Test_geo_files, 
	}; 

int _tmain (int argv, char** argc)
{

//   sg_TestProgs[1] (); 


std::shared_ptr<sy::System_context>     sys (sy::Create_context ()); 
std::shared_ptr<sy::RT_window_listener>  rtt (Create_test_bones (sys.get())); 
sy::Run_realtime_task (sys.get(), rtt.get()); 

return 0; 


}



