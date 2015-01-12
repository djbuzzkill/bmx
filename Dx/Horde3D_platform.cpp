#include "Horde3D_platform.h"


//
// Load_geo 

// Conventions
//
// Some conventions for the API. Horde3D uses a right-handed coordinate system, 
// where y is the up-axis and the positive z-axis is pointing out of the screen.  
// The rotations are specified in degrees and rotation direction is 
// counter-clockwise when looking down the axis from the the positive end 
// towards the origin.  View vectors like the camera or light direction are 
// always pointing along the negative z-axis when no transformation is applied.  
// Matrices in Horde are stored in a column-major memory layout.  When Euler 
// angles are used, the rotation order is YXZ [*vec], so the z-rotation is applied first.

H3D::Geo& H3D::Load (Geo& geo, const std::string& fname)
{
   // http://www.ted.com/playlists/85/what_does_the_future_look_like.html
   //std::string fname = "C:/Quarantine/Hardcore/HordeIntgrTest/Content/test_level/test_terrain_00.geo";

   FILE* f = fopen (fname.c_str(), "rb"); 
   Assert (f, "no such files: %s", fname.c_str ()); 

   fread (&geo.magic, 1, 4, f); 
   fread (&geo.version, 1, 4, f); 

   fread (&geo.num_Joints, 1, 4, f); 
	   
   if (geo.num_Joints) geo.inv_Bind_Mats.resize (geo.num_Joints); 

   for (size_t i (0);  i != geo.num_Joints; i++)
	   fread (geo.inv_Bind_Mats[i].ptr (), 1, 16 * sizeof (float), f);

   fread (&geo.num_Vert_Streams, 1, 4, f); 
   fread (&geo.num_Verts, 1, 4, f); 

   for (int iStr = 0; iStr < geo.num_Vert_Streams; iStr++)
   {
	   int wat = 0; 
	   int magic_ID; 
	   int sizeOf_elem; 
	   fread (&magic_ID, 1, 4, f); 
	   fread (&sizeOf_elem, 1, 4, f); 

	   switch (magic_ID)
	   {
	   case 0: 
		   geo.vPos.resize (geo.num_Verts); 
		   for (int i = 0; i < geo.num_Verts; i++)
			   fread (geo.vPos[i].ptr(), 1, 3 * sizeof (float), f); 
		   break;

	   case 1: 
		   geo.vNrm.resize (geo.num_Verts); 
		   for (int i = 0; i < geo.num_Verts; i++)
			   fread (geo.vNrm[i].ptr(), 1, 3 * sizeof (short), f); 
		   break;
		
	   case 2: // tangents
		   geo.vTan.resize (geo.num_Verts); 
		   for (int i = 0; i < geo.num_Verts; i++)
			   fread (geo.vTan[i].ptr(), 1, 3 * sizeof (short), f); 
		   break;

	   case 3: // bi tan
		   geo.vBiTan.resize (geo.num_Verts); 
		   for (int i = 0; i < geo.num_Verts; i++)
			   fread (geo.vBiTan[i].ptr(), 1, 3 * sizeof (short), f); 
		   break;

	   case 4: // joint indices 
		   geo.vJointInds.resize (geo.num_Verts); 
		   for (int i = 0; i < geo.num_Verts; i++)
			   fread (geo.vJointInds[i].ptr(), 1, 4, f); 
		   break;

	   case 5: // joint weights
		   geo.vJointWgts.resize (geo.num_Verts); 
		   for (int i = 0; i < geo.num_Verts; i++)
			   fread (geo.vJointWgts[i].ptr(), 1, 4, f); 
		   break;

	   case 6: 
		   geo.vTxc0.resize (geo.num_Verts); 
		   for (int i = 0; i < geo.num_Verts; i++)
			   fread (geo.vTxc0[i].ptr(), 1, 2 * sizeof (float), f); 
		   break;
			
	   case 7: // tx crd 1
		   geo.vTxc1.resize (geo.num_Verts); 
		   for (int i = 0; i < geo.num_Verts; i++)
			   fread (geo.vTxc1[i].ptr(), 1, 2 * sizeof (float), f); 
		   break;
		   break;

	   }
   }

   // indices
   fread (&geo.num_Inds, 1, 4, f); 
   geo.vInds.resize (geo.num_Inds); 
   for (int i = 0; i < geo.num_Inds; i++)
	   fread (&geo.vInds[i], 1, sizeof (int), f); 

   // morph targets
   fread (&geo.num_Morph_Targets, 1, 4, f); 
   for (int i = 0; i < geo.num_Morph_Targets; i++)
   {

   }
   size_t pos_File = ftell (f); 
   fclose (f); 
   f = 0;
   return geo; 
}

//
// load animation 
H3D::Anim& H3D::Load (H3D::Anim& anim, const std::string& fname)
{
   std::shared_ptr<FILE> f (fopen (fname.c_str(), "rb"), fclose); 
   Assert (f, "no such files: %s", fname.c_str ()); 

   fread (&anim.magic, 1, 4, f.get()); 
   fread (&anim.version, 4, 1, f.get()); 

   fread (&anim.n_Anims, 4, 1, f.get()); 
   fread (&anim.n_Frames, 4, 1, f.get()); 


   anim.nodes.resize (anim.n_Anims); 
   for (int iAn= 0; iAn< anim.n_Anims; iAn++)
   {
      AnimNode& node = anim.nodes[iAn]; 

      fread (node.name, 1, 256, f.get()); 
      fread (&node.compressed, 1, 1, f.get()); 

      node.transform.resize (node.compressed ? 1 : anim.n_Frames); 
      for (int iFr = 0; iFr < node.transform.size(); iFr++)
      {
         fread (&node.transform[iFr].rot, 4, 4, f.get()); 
         fread (&node.transform[iFr].transl, 4, 3, f.get()); 
         fread (&node.transform[iFr].scale, 4, 3, f.get()); 
      }
   }      
 

   return anim; 
}

/**
 * MyRegisterClass
 */
void H3D::Obj::Set_pos (const Ma::Vec3f& p)
{
   Ma::Copy (pos, p); 
   update_xf();
}

void H3D::Obj::Set_rot (const Ma::Vec3f& r)
{
   Ma::Copy (rot, r); 
   update_xf();
}

void H3D::Obj::Set_scl (const Ma::Vec3f& s)
{
	Ma::Copy (scl, s); 
	update_xf();
}

H3D::Obj::Obj ()
	: node (0)
{}

H3D::Obj::Obj (H3DNode n)
	:node (n)
{
	if (node)
		Bind (node); 
}

//
void H3D::Obj::Bind (H3DNode n)
{
	node = n; 
	if (!node)
		return;

	h3dGetNodeTransform (
      node, 
		&Ma::X (pos), &Ma::Y (pos), &Ma::Z (pos), 
		&Ma::X (rot), &Ma::Y (rot), &Ma::Z (rot), 
		&Ma::X (scl), &Ma::Y (scl), &Ma::Z (scl));
}

//
//
Ma::Vec3f& H3D::Obj::Get_pos (Ma::Vec3f& p) 
{

   //h3dSetNode
   //h3dGetNodeTransMats (


	h3dGetNodeTransform (
      node, 
		&Ma::X (pos), &Ma::Y (pos), &Ma::Z (pos), 
		&Ma::X (rot), &Ma::Y (rot), &Ma::Z (rot), 
		&Ma::X (scl), &Ma::Y (scl), &Ma::Z (scl)
      );




	return Ma::Copy (p, this->pos);
} 

// IID1 for IS 1
// RESRC ENGRDA for {XHD|IXSHD|TRE_OVERFLOW[n]} m  
//
Ma::Vec3f& H3D::Obj::Get_rot (Ma::Vec3f& r) {

	h3dGetNodeTransform (node, 
		&Ma::X (pos), &Ma::Y (pos), &Ma::Z (pos), 
		&Ma::X (rot), &Ma::Y (rot), &Ma::Z (rot), 
		&Ma::X (scl), &Ma::Y (scl), &Ma::Z (scl));

	return Ma::Copy (r, this->rot);
	} 

//
//
Ma::Vec3f& H3D::Obj::Get_scl (Ma::Vec3f& s) {

	h3dGetNodeTransform (node, 
		&Ma::X (pos), &Ma::Y (pos), &Ma::Z (pos), 
		&Ma::X (rot), &Ma::Y (rot), &Ma::Z (rot), 
		&Ma::X (scl), &Ma::Y (scl), &Ma::Z (scl));

	return Ma::Copy (s, this->scl);
	} 

//
//
void H3D::Obj::update_xf () {

	if (!node)
		return;

	h3dSetNodeTransform (
		node, 
		Ma::X (pos), Ma::Y (pos), Ma::Z (pos), 
		Ma::X (rot), Ma::Y (rot), Ma::Z (rot), 
		Ma::X (scl), Ma::Y (scl), Ma::Z (scl)
		);
	}
