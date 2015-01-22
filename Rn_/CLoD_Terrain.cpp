#include "CLoD_Terrain.h"

#include "stdio.h"

const int CLoDSharedData_indexDepth = 8; 



// Num_patch_triangles - 
size_t Binary_triangle::Num_patch_triangles (size_t level)
{
	// num tris : 2 ^ lvl, or leaf nodes
	size_t num_Tris = 1;
	for (size_t i = 0; i < level; i++) 
		num_Tris *= 2; 

	return num_Tris; 
}
	

// Num_triangle_nodes 
size_t Binary_triangle::Num_triangle_nodes (size_t level)
{
	// num nodes : 2 ^ (lvl+1) - 1
	size_t num_Nodes = 1; 
	
	for (size_t i = 0; i < (level+1); i++)
		num_Nodes *= 2; 

	return (num_Nodes - 1); 
}



// 
// step size : 1 / (lvl>>1 + 1) 
size_t Binary_triangle::Patch_division (size_t level)
{
	size_t num_Divs = 1;
	
	size_t div_Count = (1 + level) / 2; 

	for (size_t i = 0; i < div_Count; i++) 
		num_Divs *= 2;

	return num_Divs; 
}

//
//
float Binary_triangle::Patch_multiplier_step (size_t level)
{
	float num_Divs = float (Binary_triangle::Patch_division (level)); 
	// num_Divs - number of columns or row at max tesselation
	return (1.0f / num_Divs); 
}

// Vertex_patch_stride - based on tessellation level
size_t Binary_triangle::Vertex_patch_stride (size_t level)
{
	return 1 + Binary_triangle::Patch_division (level); 
}


//
//
float Build_variance_tree (std::vector<float>& varn_Tree, const BT_subdiv_node& nd, const HeightMapSampler& hgtMap)
{
	// leaf nodes have no variance
	// lvl 0 - no variance nodes	
	// lvl 1 - has 1 variance node, 2^1 - 1
	// lvl 2 - has 3 variance nodes, 2^2 - 1 


	// NOTE: variance is not difference in height value, so always positive
	if (nd.left_Child && nd.right_Child)
	{
		Ma::Vec2i mid_Crd; 
		Midpoint_uv (mid_Crd, nd.crd[2], nd.crd[1]); 

		float hgt_B = hgtMap.Get (Ma::X (nd.crd[2]), Ma::Y (nd.crd[2])); 
		float hgt_A = hgtMap.Get (Ma::X (nd.crd[1]), Ma::Y (nd.crd[1])); 

		// triVariance = abs( centerZ - ((leftZ + rightZ) / 2) );
		float interp_Hgt = 0.5f * (hgt_B + hgt_A); 
		// interp_Hgt += hgt_A;

		float actual_Hgt = hgtMap.Get (Ma::X (mid_Crd), Ma::Y (mid_Crd)); 

		float base_Variance	 = interp_Hgt - actual_Hgt;
		float child_Variance 
				= Build_variance_tree (varn_Tree, *nd.left_Child , hgtMap) 
				+ Build_variance_tree (varn_Tree, *nd.right_Child, hgtMap);

		varn_Tree[nd.varn_Ind] = fabs (base_Variance - (0.5f * child_Variance)); 
		return varn_Tree[nd.varn_Ind]; 
	}					   

	return 0.0f;

}


//
// Build_vertex_patch - patch is always vertex_Stride x vertex_Stride in size. 
std::vector<Ma::Vec2f>& Build_vertex_patch (
	std::vector<Ma::Vec2f>& pos, 
	unsigned vertex_Stride, 
	float patch_U_size,
	float patch_V_size
	)
{
	float vert_U_step = patch_U_size / (vertex_Stride - 1); 
	float vert_V_step = patch_V_size / (vertex_Stride - 1); 

	for (size_t Y = 0; Y < vertex_Stride; Y++)
		for (size_t X = 0; X < vertex_Stride; X++)
			Ma::Set (pos[(vertex_Stride * Y) + X], (vert_U_step * X), (vert_V_step * Y)); 

	return pos;

}

//
//
std::vector<Ma::Vec2f>& Build_uv_coords (
	std::vector<Ma::Vec2f>& uv, 
	bool flip_U, 
	bool flip_V,
	unsigned vertex_Stride
	)
{
	
	float uv_Step = 1.0f / (vertex_Stride - 1); 

	for (size_t Y = 0; Y < vertex_Stride; Y++)
		for (size_t X = 0; X < vertex_Stride; X++)
	{
		float u = flip_U ? (1.0f - uv_Step * X) : (uv_Step * X);
		float v = flip_V ? (1.0f - uv_Step * Y) : (uv_Step * Y);

		Ma::Set (uv[(vertex_Stride * Y) + X], u, v);
	}

	return uv; 
}

//
//
BT_subdiv_node* Subdivide_node (BT_subdiv_node* base, BT_subdiv_node::Bukkit& bukkit, int num_Lvls)
{
	Ma::Vec2i mid_Crd; 
	Midpoint_uv (mid_Crd, base->crd[1], base->crd[2]); 

	if (num_Lvls)
	{
		BT_subdiv_node* right = bukkit.Get();
		right->level = base->level + 1; 
		right->varn_Ind = Right_child_ind (base->varn_Ind); 

		Ma::Copy (right->crd[0] , mid_Crd); 
		Ma::Copy (right->crd[1] , base->crd[0]);
		Ma::Copy (right->crd[2] , base->crd[1]); 
		base->right_Child = right;
		Subdivide_node (right, bukkit, num_Lvls - 1); 

		
		BT_subdiv_node* left = bukkit.Get();
		left->level = base->level + 1;
		left->varn_Ind = Left_child_ind (base->varn_Ind); 
		Ma::Copy (left->crd[0] , mid_Crd); 
		Ma::Copy (left->crd[1] , base->crd[2]);
		Ma::Copy (left->crd[2] , base->crd[0]); 
		base->left_Child = left ;
		Subdivide_node (left, bukkit, num_Lvls - 1);
	}
	else
	{
		base->left_Child	= 0;
		base->right_Child	= 0;
	}
	// ::::::::::::::::::::::::::::::::::::::::::::::::::::::
	/*
		NOTES: vertex coords start at apex of triangles 
		and winds counter clockwise. 

	+--------------+
	|  Left       /
	|          /
	|       /
	|    /
	| /          Right
	+--------------+
	*/ 
	// ::::::::::::::::::::::::::::::::::::::::::::::::::::::
	return base;
}


//
// Runtime section
inline bool Is_Part_of_Diamond (BT_RT_node* tri) { 
	return (tri->base_Neighbor->base_Neighbor == tri ? true : false);
	}

//
// 
void Reset_node (
	BT_RT_node* n, 
	BT_RT_node*	rtNbr, 
	BT_RT_node*	ltNbr, 
	BT_RT_node*	bsNbr
	) 
{
	n->right_Neighbor	= rtNbr;
	n->left_Neighbor	= ltNbr;
	n->base_Neighbor	= bsNbr;
	// IMPORTANT: calling this function its implies 
	// we're initializing this node so we dont have 
	// children so explicitly null them
	n->right_Child	= 0;
	n->left_Child	= 0;
} 

//
// SplitTriNode
void Split_tri_node (BT_RT_node* tri, BT_RT_node::Bukkit& bukkit) {
	// direction index is defined by which way the tri apex is pointing 
	//    0	
	//  7   1	
	// 6     2	
	//  5   3
	//    4
	const unsigned dirChildIndexRt[] = {
		3, 4, 5, 6, 7, 0, 1, 2
		}; 

	const unsigned dirChildIndexLt[] = {
		5, 6, 7, 0, 1, 2, 3, 4,
		}; 

	if (tri->base_Neighbor) {
		BOOST_ASSERT (Is_Part_of_Diamond (tri)); 
		}

	//BT_RT_node
	BOOST_ASSERT (tri->right_Child == 0); 
	BOOST_ASSERT (tri->left_Child  == 0); 
	
	Ma::Vec2i uvTmp;
	//  split ourselves 
	Midpoint_uv (uvTmp, tri->uv[2], tri->uv[1]);  
	
	BT_RT_node* newRightChild = bukkit.Get();
	BT_RT_node* newLeftChild =  bukkit.Get();

	tri->right_Child = newRightChild;
	tri->left_Child = newLeftChild;

	// calc right child
	newRightChild->dir_Index = dirChildIndexRt[tri->dir_Index]; 
	newRightChild->tri_Index = Right_child_ind (tri->tri_Index); 
	Ma::Set (newRightChild->uv[0], Ma::U (uvTmp), Ma::V (uvTmp)); 
	Ma::Set (newRightChild->uv[1], Ma::U (tri->uv[0]), Ma::V (tri->uv[0])); 
	Ma::Set (newRightChild->uv[2], Ma::U (tri->uv[1]), Ma::V (tri->uv[1])); 

	if (tri->base_Neighbor) {
		Reset_node (newRightChild, newLeftChild, tri->base_Neighbor->left_Child, tri->right_Neighbor); 
		//newRightChild->ResetNode (newLeftChild, tri->baseNeighbor->leftChild, tri->rightNeighbor); 
		}
	else {
		Reset_node (newRightChild, newLeftChild, 0, tri->right_Neighbor);
		//newRightChild->ResetNode (newLeftChild, 0, tri->rightNeighbor); 
		}

	// calc left child
	newLeftChild->dir_Index = dirChildIndexLt[tri->dir_Index]; 
	newLeftChild->tri_Index = Left_child_ind (tri->tri_Index); 
	Ma::Set (newLeftChild->uv[0], Ma::U (uvTmp), Ma::V (uvTmp) ); 
	Ma::Set (newLeftChild->uv[1], Ma::U (tri->uv[2]), Ma::V (tri->uv[2])); 
	Ma::Set (newLeftChild->uv[2], Ma::U (tri->uv[0]), Ma::V (tri->uv[0])); 

	if (tri->base_Neighbor) { 
		Reset_node (newLeftChild, tri->base_Neighbor->right_Child, newRightChild, tri->left_Neighbor); 
		//newLeftChild->ResetNode (tri->baseNeighbor->rightChild, newRightChild, tri->leftNeighbor); 
	} 
	else {
		Reset_node (newLeftChild, 0, newRightChild, tri->left_Neighbor);
		//newLeftChild->ResetNode (0, newRightChild, tri->leftNeighbor); 
	}


	//after we split we have to connect adjacent tris which 
	//pointed to this tri and now set them its children
	switch (tri->dir_Index) {
		// :::::::::::::::::::::::::::::::::::::::
		case 0: 

			if (tri->right_Neighbor) { // rightNeighbor
				switch (tri->right_Neighbor->dir_Index) {
					case 4: tri->right_Neighbor->right_Neighbor = newRightChild; break;
					case 2: tri->right_Neighbor->left_Neighbor = newRightChild; break;
					case 7: tri->right_Neighbor->base_Neighbor = newRightChild; break;
					default: BOOST_ASSERT (0); break;
					}
				}
		
			if (tri->left_Neighbor) { // leftNeighbor
				switch (tri->left_Neighbor->dir_Index) {
					case 1: tri->left_Neighbor->base_Neighbor = newLeftChild;  break;
					case 6: tri->left_Neighbor->right_Neighbor = newLeftChild; break;
					case 4: tri->left_Neighbor->left_Neighbor = newLeftChild; break;
					default: BOOST_ASSERT (0); break;
					}
				}
			
			if (tri->base_Neighbor) {
				BOOST_ASSERT (tri->base_Neighbor->dir_Index == 4); //  "split shouldnt have been called"); 
				BT_RT_node* baseNbrRtCh = tri->base_Neighbor->right_Child;
				BT_RT_node* baseNbrLtCh = tri->base_Neighbor->left_Child;
				if (baseNbrRtCh && baseNbrLtCh) {
					baseNbrRtCh->left_Neighbor = newLeftChild;
					baseNbrLtCh->right_Neighbor = newRightChild;
					}
				}

			break;
	
		// :::::::::::::::::::::::::::::::::::::::
		case 1: 

			if(tri->right_Neighbor) {
				switch (tri->right_Neighbor->dir_Index) {
					case 0: tri->right_Neighbor->base_Neighbor = newRightChild; break;
					case 3: tri->right_Neighbor->left_Neighbor = newRightChild; break;
					default: BOOST_ASSERT (0); break;
					}
				}

			if (tri->left_Neighbor) { 
				switch (tri->left_Neighbor->dir_Index) {
					case 2: tri->left_Neighbor->base_Neighbor = newLeftChild; break;
					case 7: tri->left_Neighbor->right_Neighbor = newLeftChild; break;
					default: BOOST_ASSERT (0); break;
					}
				}

			if (tri->base_Neighbor) {
				BOOST_ASSERT (tri->base_Neighbor->dir_Index == 5); // , "split shouldnt have been called"); 
				BT_RT_node* baseNbrRtCh = tri->base_Neighbor->right_Child;
				BT_RT_node* baseNbrLtCh = tri->base_Neighbor->left_Child;
				if (baseNbrRtCh && baseNbrLtCh) {
					baseNbrRtCh->left_Neighbor = newLeftChild;
					baseNbrLtCh->right_Neighbor = newRightChild;
			
					}
				}

			break;

		// :::::::::::::::::::::::::::::::::::::::
		case 2: 

			if(tri->right_Neighbor)  {
				switch (tri->right_Neighbor->dir_Index) {
					case 1: tri->right_Neighbor->base_Neighbor = newRightChild; break;
					case 6: tri->right_Neighbor->right_Neighbor = newRightChild; break; 
					case 4: tri->right_Neighbor->left_Neighbor = newRightChild; break;
					default: BOOST_ASSERT (0); break;
				}
			}
			if (tri->left_Neighbor) { 
				switch (tri->left_Neighbor->dir_Index) {
					case 0: tri->left_Neighbor->right_Neighbor = newLeftChild; break;
					case 3: tri->left_Neighbor->base_Neighbor = newLeftChild; break;
					case 6: tri->left_Neighbor->left_Neighbor = newLeftChild; break;
					default: BOOST_ASSERT (0); break;
				}
			}
			if (tri->base_Neighbor) {
				BOOST_ASSERT (tri->base_Neighbor->dir_Index == 6); // , "split shouldnt have been called"); 
				BT_RT_node* baseNbrRtCh = tri->base_Neighbor->right_Child;
				BT_RT_node* baseNbrLtCh = tri->base_Neighbor->left_Child;
				if (baseNbrRtCh && baseNbrLtCh) {
					baseNbrRtCh->left_Neighbor = newLeftChild;
					baseNbrLtCh->right_Neighbor = newRightChild;
				}
			}

			break;

		// :::::::::::::::::::::::::::::::::::::::
		case 3: 

			if (tri->right_Neighbor) { 
				switch (tri->right_Neighbor->dir_Index) {
					case 2: tri->right_Neighbor->base_Neighbor = newRightChild; break;
					case 5: tri->right_Neighbor->left_Neighbor = newRightChild; break;
					case 7: tri->right_Neighbor->right_Neighbor = newRightChild; break;
					default: BOOST_ASSERT (0); break;
				}
			}
			if (tri->left_Neighbor) { 
				switch (tri->left_Neighbor->dir_Index) {
					case 4: tri->left_Neighbor->base_Neighbor = newLeftChild; break;
					case 1: tri->left_Neighbor->right_Neighbor = newLeftChild; break;
					case 7: tri->left_Neighbor->left_Neighbor= newLeftChild; break;
					default: BOOST_ASSERT (0); break;
					}
				}
		
			if (tri->base_Neighbor) {
				BOOST_ASSERT (tri->base_Neighbor->dir_Index == 7); // , "split shouldnt have been called"); 
				BT_RT_node* baseNbrRtCh = tri->base_Neighbor->right_Child;
				BT_RT_node* baseNbrLtCh = tri->base_Neighbor->left_Child;
				if (baseNbrRtCh && baseNbrLtCh) {
					baseNbrRtCh->left_Neighbor = newLeftChild;
					baseNbrLtCh->right_Neighbor = newRightChild;
					}
				}
			break;
		// :::::::::::::::::::::::::::::::::::::::
		case 4: 
			if (tri->right_Neighbor) { 
				switch (tri->right_Neighbor->dir_Index) {
					case 6: tri->right_Neighbor->left_Neighbor = newRightChild; break;
					case 3: tri->right_Neighbor->base_Neighbor = newRightChild; break;
					case 0: tri->right_Neighbor->right_Neighbor = newRightChild; break;
					default: BOOST_ASSERT (0); break;
				}
			}
			if (tri->left_Neighbor) { 
				switch (tri->left_Neighbor->dir_Index) {
					case 2: tri->left_Neighbor->right_Neighbor = newLeftChild; break;
					case 5: tri->left_Neighbor->base_Neighbor = newLeftChild; break;
					case 0: tri->left_Neighbor->left_Neighbor = newLeftChild; break;
					default: BOOST_ASSERT (0); break;
				}
			}
			if (tri->base_Neighbor) {
				BOOST_ASSERT (tri->base_Neighbor->dir_Index == 0); // , "split shouldnt have been called"); 
				BT_RT_node* baseNbrRtCh = tri->base_Neighbor->right_Child;
				BT_RT_node* baseNbrLtCh = tri->base_Neighbor->left_Child;
				if (baseNbrRtCh && baseNbrLtCh) {
					baseNbrRtCh->left_Neighbor = newLeftChild;
					baseNbrLtCh->right_Neighbor = newRightChild;
					}
				}
			break;
		// :::::::::::::::::::::::::::::::::::::::
		case 5: 
			if (tri->right_Neighbor) { 

				switch (tri->right_Neighbor->dir_Index) {
					case 4: tri->right_Neighbor->base_Neighbor = newRightChild; break;
					case 7: tri->right_Neighbor->left_Neighbor = newRightChild; break;
					default: BOOST_ASSERT (0); break;
				}
			}
			if (tri->left_Neighbor) {
				switch (tri->left_Neighbor->dir_Index) {
					case 6: tri->left_Neighbor->base_Neighbor = newLeftChild; break;
					case 3: tri->left_Neighbor->right_Neighbor = newLeftChild; break;
					default: BOOST_ASSERT (0); break;
				}
			}
			if (tri->base_Neighbor) {
				BOOST_ASSERT (tri->base_Neighbor->dir_Index == 1); // , "split shouldnt have been called"); 
				BT_RT_node* baseNbrRtCh = tri->base_Neighbor->right_Child;
				BT_RT_node* baseNbrLtCh = tri->base_Neighbor->left_Child;
				if (baseNbrRtCh && baseNbrLtCh) {
					baseNbrRtCh->left_Neighbor = newLeftChild;
					baseNbrLtCh->right_Neighbor = newRightChild;
				}
			}
			break;
		// :::::::::::::::::::::::::::::::::::::::
		case 6: 
			if (tri->right_Neighbor) { 
				switch (tri->right_Neighbor->dir_Index) {
					case 5: tri->right_Neighbor->base_Neighbor = newRightChild; break;
					case 2: tri->right_Neighbor->right_Neighbor = newRightChild; break;
					case 0: tri->right_Neighbor->left_Neighbor = newRightChild; break;
					default: BOOST_ASSERT (0); break;
				}
			}
			if (tri->left_Neighbor) { 
				switch (tri->left_Neighbor->dir_Index) {
					case 4: tri->left_Neighbor->right_Neighbor = newLeftChild; break;
					case 2: tri->left_Neighbor->left_Neighbor = newLeftChild; break;
					case 7: tri->left_Neighbor->base_Neighbor = newLeftChild; break;
					default: BOOST_ASSERT (0); break;
				}
			}
			if (tri->base_Neighbor) {
				BOOST_ASSERT (tri->base_Neighbor->dir_Index == 2); // , "split shouldnt have been called"); 
				BT_RT_node* baseNbrRtCh = tri->base_Neighbor->right_Child;
				BT_RT_node* baseNbrLtCh = tri->base_Neighbor->left_Child;
				if (baseNbrRtCh && baseNbrLtCh) {
					baseNbrRtCh->left_Neighbor = newLeftChild;
					baseNbrLtCh->right_Neighbor = newRightChild;
				}
			}
			break;
		// :::::::::::::::::::::::::::::::::::::::
		case 7: 
			if (tri->right_Neighbor) { 
				switch (tri->right_Neighbor->dir_Index) {
					case 6: tri->right_Neighbor->base_Neighbor = newRightChild; break;
					case 3: tri->right_Neighbor->right_Neighbor = newRightChild; break;
					case 1: tri->right_Neighbor->left_Neighbor = newRightChild; break;
					default: BOOST_ASSERT (0); break;
				}
			}
			if (tri->left_Neighbor) {
				switch (tri->left_Neighbor->dir_Index) {
					case 5: tri->left_Neighbor->right_Neighbor = newLeftChild; break;
					case 3: tri->left_Neighbor->left_Neighbor = newLeftChild; break;
					case 0: tri->left_Neighbor->base_Neighbor = newLeftChild; break;
					default: BOOST_ASSERT (0); break;
				}
			}
			if (tri->base_Neighbor) {

				if (tri->base_Neighbor->dir_Index != 3)
				{
					int dbgInt = 0; 
				}
				BOOST_ASSERT (tri->base_Neighbor->dir_Index == 3) ; // , "split shouldnt have been called"); 
				BT_RT_node* baseNbrRtCh = tri->base_Neighbor->right_Child;
				BT_RT_node* baseNbrLtCh = tri->base_Neighbor->left_Child;
				if (baseNbrRtCh && baseNbrLtCh) {
					baseNbrRtCh->left_Neighbor = newLeftChild;
					baseNbrLtCh->right_Neighbor = newRightChild;
					}
				}
			break;
		// :::::::::::::::::::::::::::::::::::::::
		default: 
			BOOST_ASSERT (0); break;

	}
}

//
// SplitTriNodeRecurs
void Split_node_recurs (BT_RT_node* tri, BT_RT_node::Bukkit& bukkit) {
	// 1. The Node is part of a Diamond - Split the node and its Base Neighbor.
	// 2. The Node is on the edge of the mesh - Trivial, only split the node.
	// 3. The Node is not part of a Diamond - Force Split the Base Neighbor.
	BT_RT_node* baseNbr = tri->base_Neighbor;

	if (baseNbr) {
		// BOOST_ASSERT (tri->baseNeighbor, "wtf"); 
		while (!Is_Part_of_Diamond (tri)) {
			Split_node_recurs (tri->base_Neighbor, bukkit); 
			}

		baseNbr = tri->base_Neighbor;
		Split_tri_node (baseNbr, bukkit); 
		Split_tri_node (tri, bukkit); 
		// because when we Split_tri_node (tri->baseNeighbor), 
		// tri did have children yet so couldnt update them.
		tri->right_Child->left_Neighbor = baseNbr->left_Child;
		tri->left_Child->right_Neighbor = baseNbr->right_Child;
		}
	else {
		Split_tri_node (tri, bukkit); 
		}
}



// SubDivideRecurse 
void Subdivide_recurs (BT_RT_node* tri, int* varnTree, BT_RT_node::Bukkit& bukkit, int tolerance) {

	unsigned triInd = tri->tri_Index;
	// hardcode
	if (triInd < CLoDSharedData_indexDepth) {

		if (tri->left_Child && tri->right_Child) {
			Subdivide_recurs (tri->right_Child, varnTree, bukkit, tolerance); 
			Subdivide_recurs (tri->left_Child, varnTree, bukkit, tolerance); 
			}
		else if (varnTree[triInd] >= tolerance) {
			//BOOST_ASSERT (tri->leftChild == 0, "leftChild not null"); 
			//BOOST_ASSERT (tri->rightChild == 0, "rightchild not null");
			Split_node_recurs (tri, bukkit);
			Subdivide_recurs (tri->right_Child, varnTree, bukkit, tolerance); 
			Subdivide_recurs (tri->left_Child, varnTree, bukkit, tolerance); 
			}
		}
	else {
		int dbgInt = 0; 
		}
}




CLoD_patch& Initialize_patch (CLoD_patch& patch, const CLoD_terrain::Params& params)
{

	
	// ?? Register_patch (patch, terrain); 


	// 
	{

		// !! the variance trees are built on this particular order, 
		// !! just like in tools.  make sure these are consistent with tools 
		Ma::Set (patch.triBaseRight.uv[0], params.stride - 1, params.stride - 1); 
		Ma::Set (patch.triBaseRight.uv[1], params.stride - 1, 0); 
		Ma::Set (patch.triBaseRight.uv[2], 0, params.stride - 1); 
		patch.triBaseRight.dir_Index = 3; 
		patch.triBaseRight.tri_Index = 1; 

		Ma::Set (patch.triBaseLeft.uv[0], 0, 0); 
		Ma::Set (patch.triBaseLeft.uv[1], 0, params.stride - 1); 
		Ma::Set (patch.triBaseLeft.uv[2], params.stride - 1, 0); 
		patch.triBaseLeft.dir_Index = 7; 
		patch.triBaseLeft.tri_Index = 1;
	}

	// init height texture 
	{
		BOOST_ASSERT (0); 
		//Rn::Texture2D::Params params = {
		//	Rn::Texture::TF_1F, 
		//	1024, 1024,
		//	0
		//};

		std::vector<float> data ( 64 * 64 ); 

		patch.height_Texture = new Rn::Texture; 
		Rn::Load_2D_image (
			*patch.height_Texture, 
			0, 64, 64, 0, 
			Rn::Texture::TF_1F, 
			Rn::Texture::TF_1F, 
			(const void*) data.data()
			); 

		//Rn::CreateTexture2D (
	}

	// init normal map
	{}

	
	
	{ // quantized variance trees 
		patch.rightVarianceTree; 
		//	float quantVariance = CLoDSharedData::varianceScale / shared->resRef->maxTreeVariance;
		//	//
		//	rightVarianceTree = new int[resRef->varianceTreeRt.size()];
		//	for (size_t i = 0; i < resRef->varianceTreeRt.size(); i++) {
		//		rightVarianceTree[i] = 
		//				static_cast<int>(quantVariance * resRef->varianceTreeRt[i]); 

		//		BOOST_ASSERT (rightVarianceTree[i] <= static_cast<int> (CLoDSharedData::varianceScale), "how?"); 
		//	}

		patch.leftVarianceTree;
		//	leftVarianceTree = new int[resRef->varianceTreeLt.size ()];
		//	//
		//	for (size_t i = 0; i < resRef->varianceTreeLt.size (); i++) {
		//		leftVarianceTree[i] = 
		//			static_cast<int>(quantVariance * resRef->varianceTreeLt[i]); 

		//		BOOST_ASSERT (leftVarianceTree[i] <= static_cast<int> (CLoDSharedData::varianceScale), "how?"); 

		//	}
	}

	// params.stride
	return patch;
}


bool Load_patch	(CLoD_patch& patch)
{
	// variance tree is local, 


	// load height texture
	// load image image texture

	return false; 
}

bool Unload_patch (CLoD_patch& patch)
{

	// load height texture
	// load image image texture
	return false; 

}

//
//
CLoD_patch& Create_patch (CLoD_patch& patch)
{
	return patch; 
}

//
CLoD_terrain& Load_group (
	CLoD_terrain&	terr, 
	Terrain_group_desc& desc
	)
{

	(terr.group_List.push_back) (CLoD_terrain::Group()); 
	CLoD_terrain::Group& grp =  (terr.group_List.back)(); 

	grp.fname = desc.fname; 
	grp.arr.resize (desc.num_Patches); 
	
	for (size_t i  = 0; i < desc.patch_Indices.size (); i++)
	{
		//CLoD_patch::Index ind = {
		//	Ma::U (desc.patch_Indices[i]), Ma::V (desc.patch_Indices[i])
		//	}; 
		CLoD_patch& p	= grp.arr[i]; 
		p.patch_Name    = desc.patch_Names[i];  
		p.easting_Ind	= desc.patch_Indices[i].en.e; 
		p.northing_Ind	= desc.patch_Indices[i].en.n; 
		//p.easting_Ind	= Ma::U (desc.patch_Indices[i]); 
		//p.northing_Ind	= Ma::V (desc.patch_Indices[i]); 
		//p.leftVarianceTree.resize (69); 
		//p.rightVarianceTree.resize (69);

		if (terr.patch_Map.count (desc.patch_Indices[i].ui))
			continue; // something alread exists, skip
	
		terr.patch_Map[desc.patch_Indices[i].ui] = &p; 
	}

	return terr; 
}


//
// Initialize_patch_group 
CLoD_terrain& Initialize_patch_group (CLoD_terrain&	terr, const std::string& grp_Name)
{
	// should this be separate from Load_group?  
	// load variance trees and setup global params, BT_RT_nodes - tri_Dir, tri_Ind..
	// connect adjacent patches, ...etc. 

	// height & color map defer

	return terr; 
}

//
//void CLoDTerrainPatch::Create (ResourceObject* res) {
//
//	BOOST_ASSERT (res->EntryType () == ID_TERRAIN_CLOD_PATCH, "invalid res");
//	resRef = reinterpret_cast<TerrCLoDPatchRes*>(res);
//
//	BOOST_ASSERT (resRef->shared.ptr->uStride == resRef->shared.ptr->vStride, "Patch uStride/vStride mismatch"); 
//
//
//	gridStride = resRef->shared.ptr->uStride;
//
//	// set the current shared data first
//	CLoDSharedData* shared = CLoDSharedData::Get (resRef->shared.ptr); 
//
//	// add myself to the shared patch list
//	{	std::vector<CLoDTerrainPatch*>& patchArr = shared->patchField; 
//
//		int thisInd = (resRef->indPosZ  * shared->patchStride) + resRef->indPosX; 
//		shared->RegisterPatch (thisInd, this); 
//	}
//
//	/// !! the variance trees are built on this particular order, 
//	/// !! just like in tools.  make sure these are consistent with tools 
//	{
//		short patchInd = (resRef->shared.ptr->uStride -1); 
//
//		set_uv (triBaseRight.uv[0], patchInd, patchInd); 
//		set_uv (triBaseRight.uv[1], patchInd, 0); 
//		set_uv (triBaseRight.uv[2], 0, patchInd); 
//		triBaseRight.dirIndex = 3; 
//		triBaseRight.triIndex = 1; 
//
//		set_uv (triBaseLeft.uv[0], 0, 0); 
//		set_uv (triBaseLeft.uv[1], 0, patchInd); 
//		set_uv (triBaseLeft.uv[2], patchInd, 0); 
//		triBaseLeft.dirIndex= 7; 
//		triBaseLeft.triIndex = 1; 
//	}
//	//{ /*[create dynamic index buffer]*/
//	//	IndexBufferParams params; 
//	//	params.indexTyp = Index16; 
//	//	params.numElems = 2 * 3 * (shared->uStride - 1) * (shared->vStride - 1);
//	//	params.source = 0; 
//	//	params.usage = BufferUsageDynamicDraw; 
//
//	//	CreateIndexBuffer (&params, &indBuffer); 
//	//	BOOST_ASSERT (indBuffer, "create IB failed"); 
//	//}
//
//
//	{ /*[create height map]*/
//		TextureParams2D params; 
//		params.format = TextureFormatEnum (resRef->hgtMap.ptr->fmt);
//		params.width = resRef->hgtMap.ptr->width;
//		params.height = resRef->hgtMap.ptr->height;
//		params.source = resRef->hgtMap.ptr->data;
//		CreateTexture2D (&params, &heightMap); 
//		BOOST_ASSERT (heightMap, "bad heightmap ");
//	}
//
//	{ /*[create normal map]*/
//
//		//TextureParams2D params; 
//		//params.format = TextureFormatEnum (resRef->nrmMap.ptr->fmt);
//		//params.width = resRef->nrmMap.ptr->width;
//		//params.height = resRef->nrmMap.ptr->height;
//		//params.source = resRef->nrmMap.ptr->data;
//		//CreateTexture2D (&params, &normalMap); 
//
//		TextureParams2D params; 
//
//		int numPxls = resRef->nrmMap.ptr->width * resRef->nrmMap.ptr->height; 
//
//		vector3f* normf =  new vector3f[numPxls];
//		col3ub* col = reinterpret_cast<col3ub*>(resRef->nrmMap.ptr->data); 
//
//		for (int i = 0; i < numPxls; i++) {
//
//			normf[i].x = col[i].r;
//			normf[i].y = col[i].g;
//			normf[i].z = col[i].b;
//
//			Vec3Scale (&normf[i], &normf[i], 1.0f/255.0f);
//
//			int fsd = 0;
//			fsd++;
//			//AddVec3 (&normf[i], &normf[i],-0.5f); 
//		}
//
//		params.format = TextureFormat3F;
//		params.width = resRef->nrmMap.ptr->width;
//		params.height = resRef->nrmMap.ptr->height;
//		params.source = (vector3f*) normf;
//		CreateTexture2D (&params, &normalMap); 
//
//		delete[] normf;
//
//
//		BOOST_ASSERT (normalMap, "bad normal map");
//	}
//	
//
//	
//	{ /* [quantized variance trees] */
//
//		float quantVariance = CLoDSharedData::varianceScale / shared->resRef->maxTreeVariance;
//		//
//		rightVarianceTree = new int[resRef->varianceTreeRt.size()];
//		for (size_t i = 0; i < resRef->varianceTreeRt.size(); i++) {
//			rightVarianceTree[i] = 
//					static_cast<int>(quantVariance * resRef->varianceTreeRt[i]); 
//
//			BOOST_ASSERT (rightVarianceTree[i] <= static_cast<int> (CLoDSharedData::varianceScale), "how?"); 
//		}
//		
//
//
//		leftVarianceTree = new int[resRef->varianceTreeLt.size ()];
//		//
//		for (size_t i = 0; i < resRef->varianceTreeLt.size (); i++) {
//			leftVarianceTree[i] = 
//				static_cast<int>(quantVariance * resRef->varianceTreeLt[i]); 
//
//			BOOST_ASSERT (leftVarianceTree[i] <= static_cast<int> (CLoDSharedData::varianceScale), "how?"); 
//
//		}
//	}
//}

//
//CLoD_terrain::CLoD_terrain () 
//	: group_List (), patch_Map ()
//{
//
//}

//
// CLoD_terrain  CLoD_terrain  CLoD_terrain  CLoD_terrain 
void CLoD_terrain::Update_visible_area (class View_params* params)
{

}

//
// 
void CLoD_terrain::Update_patch_topology ()
{

}

//
// 
void CLoD_terrain::set_patch_ (unsigned short e, unsigned short n, CLoD_patch* p)
{
	CLoD_patch::Index ind = {e, n}; 
	patch_Map[ind.ui] = p; 
}

//
// 
const CLoD_patch* CLoD_terrain::Lookup_patch (
	unsigned short e, 
	unsigned short n
	) const
{
	CLoD_patch::Index ind = {e, n}; 

	PatchMap::const_iterator iter = patch_Map.find (ind.ui); 

	if (iter != patch_Map.end ())
		return iter->second;

	return 0;
}

//
// variance subdivision
void CLoD_terrain::Tesselate_terrain () {

}

//
//
void CLoD_terrain::Build_index_buffer () {
}

//
//
void CLoD_terrain::Render () {
}



