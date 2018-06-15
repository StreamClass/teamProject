
import os
import math
from math import radians
from struct import *

import bpy
from bpy.props import *
from mathutils import *
from bpy_extras.image_utils import load_image

#Default Config
def_scale = 0.01
def_axis = 2
def_smooth = True
def_shadeless = False
def_remove_doubles = False
def_mirror = True
def_cycles = False

#Config Class
class OptionConfig:
	def __init__(self, context, path, 
	scale=def_scale, 
	axis=def_axis, 
	mirror=def_mirror, 
	smooth=def_smooth, 
	shadeless=def_shadeless, 
	remove_doubles=def_remove_doubles,
	cycles=def_cycles):
	
		self.context = context
		self.path = path
		self.scale = scale
		self.axis = int(axis)
		self.smooth = smooth
		self.shadeless = shadeless
		self.mirror = mirror
		self.remove_doubles = remove_doubles
		self.cycles = cycles
		
class MqoEdge:
	def __init__(self):
		self.begin_index = -1
		self.end_index = -1
		self.material = -1
		self.beginUV = Vector((0.0, 0.0, 0.0))
		self.endUV = Vector((0.0, 0.0, 0.0))
		
class ColorValue:
	def __init__(self, r = 1, g = 1, b = 1, a = 1):
		self.r = r
		self.g = g
		self.b = b
		self.a = a
		
	def __str__(self):
		return str(self.r) + ";" + str(self.g) + ";" + str(self.b) + ";" + str(self.a);
		
	def __mul__(self, value):
		color = ColorValue(self.r * value, self.g * value, self.b * value, self.a)
		return color
		
	def toArray(self):
		return [self.r, self.g, self.b, self.a]
		
class MaterialValue:
	def __init__(self):
		self.color = ColorValue()
		self.diffuse = 0.0
		self.specular = 0.0
		self.ambient = 0.0
		self.emissive = 0.0
		self.power = 0.0
		self.texture = ""
		self.name = ""
		
class ObjectInfoBuf:
	def __init__(self):
		self.name = ""
		self.depth = 0
		self.mirror_type = 0
		self.mirror_axis = 0
		self.visible = 0
		self.lock = 0
		self.lathe_enable = False
		self.lathe_axis = 0
		self.lathe_seg = 0.0
		self.parent = None
		self.children = []

class ModelMesh:
	def __init__(self):
		self.materials = []
		self.vertices = []
		self.edges = []
		self.faces = []
		self.useMtrl = []
		self.mtrlIndex = []
		self.texcoords = []
		self.objects = []
	
	#Material
	def readMaterials(self, fp, count):
		for mtrlIndex in range(count):
			txt = fp.readline().decode("sjis")
			mtrl = MaterialValue()
			
			#Name
			start = txt.find("\"")
			txt = txt[start + 1:len(txt)]
			end = txt.find("\"")
			mtrl.name = txt[0:end]
			
			#Color
			start = txt.rfind(" col(")
			txt = txt[start + 1:len(txt)]
			start = txt.find("(")
			end = txt.find(")")
			tmp = txt[start + 1:end].split()
			
			col = []
			for value in tmp:
				col.append(float(value))
				
			color = ColorValue(col[0], col[1], col[2], col[3])
			
			#Value
			valueName=["dif", "amb", "emi", "spc", "power", "tex"]
			result = []
			
			for value in valueName:
				start = txt.find(value)
				
				if(start > -1):
					st = txt[start+1:len(txt)]
					start = st.find("(")
					end = st.find(")")
					result.append(st[start + 1:end])
			
			mtrl.color = color
			mtrl.diffuse = float(result[0])
			mtrl.ambient = float(result[1])
			mtrl.emissive = float(result[2])
			mtrl.specular = float(result[3])
			mtrl.power = float(result[4])
			
			#Texture
			if(len(result) == len(valueName)):
				mtrl.texture = result[5][1:len(result[5]) - 1]
				
			self.materials.append(mtrl)
			
	#Vertex
	def readVertices(self, fp, count):
		vertices = []
		
		for vertexIndex in range(count):
			txt = fp.readline().split()
			vertex = []
			
			for value in txt:
				vertex.append(float(value.strip()))
			
			pos = Vector((vertex[0], vertex[1], vertex[2]))
			vertices.append(pos)
			
		self.vertices.append(vertices)
		
	def readBytesVertices(self, fp, count):
		vertices = []
		
		for vertexIndex in range(count):
			vertex = []
			
			for i in range(3):
				txt = fp.read(4)
				vertex.append(unpack('f', txt)[0])
			
			pos = Vector((vertex[0], vertex[1], vertex[2]))
			vertices.append(pos)
			
		self.vertices.append(vertices)
		
	def readIndices(self, fp, count):
		index1 = [[0, 2, 1], [2, 1, 0]]
		index2 = [[0, 3, 2, 1], [2, 1, 0, 3]]
		index3 = [0, 1]
		
		faces = []
		edges = []
		mtrlIndex = []
		useMtrl = []
		texcoord = []
		defMtrl = 0
		
		for faceIndex in range(count):
			txt = fp.readline().decode("sjis")
			
			#Face
			start = txt.find("(")
			end = txt.find(")")
			index_tmp = txt[start + 1:end].split()
			face = len(index_tmp)
			index_id = index1[0]

			#Triangle
			if(face == 3):
				if(int(index_tmp[index_id[2]]) == 0):
					index_id = index1[1]
					
			#Quad
			elif(face == 4):
				index_id = index2[0]
				
				if(int(index_tmp[index_id[2]]) == 0 or int(index_tmp[index_id[3]]) == 0):
					index_id = index2[1]
					
			#Edge
			elif(face == 2):
				index_id = index3
			
			#UV
			uv = []
			
			for i in range(face):
				uv.append( Vector((0.0, 0.0, 0.0)) )
				
			start = txt.find("UV(")
			if(start > -1):
				uv_txt = txt[start + 1:len(txt)]
				start = uv_txt.find("(");
				end = uv_txt.find(")");
				tmp = uv_txt[start + 1:end].split()
				
				for i, index in enumerate(index_id):
					uv_index = index << 1
					uv[i] = Vector((float(tmp[uv_index]), float(tmp[uv_index + 1]), 0))
			
			#Material ID
			mtrlID = txt.find("M(")
			if(mtrlID > -1):
				mtrl_txt = txt[mtrlID + 1:len(txt)]
				start = mtrl_txt.find("(");
				end = mtrl_txt.find(")");
				mtrlID = int(mtrl_txt[start + 1:end])
				
			else:
				if(defMtrl == 0):
					defParts = MaterialValue()
					defParts.color = ColorValue(1, 1, 1, 1)
					defParts.diffuse = 0.8
					defParts.specular = 0
					defParts.ambient = 0
					defParts.emissive = 0.6
					defParts.power = 5
					defParts.name = "default"
					self.materials.append(defParts)
					defMtrl = 1
				
				mtrlID = len(self.materials) - 1
				
			if(not mtrlID in useMtrl):
				useMtrl.append(mtrlID)
				
			if(face == 2):
				edge = MqoEdge()
				edge.begin_index = int(index_tmp[0])
				edge.end_index = int(index_tmp[1])
				edge.material = mtrlID
				
				if(len(uv) > 0):
					edge.beginUV = uv[0]
					edge.endUV = uv[1]
					
				edges.append(edge)
				continue
			
			#Index
			indices = []
			
			for index in index_id:
				indices.append(int(index_tmp[index]))
				
			faces.append(indices)
			texcoord.append(uv)
			mtrlIndex.append(mtrlID)
			
		self.faces.append(faces)
		self.edges.append(edges)
		self.mtrlIndex.append(mtrlIndex)
		self.useMtrl.append(useMtrl)
		self.texcoords.append(texcoord)
		
	@staticmethod
	def load(config):
		fp = open(config.path, "rb")
		model = ModelMesh()
		
		while(1):
			txt = fp.readline()
			if(len(txt) == 0):
				break
				
			#Material
			index = txt.find(b"Material")
			if(index > -1):
				num = int( txt[index:len(txt) - 1].split()[1] )
				model.readMaterials(fp, num)
				
			#Object
			index = txt.find(b"Object")
			if(index > -1):
				objectIndex = len(model.objects)
				name = txt[index:len(txt) - 1].split()[1].decode("sjis")
				
				objectBuf = ObjectInfoBuf()
				objectBuf.name = name.replace("\"", "")
				
				model.objects.append(objectBuf)
				
				match_int = [b"depth", b"mirror ", b"mirror_axis", b"visible", b"locking", b"lathe ", b"lathe_axis", b"lathe_seg"]
				match_float = []
				match = [["int", match_int], ["float", match_float]]
				result = []
				
				for i, list in enumerate(match):
					result.append([])
					
					for j in range(len(list[1])):
						result[i].append(0)
						
				while((txt.find(b"vertex ") < 0) and (txt.find(b"BVertex ") < 0)):
						txt = fp.readline()
						
						for valueIndex, list in enumerate(match):
							for listIndex, txt_value in enumerate(list[1]):
								index = txt.find(txt_value)
								
								if(index > -1):
									value = txt[index:len(txt) - 1]
									value = value.split()[1].strip()
									
									if(list[0] == "int"):
										result[valueIndex][listIndex] = int(value)
										
									elif(list[0] == "float"):
										result[valueIndex][listIndex] = float(value)
							
				objectBuf.depth = result[0][0]
				objectBuf.mirror_type = result[0][1]
				objectBuf.mirror_axis = result[0][2]
				objectBuf.visible = result[0][3]
				objectBuf.lock = result[0][4]
				objectBuf.lathe_enable = (result[0][5] == 3)
				objectBuf.lathe_axis = result[0][6]
				objectBuf.lathe_seg = result[0][7]
				
				if(objectBuf.mirror_type and objectBuf.mirror_axis == 0):
					objectBuf.mirror_axis = 1
					
				#vertex
				index = txt.find(b"vertex")
				if(index > -1):
					vertexCnt = int( txt[index:len(txt) - 1].split()[1] )
					model.readVertices(fp, vertexCnt)
					
				elif(txt.find(b"BVertex") > -1):
					tmp = fp.readline().split()
					
					if(len(tmp) == 2):
						vertexCnt = int(tmp[1:len(txt) - 2]) / (4 * 3)
					else:
						vertexCnt = int(tmp[1])
					
					model.readBytesVertices(fp, vertexCnt)
				
				#face
				index = txt.find(b"face ")
				while(index < 0):
					txt = fp.readline()
					index = txt.find(b"face")
					
				faceCnt = int( txt[index:len(txt) - 1].split()[1] )
				model.readIndices(fp, faceCnt)
				
		fp.close()
		
		#Lathe
		axisX = [0, Vector((1.0, 0.0, 0.0)), Vector((0.0, 1.0, 0.0))]
		axisY = [1, Vector((0.0, 1.0, 0.0)), Vector((1.0, 0.0, 0.0))]
		axisZ = [2, Vector((0.0, 0.0, 1.0)), Vector((0.0, 1.0, 0.0))]
		axisList = [axisX, axisY, axisZ]
		
		for objIndex, obj in enumerate(model.objects):
			if(obj.lathe_enable):
				edges = model.edges[objIndex]
				vertices = model.vertices[objIndex]
				faces = model.faces[objIndex]
				texcoord = model.texcoords[objIndex]
				mtrlIndex = model.mtrlIndex[objIndex]
				
				if(len(edges) > 0):
					for edge in edges:
						rays = [vertices[edge.begin_index], vertices[edge.end_index]]
						uv = [edge.beginUV, edge.endUV]
						length = [rays[0].length, rays[1].length]
						normal = [0, 0]
						
						if(length[0] > length[1]):
							rays = [vertices[edge.end_index], vertices[edge.begin_index]]
							uv = [edge.endUV, edge.beginUV]
							length = [rays[0].length, rays[1].length]
						
						#Normalize
						for rayIndex, ray in enumerate(rays):
							if(length[rayIndex] > 0.0):
								normal[rayIndex] = ray / length[rayIndex]
						
						for axisIndex, axis in enumerate(axisList):
							if(axis[0] == obj.lathe_axis):
								result = [0, 0]
								
								for rayIndex, ray in enumerate(rays):
									result[rayIndex] = ray
									dot = Vector.dot(axis[1], normal[rayIndex])
									angle = math.radians(90.0 - math.degrees(math.acos(dot)))
									upper = Vector.cross(axis[1], normal[rayIndex])
									
									if(upper.x != 0.0 or upper.y != 0.0 or upper.z != 0.0):
										upper.normalize()
										result[rayIndex] = normal[rayIndex] * Matrix.Rotation(angle, 4, upper)
										result[rayIndex].normalize()
										
										dot = Vector.dot(result[rayIndex], axis[2])
										upper = Vector.cross(result[rayIndex], axis[2])
										
										if(upper.x != 0.0 or upper.y != 0.0 or upper.z != 0.0):
											upper.normalize()
											result[rayIndex] = ray * Matrix.Rotation(math.acos(dot), 4, upper)
								
								#Segment
								angle_add = 360.0 / obj.lathe_seg
								uv_add = 1.0 / obj.lathe_seg
								verts = []
								uvs = []
								
								for seg in range(obj.lathe_seg):
									rotate = Matrix.Rotation(math.radians(angle_add * seg), 4, axis[1])
									
									for posIndex, pos in enumerate(result):
										verts.append(pos * rotate)
										uvs.append(uv[posIndex])
									
								box = verts[0:2]
								start = verts[0:2]
								box_tex = uvs[0:2]
								start_tex = uvs[0:2]
								
								positions = []
								tex_tmp = []
								
								for i in range(2):
									verts.pop(0)
									uvs.pop(0)
									
								while(len(verts) > 0):
									#Acquisition of segment
									ray = verts[0:4-len(box)]
									ray_tex = uvs[0:4-len(box_tex)]
									
									for i in range(len(ray)):
										verts.pop(0)
										uvs.pop(0)
										
									#Check on repetition coordinates
									for pos in box:
										if(ray.count(pos) > 0):
											ray_tex.pop(ray.index(pos))
											ray.remove(pos)
											break
											
									box.extend(ray)
									box_tex.extend(ray_tex)
									
									index_id1 = [0, 2, 1]
									index_id2 = [0, 1, 3, 2]
									index_id = []
									
									if(len(box) == 3):
										index_id = index_id1
									else:
										index_id = index_id2
									
									#Making to both sides
									for side in range(2):
										mtrlIndex.append(edge.material)
										face = []
										
										for posIndex, targetIndex in enumerate(index_id):
											count = len(positions)
											
											if(box[targetIndex] in positions):
												count = positions.index(box[targetIndex])
											else:
												positions.append(box[targetIndex])
												tex_tmp.append(box_tex[targetIndex])
												
											face.append(count)
											
										#Face
										tex = []
										for index in range(len(face)):
											tex.append(tex_tmp[face[index]])
											face[index] += len(vertices)
											
										faces.append(face)
										texcoord.append(tex)
										
										#Replacement of index
										index_id.reverse()
										
									if(len(box) > 3):
										for i in range(2):
											box.pop(0)
											box_tex.pop(0)
									else:
										box.pop(1)
										box_tex.pop(1)
										
									if(len(verts) == 0 and len(start) != 0):
										verts = start
										uvs = start_tex
								
								for pos in positions:
									vertices.append(pos)
		#Mirror
		if(config.mirror):
			axisX = [1, Vector((1.0, 0.0, 0.0))]
			axisY = [2, Vector((0.0, 1.0, 0.0))]
			axisZ = [4, Vector((0.0, 0.0, 1.0))]
			axisList = [axisX, axisY, axisZ]
				
			for objIndex, obj in enumerate(model.objects):
				mirror_axis = obj.mirror_axis
				
				vertices = model.vertices[objIndex]
				faces = model.faces[objIndex]
				texcoord = model.texcoords[objIndex]
				mtrlIndex = model.mtrlIndex[objIndex]
				
				for axis in axisList:
					if(mirror_axis & axis[0]):
						vertexCnt = len(vertices)
						
						for vertexIndex in range(vertexCnt):
							pos = vertices[vertexIndex] * Matrix.Scale(-1.0, 4, axis[1])
							vertices.append(pos)
							
						indexCnt = len(faces)
						
						for faceIndex in range(indexCnt):
							face = faces[faceIndex]
							uv = texcoord[faceIndex]
							count = len(face)
							reverseFaces = []
							reverseTexcoord = []
							
							for index in range(count):
								reverseFaces.append(face[count - index - 1] + vertexCnt)
								reverseTexcoord.append(uv[count - index - 1])
							
							faces.append(reverseFaces)
							texcoord.append(reverseTexcoord)
							mtrlIndex.append(mtrlIndex[faceIndex])
							
		return model
		
def loadMQO(config):
	print("FilePath:" + config.path)
	
	scene = config.context.scene
	scene.cursor_location = [0, 0, 0]
	
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
		
	#Load
	model = ModelMesh.load(config)
	
	#Current Active object
	active_obj = scene.objects.active
	if(active_obj != None):
		bpy.ops.object.mode_set(mode="OBJECT")
		active_obj.select = False
	
	#Material
	materials = []
	
	for material in model.materials:
		#Material
		mat = bpy.data.materials.new(material.name)
		
		#Diffuse
		mat.diffuse_color = material.color.toArray()[0:3]
		mat.diffuse_intensity = material.diffuse
		
		#Emission
		mat.emit = material.emissive
		
		#Ambient
		mat.ambient = material.ambient
		
		#Specular
		mat.specular_color = [material.specular, material.specular, material.specular]
		mat.specular_hardness = material.power
		
		#Alpha
		mat.use_transparency = True
		mat.alpha = mat.specular_alpha = material.color.a
		
		#Shadeless
		mat.use_shadeless = config.shadeless
		
		#Texture
		img = None
		if(len(material.texture) != 0):
			pathName = material.texture.replace("\\", "/")
			
			if(not os.path.exists(pathName)):
				dirName = os.path.dirname(config.path.replace("\\", "/"))
				pathName = dirName + "/" + pathName
				
			msg = "TexturePath:" + pathName + " ...... "
				
			if(os.path.exists(pathName) == True):
				#Create Image
				tex = bpy.data.textures.new("Tex_" + material.name, type="IMAGE")
				tex.image = img = load_image(pathName, os.path.dirname(config.path))
				
				#Set Texture
				tex.factor_red = material.color.r
				tex.factor_green = material.color.g
				tex.factor_blue = material.color.b
				
				#Set TextureSlot
				tex_slot = mat.texture_slots.add()
				tex_slot.texture = tex
				tex_slot.texture_coords = 'UV'
				tex_slot.use_map_color_diffuse = tex_slot.use_map_alpha = True
				tex_slot.alpha_factor = mat.alpha
				mat.alpha = 0.0
				
				msg += "Succeed."
				
			else:
				msg += "Failed."
			
			print(msg)
			
		materials.append([mat, img])
		
		#CyclesRender
		if config.cycles:
			bpy.context.scene.render.engine = 'CYCLES'
			
			mat.use_nodes = True
			mat.node_tree.links.clear()
			
			#Diffuse RGB
			diffuse_bsdf = [target for target in mat.node_tree.nodes if(target.type == 'BSDF_DIFFUSE')]
			diffuse_bsdf = diffuse_bsdf[0]
			
			diffuse_bsdf.inputs['Color'].default_value = material.color.toArray()
			diffuse_bsdf.inputs['Roughness'].default_value = 0.0
			diffuse_output = diffuse_bsdf.outputs['BSDF']
			
			#Alpha
			if(img != None):
				#Diffuse Color Mix
				diffuse_mix = mat.node_tree.nodes.new('ShaderNodeMixRGB')
				diffuse_mix.blend_type = 'MULTIPLY'
				diffuse_mix.use_clamp = True
				
				diffuse_mix.inputs['Fac'].default_value = 1.0
				diffuse_mix.inputs['Color1'].default_value = material.color.toArray()
				mat.node_tree.links.new(diffuse_bsdf.inputs['Color'], diffuse_mix.outputs['Color'])
				
				#Texture Image
				diffuse_texture = mat.node_tree.nodes.new('ShaderNodeTexImage')
				diffuse_texture.image = img
				mat.node_tree.links.new(diffuse_mix.inputs[2], diffuse_texture.outputs['Color'])
				
				#Texture Alpha
				texture_alpha = mat.node_tree.nodes.new('ShaderNodeBsdfTransparent')
				texture_alpha.inputs['Color'].default_value = [1.0, 1.0, 1.0, 1.0]
				
				#Diffuse Texture Color Mix
				texture_mix = mat.node_tree.nodes.new('ShaderNodeMixShader')
				mat.node_tree.links.new(texture_mix.inputs['Fac'], diffuse_texture.outputs['Alpha'])
				mat.node_tree.links.new(texture_mix.inputs[1], texture_alpha.outputs['BSDF'])
				mat.node_tree.links.new(texture_mix.inputs[2], diffuse_output)
				diffuse_output = texture_mix.outputs['Shader']
				
			#Alpha
			rgba_alpha = mat.node_tree.nodes.new('ShaderNodeBsdfTransparent')
			rgba_alpha.inputs['Color'].default_value = [1.0, 1.0, 1.0, 1.0]
			
			#ARGB Color
			rgba_mix = mat.node_tree.nodes.new('ShaderNodeMixShader')
			rgba_mix.inputs['Fac'].default_value = material.color.a
			mat.node_tree.links.new(rgba_mix.inputs[1], rgba_alpha.outputs['BSDF'])
			mat.node_tree.links.new(rgba_mix.inputs[2], diffuse_output)
			
			material_output = [target for target in mat.node_tree.nodes if(target.type == 'OUTPUT_MATERIAL')]
			material_output = material_output[0]
			
			mat.node_tree.links.new(material_output.inputs['Surface'], rgba_mix.outputs['Shader'])
			mat.node_tree.links.new(material_output.inputs['Volume'], rgba_mix.outputs['Shader'])
		
	#Mesh
	tree = []
	objects = []
	
	for objIndex, vertices in enumerate(model.vertices):
		objInfo = model.objects[objIndex]
		
		useMtrl = model.useMtrl[objIndex]
		mtrlIndex = model.mtrlIndex[objIndex]
		faces = model.faces[objIndex]
		texcoord = model.texcoords[objIndex]
		
		#Mesh
		me = bpy.data.meshes.new(objInfo.name)
		obj = bpy.data.objects.new(me.name, me)
		obj.select = True
		objects.append(obj)
		
		scene.objects.link(obj)
		scene.objects.active = obj
		bpy.ops.object.mode_set(mode="OBJECT")
		
		#Tree
		if(len(tree) > objInfo.depth):
			tree[objInfo.depth] = [objInfo, obj]
		else:
			tree.append([objInfo, obj])
		
		if(objInfo.depth > 0):
			tree[objInfo.depth - 1][0].children.append([objInfo, obj])
			objInfo.parent = tree[objInfo.depth - 1][0]
		
		#Use Material
		mtrlMap = {}
		
		for i, mtrlID in enumerate(useMtrl):
			me.materials.append(materials[mtrlID][0])
			mtrlMap[mtrlID] = i
		
		me.vertices.add(len(vertices))
		me.tessfaces.add(len(faces))
		
		for vertexIndex, pos in enumerate(vertices):
			me.vertices[vertexIndex].co = transformMatrix * pos
			
		for faceIndex, face in enumerate(faces):
			for index, value in enumerate(face):
				me.tessfaces[faceIndex].vertices_raw[index] = value
				me.tessfaces[faceIndex].use_smooth = config.smooth
				
		#Texcoord
		me.uv_textures.new()
		uv_faces = me.tessface_uv_textures[0]
		
		for i in range(len(uv_faces.data)):
			me.tessfaces[i].material_index = mtrlMap[mtrlIndex[i]]
			img = materials[mtrlIndex[i]][1]
			
			if(img):
				uv = uv_faces.data[i]
				uv.image = img
				
				uv.uv1 = [texcoord[i][0].x, 1 - texcoord[i][0].y]
				uv.uv2 = [texcoord[i][1].x, 1 - texcoord[i][1].y]
				uv.uv3 = [texcoord[i][2].x, 1 - texcoord[i][2].y]
				
				if(len(texcoord[i]) > 3):
					uv.uv4 = [texcoord[i][3][0], 1 - texcoord[i][3][1]]
					
		#Update
		me.validate()
		me.update()
		
	#Remove Doubles
	if config.remove_doubles:
		for obj in objects:
			obj.select = True
			scene.objects.active = obj
			
			bpy.ops.object.mode_set(mode='EDIT')
			bpy.ops.mesh.remove_doubles()
			bpy.ops.object.mode_set(mode='OBJECT')
			
	#Create Group
	name, ext = os.path.splitext(os.path.basename(config.path))
	group = bpy.data.objects.new(name + ext.replace(".", "_"), None)
	scene.objects.link(group)
	
	#Tree
	for i, obj in enumerate(model.objects):
		for child in obj.children:
			child[1].parent = objects[i]
		
		if(obj.parent == None):
			objects[i].parent = group
			
		objects[i].hide = (obj.visible == 0)
		objects[i].hide_select = (obj.lock == 1)
		
	scene.update()
	
AxisModes = []
AxisModes.append(("0", "Default", ""))
AxisModes.append(("1", "AxisY(Rotate X-90)", ""))
AxisModes.append(("2", "AxisZ(Rotate X90)", ""))

class MetasequoiaImporter(bpy.types.Operator):
    """Import to the Metasequoia format (.mqo)"""

    bl_idname = "import.metasequoia"
    bl_label = "Import Metasequoia"
    filename_ext = ".mqo"
    filter_glob = StringProperty(default="*.mqo", options={'HIDDEN'})
    
    filepath = StringProperty()
    filename = StringProperty()
    directory = StringProperty()
    
    #Option
    ImportScale = FloatProperty(name="Scale", description="Change of scale value.", min=0.01, max=100, soft_min=0.01, soft_max=10, default=def_scale)
    ImportAxis = EnumProperty(name="Upper Axis", description="Setting of the above axis.", items=AxisModes, default=str(def_axis))
    ImportMirror = BoolProperty(name="Mirror", description="Reflection of specular separation.", default=def_mirror)
    ImportSmooth = BoolProperty(name="Smooth", description="Smoothing of surface.", default= def_smooth)
    ImportShadeless = BoolProperty(name="Shadeless", description="Shadeless of material.", default= def_shadeless)
    ImportRemoveDoubles = BoolProperty(name="Remove Doubles", description="Remove duplicate vertices.", default= def_remove_doubles)
    ImportCycles = BoolProperty(name="Cycles", description="Cycles render.", default= def_cycles)
    
    #Config
    def execute(self, context):
        loadMQO( OptionConfig(
            context,
            self.filepath,
            self.ImportScale,
            self.ImportAxis,
            self.ImportMirror,
            self.ImportSmooth,
            self.ImportShadeless,
            self.ImportRemoveDoubles,
            self.ImportCycles) )
            
        return {"FINISHED"}

    def invoke(self, context, event):
        WindowManager = context.window_manager
        WindowManager.fileselect_add(self)
        return {"RUNNING_MODAL"}
