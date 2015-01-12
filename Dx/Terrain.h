#ifndef _Terrain_
#define _Terrain_




#include <memory>
#include <vector>
#include "VecMath.h"
#include "Utility.h"


namespace Tr 
{ 

	
struct HeightMapSampler
{
	// dimensions have to match the vertex stride of patch
	virtual float Get (unsigned u, unsigned v) const = 0;

protected: 	
	HeightMapSampler () {}
}; 

//
// Binary Triangle Node 
struct BT_subdiv_node {

	typedef Ut::Bukkit<BT_subdiv_node> Bukkit; 


	BT_subdiv_node () : level (0), left_Child (0), right_Child (0), varn_Ind (1) {
		}

	BT_subdiv_node(unsigned vID, int lvl) : level (lvl), left_Child (0), right_Child (0), varn_Ind (vID) {
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
inline Ma::Vec2i& Midpoint_uv (Ma::Vec2i& out, const Ma::Vec2i& ptA, const Ma::Vec2i& ptB)
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



//
// Variance Tree 
float Build_variance_tree (
	std::vector<float>& varnTree, 
	const BT_subdiv_node& base, 
	const HeightMapSampler& hgtMap
	); 

// 
std::vector<Ma::Vec2f>& Build_vertex_patch (
	std::vector<Ma::Vec2f>& pos, 
	unsigned				vertex_Stride, 
	float					patch_Size
	);
// 
std::vector<Ma::Vec2f>& Build_uv_coords (
	std::vector<Ma::Vec2f>& uv, 
	unsigned				vertex_Stride 
	);



// BT_RT_node - runtime node
struct BT_RT_node
{
	typedef Ut::Bukkit<BT_RT_node> Bukkit; 
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
	Ma::Vec2us uv[3]; 
}; 


//
// CLoD_Patch 
struct CLoD_Patch 
{
	unsigned easting_Ind;
	unsigned northing_Ind;

	int numIndices; 
	unsigned indOffset;

	int gridStride; 

	class _index_buffer_*	index_Buffer; 
	class _height_map_*		height_Map; 
	
	//Texture2D* normalMap; 

	int		visibleCounter;
	bool	isUpdated;

	// adjacent patches
	CLoD_Patch* nextU; 
	CLoD_Patch* nextV; 
	CLoD_Patch* prevU; 
	CLoD_Patch* prevV; 

	//TerrCLoDPatchRes* resRef; 
	BT_RT_node triBaseRight;
	BT_RT_node triBaseLeft;

	int* rightVarianceTree;
	int* leftVarianceTree;
}; 


// reset patches before tesselation
CLoD_Patch& Reset_patch (CLoD_Patch& patch); 

void Tesselate_patch (CLoD_Patch* patch, class View_params* params, BT_RT_node::Bukkit& bukkit); 

// collect indices for left & right tri
void Collect_indices (unsigned short* memPtr, int& curCount, const CLoD_Patch& patch); 

//
//
class CLoD_terrain 
	{
public:		

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

private: 
	CLoD_terrain () {
		}
	};  


}

#endif
