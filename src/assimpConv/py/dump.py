import bpy
import tempfile
import os


class Bake:
  """
  Helps to bake the animation data before export
  """
  pass

  
  
class Dump:
  """
  Export everyhting out of blender
  """
  _cmd_dump = "collada"
  
  pass
  
  

class Collada:
  """
  Export scene to a tempfile as collada, then push the data to the server
  """

  _cmd_dae = "collada"

  def __init__(self, connection):
    self.c = connection
    
    self.triangulate = True
    self.use_texture_copies = False
    self.include_material_textures = True
    self.include_uv_textures = True
    
    pass
  
  def _save_collada(self):
    # https://docs.blender.org/api/blender_python_api_current/bpy.ops.wm.html
    bpy.ops.wm.collada_export(\
      filepath=self.tmp.name, \
      #apply_modifier=True, \
      triangulate=self.triangulate, \
      use_texture_copies=self.use_texture_copies, \
      include_material_textures=self.include_material_textures, \
      include_uv_textures=self.include_uv_textures \
    )
    pass
    
  def __enter__(self):
    self.tmp = tempfile.NamedTemporaryFile(delete=False)
    self.tmp.close()
    os.unlink(self.tmp.name)
    
    _save_collada()
    
    return self
    
  def __exit__(self, type, value, traceback):
    daefile = self.tmp.name + ".dae"
    if os.path.exists(daefile):
      with open(daefile) as f:
        print("Reading dae")
        self.c.send(self._cmd_dae, f.read())
      os.unlink(daefile)
    
  