#include "Terrain.h"



namespace Tr
{
//
//
float Build_variance_tree (std::vector<float>& varn_Tree, const BT_subdiv_node& base, const HeightMapSampler& hgtMap)
{
	// NOTE: variance is not difference in height value, so always positive
	if (base.left_Child && base.right_Child)
	{
		Ma::Vec2i mid_Crd; 
		Midpoint_uv(mid_Crd, base.crd[2], base.crd[1]); 

		float hgt_B = hgtMap.Get ( Ma::X (base.crd[2]), Ma::Y (base.crd[2])); 
		float hgt_A = hgtMap.Get ( Ma::X (base.crd[2]), Ma::Y (base.crd[2])); 
		float interp_Hgt = 0.5f * (hgt_B - hgt_A); 
		interp_Hgt += hgt_A;

		float actual_Hgt = hgtMap.Get (Ma::X (mid_Crd), Ma::Y (mid_Crd)); 

		float base_Variance	 = interp_Hgt - actual_Hgt;
		float child_Variance 
				= Build_variance_tree (varn_Tree, *base.left_Child , hgtMap) 
				+ Build_variance_tree (varn_Tree, *base.right_Child, hgtMap);

		varn_Tree[base.varn_Ind] = fabs (base_Variance - (0.5f * child_Variance)); 
		return varn_Tree[base.varn_Ind]; 
	}					   

	return 0.0f;

}


//
//
std::vector<Ma::Vec2f>& Build_vertex_patch (std::vector<Ma::Vec2f>& pos, unsigned vertex_Stride, float patch_Size)
{
	float vert_Step = patch_Size / (vertex_Stride - 1); 

	for (size_t Y = 0; Y < vertex_Stride; Y++)
		for (size_t X = 0; X < vertex_Stride; X++)
			Ma::Set (pos[(vertex_Stride * Y) + X], (vert_Step * X), (vert_Step * Y)); 

	return pos;

}


std::vector<Ma::Vec2f>& Build_uv_coords (std::vector<Ma::Vec2f>& uv, unsigned vertex_Stride)
{
	
	float uv_Step = 1.0f / (vertex_Stride - 1); 

	for (size_t Y = 0; Y < vertex_Stride; Y++)
		for (size_t X = 0; X < vertex_Stride; X++)
	{
		float u = (uv_Step * X);
		float v = 1.0f - (uv_Step * Y);
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


/*
	inline bool IsPartOfDiamond (BiTriangleNode* tri) { 
		return (tri->baseNeighbor->baseNeighbor == tri) ? true : false;
	}


	inline void SplitTriNode (BiTriangleNode* tri) {
		// direction index is defined by which way the tri apex is pointing 
		//    0	
		//  7   1	
		// 6     2	
		//  5   3
		//    4
		unsigned dirChildIndexRt[] = {
			3, 4, 5, 6, 7, 0, 1, 2
		}; 

		unsigned dirChildIndexLt[] = {
			5, 6, 7, 0, 1, 2, 3, 4,
		}; 

		if (tri->baseNeighbor) {
			BreakAssert (IsPartOfDiamond (tri), "not part of diamond?"); 
		}
		BreakAssert (tri->rightChild == 0, ""); 
		BreakAssert (tri->leftChild  == 0, ""); 


		vector2s uvTmp;
		//  split ourselves 
		mid_uv (uvTmp, tri->uv[2], tri->uv[1]);  

		BiTriangleNode* newRightChild = CLoDSharedData::GetTriNode ();
		BiTriangleNode* newLeftChild = CLoDSharedData::GetTriNode ();

		tri->rightChild = newRightChild;
		tri->leftChild = newLeftChild;

		// calc right child
		newRightChild->dirIndex = dirChildIndexRt[tri->dirIndex]; 
		newRightChild->triIndex = RightChildInd (tri->triIndex); 
		set_uv (newRightChild->uv[0], uvTmp.u, uvTmp.v); 
		set_uv (newRightChild->uv[1], tri->uv[0].u, tri->uv[0].v); 
		set_uv (newRightChild->uv[2], tri->uv[1].u, tri->uv[1].v); 

		if (tri->baseNeighbor) {
			newRightChild->ResetNode (newLeftChild, tri->baseNeighbor->leftChild, tri->rightNeighbor); 
		}
		else {
			newRightChild->ResetNode (newLeftChild, 0, tri->rightNeighbor); 
		}

		// calc left child
		newLeftChild->dirIndex = dirChildIndexLt[tri->dirIndex]; 
		newLeftChild->triIndex = LeftChildInd (tri->triIndex); 
		set_uv (newLeftChild->uv[0], uvTmp.u, uvTmp.v); 
		set_uv (newLeftChild->uv[1], tri->uv[2].u, tri->uv[2].v); 
		set_uv (newLeftChild->uv[2], tri->uv[0].u, tri->uv[0].v); 

		if (tri->baseNeighbor) { 
			newLeftChild->ResetNode (tri->baseNeighbor->rightChild, newRightChild, tri->leftNeighbor); 
		} 
		else {
			newLeftChild->ResetNode (0, newRightChild, tri->leftNeighbor); 
		}

		// tri->leftChild->SetAdjacent (0, tri->rightChild, tri->leftNeighbor); 

		//after we split we have to connect adjacent tris which 
		//pointed to this tri and now set them its children

		switch (tri->dirIndex) {
			// :::::::::::::::::::::::::::::::::::::::
			case 0: 

				if (tri->rightNeighbor) { // rightNeighbor
					switch (tri->rightNeighbor->dirIndex) {
						case 4: tri->rightNeighbor->rightNeighbor = newRightChild; break;
						case 2: tri->rightNeighbor->leftNeighbor = newRightChild; break;
						case 7: tri->rightNeighbor->baseNeighbor = newRightChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}
			
				if (tri->leftNeighbor) { // leftNeighbor
					switch (tri->leftNeighbor->dirIndex) {
						case 1: tri->leftNeighbor->baseNeighbor = newLeftChild;  break;
						case 6: tri->leftNeighbor->rightNeighbor = newLeftChild; break;
						case 4: tri->leftNeighbor->leftNeighbor = newLeftChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}

				if (tri->baseNeighbor) {
					Assert (tri->baseNeighbor->dirIndex == 4, "split shouldnt have been called"); 
					BiTriangleNode* baseNbrRtCh = tri->baseNeighbor->rightChild;
					BiTriangleNode* baseNbrLtCh = tri->baseNeighbor->leftChild;
					if (baseNbrRtCh && baseNbrLtCh) {
						baseNbrRtCh->leftNeighbor = newLeftChild;
						baseNbrLtCh->rightNeighbor = newRightChild;
					}
				}

				break;
		
			// :::::::::::::::::::::::::::::::::::::::
			case 1: 

				if(tri->rightNeighbor) {
					switch (tri->rightNeighbor->dirIndex) {
						case 0: tri->rightNeighbor->baseNeighbor = newRightChild; break;
						case 3: tri->rightNeighbor->leftNeighbor = newRightChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}

				if (tri->leftNeighbor) { 
					switch (tri->leftNeighbor->dirIndex) {
						case 2: tri->leftNeighbor->baseNeighbor = newLeftChild; break;
						case 7: tri->leftNeighbor->rightNeighbor = newLeftChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}

				if (tri->baseNeighbor) {
					Assert (tri->baseNeighbor->dirIndex == 5, "split shouldnt have been called"); 
					BiTriangleNode* baseNbrRtCh = tri->baseNeighbor->rightChild;
					BiTriangleNode* baseNbrLtCh = tri->baseNeighbor->leftChild;
					if (baseNbrRtCh && baseNbrLtCh) {
						baseNbrRtCh->leftNeighbor = newLeftChild;
						baseNbrLtCh->rightNeighbor = newRightChild;
				
					}
				}

				break;

			// :::::::::::::::::::::::::::::::::::::::
			case 2: 

				if(tri->rightNeighbor)  {
					switch (tri->rightNeighbor->dirIndex) {
						case 1: tri->rightNeighbor->baseNeighbor = newRightChild; break;
						case 6: tri->rightNeighbor->rightNeighbor = newRightChild; break; 
						case 4: tri->rightNeighbor->leftNeighbor = newRightChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}
				if (tri->leftNeighbor) { 
					switch (tri->leftNeighbor->dirIndex) {
						case 0: tri->leftNeighbor->rightNeighbor = newLeftChild; break;
						case 3: tri->leftNeighbor->baseNeighbor = newLeftChild; break;
						case 6: tri->leftNeighbor->leftNeighbor = newLeftChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}
				if (tri->baseNeighbor) {
					Assert (tri->baseNeighbor->dirIndex == 6, "split shouldnt have been called"); 
					BiTriangleNode* baseNbrRtCh = tri->baseNeighbor->rightChild;
					BiTriangleNode* baseNbrLtCh = tri->baseNeighbor->leftChild;
					if (baseNbrRtCh && baseNbrLtCh) {
						baseNbrRtCh->leftNeighbor = newLeftChild;
						baseNbrLtCh->rightNeighbor = newRightChild;
					}
				}

				break;

			// :::::::::::::::::::::::::::::::::::::::
			case 3: 

				if (tri->rightNeighbor) { 
					switch (tri->rightNeighbor->dirIndex) {
						case 2: tri->rightNeighbor->baseNeighbor = newRightChild; break;
						case 5: tri->rightNeighbor->leftNeighbor = newRightChild; break;
						case 7: tri->rightNeighbor->rightNeighbor = newRightChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}
				if (tri->leftNeighbor) { 
					switch (tri->leftNeighbor->dirIndex) {
						case 4: tri->leftNeighbor->baseNeighbor = newLeftChild; break;
						case 1: tri->leftNeighbor->rightNeighbor = newLeftChild; break;
						case 7: tri->leftNeighbor->leftNeighbor= newLeftChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}
			
				if (tri->baseNeighbor) {
					Assert (tri->baseNeighbor->dirIndex == 7, "split shouldnt have been called"); 
					BiTriangleNode* baseNbrRtCh = tri->baseNeighbor->rightChild;
					BiTriangleNode* baseNbrLtCh = tri->baseNeighbor->leftChild;
					if (baseNbrRtCh && baseNbrLtCh) {
						baseNbrRtCh->leftNeighbor = newLeftChild;
						baseNbrLtCh->rightNeighbor = newRightChild;
					}
				}
				break;
			// :::::::::::::::::::::::::::::::::::::::
			case 4: 
				if (tri->rightNeighbor) { 
					switch (tri->rightNeighbor->dirIndex) {
						case 6: tri->rightNeighbor->leftNeighbor = newRightChild; break;
						case 3: tri->rightNeighbor->baseNeighbor = newRightChild; break;
						case 0: tri->rightNeighbor->rightNeighbor = newRightChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}
				if (tri->leftNeighbor) { 
					switch (tri->leftNeighbor->dirIndex) {
						case 2: tri->leftNeighbor->rightNeighbor = newLeftChild; break;
						case 5: tri->leftNeighbor->baseNeighbor = newLeftChild; break;
						case 0: tri->leftNeighbor->leftNeighbor = newLeftChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}
				if (tri->baseNeighbor) {
					Assert (tri->baseNeighbor->dirIndex == 0, "split shouldnt have been called"); 
					BiTriangleNode* baseNbrRtCh = tri->baseNeighbor->rightChild;
					BiTriangleNode* baseNbrLtCh = tri->baseNeighbor->leftChild;
					if (baseNbrRtCh && baseNbrLtCh) {
						baseNbrRtCh->leftNeighbor = newLeftChild;
						baseNbrLtCh->rightNeighbor = newRightChild;
					}
				}
				break;
			// :::::::::::::::::::::::::::::::::::::::
			case 5: 
				if (tri->rightNeighbor) { 

					switch (tri->rightNeighbor->dirIndex) {
						case 4: tri->rightNeighbor->baseNeighbor = newRightChild; break;
						case 7: tri->rightNeighbor->leftNeighbor = newRightChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}
				if (tri->leftNeighbor) {
					switch (tri->leftNeighbor->dirIndex) {
						case 6: tri->leftNeighbor->baseNeighbor = newLeftChild; break;
						case 3: tri->leftNeighbor->rightNeighbor = newLeftChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}
				if (tri->baseNeighbor) {
					Assert (tri->baseNeighbor->dirIndex == 1, "split shouldnt have been called"); 
					BiTriangleNode* baseNbrRtCh = tri->baseNeighbor->rightChild;
					BiTriangleNode* baseNbrLtCh = tri->baseNeighbor->leftChild;
					if (baseNbrRtCh && baseNbrLtCh) {
						baseNbrRtCh->leftNeighbor = newLeftChild;
						baseNbrLtCh->rightNeighbor = newRightChild;
					}
				}
				break;
			// :::::::::::::::::::::::::::::::::::::::
			case 6: 
				if (tri->rightNeighbor) { 
					switch (tri->rightNeighbor->dirIndex) {
						case 5: tri->rightNeighbor->baseNeighbor = newRightChild; break;
						case 2: tri->rightNeighbor->rightNeighbor = newRightChild; break;
						case 0: tri->rightNeighbor->leftNeighbor = newRightChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}
				if (tri->leftNeighbor) { 
					switch (tri->leftNeighbor->dirIndex) {
						case 4: tri->leftNeighbor->rightNeighbor = newLeftChild; break;
						case 2: tri->leftNeighbor->leftNeighbor = newLeftChild; break;
						case 7: tri->leftNeighbor->baseNeighbor = newLeftChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}
				if (tri->baseNeighbor) {
					Assert (tri->baseNeighbor->dirIndex == 2, "split shouldnt have been called"); 
					BiTriangleNode* baseNbrRtCh = tri->baseNeighbor->rightChild;
					BiTriangleNode* baseNbrLtCh = tri->baseNeighbor->leftChild;
					if (baseNbrRtCh && baseNbrLtCh) {
						baseNbrRtCh->leftNeighbor = newLeftChild;
						baseNbrLtCh->rightNeighbor = newRightChild;
					}
				}
				break;
			// :::::::::::::::::::::::::::::::::::::::
			case 7: 
				if (tri->rightNeighbor) { 
					switch (tri->rightNeighbor->dirIndex) {
						case 6: tri->rightNeighbor->baseNeighbor = newRightChild; break;
						case 3: tri->rightNeighbor->rightNeighbor = newRightChild; break;
						case 1: tri->rightNeighbor->leftNeighbor = newRightChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}
				if (tri->leftNeighbor) {
					switch (tri->leftNeighbor->dirIndex) {
						case 5: tri->leftNeighbor->rightNeighbor = newLeftChild; break;
						case 3: tri->leftNeighbor->leftNeighbor = newLeftChild; break;
						case 0: tri->leftNeighbor->baseNeighbor = newLeftChild; break;
						default: BreakAssert (0, "wtf"); break;
					}
				}
				if (tri->baseNeighbor) {

					if (tri->baseNeighbor->dirIndex != 3)
					{
						int dbgInt = 0; 
					}
					Assert (tri->baseNeighbor->dirIndex == 3, "split shouldnt have been called"); 
					BiTriangleNode* baseNbrRtCh = tri->baseNeighbor->rightChild;
					BiTriangleNode* baseNbrLtCh = tri->baseNeighbor->leftChild;
					if (baseNbrRtCh && baseNbrLtCh) {
						baseNbrRtCh->leftNeighbor = newLeftChild;
						baseNbrLtCh->rightNeighbor = newRightChild;
					}
				}
				break;
			// :::::::::::::::::::::::::::::::::::::::
			default: 
				BreakAssert (0, "wtf"); break;

		}
	}

	//
	void SplitTriNodeRecurs (BiTriangleNode* tri) {
		// 1. The Node is part of a Diamond - Split the node and its Base Neighbor.
		// 2. The Node is on the edge of the mesh - Trivial, only split the node.
		// 3. The Node is not part of a Diamond - Force Split the Base Neighbor.
		BiTriangleNode* baseNbr = tri->baseNeighbor; 

		if (baseNbr) {
	//		Assert (tri->baseNeighbor, "wtf"); 
			while (!IsPartOfDiamond (tri)) {
				SplitTriNodeRecurs (tri->baseNeighbor); 
			}

			baseNbr = tri->baseNeighbor;
			SplitTriNode (baseNbr); 
			SplitTriNode (tri); 
			// because when we SplitTriNode (tri->baseNeighbor), 
			// tri did have children yet so couldnt update them.
			tri->rightChild->leftNeighbor = baseNbr->leftChild;
			tri->leftChild->rightNeighbor = baseNbr->rightChild;
		}
		else {
			SplitTriNode (tri); 
		}
	}

	//
	void SubDivideRecurse (BiTriangleNode* tri, float* varnTree, float tolerance) {

		unsigned triInd = tri->triIndex; 
		// hardcode
		if (triInd < CLoDSharedData::indexDepth) {

			if (tri->leftChild && tri->rightChild) {
				SubDivideRecurse (tri->rightChild, varnTree, tolerance); 
				SubDivideRecurse (tri->leftChild, varnTree, tolerance); 
			}
			else if (varnTree[triInd] > tolerance) 
			{

				//Assert (tri->leftChild == 0, "leftChild not null"); 
				//Assert (tri->rightChild == 0, "rightchild not null"); 
				SplitTriNodeRecurs (tri);
				SubDivideRecurse (tri->rightChild, varnTree, tolerance); 
				SubDivideRecurse (tri->leftChild, varnTree, tolerance); 
			}
		}
		else {
			int dbgInt = 0; 
		}
	}

	//
	void SubDivideRecurse (BiTriangleNode* tri, int* varnTree, int tolerance) {

		unsigned triInd = tri->triIndex;
		// hardcode
		if (triInd < CLoDSharedData::indexDepth) {

			if (tri->leftChild && tri->rightChild) {
				SubDivideRecurse (tri->rightChild, varnTree, tolerance); 
				SubDivideRecurse (tri->leftChild, varnTree, tolerance); 
			}
			else if (varnTree[triInd] >= tolerance)
			{
				//Assert (tri->leftChild == 0, "leftChild not null"); 
				//Assert (tri->rightChild == 0, "rightchild not null"); 

				SplitTriNodeRecurs (tri);
				SubDivideRecurse (tri->rightChild, varnTree, tolerance); 
				SubDivideRecurse (tri->leftChild, varnTree, tolerance); 
			}
		}
		else {
			int dbgInt = 0; 
		}
	}

*/

//
// CLoD_terrain  CLoD_terrain  CLoD_terrain  CLoD_terrain 
void CLoD_terrain::Update_visible_area (class View_params* params)
{
}

void CLoD_terrain::Update_patch_topology ()
{
}

// variance subdivision
void CLoD_terrain::Tesselate_terrain ()
{
}

void CLoD_terrain::Build_index_buffer ()
{}

void CLoD_terrain::Render () {
}

}