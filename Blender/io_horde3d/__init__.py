bl_info = {
    "name": "Export Horde 3D runtime",
    "author": "djbuzzkill",
    "blender": (2, 5, 7),
    "location": "File > Import-Export",
    "description": "Export Horde 3D, geom, scenes, anims ",
    "warning": "",
    "tracker_url": "",
    "support": "TESTING",
    "category": "Import-Export"
    }

#
#
import bpy
import H3D 
import Dx
import os


#
#
if "bpy" in locals ():
    import imp
    from bpy_extras.io_utils import ExportHelper
    from bpy.props import StringProperty, BoolProperty, EnumProperty
    from bpy.types import Operator

#
#
DEBUG_print = False
# NetExtender password
# L3m0nL!m3

#
# Build_compiled_from_scene - used in Operator.
def Build_geo_from_scene (scn): 
    comp_Geo = Dx.Make_geo ()

    for m in [x for x in scn.objects if x.type == "MESH"]: 
        Dx.Accum_geo (comp_Geo, Dx.Extract_geo (m), "material/" + scn.name + ".mtrl")

    [Dx.Accum_geo (comp_Geo, Dx.Extract_geo (x), "material/" + scn.name + ".mtrl") for x in scn.objects if x.type == "MESH"]

    for x in scn.objects:  
        if x.type == "ARMATURE": 
            comp_Geo['joints'] = Dx.Extract_skeleton (x)

    return comp_Geo

# 
# the new new function for new function for articulated models
def Build_Model_from_scene (exportr, scn): 


    # do we have armature
    a  = None
    for o in scn.objects: 
        if o.type == 'ARMATURE': 
            a = 0
            break

    # 
    # the meshes
    geos = [H3C.Extract_mesh (g, arma) for g in scn.objects if g.type == 'MESH']


    DBG_model_print = False
    if DBG_model_print and exportr: 
        f = open ( 'c:/usr/build_model_print.txt', 'w')
        f.write ("something\n")
        f.write (os.path.splitext(exportr.filepath)[0])
        f.write ("\n")
        f.write (os.path.splitext(exportr.filepath)[1])
        f.write ("\n")
        f.write (os.path.basename (exportr.filepath))
        f.write ("\n")
        f.write (os.path.dirname (exportr.filepath))
        f.write ("\nend")
        f.close ()
        # raise AssertionError  (DBG_model_print)

    H3D.Write_geo (exportr.filepath, geos, a)
    H3D.Write_scene (exportr.filepath + ".scene", scn.name, "model/" + os.path.basename (exportr.filepath), geos, a)

    return geos, a


#
#
# Horde3d Export Operator
class Horde3DExportOp (Operator, ExportHelper):

    '''This appears in the tooltip of the operator and in the generated docs'''
    # important since its how bpy.ops.import_test.some_data is constructed

    # "just dont change this"
    bl_idname = "export_test.some_data"


    bl_label = "Export Some Data"

    # ExportHelper mixin class uses this
    filename_ext = ".geo" 

    #
    # what the menu option runs when selected
    def execute (self, context):

        if (DEBUG_print): 
            f = open ( 'c:/usr/test.txt', 'w')
            f.write ("something\n")
            f.write (os.path.splitext(self.filepath)[0])
            f.write ("\n")
            f.write (os.path.splitext(self.filepath)[1])
            f.write ("\n")
            f.write (os.path.basename (self.filepath))
            f.write ("\n")
            f.write (os.path.dirname (self.filepath))
            f.write ("\nend")
            f.close ()
            # end dbg

        # when we get serious, turn this on
        DBG_current_export_method  = False
        
        if (DBG_current_export_method): 
            # collect all geos into one compiled
            comp_Geo = Build_geo_from_scene (context.scene)
            # write geo
            Dx.Write_geo_data (self.filepath, comp_Geo)
            # write .scene
            fname = self.filepath + ".scene"
            cxsn = context.scene.name
            geo_file =  "model/" + os.path.basename (self.filepath)
            Dx.Write_model_graph (fname, cxsn, geo_file , comp_Geo)
        else: 
            # replace the above
            Build_Model_from_scene (self, context.scene)
        
        # thats what Operators return
        return {'FINSHED'}

#
#
TEST_save_path = "c:/usr/dump/"

def Write_edit_bones (fname, arma): 
    f = open (TEST_save_path + fname, "wb")

    for b in arma.edit_bones: 
        Write_edit_bone (f, b)

    f.close ()

#
#                    
def Write_edit_bone (f, b): 
    Dx.write_string_pad (f, b.name, 64)
    #
    if (b.parent): 
        Dx.write_string_pad (f, b.parent.name, 64)
    else: 
        Dx.write_string_pad (f, 'nada', 64)
        
    # roll
    Dx.write_f32 (f, b.roll)

    # head
    Dx.write_f32 (f, b.head[0])
    Dx.write_f32 (f, b.head[1])
    Dx.write_f32 (f, b.head[2])

    #
    Dx.write_f32 (f, b.x_axis[0])
    Dx.write_f32 (f, b.x_axis[1])
    Dx.write_f32 (f, b.x_axis[2])
    #
    Dx.write_f32 (f, b.y_axis[0])
    Dx.write_f32 (f, b.y_axis[1])
    Dx.write_f32 (f, b.y_axis[2])
    #
    Dx.write_f32 (f, b.z_axis[0])
    Dx.write_f32 (f, b.z_axis[1])
    Dx.write_f32 (f, b.z_axis[2])
    # matrix
    Dx.write_f32  (f, b.matrix.row[0][0])
    Dx.write_f32  (f, b.matrix.row[0][1])
    Dx.write_f32  (f, b.matrix.row[0][2])
    Dx.write_f32  (f, b.matrix.row[0][3])

    Dx.write_f32  (f, b.matrix.row[1][0])
    Dx.write_f32  (f, b.matrix.row[1][1])
    Dx.write_f32  (f, b.matrix.row[1][2])
    Dx.write_f32  (f, b.matrix.row[1][3])

    Dx.write_f32  (f, b.matrix.row[2][0])
    Dx.write_f32  (f, b.matrix.row[2][1])
    Dx.write_f32  (f, b.matrix.row[2][2])
    Dx.write_f32  (f, b.matrix.row[2][3])

    Dx.write_f32  (f, b.matrix.row[3][0])
    Dx.write_f32  (f, b.matrix.row[3][1])
    Dx.write_f32  (f, b.matrix.row[3][2])
    Dx.write_f32  (f, b.matrix.row[3][3])




#
#

def Write_bones (fname, arma): 
    f = open (TEST_save_path + fname, "wb")
    for b in arma.bones: 
        Write_bone (f, b)
    f.close ()


#
#
def Write_bone (f, bon): 

    Dx.write_string_pad (f, bon.name, 64)
    # fwrite (filler)

    if (bon.parent): 
        Dx.write_string_pad (f, bon.parent.name, 64)
    else: 
        Dx.write_string_pad (f, 'nada', 64)
    # fwrite (filler) 


    # head pos
    Dx.write_f32  (f, bon.head[0])
    Dx.write_f32  (f, bon.head[1])
    Dx.write_f32  (f, bon.head[2])
    # head arma space
    Dx.write_f32  (f, bon.head_local[0])
    Dx.write_f32  (f, bon.head_local[1])
    Dx.write_f32  (f, bon.head_local[2])


    # head pos
    Dx.write_f32  (f, bon.tail[0])
    Dx.write_f32  (f, bon.tail[1])
    Dx.write_f32  (f, bon.tail[2])
    # head arma space
    Dx.write_f32  (f, bon.tail_local[0])
    Dx.write_f32  (f, bon.tail_local[1])
    Dx.write_f32  (f, bon.tail_local[2])


    # ?? 
    Dx.write_f32  (f, bon.x_axis[0])
    Dx.write_f32  (f, bon.x_axis[1])
    Dx.write_f32  (f, bon.x_axis[2])

    Dx.write_f32  (f, bon.y_axis[0])
    Dx.write_f32  (f, bon.y_axis[1])
    Dx.write_f32  (f, bon.y_axis[2])

    Dx.write_f32  (f, bon.z_axis[0])
    Dx.write_f32  (f, bon.z_axis[1])
    Dx.write_f32  (f, bon.z_axis[2])

    # ??
    Dx.write_f32  (f, bon.matrix.row[0][0])
    Dx.write_f32  (f, bon.matrix.row[0][1])
    Dx.write_f32  (f, bon.matrix.row[0][2])
    Dx.write_f32  (f, bon.matrix.row[1][0])
    Dx.write_f32  (f, bon.matrix.row[1][1])
    Dx.write_f32  (f, bon.matrix.row[1][2])
    Dx.write_f32  (f, bon.matrix.row[2][0])
    Dx.write_f32  (f, bon.matrix.row[2][1])
    Dx.write_f32  (f, bon.matrix.row[2][2])

    # ??
    Dx.write_f32  (f, bon.matrix_local.row[0][0])
    Dx.write_f32  (f, bon.matrix_local.row[0][1])
    Dx.write_f32  (f, bon.matrix_local.row[0][2])
    Dx.write_f32  (f, bon.matrix_local.row[0][3])

    Dx.write_f32  (f, bon.matrix_local.row[1][0])
    Dx.write_f32  (f, bon.matrix_local.row[1][1])
    Dx.write_f32  (f, bon.matrix_local.row[1][2])
    Dx.write_f32  (f, bon.matrix_local.row[1][3])

    Dx.write_f32  (f, bon.matrix_local.row[2][0])
    Dx.write_f32  (f, bon.matrix_local.row[2][1])
    Dx.write_f32  (f, bon.matrix_local.row[2][2])
    Dx.write_f32  (f, bon.matrix_local.row[2][3])
 
    Dx.write_f32  (f, bon.matrix_local.row[3][0])
    Dx.write_f32  (f, bon.matrix_local.row[3][1])
    Dx.write_f32  (f, bon.matrix_local.row[3][2])
    Dx.write_f32  (f, bon.matrix_local.row[3][3])


#
#
# Only needed if you want to add into a dynamic menu
def menu_horde3d_export (self, context):
    self.layout.operator(Horde3DExportOp.bl_idname, text="Export Horde3D runtime (.geo, .xml)")

def register ():
    bpy.utils.register_class (Horde3DExportOp)
    bpy.types.INFO_MT_file_export.append(menu_horde3d_export)

def unregister ():
    bpy.utils.unregister_class (Horde3DExportOp)
    bpy.types.INFO_MT_file_export.remove(menu_horde3d_export)

# 
# 
if __name__ == "__main__":
    register() 
