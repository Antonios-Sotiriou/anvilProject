
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

def aquire_animation_data(context, filepath, settings):
    scene = context.scene
    armature = context.scene.objects.get("Armature")

    general_data = {
        "number_of_frames" : 0,
        "number_of_objects" : 0,
        "animation_data" : []
     }

    general_data["number_of_frames"] = int((settings.frame_end - settings.frame_start) / settings.frame_step) + 1
    for bone in armature.pose.bones:
        animation_data = {
            "object_name" : "",
            "number_of_children" : 0,
            "children" : [],
            "location" : [],
            "rotation_quaternion" : [],
            "scale" : []
        }
        animation_data["object_name"] = bone.name
        animation_data["number_of_children"] = len(bone.children)
        for child in bone.children:
            animation_data["children"].append(child.name)
    
        general_data["number_of_objects"] += 1
        general_data["animation_data"].append(animation_data)

    i = 0
    for bone in armature.pose.bones:
        for fr in range(settings.frame_start, settings.frame_end, settings.frame_step):
            scene.frame_set(fr)
            general_data["animation_data"][i]["location"].append(bone.location.x)
            general_data["animation_data"][i]["location"].append(bone.location.y)
            general_data["animation_data"][i]["location"].append(bone.location.z)
            general_data["animation_data"][i]["location"].append(1.0)
            general_data["animation_data"][i]["rotation_quaternion"].append(bone.rotation_quaternion.w)
            general_data["animation_data"][i]["rotation_quaternion"].append(bone.rotation_quaternion.x)
            general_data["animation_data"][i]["rotation_quaternion"].append(bone.rotation_quaternion.y)
            general_data["animation_data"][i]["rotation_quaternion"].append(bone.rotation_quaternion.z)
            general_data["animation_data"][i]["scale"].append(bone.scale.x)
            general_data["animation_data"][i]["scale"].append(bone.scale.y)
            general_data["animation_data"][i]["scale"].append(bone.scale.z)
            general_data["animation_data"][i]["scale"].append(1.0)
        i += 1

    write_animation_data(filepath, general_data)

    # That is to delete the option created in File > Export so we dont fill the whole area while we are testing.
    # Should be removed before publishing.
    print("Unregistering...\n")
    unregister()

    return {'FINISHED'}

def write_animation_data(filepath, data):
    f = open(filepath, "w", encoding= "utf-8")

    f.write("no %d\n" % (data["number_of_objects"]))
    f.write("nf %d\n" % (data["number_of_frames"]))

    for i in data["animation_data"]:
        f.write("nm %s\n" % (i["object_name"]))
        f.write("nc %d\n" % (i["number_of_children"]))

        for x in range(0, len(i["children"])):
            f.write("ch %s\n" % (i["children"][x]))
        for x in range(0, len(i["location"]), 4):
            f.write("lc %f %f %f %f\n" % (i["location"][x], i["location"][x + 1], i["location"][x + 2], i["location"][x + 3]))
        for x in range(0, len(i["rotation_quaternion"]), 4):
            f.write("rq %f %f %f %f\n" % (i["rotation_quaternion"][x], i["rotation_quaternion"][x + 1], i["rotation_quaternion"][x + 2], i["rotation_quaternion"][x + 3]))
        for x in range(0, len(i["scale"]), 4):
            f.write("sc %f %f %f %f\n" % (i["scale"][x], i["scale"][x + 1], i["scale"][x + 2], i["scale"][x + 3]))

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
        default = 240
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
    
    
