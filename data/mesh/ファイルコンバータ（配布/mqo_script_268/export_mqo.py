
import os
import shutil
from math import radians

import bpy
from bpy.props import *
from mathutils import *

#Default Config
def_scale = 100
def_axis = 1
def_shader = 2
def_selection = 0

#Config Class
class OptionConfig:
	def __init__(self, context, path, scale=def_scale, axis=def_axis, shader=def_shader, selection=def_selection):
		self.context = context
		self.path = path
		self.scale = scale
		self.axis = int(axis)
		self.shader = shader
		self.selection = int(selection)

class OutputData:
	def __init__(self):
		self.mesh = None
		self.matrix = None
		self.depth = 0
		self.visivle = 15
		self.lock = 0
		self.obj = None
		
#Writing Metasequoia File
def writeMQO(config):
	print("\n----------Export----------\n")
	print("Filename:" + config.path)
	
	scene = config.context.scene
	buf = open(config.path, "w")
	
	buf.write("Metasequoia Document\n")
	buf.write("Format Text Ver 1.0\n\n")
	
	#Camera
	buf.write("Scene {\n")
	buf.write("\tpos 0.0000 0.0000 1806.0579\n")
	buf.write("\tlookat -0.4346 9.5125 14.4147\n")
	buf.write("\thead 7.1400\n")
	buf.write("\tpich 0.2700\n")
	buf.write("\tortho 0\n")
	buf.write("\tzoom2 8.0579\n")
	buf.write("\tamb 0.250 0.250 0.250\n")
	buf.write("}\n")
	
	meshes = []
	mtrls = []
	mtrlnames = []
	
	objects = scene.objects.values()
	
	#Selection Objects
	if(config.selection):
		objects = config.context.selected_objects #Selected Only
	
	mesh_objects = [target for target in objects if target.type == "MESH"]
	
	for obj in mesh_objects:
		#Get Mesh
		mesh = obj.data
		materials = mesh.materials
		
		for mtrl in materials:
			if(not mtrl.name in mtrlnames):
				mtrlnames.append(mtrl.name)
				mtrls.append(mtrl)
				
		#Set Level
		parent = obj.parent
		level = 0
		
		while(parent):
			if((parent in mesh_objects) and parent.type=="MESH"):
				level += 1
			else:
				break
				
			parent = parent.parent
			
		#Hidden
		visible = 15
		if(obj.hide):
			visible = 0
			
		#Lock
		lock = 0
		if(obj.hide_select):
			lock = 1
			
		data = OutputData()
		data.obj = obj
		data.mesh = mesh
		data.matrix = obj.matrix_world.copy()
		data.depth = level
		data.visible = visible
		data.lock = lock
		
		meshes.append(data)
		
	#Material
	if(len(mtrls) > 0):
		buf.write("Material " + str(len(mtrls)) + " {\n")
		for mtrl in mtrls:
			buf.write("\t\"" + mtrl.name + "\" ")
			buf.write("shader(" + str(config.shader) + ") ")
			buf.write("col(" + "{:.3f} {:.3f} {:.3f} {:.3f}".format(mtrl.diffuse_color[0], mtrl.diffuse_color[1], mtrl.diffuse_color[2], mtrl.alpha) + ") ")
			buf.write("dif(" + "{:.3f}".format(mtrl.diffuse_intensity) + ") ")
			buf.write("amb(" + "{:.3f}".format(mtrl.mirror_color[0]) + ") ")
			buf.write("emi(" + "{:.3f}".format(mtrl.emit) + ") ")
			buf.write("spc(" + "{:.3f}".format(mtrl.specular_color[0]) + ") ")
			buf.write("power(" + "{:.2f}".format(mtrl.specular_intensity) + ")")
			
			#Texture
			slots = mtrl.texture_slots
			for slot in slots:
				if(slot != None and slot.texture.type == "IMAGE"):
					imagePath = slot.texture.image.filepath
					imageName = os.path.basename(imagePath)
					
					buf.write(" tex(\"" + imageName + "\")")
					
					#Copy ImageFile
					if(os.path.exists(imagePath)):
						dirName = os.path.dirname(config.path).replace("\\", "/")
						
						if(not os.path.exists(dirName + "/" + imageName)):
							shutil.copy(imagePath, dirName)
						
			buf.write("\n")
		buf.write("}\n")
		
	#Object
	for data in meshes:
		obj = data.obj
		mesh = data.mesh
		matrix = data.matrix
		level = data.depth
		visible = data.visible
		lock = data.lock
		
		#Base data
		buf.write("Object \"" + mesh.name + "\" {\n")
		buf.write("\tdepth " + str(level) + "\n")
		buf.write("\tfolding 0\n")
		buf.write("\tscale 1.000000 1.000000 1.000000\n")
		buf.write("\trotation 0.000000 0.000000 0.000000\n")
		buf.write("\ttranslation 0.000000 0.000000 0.000000\n")
		buf.write("\tvisible " + str(visible) + "\n")
		buf.write("\tlocking " + str(lock) + "\n")
		buf.write("\tshading 1\n")
		buf.write("\tfacet 60.0\n")
		buf.write("\tcolor 0.000 0.000 0.000\n")
		buf.write("\tcolor_type 0\n")
		
		#Investigated whether a polygon is a triangle side
		anti_triangles = [target for target in mesh.polygons if len(target.vertices) > 4]
		anti_cnt = len(anti_triangles)
		
		if(anti_cnt > 0):
			print("\n{0} polygons which are not triangles are contained. these is changed into a triangle...".format(anti_cnt))
			
			#Current Active object
			active_obj = scene.objects.active
			if(active_obj != None):
				bpy.ops.object.mode_set(mode="OBJECT")
				active_obj.select = False
			
			obj.select = True
			scene.objects.active = obj
			
			#Selected quads vertex
			for polygon in anti_triangles:
				for index in polygon.vertices:
					vertex =  mesh.vertices[index]
					vertex.select = True
					
			#Changes into a triangle
			bpy.ops.object.mode_set(mode="EDIT")
			bpy.ops.mesh.quads_convert_to_tris()
			bpy.ops.object.mode_set(mode="OBJECT")
			
			#Mesh Update
			mesh.validate()
			mesh.update()
			obj.select = False
			
			anti_triangles = [target for target in mesh.polygons if len(target.vertices) != 3]
			anti_cnt = len(anti_triangles)
			
			if(anti_cnt > 0):
				print("Error: failed in {0} polygon conversion.".format(anti_cnt))
				return
				
			else:
				print("Conversion was completed.")
		
		#Vertex
		buf.write("\tvertex " + str(len(mesh.vertices)) + " {\n")
		transformMatrix = Matrix()
		
		#Scaling
		transformMatrix *= Matrix.Scale(config.scale, 4, Vector((1, 0, 0)))
		transformMatrix *= Matrix.Scale(config.scale, 4, Vector((0, 1, 0)))
		transformMatrix *= Matrix.Scale(config.scale, 4, Vector((0, 0, 1)))
		
		#Change AxisY
		if(config.axis == 1):
			transformMatrix *= Matrix.Rotation(radians(-90), 4, "X")
			
		#Change AxisZ
		if(config.axis == 2):
			transformMatrix *= Matrix.Rotation(radians(90), 4, "X")
		
		for vertex in mesh.vertices:
			#Transform
			position = transformMatrix * matrix * vertex.co.copy()
			buf.write("\t\t" + "{:.4f} {:.4f} {:.4f}".format(position[0], position[1], position[2]) + "\n")
			
		buf.write("\t}\n")
		
		#Index
		mtrl = mesh.materials
		
		buf.write("\tface " + str(len(mesh.polygons)) + " {\n")
		index2 = [0,1]
		index3 = [0,2,1]
		index4 = [0,3,2,1]
		total_index = 0
		
		for polygon in mesh.polygons:
			count = len(polygon.vertices)
			use_index = []
			
			if(count == 2):
				use_index = index2
			elif(count == 3):
				use_index = index3
			else:
				use_index = index4
					
			buf.write("\t\t" + str(count) + " V(")
			
			#Write FaceID
			for i in range(count):
				buf.write(str(polygon.vertices[use_index[i]]))
					
				if(i < count - 1):
					buf.write(" ")
				else:
					buf.write(") ")
					
			#Write MaterialID
			if(len(mtrl) > 0):
				ma = mtrl[polygon.material_index]
				buf.write("M(" + str(mtrls.index(ma)) + ")")
				
			#Write Texcoord
			if((len(mesh.uv_layers) > 0) and (mesh.uv_layers.active != None)):
				buf.write(" UV(")
				
				for i in range(count):
					uv = mesh.uv_layers.active.data[use_index[i] + total_index].uv
					buf.write("{:.5f} {:.5f}".format(uv[0], 1 - uv[1]))
					
					if(i < count - 1):
						buf.write(" ")
					else:
						buf.write(")")
						
			buf.write("\n")
			total_index += count
			
		buf.write("\t}\n")
		buf.write("}\n")
		
	buf.write("Eof")
	buf.close()
	
	print("finished!")
	
AxisModes = []
AxisModes.append(("0", "Default", ""))
AxisModes.append(("1", "AxisY(Rotate X-90)", ""))
AxisModes.append(("2", "AxisZ(Rotate X90)", ""))

ShaderModes = []
ShaderModes.append(("0", "Classic", ""))
ShaderModes.append(("1", "Constant", ""))
ShaderModes.append(("2", "Lambert", ""))
ShaderModes.append(("3", "Phong", ""))
ShaderModes.append(("4", "Blinn", ""))

SystemModes = []
SystemModes.append(("0", "Right-Hand(Default)", ""))
SystemModes.append(("1", "Left-Hand", ""))

SelectionModes = []
SelectionModes.append(("0", "All Objects", ""))
SelectionModes.append(("1", "Selected Only", ""))

class MetasequoiaExporter(bpy.types.Operator):
    """Export to the Metasequoia format (.mqo)"""

    bl_idname = "export.metasequoia"
    bl_label = "Export Metasequoia"
    filename_ext = ".mqo"
    filter_glob = StringProperty(default="*.mqo", options={'HIDDEN'})
    
    filepath = StringProperty()
    filename = StringProperty()
    directory = StringProperty()
    
    #Option
    ExportScale = FloatProperty(name="Scale", description="Change of scale value.", min=0.01, max=100, soft_min=0.01, soft_max=10, default=def_scale)
    ExportAxis = EnumProperty(name="Upper Axis", description="Setting of the above axis.", items=AxisModes, default=str(def_axis))
    ExportShader = EnumProperty(name="Select Shader", description="Schaeder's selection.", items=ShaderModes, default=str(def_shader))
    ExportSelection = EnumProperty(name="Selection Objects", description="Selection of output object.", items=SelectionModes, default=str(def_selection))
    
    #Config
    def execute(self, context):
        writeMQO( OptionConfig(context, self.filepath, self.ExportScale, self.ExportAxis, self.ExportShader, self.ExportSelection) )
        return {"FINISHED"}

    def invoke(self, context, event):
        WindowManager = context.window_manager
        WindowManager.fileselect_add(self)
        return {"RUNNING_MODAL"}
