bl_info = {
    "name": "Dx Standalone",
    "author": "djbuzzkill",
    "blender": (2, 5, 7),
    "location": "File > Import-Export",
    "description": "E9 , geom, scenes, anims ",
    "warning": "",
    "tracker_url": "",
    "support": "TESTING",
    "category": "Import-Export"
    }

#
# imports
import bpy
import struct 
# import H3D
import array
import mathutils
import os
from xml.etree.ElementTree import * 



#
# constants 
MgID_Position = 0
MgID_Normal = 1
MgID_Tangent = 2
MgID_Bitangent = 3
MgID_JointIndices = 4
MgID_JointWeights = 5
MgID_TexCoord0 = 6
MgID_TexCoord1 = 7

#
# Dx lib - # general stuff ok, blender stuff ok.
#

# !! Extract_skeleton () !!
# .scene TODO, write joint hierarchy, zero rotations, translations relative to parent, assign joint indices to each bone/joint
# 
#

#
# .geo TODO, write out inverse bind matrices, only store relative pos to parent, identity rotations
#
#

RD_UCHAR = 1
RD_VEC2UC = 2
RD_VEC3UC = 3
RD_VEC4UC = 4

RD_CHAR = 5
RD_VEC2C = 6
RD_VEC3C = 7
RD_VEC4C = 8

RD_SHORT = 9
RD_VEC2S = 10
RD_VEC3S =11
RD_VEC4S = 12

RD_USHORT = 13
RD_VEC2US = 14
RD_VEC3US = 15
RD_VEC4US = 16

RD_FLOAT = 17
RD_VEC2F = 18
RD_VEC3F = 19
RD_VEC4F = 20

RD_DOUBLE = 21
RD_VEC2D = 22
RD_VEC3D = 23
RD_VEC4D = 24

RD_INT = 25
RD_VEC2I = 26
RD_VEC3I = 27
RD_VEC4I = 28

RD_UINT = 29
RD_VEC2UI = 30
RD_VEC3UI = 31
RD_VEC4UI = 32


#
if "bpy" in locals():
    import imp

SIZEOF_LABEL = 128
SizeOf_nodeName = 256

def wtf (): 
    return 'lolz'


#
Ob  =  type ('Ob', (object,), {})

#
# phat tracks
# Shakedown - At Night
#
# EA7FECE5 
#

#
# write primitive data
def write_u8 (file, val):
    file.write (struct.pack ("<B", val))

#
def write_u16 (file, val):
    file.write (struct.pack ( "<H", val))

#
def write_s16 (file, val):
    file.write (struct.pack ( "<h", val))

#
def write_s32 (file, val):
    file.write (struct.pack ( "<i", val))

# 
def write_u32 (file, val):
    file.write (struct.pack ( "<I", val))

#
def write_f32 (file, val):
    file.write (struct.pack ( "<f", val))

#
def write_mat44 (f, m): 
    #
    elem_count = 0
    for r in m.row: 
        for e in r: 
            write_f32 (f, e)
            elem_count = 1 + elem_count

    return 4 * elem_count



#
def write_char_array   (f, s, sz):
    strlen = len (s)
    ## 
    assert (strlen <= sz)
    for i in range (sz):
        if i < strlen:
            f.write (struct.pack ("<c", s[i:i+1]))
        else:
            f.write (struct.pack ( "<c", chr (0)))
    
    return sz

#
#
def write_byte_array (f, ba): 
    for i in range (len (ba)): 
        f.write (struct.pack ("<c", ba[i:i+1] ))


def write_string_pad (f, s, sz): 
    ba = bytes (s, 'utf-8')
    len_Str = len (ba)
    len_Pad = sz - len_Str

    f.write (ba)

    for i in range (len_Pad): 
        f.write (b'\0')


# 
# a tag was our short string data text 
def write_label (f, val):
    strlen = len (val)
    assert (strlen < SIZEOF_LABEL)
    for i in range(SIZEOF_LABEL):
        if i < strlen:
            f.write (struct.pack ("<c", val[i]))
        else:
            f.write (struct.pack ( "<c", chr (0)))
    
    return SIZEOF_LABEL

# 
#
def write_tag (file, val):
    strlen = len (val)
    assert (strlen < SZ_TAG)
    for i in range(SZ_TAG):
        if i < strlen and i < SZ_TAG:
            file.write (struct.pack ("<c", val[i]))
        else:
            file.write (struct.pack ( "<c", chr (0)))
    return SZ_TAG


#
# write_color_3b
def write_color_3b (file, col):
    file.write (struct.pack ("<B", col[0]))
    file.write (struct.pack ("<B", col[1]))
    file.write (struct.pack ("<B", col[2]))
    
#
# write_txco_2f
def write_txco_2f  (file, uv):
    file.write (struct.pack ( "<f", uv[0]))
    file.write (struct.pack ( "<f", uv[1]))


# 
# write_saveendian
def write_saveendian (file):
    endianid = 0
    endianid |=  (ord ("6") << 24)
    endianid |=  (ord ("9") << 16)
    endianid |=  (ord ("6") << 8)
    endianid |=  ord ("9")
    file.write (struct.pack ( "<I", endianid))
    return SZ_U32




 
#
# WTF is a DX_bone?? 
class DX_bone (): 
    def __init__ (self, name_, parent_ ): 
        # basic stuff
        self.name = name_
        self.parent = parent_
        self.children = []
        # relative to parent
        self.rel_Mat = mathutils.Matrix () 
        self.rel_Pos = mathutils.Vector ()


#
# 
def Compute_Dx_bone (Dx_bon, Bl_bon): 
    #
    return 'something'


#
#
def Build_tree_recurse (cur_Node, Bl_node): 

    Compute_Dx_bone  (cur_Node, Bl_node)

    for child in Bl_node: 
        new_Child = Dx_bone (child.name, cur_Node)
        cur_Node.append (new_Child)
        Build_tree_recurse (new_Child, child)

    return cur_Node

#
#
def Build_bone_hierarchy (arma, obj): 
    return Build_tree_recurse  (Dx_bone ('root', None),  arma.bones['root'])


#
# Blender Data
#
# Armature -> Object -> map<vertGroupIndex, boneIndex>
def Build_VGrpInd_2_BoneInd  (arma, obj): 
    #
    # from the Armature
    boneIndexMap = {}
    for i, b in enumerate (arma.bones): 
        boneIndexMap[ b.name ] = i

    # from the Object 
    vertGroupIndexMap = {}
    for g in obj.vertex_groups: 
        vertGroupIndexMap[g.name] = g.index

    # map vg inds to joint inds
    VGroupIndex_2_JointIndex = {}
    for b in arma.bones: 
        VGroupIndex_2_JointIndex[ vertGroupIndexMap[b.name] ] = boneIndexMap[b.name]

    return VGroupIndex_2_JointIndex
 

#
# armature ->  map< bone name, index >
def Gen_BoneInd_table (arma): 
    #
    # from the Armature
    boneIndexMap = {}
    for i, b in enumerate (arma.bones): 
        boneIndexMap[ b.name ] = i
        
    return boneIndexMap


#
# 
class H3D_Joint (): 

    def __init__ (self, bl_joint, i): 
        # x_axis, y_axis, z_axis, matrix_local, vector, 
        self.name = bl_joint.name
        self.index = i
        self.children = []
        self.pos_loc = [bl_joint.matrix_local[0][3], bl_joint.matrix_local[1][3], bl_joint.matrix_local[2][3]]
        self.parent = None

#
#
def Print_skeleton_bones (skel): 
   
    print ( (skel.name, skel.index, skel.pos_loc) )
    
    for ch in skel.children: 
        Print_skeleton_bones (ch)


#
# Assign_parent () - assign parent relation 
def Assign_parent (root_bone): 
    
    for ch in root_bone.children: 
        ch.parent = root_bone

    for ch in root_bone.children: 
        Assign_parent (ch)

    return root_bone


######

#
def map_bone ( out, f, n ):
    
    f ( out , n )

    for ch in n.children: 
        f (out, f, ch)

    return n


# skel is a tree
# arma -> skel
def Extract_skeleton_ (arma_Obj): 
    # skeleton will be used to write joints in the  .scene
    arma = arma_Obj.data

    bone_Joint_table = {}

    joint_ind_counter = 0

    #create the skeleton tree
    for b in arma.bones: 
        bone_Joint_table[b] = H3D_Joint (b, joint_ind_counter)
        joint_ind_counter = joint_ind_counter + 1

    # gon' b a return value
    skel = None
    for k, v in bone_Joint_table.items (): 
        if k.parent: 
            bone_Joint_table[k.parent].children.append (v)
        else: 
            skel = v

    return Assign_parent (skel)



#
def make_bone_map (m, bones): 
    
    for i, v in enumerate (bones): 
        m[i] = v

    return m

# 
# Relative_pos () - position relative to parent
def bone_relative_pos (b): 

    if b.parent: 
        p = b.parent
        return mathutils.Vector ( [b.head_local[i] - p.head_local[i] for i in range (3)] )
    else: 
        return mathutils.Vector ( [b.head_local[i] for i in range(3)] )

# Deprecated superceded by Extract_Mesh
# Extract_geo_data :: obj -> geo
# 
def Extract_geo (obj): 

    # blender axes come in, geo contains gl axes 
    # blender axes get switched to opengl axes here 
    # generate tris from polygons
    
    # mesh
    m = obj.data
    loc = obj.location
    #obj_Name = obj.name

    indices = []

    # turn n-polygons into triangles
    for p in m.polygons: 
        poly_len =  len (p.vertices) 
        if poly_len == 3: 
            indices.extend (p.vertices)  
        elif  poly_len  == 4:
            indices.extend (p.vertices[0:3])
            indices.extend ([p.vertices[i] for i in [2, 3, 0]])
        elif poly_len == 5:
            indices.extend (p.vertices[0:3])
            indices.extend ([p.vertices[i] for i in [0, 2, 3]])
            indices.extend ([p.vertices[i] for i in [0, 3, 4]])
        else: 
            raise AssertionError  (obj.name +" unhandled polygon size: " + str (poly_len))                     

    #
    # articulated model ? collect bone_Influences for bone_Inds, bone_Weights
    bone_Inds =  []
    bone_Weights =  []
    # look for armture 
    # we will only use parented mesh approach for articulated models 
    if obj.parent and obj.parent.type == 'ARMATURE': 
        bone_Influences = []
        # this arma has: vertex_groups & data (Armature)
        arma = obj.parent.data
        bone_Map =  Gen_BoneInd_table (arma) 
        vert_groups = obj.vertex_groups

        for v in m.vertices: 
            infl = []

            for vge in v.groups: 
                # iterate over all vge's for this vertex
                infl.append ( ( vge.weight , bone_Map [ vert_groups[vge.group].name ] ) )
                # per vge 

            bone_Influences.append (infl)
            # per vert 


        # bone_Inds 
        # bone_Weights 
        for infl in bone_Influences: 
            len_Infl = len (infl)
            if len_Infl == 1: 
                bone_Inds.append ( (infl[0][1],  0, 0, 0) )
                bone_Weights.append ( (infl[0][0],  0.0, 0.0,  0.0) )

            elif len_Infl == 2: 
                bone_Inds.append ( (infl[0][1],  infl[1][1], 0, 0) )
                bone_Weights.append ( (infl[0][0],  infl[1][0], 0.0, 0.0) )

            elif len_Infl == 3: 
                bone_Inds.append ( (infl[0][1],  infl[1][1], infl[2][1], 0) )
                bone_Weights.append ( (infl[0][0],  infl[1][0], infl[2][0], 0.0) )

            elif len_Infl == 4: 
                bone_Inds.append ( (infl[0][1],  infl[1][1], infl[2][1], infl[3][1]) )
                bone_Weights.append ( (infl[0][0],  infl[1][0], infl[2][0], infl[3][0]) )

            else: 
                bone_Inds.append ( (0, 0, 0, 0) )
                bone_Weights.append ( (0.0, 0.0, 0.0, 0.0) )

        # if parent == armature



    # generate tangent
    #

    # generate bitangent
    #

    #
    # generate txcoords from uv's 
    texcrd0 = [mathutils.Vector () for x in range (len(m.vertices))]
    uvcoords = m.uv_layers[0].data
    uv_ind = 0
    for p in m.polygons: 
        # i is vetex index
        for i in p.vertices: 
            # succesive uv is written into vertex, with over writes
            texcrd0[i] = uvcoords[uv_ind].uv
            # incr 
            uv_ind = uv_ind + 1

    #
    # not really flipped verts (fl), but 'rotated' to match opengl coord system
    fl_verts = [(v.co[1], v.co[2], v.co[0]) for v in m.vertices]
    fl_norms = [(v.normal[1], v.normal[2], v.normal[0]) for v in m.vertices]
    # generate 2nd uv set
        
    return  {
        'name' : obj.name, 
        'material' : 'dumb.mtrl', 
        'triindices' : indices, 
        'positions' : fl_verts, # [v.co for v in m.vertices], 
        'normals' :  fl_norms, # [v.normal for v in m.vertices], 
        'texcoord0' : texcrd0, 
        'translation' : (loc[1], loc[2], loc[0]), 
        'bone_Inds' : bone_Inds, 
        'bone_Weights' : bone_Weights, 
        'tangents' : [],
        'bi_Normals' : [], 
        'bi_Tanents' : []
        }



#
# Mesh_params
#class Mesh_params (): 
#
#    def __init__ (self, n, s, c, vS, vE, m, pos, rot, scl): 
#        self.name = n
#        self.batchStart = s
#        self.batchCount = c
#        self.vertRStart  = vS
#        self.vertREnd = vE
#        self.mtrlPath = m
#        self.vPos = pos
#        self.vRot = rot
#        self.vScl = scl

#
# 
def Accum_geo (acc_Geo,  geo, mtrl_Path = None): 
    # batchStart   first vertex index in geometry {required}
    # batchCount number of vertex indices in geometry {required}
    # vertRStart   minimum vertex array index contained in indices of geometry  {required}
    # vertREnd	  maximum vertex array index contained in indices of geometry  {required}

    # indices[batchStart],  indices[batchStart+batchCount]
    # verts[vertRStart], verts[vertREnd]
    if (mtrl_Path):
        use_mtrl = mtrl_Path
    else:
        use_mtrl = "material/" + geo['name'] + ".mtrl"

    if 'mesh_List' not in acc_Geo: 
        acc_Geo['mesh_List'] = []
    
    # how many verts currently in acc_Geo, this better be the same for all vert attributes
    sizeOf_Verts_acc = len (acc_Geo['positions'])
    # new_Inds reflects new vert loc in new compiled vert array
    new_Inds = [x + sizeOf_Verts_acc for x in geo['triindices']]
    batch_Start =  len (acc_Geo['triindices'])
    batch_Count = len (new_Inds)
    vert_RStart = len (acc_Geo['positions'])
    vert_REnd = vert_RStart + len (geo['positions']) - 1
    
    # update acc_Geo
    acc_Geo['triindices'].extend (new_Inds)
    acc_Geo['positions'].extend (geo['positions'])
    acc_Geo['normals'].extend (geo['normals'])
    acc_Geo['texcoord0'].extend (geo['texcoord0'])

    # acc_Geo['mesh_List'].append (  (geo['name'], batch_Start, batch_Count,  vert_RStart, vert_REnd, use_mtrl) )
    acc_Geo['mesh_List'].append (
        Mesh_params (
            geo['name'], 
            batch_Start, 
            batch_Count,  
            vert_RStart, 
            vert_REnd, 
            use_mtrl, 
            geo['translation']
            )
        )
    
    return acc_Geo, True


#
# Write_geo_data - superceded by H3D.Write_geo (); 
# write .geo file for a compiled geometry 
def Write_geo_data (fname, geo): 
 
    f = open (fname, "wb")

    # header
    H3D.Write_H3DG_v5 (f)
    
    nada = "nada"

    # joints
    num_Joints = 0
    write_u32 (f, num_Joints)

    # geo['material']
    # geo['name']

    # num_VertAttribs 
    num_VertAttribs = 0

    if len (geo['positions']) > 0: 
        num_VertAttribs = 1 + num_VertAttribs

    if len( geo['normals']) > 0: 
        num_VertAttribs = 1 + num_VertAttribs 

    if len (geo['texcoord0']) > 0: 
        num_VertAttribs = 1 + num_VertAttribs

    if len (geo['bone_Inds']) > 0: 
        num_VertAttribs = 1 + num_VertAttribs

    if len (geo['bone_Weights']) > 0: 
        num_VertAttribs = 1 + num_VertAttribs

    if len (geo['tangents']) > 0: 
        num_VertAttribs = 1 + num_VertAttribs

    if len (geo['bi_Normals']) > 0: 
        num_VertAttribs = 1 + num_VertAttribs

    if len (geo['bi_Tangents']) > 0: 
        num_VertAttribs = 1 + num_VertAttribs

    # numVertexStreams
    write_s32 (f, num_VertAttribs)
    # numVertices - all should be same as pos's
    write_s32 (f, len (geo['positions']))

    #
    # write_s32 (f, Bitangent_magic_ID)
    # write_s32 (f, JointIndices_magic_ID)
    # write_s32 (f, JointWeights_magic_ID)
    # write_s32 (f, TexCoord1_magic_ID)

    # pos
    if len (geo['positions']) > 0: 
        write_u32 (f, Position_magic_ID)
        write_u32 (f, 12) # 3 * sizeof(float)
        for x in  geo['positions']: 
            write_f32 (f, x[0])
            write_f32 (f, x[1])
            write_f32 (f, x[2])

    # norms
    if len(geo['normals']) > 0: 
        write_u32 (f, Normal_magic_ID)
        write_u32 (f, 6) # 3 * sizeof(short)
        for x in  geo['normals']: 
            write_s16 (f,  int (x[0] * 32767))
            write_s16 (f,  int (x[1] * 32767))
            write_s16 (f,  int (x[2] * 32767))

    # tangents
    # write_u32 (f, Tangent_magic_ID)
    
    # bitangents

    # tx crd 0
    # write_u32 (f, TexCoord0_magic_ID)
    if len (geo['texcoord0']) > 0: 
        write_u32 (f, TexCoord0_magic_ID)
        write_u32 (f, 8) # 2 * sizeof(float)
        for x in geo['texcoord0']: 
            write_f32 (f, x[0])
            write_f32 (f, x[1])


    # indices
    write_u32 (f, len (geo['triindices']))

    for i in geo['triindices']: 
        write_s32 (f, i)

    # morph targets
    num_MrphTargets = 0
    write_s32 (f, num_MorphTargets)
    
    # end Write_geo_data
    f.close ()



#
#  XML_node - a simple node for H3D XML files
class XML_node (): 
    def __init__ (self, tag): 
        self.typeName  = tag
        self.attributes = []
        self.children = [] 

      
# Deprecated
# Build_XML_graph - build a H3D XML <Model />  description for a .scene file
def Build_XML_graph (mod_Name, geo_File, geo): 
    model = XML_node ('Model')
    model.attributes.extend  ([("name", mod_Name) , ("geometry", geo_File)])
    mesh_List = geo['mesh_List']

    skel_Root = None
    if  'joints' in geo: 
        # skel_Root = 
        pass 

    for m in geo['mesh_List']: 
        child = XML_node ('Mesh')
        child.attributes.extend ( 
            [
                ('name', m.name) , 
                ('material', m.mtrlPath) , 
                ('batchStart', str (m.batchStart)), 
                ('batchCount', str(m.batchCount)), 
                ('vertRStart', str (m.vertRStart)), 
                ('vertREnd', str (m.vertREnd)), 
                ('tx', str (m.vPos[0])), 
                ('ty', str (m.vPos[1])), 
                ('tz', str (m.vPos[2]))
                ] 

#                ('name', m[0]) , 
#              ('material', m[5]) , 
#                ('batchStart', str (m[1])), 
#                ('batchCount', str(m[2])), 
#                ('vertRStart', str (m[3])), 
#                ('vertREnd', str (m[4])) 
#                ] 
            )
        model.children.append  (child)

    return model



#
# Mesh - actually more of a simple mesh (sub_geo?)
# this is like a 'sub geo' ?  dont know yet, figure it out.
# the data this contains goes into a geo and a mesh desc.
class Mesh () : 
    '''I am Mesh'''
    # Mesh me
    def __init__ (self, nm, m, i, p, n, t, bT, t0, t1, bI, bW, vgm, loc): 
        'construct  a geo instance'
        self.name = nm
        self.material = m
        self.triindices = i
        self.positions = p
        self.normals = n
        self.tangents = t
        self.bi_Tangents = t
        self.texcoord0 = t0
        self.texcoord1 = t1
        self.bone_Inds = bI
        self.bone_Weights = bW
        self.vg_map = vgm
        self.location = loc



#
#
def Extract_mesh (obj, arma): 
    # blender axes come in, geo contains gl axes 
    # blender axes get switched to opengl axes here 
    # generate tris from polygons
    
    # mesh
    m = obj.data
    loc = obj.location

    #obj_Name = obj.name
    vindices = []
    # turn n-polygons into triangles
    # meshes should be already triangulated
    for p in m.polygons: 
        poly_len =  len (p.vertices) 
        if poly_len  == 3: 
            vindices.extend (p.vertices)  
        elif  poly_len  == 4:
            vindices.extend (p.vertices[0:3])
            vindices.extend ([p.vertices[i] for i in [2, 3, 0]])
        elif poly_len == 5:
            vindices.extend (p.vertices[0:3])
            vindices.extend ([p.vertices[i] for i in [0, 2, 3]])
            vindices.extend ([p.vertices[i] for i in [0, 3, 4]])
        else: 
            raise AssertionError  (obj.name +" unhandled polygon size: " + str (poly_len))                     
    # ! each bone has an associated vertex group of the same name
    # 
    # ! mesh object  has a list vertex groups any of which every vertex may  belong to 
    # each vertex will reference its index into this vertgroup list
    # 
    # ! we build a hierarchic bone list with different indces <-- we do?
    # 
    # ? are the vert group order same as the bone index IDs? no.
    #
    # ! we use our index order but the verts reference the vert group index order
    # 
    # ! we map vg-ID to our-ID  -> map<vg-ID, our-ID>
    # 
    # ! the bone order is  different in the one we produce with ml_depth
    #
    # ! 
    #
    #
    #
    #
    # articulated model ? collect bone_Influences for bone_Inds, bone_Weights
    bone_Inds =  []
    bone_Weights =  []

    # look for armature 
    # we will only use Blender 'parented mesh' approach for articulated models 
    vgmap = {}
    if arma: 
        # 
        print ( '\nhave skeleton')

        for i, vg in enumerate (obj.vertex_groups): 
            vgmap[i] = vg.name


        
        # 07/07/2013 - this seems to match the indices of the vert groups 
        # but we dont know is will always be correct
        for v in m.vertices: 
            # ! we have to normalize the weights 
            wgts_ = [vge.weight for vge in v.groups]
            inds_  =  [vge.group for vge in v.groups]

            while len(wgts_) < 4: 
                wgts_.append (0.0)
                
            while len (inds_) < 4:
                inds_.append (0)

            binds = inds_
            bwgts = mathutils.Vector (wgts_)

            bone_Inds.append ( (binds[0], binds[1], binds[2], binds[3]) )

            wgtsum = sum (bwgts)
            bone_Weights.append ( (bwgts[0]/wgtsum, bwgts[1]/wgtsum, bwgts[2]/wgtsum, bwgts[3]/wgtsum) )

    #
    # generate tangent

    #
    # generate bitangent

    #
    # generate txcoords from uv's 
    texcrd0 = [mathutils.Vector () for x in range (len(m.vertices))]
    uvcoords = m.uv_layers[0].data
    uv_ind = 0
    for p in m.polygons: 
        # i is vetex index
        for i in p.vertices: 
            # succesive uv is written into vertex, with over writes
            texcrd0[i] = uvcoords[uv_ind].uv
            # incr 
            uv_ind = uv_ind + 1

    #
    # not really flipped verts (fl), but 'shifted' to match opengl coord system
    fl_verts = [(v.co[1], v.co[2], v.co[0]) for v in m.vertices]
    fl_norms = [(v.normal[1], v.normal[2], v.normal[0]) for v in m.vertices]
    # generate 2nd uv set

    return Mesh (
        obj.name, 
        'dumb.mtrl', 
        vindices, 
        fl_verts, # [v.co for v in m.vertices], 
        fl_norms, # [v.normal for v in m.vertices], 
        None,  # 'tangents' : [],
        None, # 'bi_Tanents' : [], 
        texcrd0, 
        None, 
        bone_Inds, 
        bone_Weights, 
        vgmap, 
        (loc[1], loc[2], loc[0]))        


#
#

#        self.triindices = i
#      self.positions = p
#     self.normals = n
#    self.tangents = t
#   self.bi_Tangents = t
#  self.texcoord0 = t0
# self.texcoord1 = t1
#  self.bone_Inds = bI
#  self.bone_Weights = bW
# self.vg_map = vgm
#  self.location = loc

def Write_mesh_DX_res (m, fname): 
    f = open (fname, 'wb')

    print ("\n...saving", m.name, "out to: ", fname)
    print ("label length is 32")

    if (f): 
        write_string_pad (f, m.name, 32)

        #num fields
        write_u32 (f, 4)

        write_string_pad (f, "indices", 32) 
        write_u32 (f, RD_USHORT)
        write_u32 (f, len (m.triindices))
        for e in m.triindices: 
            write_u16 (f, e)
            
        write_string_pad (f, "posi", 32)
        write_u32 (f, RD_VEC3F)
        write_u32 (f, len (m.positions))
        for e in m.positions: 
            write_f32 (f, e[1])
            write_f32 (f, e[2])
            write_f32 (f, e[0])

        write_string_pad (f, "norms", 32)
        write_u32 (f, RD_VEC3F)
        write_u32 (f, len (m.normals))
        for e in m.normals: 
            write_f32(f, e[1])
            write_f32(f, e[2])
            write_f32(f, e[0])

        write_string_pad (f, "txcd0", 32)
        write_u32 (f, RD_VEC2F)
        write_u32 (f, len (m.texcoord0))
        for e in m.texcoord0: 
            write_f32 (f, e[0])
            write_f32 (f, e[1])
    else: 
        print ("Opening", fname, "failed")
        pass



#
# Write_XML_node
def Write_XML_node (f, node, lvl): 

    for i in range (lvl): 
        f.write ("  ")

    f.write ( "<" + node.typeName)

    for att in node.attributes: 
        f.write (" " + att[0] + " = \"" + att[1] + "\"")
    
    if len (node.children) > 0: 
        f.write (">\n")

        for ch in node.children: 
            Write_XML_node (f, ch, lvl + 1)

        for i in range (lvl): 
            f.write ("  ")

        f.write ("</" + node.typeName + ">\n")

    else: 
        f.write ( " />\n")



#
# Write_model_graph
def Write_model_graph (fname, mod_Name,  geo_File, geo): 
    model = Build_XML_graph (mod_Name, geo_File, geo)
    f = open (fname, 'w')
    Write_XML_node (f, model, 0)
    f.close ()


# 
def accum_node (out, n): 
    # only this function treats out as list
    out.append (n)

#  
def bone_name_index  (m, n): 
    #
    m[n.name] = n.index

# add self to map by its index
def bone_index_ob (out, n): 
    out [ n.index ] = n 


#
#
def map_dp  (out, f, n): 

    f (out, n )

    for ch in n.children: 
        map_dp  (out, f, ch)

    return out

#
# 
def register () : 
    pass 


def unregister (): 
    pass

# 
# main
if __name__ == "__main__":
    register ()
