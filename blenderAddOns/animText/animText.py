
bl_info = {
    "name": "AnimText",
    "author": "Antonios Sotiriou",
    "version": (1, 0),
    "blender": (3, 3, 0),
    "location": "File > Export > AnimText",
    "description": "Writes location, rotation_quaternion, scale, to a text file.",
    "warning": "",
    "doc_url": "",
    "category": "Animation",
}

import bpy
import time

def recursiveTransformation(arm, bone, mat):
    local = arm.data.bones[bone.name].matrix_local
    basis = arm.pose.bones[bone.name].matrix_basis

    parent = arm.data.bones[bone.name].parent
    if parent == None:
        mat = local @ basis
    else:
        parent_local = arm.data.bones[parent.name].matrix_local
        mat = recursiveTransformation(arm, parent, mat) @ (parent_local @ local) @ basis

    return mat

def aquire_animation_data(context, filepath, settings):
    scene = context.scene
    armature = context.scene.objects.get("Armature")

    general_data = {
        "number_of_frames" : 0,
        "number_of_bones" : 0,
        "animation_data" : []
     }

    general_data["number_of_frames"] = int((settings.frame_end - settings.frame_start) / settings.frame_step) + 1
    # general_data["number_of_frames"] = settings.frame_end - settings.frame_start
    for bone in armature.pose.bones:
        animation_data = {
            "bone_name" : "",
            "number_of_children" : 0,
            "children" : [],
            "location" : [],
            "rotation_quaternion" : [],
            "scale" : [],
            "bone_matrix" : []
        }
        animation_data["bone_name"] = bone.name
        animation_data["number_of_children"] = len(bone.children)
        for child in bone.children:
            animation_data["children"].append(child.name)
    
        general_data["number_of_bones"] += 1
        general_data["animation_data"].append(animation_data)

    i = 0
    for bone in armature.pose.bones:
        for fr in range(settings.frame_start, settings.frame_end, settings.frame_step):
            scene.frame_set(fr)

            bone_mat = armature.matrix_world.transposed() @ bone.matrix_basis
            mat_transp = bone_mat.transposed()
            loc = mat_transp.to_translation()
            rot = bone_mat.to_quaternion()
            scl = bone_mat.to_scale()

            general_data["animation_data"][i]["location"].append(loc.x)
            general_data["animation_data"][i]["location"].append(loc.z)
            general_data["animation_data"][i]["location"].append(loc.y * -1)
            general_data["animation_data"][i]["location"].append(1.0)
            general_data["animation_data"][i]["rotation_quaternion"].append(rot.w)
            general_data["animation_data"][i]["rotation_quaternion"].append(rot.x)
            general_data["animation_data"][i]["rotation_quaternion"].append(rot.y)
            general_data["animation_data"][i]["rotation_quaternion"].append(rot.z)
            general_data["animation_data"][i]["scale"].append(scl.x)
            general_data["animation_data"][i]["scale"].append(scl.y)
            general_data["animation_data"][i]["scale"].append(scl.z)
            general_data["animation_data"][i]["scale"].append(1.0)

            
            for x in range(0, 4):
                if x == 3:
                    # We must swap the matrix Translation part y and z values because they are different in blender than our engine.
                    general_data["animation_data"][i]["bone_matrix"].append(bone_mat[x][0])
                    general_data["animation_data"][i]["bone_matrix"].append(bone_mat[x][2])
                    # We must reverse z value here because in blender -z is forward.
                    general_data["animation_data"][i]["bone_matrix"].append(bone_mat[x][1] * -1)
                    general_data["animation_data"][i]["bone_matrix"].append(bone_mat[x][3])
                else:
                    for y in range(0, 4):
                        general_data["animation_data"][i]["bone_matrix"].append(bone_mat[x][y])

        i += 1

    write_animation_data(filepath, general_data)

    # That is to delete the option created in File > Export so we dont fill the whole area while we are testing.
    # Should be removed before publishing.
    print("Unregistering...\n")
    unregister()

    return {'FINISHED'}

def write_animation_data(filepath, data):
    f = open(filepath, "w", encoding= "utf-8")

    f.write("no %d\n" % (data["number_of_bones"]))
    f.write("nf %d\n" % (data["number_of_frames"]))

    for i in data["animation_data"]:
        f.write("nm %s\n" % (i["bone_name"]))
        f.write("nc %d\n" % (i["number_of_children"]))

        for x in range(0, len(i["children"])):
            f.write("ch %s\n" % (i["children"][x]))
        for x in range(0, len(i["location"]), 4):
            f.write("lc %f %f %f %f\n" % (i["location"][x], i["location"][x + 1], i["location"][x + 2], i["location"][x + 3]))
        for x in range(0, len(i["rotation_quaternion"]), 4):
            f.write("rq %f %f %f %f\n" % (i["rotation_quaternion"][x], i["rotation_quaternion"][x + 1], i["rotation_quaternion"][x + 2], i["rotation_quaternion"][x + 3]))
        for x in range(0, len(i["scale"]), 4):
            f.write("sc %f %f %f %f\n" % (i["scale"][x], i["scale"][x + 1], i["scale"][x + 2], i["scale"][x + 3]))
        for x in range(0, len(i["bone_matrix"]), 16):
            f.write("bm %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f %f\n" % (
                i["bone_matrix"][x],      i["bone_matrix"][x + 1],  i["bone_matrix"][x + 2],  i["bone_matrix"][x + 3],
                i["bone_matrix"][x + 4],  i["bone_matrix"][x + 5],  i["bone_matrix"][x + 6],  i["bone_matrix"][x + 7],
                i["bone_matrix"][x + 8],  i["bone_matrix"][x + 9],  i["bone_matrix"][x + 10], i["bone_matrix"][x + 11],
                i["bone_matrix"][x + 12], i["bone_matrix"][x + 13], i["bone_matrix"][x + 14], i["bone_matrix"][x + 15]
            ))

    f.close()
    
    return {'FINISHED'}

# ExportHelper is a helper class, defines filename and
# invoke() function which calls the file selector.
from bpy_extras.io_utils import ExportHelper
from bpy.props import StringProperty, BoolProperty, IntProperty
from bpy.types import Operator


class ExportAnimationData(Operator, ExportHelper):
    """This appears in the tooltip of the operator and in the generated docs"""
    bl_idname = "export_animation_data.animation_data"  # important since its how bpy.ops.import_test.some_data is constructed
    bl_label = "Export Animation Data"
    
    # ExportHelper mixin class uses this
    filename_ext = ".txt"

    filter_glob: StringProperty(
        default = "*.txt",
        options = {'HIDDEN'},
        maxlen = 255,  # Max internal buffer length, longer would be clamped.
    )

    # List of operator properties, the attributes will be assigned
    # to the class instance from the operator settings before calling.
    export_parent: BoolProperty(
        name = "Export Parent",
        description = "Exports parent translations when enabled",
        default = True
    )

    frame_start: IntProperty(
        name = "Frame Start",
        description = "The first frame to be Exported",
        default = 1
    )
    frame_end: IntProperty(
        name = "Frame End",
        description = "The last frame to be Exported",
        default = 41
    )
    frame_step: IntProperty(
        name = "Frame Step",
        description = "Export a frame every Step Frames",
        default = 1
    )

    def execute(self, context):
        # Time the programm execution.
        t = time.time()
        
        aquire_animation_data(context, self.filepath, self.properties)

        t_delta = time.time() - t
        print("Export completed in %f secs" % (t_delta))
        return { "FINISHED" }


# Only needed if you want to add into a dynamic menu
def menu_func_export(self, context):
    self.layout.operator(ExportAnimationData.bl_idname, text="AnimText")

# Register and add to the "file selector" menu (required to use F3 search "Text Export Operator" for quick access).
def register():
    bpy.utils.register_class(ExportAnimationData)
    bpy.types.TOPBAR_MT_file_export.append(menu_func_export)

def unregister():
    bpy.utils.unregister_class(ExportAnimationData)
    bpy.types.TOPBAR_MT_file_export.remove(menu_func_export)

if __name__ == "__main__":
    register()

    # test call
    bpy.ops.export_animation_data.animation_data('INVOKE_DEFAULT')
    
    
