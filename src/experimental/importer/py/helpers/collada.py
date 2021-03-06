import bpy
import tempfile

import os
import sys
import inspect

from . import Dumpable
from godot.export_dae import DaeExporter


class GodotExporter(Dumpable):
    """ Uses godot exporter to dunp stuff to collada 
    https://github.com/godotengine/collada-exporter
    """
    
    # These are the params that should be set up later on:
    params = {}
    # params["use_export_selected"] = False,
    # params["use_mesh_modifiers"] = False,
    # params["use_tangent_arrays"] = False,
    # params["use_triangles"] = False,
    # params["use_copy_images"] = False,
    # params["use_active_layers"] = True,
    # params["use_exclude_ctrl_bones"] = True,
    # params["use_anim"] = False,
    # params["use_anim_action_all"] = False,
    # params["use_anim_skip_noexp"] = True,
    # params["use_anim_optimize"] = True,
    # params["use_shape_key_export"] = False,
    # params["anim_optimize_precision"] = 6.0,
    # params["use_metadata"] = True,
    
    def __init__(self):
        Dumpable.__init__(self)
        pass

    def report(self, a, b):
        print(b)
        
    def reprJSON(self):
        tfile = tempfile.NamedTemporaryFile()
        tname = tfile.name
        # self._save_collada(tname)
        tfile.close()

        daefile = tname + ".dae"
        
        dae = ""
        with DaeExporter(daefile, self.params, self) as exp:
            exp.export()
        
        if os.path.exists(daefile):
            with open(daefile) as f:
                print("Reading dae")
                dae = f.read()
            os.unlink(daefile) 

        return dae
        

class InternalExporter(Dumpable):
    """
    Export scene to a tempfile as collada, then push the data to the server
    """
    
    def __init__(self):
        Dumpable.__init__(self)
        
        self.triangulate = True
        self.use_texture_copies = False
        
        # Blender does not exports texture filenames, but Godot would
        # that will be refractored lateron (or something)
        self.include_material_textures = False  
        self.include_uv_textures = True

        pass #ctor

    def reprJSON(self):
        tfile = tempfile.NamedTemporaryFile()
        tname = tfile.name
        self._save_collada(tname)
        tfile.close()

        daefile = tname + ".dae"
        
        dae = ""
        
        if os.path.exists(daefile):
            with open(daefile) as f:
                print("Reading dae")
                dae = f.read()
            os.unlink(daefile) 

        return dae
        
    def get_cmd(self):
        return self._cmd
        
    def _save_collada(self, tname):
        # https://docs.blender.org/api/blender_python_api_current/bpy.ops.wm.html
        bpy.ops.wm.collada_export(
            filepath=tname, \
            #apply_modifier=True, \
            triangulate=self.triangulate, \
            use_texture_copies=self.use_texture_copies, \
            include_material_textures=self.include_material_textures, \
            include_uv_textures=self.include_uv_textures \
        )
        pass
