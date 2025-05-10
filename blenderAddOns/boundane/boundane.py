from sys import maxsize
import os
import time
import bpy
from random import randint, uniform

def time_execution():
    if len(bpy.context.selected_objects) == 0:
        print("No meshes selected")
        return 0

    t = time.time()  # Measure function execution time if 1 or more meshes have been selected.

    minX = maxsize
    maxX = -maxsize - 1
    minY = maxsize
    maxY = -maxsize - 1
    minZ = maxsize
    maxZ = -maxsize - 1

    for o in bpy.context.selected_objects:
        if o.type != 'ARMATURE':
            bpy.ops.object.transform_apply()
            for v in o.data.vertices:
                if v.select == True:
                    minX = v.co[0] if (v.co[0] < minX) else minX
                    maxX = v.co[0] if (v.co[0] > maxX) else maxX
                    minY = v.co[1] if (v.co[1] < minY) else minY
                    maxY = v.co[1] if (v.co[1] > maxY) else maxY
                    minZ = v.co[2] if (v.co[2] < minZ) else minZ
                    maxZ = v.co[2] if (v.co[2] > maxZ) else maxZ

    bpy.ops.mesh.primitive_cube_add(size = 2, enter_editmode = False, align = 'WORLD', location = (0, 0, 0), scale = (1, 1, 1))
    bpy.ops.object.mode_set(mode = 'EDIT')
    bpy.ops.mesh.quads_convert_to_tris(quad_method = 'BEAUTY', ngon_method = 'BEAUTY')
    bpy.ops.object.mode_set(mode = 'OBJECT')
    bpy.context.object.name = "boundane"
    bpy.context.object.display_type = "WIRE"

    bpy.context.object.data.vertices[0].co = [minX, minY, minZ]
    bpy.context.object.data.vertices[1].co = [minX, minY, maxZ]
    bpy.context.object.data.vertices[2].co = [minX, maxY, minZ]
    bpy.context.object.data.vertices[3].co = [minX, maxY, maxZ]
    bpy.context.object.data.vertices[4].co = [maxX, minY, minZ]
    bpy.context.object.data.vertices[5].co = [maxX, minY, maxZ]
    bpy.context.object.data.vertices[6].co = [maxX, maxY, minZ]
    bpy.context.object.data.vertices[7].co = [maxX, maxY, maxZ]


    print(minX, minY, minZ)
    print(maxX, maxY, maxZ)

    delta_t = time.time() - t
    print("Execution time: %f\n" % (delta_t))

if __name__ == "__main__":
    time_execution()