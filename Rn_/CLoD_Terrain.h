#ifndef _CLoD_Terrain_
#define _CLoD_Terrain_

#include <memory>
#include <vector>
#include "Ma/VecMath.h"
#include "Rn/Render.h"
#include "NITF.h"
#include "boost/assert.hpp"
#include <cmath>

//
template<typename _BT_Ty>
	struct BT_Bukkit : public std::vector<_BT_Ty>
	{
	BT_Bukkit (size_t c) : std::vector<_BT_Ty> (c), counter (0) {
		}

	_BT_Ty* Get () {
		return &at (counter++); 
		} 

	size_t counter; 
	};

namespace Binary_triangle 
{
	// 0 is a level

	// total number of triangles at a tesselation level
	// also Num_patch_triangles = SizeOf_variance_tree
	size_t	Num_patch_triangles (size_t level); 

	// all nodes including inner nodes
	size_t	Num_triangle_nodes	(size_t level); 


	// Patch_division - used for num 
	// discrete steps to compute across patch
	size_t Patch_division		(size_t level); 

	// multiplier to find step
	float Patch_multiplier_step (size_t level); 

	// num verts per row/col
	size_t	Vertex_patch_stride	(size_t level); 
} 

//
// Binary Triangle Node 
struct BT_subdiv_node {

	typedef BT_Bukkit<BT_subdiv_node> Bukkit; 


	BT_subdiv_node () 
		: level (0)
		, left_Child (0)
		, right_Child (0)
		, varn_Ind (1) {
		}

	BT_subdiv_node (unsigned vID, int lvl) 
		: level (lvl)
		, left_Child (0)
		, right_Child (0)
		, varn_Ind (vID) {
		}

	int				level; 
	unsigned		varn_Ind; 
	Ma::Vec2i		crd[3];
	BT_subdiv_node*	left_Child; 
	BT_subdiv_node*	right_Child; 
	}; 


//
inline Ma::Vec2i& Half_uv (Ma::Vec2i& uv)
{
	Ma::X (uv) /= 2;
	Ma::Y (uv) /= 2;
	return uv;
}

//
inline Ma::Vec2i& Midpoint_uv (
	Ma::Vec2i& out, const Ma::Vec2i& ptA, const Ma::Vec2i& ptB)
{
	Ma::Vec2i v_Tmp;
	
	Ma::Add (
		out, ptA, Half_uv (Ma::Sub (v_Tmp, ptB, ptA)));

	return out; 
}


inline unsigned Left_child_ind (unsigned cur_Ind) {
	return (cur_Ind << 0x1); 
	}

inline unsigned Right_child_ind (unsigned cur_Ind) {
	return (cur_Ind << 0x1) + 1; 
	}

inline unsigned Parent_ind (unsigned cur_Ind) {
	return (cur_Ind >> 0x1); 
	}



BT_subdiv_node* Subdivide_node (BT_subdiv_node* base, BT_subdiv_node::Bukkit& bukkit, int numLevels); 


struct HeightMapSampler
{
	// dimensions have to match the vertex stride of patch
	virtual ~HeightMapSampler (){}
	virtual float Get (unsigned u, unsigned v) const = 0;

protected: 	
	HeightMapSampler () {}
}; 


//
// Build_variance_tree - index 0 is not used, only inner nodes have variance
float Build_variance_tree (
	std::vector<float>& varnTree, 
	const BT_subdiv_node& base, 
	const HeightMapSampler& hgtMap
	); 


// U moves fastest
std::vector<Ma::Vec2f>& Build_vertex_patch (
	std::vector<Ma::Vec2f>& pos, 
	unsigned vertex_Stride, 
	float patch_U_size,
	float patch_V_size
	); 



// 
std::vector<Ma::Vec2f>& Build_uv_coords (
	std::vector<Ma::Vec2f>& uv, 
	bool					flip_U, 
	bool					flip_V,
	unsigned				vertex_Stride 
	);



// BT_RT_node - runtime node
struct BT_RT_node 
{
	typedef BT_Bukkit<BT_RT_node> Bukkit; 
	// orientation of this triangle
	unsigned dir_Index;	
	// variance tree index
	unsigned tri_Index;

	BT_RT_node* left_Neighbor;
	BT_RT_node* right_Neighbor;
	BT_RT_node* base_Neighbor;

	BT_RT_node* right_Child;
	BT_RT_node* left_Child;

	// indices
	Ma::Vec2i uv[3]; 
}; 

//
// Reset_node 
void Reset_node (
	BT_RT_node* n, 
	BT_RT_node*	rtNbr, 
	BT_RT_node*	ltNbr, 
	BT_RT_node*	bsNbr
	);

//
// CLoD_patch 
struct CLoD_patch 
{
	// level metrics 

	// step size : 1 / (lvl>>2 + 1) 
	// num tris : 2 ^ lvl, or leaf nodes
	// num nodes : 2 ^ (lvl+1) - 1
	// vert strid : (2 ^ lvl) + 1
	// 

	typedef BT_Bukkit<CLoD_patch> Bukkit; 

	union Index {
		struct { unsigned short e, n; 
			} en; 

		unsigned ui; 
		}; 

	// 
	std::string		patch_Name; 

	unsigned short	easting_Ind;
	unsigned short	northing_Ind;

	// this is the multiplier to compute: 
	// (grid_Stride * u) + v  into linear array
	// this can be a shared variable 

	int grid_Stride; 

	// 
	Rn::IndexBuffer*		index_Buffer; 
	Rn::Texture*			height_Texture; 
	//Rn::Texture2D* normal_Map; 


	// a hack still ? 
	int		visibleCounter;

	// is this correct? 
	bool	isUpdated;

	// only actives are connected? 
	bool	is_Active; 

	// used during for DrawElement (,,num_Indices, index_Offset * sizeof(indType)); 
	// num tri indices collected  during patch tesselation
	int num_Indices; 
	// offset into index buffer if using shared IB
	unsigned index_Offset;


	// adjacent patches
	CLoD_patch* nextU; 
	CLoD_patch* nextV; 
	CLoD_patch* prevU; 
	CLoD_patch* prevV; 

	//TerrCLoDPatchRes* resRef; 
	BT_RT_node triBaseRight;
	BT_RT_node triBaseLeft;

	std::vector<int> rightVarianceTree;
	std::vector<int> leftVarianceTree;
}; 


bool Is_ready		(CLoD_patch& patch);
bool Load_patch		(CLoD_patch& patch); 
bool Unload_patch	(CLoD_patch& patch); 


// reset patches before tesselation
CLoD_patch& Reset_patch (CLoD_patch& patch); 

void Tesselate_patch (CLoD_patch* patch, struct View_params* params, BT_RT_node::Bukkit& bukkit); 



// collect indices for left & right tri
void Collect_indices (unsigned short* memPtr, int& curCount, const CLoD_patch& patch); 





// 
// 
struct Terrain_group_desc
{
	char						fname[64]; 
	unsigned					num_Patches;		 
	float						grid_Stride;	
	unsigned					num_Tess_levels;

	std::vector<std::string>		patch_Names;	
	std::vector<CLoD_patch::Index>	patch_Indices;	
};

//
//
class CLoD_terrain 
	{
public:		

	//
	typedef std::map<unsigned, CLoD_patch*> PatchMap; 
	//
	struct Params {
		short	stride; 
		int		IB_mode; // = { shared, independent };  
		}; 
	//

	//
	struct Group : public TypeDef<Group> {
		// name of table listing patches
		std::string				fname; 
		// patch buff for this group
		std::vector<CLoD_patch>	arr;
		}; 

	// 
	CLoD_terrain () { 
		}

	// determine patches that are in view
	// what patches need to be loaded/or unloaded
	void Update_visible_area (class View_params* params); 
	//
	// patch adjacency are properly connected
	// so we can properly tesselate 
	void Update_patch_topology (); 
	
	// variance subdivision
	void Tesselate_terrain (); 

	// now tesselation is complete 
	// build the index buffer
	void Build_index_buffer (); 

	// 
	void Render (); 
	const CLoD_patch* Lookup_patch (unsigned short e, unsigned short n) const;


	//	
	Group::List	group_List; 
	PatchMap	patch_Map;
	
	
private: 

	// 
	void set_patch_ (unsigned short e, unsigned short n, CLoD_patch* p);

	};  



// CLoD_terrain 
// Terrain_group_desc




#endif //_CLoD_Terrain_
