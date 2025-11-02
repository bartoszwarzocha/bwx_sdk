/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_scene_loader.cpp
// Purpose:     BWX_SDK Libarary; OpenGL Scene Loader
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <bwx_sdk/bwx_core/bwx_string.h>
#include <bwx_sdk/bwx_gl/bwx_gl_scene_loader.h>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <iostream>
#include <string>
#include <memory>
#include <span>
#include <vector>
#include <algorithm>
#include <cstring>
#include <execution>

namespace bwx_sdk {

	bwxGLSceneLoader::bwxGLSceneLoader() : m_assimpScene(NULL), m_convertBlenderCoords(false)
	{
		this->m_assimpFlags =
			//aiProcess_CalcTangentSpace |							// Generate tangents and bitangents
			aiProcess_JoinIdenticalVertices |						// Join identical vertices/ optimize indexing
			//aiProcess_MakeLeftHanded |							// Convert everything to D3D left handed space
			aiProcess_Triangulate |									// Make sure we're triangles
			//aiProcess_RemoveComponent |							// Remove some parts of the data
			//aiProcess_GenNormals |								// Generate normals if there are none
			aiProcess_GenSmoothNormals |							// Generate smooth normals
			//aiProcess_SplitLargeMeshes |							// Split large, unrenderable meshes into submeshes
			//aiProcess_PreTransformVertices |						// Transforms scene hierarchy into one root with geometry
			//aiProcess_LimitBoneWeights |							// Limit bone weights to 4 per vertex
			//aiProcess_ValidateDataStructure |						// Validate data structure
			//aiProcess_ImproveCacheLocality |						// Improve cache locality of the output vertices
			//aiProcess_RemoveRedundantMaterials |					// Remove redundant materials
			//aiProcess_FixInfacingNormals |						// Fix infacing normals
			//aiProcess_PopulateArmatureData |						// Populate armature data
			//aiProcess_SortByPType |								// Sort meshes by primitive type
			//aiProcess_FindDegenerates |							// Find degenerate primitives and remove them
			//aiProcess_FindInvalidData |							// Find invalid data (e.g. invalid normal vector)
			//aiProcess_GenUVCoords |								// Convert spherical, cylindrical, box and planar mapping to proper
			//aiProcess_TransformUVCoords |							// Preprocess UV transformations (scaling, translation ...)
			//aiProcess_FindInstances |								// Detect and join identical vertex data sets
			aiProcess_OptimizeMeshes |								// Optimize mesh data
			//aiProcess_OptimizeGraph |								// Optimize the scene hierarchy
			//aiProcess_FlipUVs |									// Flip UV coordinates along the y-axis
			//aiProcess_FlipWindingOrder |							// Flip face winding order
			//aiProcess_SplitByBoneCount |							// Split meshes with too many bones
			//aiProcess_Debone |									// Remove bones
			//aiProcess_GlobalScale |								// Scale the whole scene
			//aiProcess_EmbedTextures |								// Embed textures into the model
			//aiProcess_ForceGenNormals |							// Force generate normals
			//aiProcess_DropNormals |								// Drop normals
			aiProcess_GenBoundingBoxes;								// Generate bounding boxes
	}

	void bwxGLSceneLoader::ConvertMat33(const aiMatrix3x3& from, glm::mat3& to)
	{
		std::memcpy(glm::value_ptr(to), &from, sizeof(aiMatrix3x3));

		//to = glm::mat3(
		//	from.a1, from.b1, from.c1,
		//	from.a2, from.b2, from.c2,
		//	from.a3, from.b3, from.c3
		//);

		// Old approach...
		//to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3;
		//to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3;
		//to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3;
	}

	void bwxGLSceneLoader::ConvertMat44(const aiMatrix4x4& from, glm::mat4& to)
	{
		std::memcpy(glm::value_ptr(to), &from, sizeof(aiMatrix4x4));

		//to = glm::mat4(
		//	from.a1, from.b1, from.c1, from.d1,
		//	from.a2, from.b2, from.c2, from.d2,
		//	from.a3, from.b3, from.c3, from.d3,
		//	from.a4, from.b4, from.c4, from.d4
		//);

		// Old approach...
		//to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
		//to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
		//to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
		//to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
	}

	bool bwxGLSceneLoader::Load(const std::string& file, std::shared_ptr<bwxGLScene> scene, unsigned int flags)
	{
		// TODO: Porządki w loaderze

		if (file.empty()) return false;

		Assimp::Importer importer;
		m_assimpScene = importer.ReadFile(file, m_assimpFlags);

		if (!m_assimpScene || m_assimpScene->mFlags == AI_SCENE_FLAGS_INCOMPLETE || !m_assimpScene->mRootNode)
		{
			std::cout << "Scene load error: " << importer.GetErrorString() << std::endl;
			return false;
		}

		// Tests --------------------------------------------------------------
		std::cout << "Children:   " << m_assimpScene->mRootNode->mNumChildren << std::endl;
		std::cout << "Meshes:     " << (m_assimpScene->HasMeshes() ? "YES" : " NO") << " (" << m_assimpScene->mNumMeshes << ")" << std::endl;
		std::cout << "Cameras:    " << (m_assimpScene->HasCameras() ? "YES" : " NO") << " (" << m_assimpScene->mNumCameras << ")" << std::endl;
		std::cout << "Lights:     " << (m_assimpScene->HasLights() ? "YES" : " NO") << " (" << m_assimpScene->mNumLights << ")" << std::endl;
		std::cout << "Materials:  " << (m_assimpScene->HasMaterials() ? "YES" : " NO") << " (" << m_assimpScene->mNumMaterials << ")" << std::endl;
		std::cout << "Textures:   " << (m_assimpScene->HasTextures() ? "YES" : " NO") << " (" << m_assimpScene->mNumTextures << ")" << std::endl;
		std::cout << "Animations: " << (m_assimpScene->HasAnimations() ? "YES" : " NO") << " (" << m_assimpScene->mNumAnimations << ")" << std::endl;
		std::cout << std::endl;

		for (auto child : std::span(m_assimpScene->mRootNode->mChildren, m_assimpScene->mRootNode->mNumChildren))
		{
			std::cout << "Child name  : " << child->mName.C_Str() << std::endl;
			std::cout << "Num meshes  : " << child->mNumMeshes << std::endl;
			std::cout << "Num children: " << child->mNumChildren << std::endl;
			std::cout << std::endl;
		}
		// Tests --------------------------------------------------------------

		this->m_generateShaders = (flags & bwxGL_SCENE_LOADER_GEN_SHADERS);
		this->m_convertBlenderCoords = (flags & bwxGL_SCENE_LOADER_CONV_BLENDER_COORDS);

		if (flags & bwxGL_SCENE_LOADER_CAMERAS) this->ProcessCameras(scene);
		if (flags & bwxGL_SCENE_LOAER_LIGHTS) this->ProcessLights(scene);
		if (flags & bwxGL_SCENE_LOADER_MODELS) this->ProcessModels(scene, (flags & bwxGL_SCENE_LOADER_ANIMATIONS));

		return true;
	}

	bool bwxGLSceneLoader::LoadCameras(const std::string& file, std::shared_ptr<bwxGLScene> scene)
	{
		return this->Load(file, scene, bwxGL_SCENE_LOADER_CAMERAS);
	}

	bool bwxGLSceneLoader::LoadLights(const std::string& file, std::shared_ptr<bwxGLScene> scene)
	{
		return this->Load(file, scene, bwxGL_SCENE_LOAER_LIGHTS);
	}

	bool bwxGLSceneLoader::LoadModels(const std::string& file, std::shared_ptr<bwxGLScene> scene)
	{
		return this->Load(file, scene, bwxGL_SCENE_LOADER_MODELS);
	}

	bool bwxGLSceneLoader::LoadModelsWithAnimations(const std::string& file, std::shared_ptr<bwxGLScene> scene)
	{
		return this->Load(file, scene, bwxGL_SCENE_LOADER_MODELS | bwxGL_SCENE_LOADER_ANIMATIONS);
	}

	void bwxGLSceneLoader::ProcessCameras(std::shared_ptr<bwxGLScene> scene)
	{
		if (!m_assimpScene || m_assimpScene->mNumCameras == 0) return;

		// Create a map of node names to avoid multiple iterations O(1)
		std::unordered_map<std::string, aiNode*> nodeMap;
		for (unsigned int j = 0; j < m_assimpScene->mRootNode->mNumChildren; j++)
		{
			aiNode* node = m_assimpScene->mRootNode->mChildren[j];
			nodeMap[node->mName.C_Str()] = node;
		}

		for (aiCamera* c : std::span(m_assimpScene->mCameras, m_assimpScene->mNumCameras))
		{
			/*
			auto cam = std::make_shared<bwxGLCamera>(bwxGL_CAMERA_TYPE::CAMERA_TYPE_FPP);
			cam->SetName(c->mName.C_Str());

			cam->SetPosition(glm::vec3(c->mPosition.x, c->mPosition.y, c->mPosition.z));
			cam->SetRotation(glm::vec3(c->mLookAt.x, c->mLookAt.y, c->mLookAt.z));
			cam->SetProjectionPerspective(glm::degrees(c->mHorizontalFOV), c->mAspect, c->mClipPlaneNear, c->mClipPlaneFar);

			// Get the camera node from the map O(1)
			auto it = nodeMap.find(c->mName.C_Str());
			if (it != nodeMap.end())
			{
				glm::mat4 trans;
				ConvertMat44(it->second->mTransformation, trans);
				cam->SetTransformMatrix(trans);
			}

			scene->AddCamera(cam);
			*/
		}
	}

	void bwxGLSceneLoader::ProcessLights(std::shared_ptr<bwxGLScene> scene)
	{
		if (!m_assimpScene || m_assimpScene->mNumLights == 0) return;

		// Create a map of node names to avoid multiple iterations O(1)
		std::unordered_map<std::string, aiNode*> nodeMap;
		for (unsigned int j = 0; j < m_assimpScene->mRootNode->mNumChildren; j++)
		{
			aiNode* node = m_assimpScene->mRootNode->mChildren[j];
			nodeMap[node->mName.C_Str()] = node;
		}

		for (aiLight* l : std::span(m_assimpScene->mLights, m_assimpScene->mNumLights))
		{
			/*
			auto light = std::make_shared<bwxGLLight>(
				static_cast<bwxGL_LIGHT_TYPE>(l->mType),
				glm::vec3(l->mPosition.x, l->mPosition.y, l->mPosition.z)
			);

			light->SetName(l->mName.C_Str());
			light->SetAmbient(l->mColorAmbient.r, l->mColorAmbient.g, l->mColorAmbient.b);
			light->SetDiffuse(l->mColorDiffuse.r, l->mColorDiffuse.g, l->mColorDiffuse.b);
			light->SetObjectColor(light->GetDiffuse());
			light->SetSpecular(l->mColorSpecular.r, l->mColorSpecular.g, l->mColorSpecular.b);
			light->SetAttenuationLinear(l->mAttenuationLinear);
			light->SetAttenuationQuadric(l->mAttenuationQuadratic);
			light->SetAttenuationConstant(l->mAttenuationConstant);
			light->SetInnerCone(l->mAngleInnerCone);
			light->SetOuterCone(l->mAngleOuterCone);

			// Get the light node from the map O(1)
			auto it = nodeMap.find(l->mName.C_Str());
			if (it != nodeMap.end())
			{
				glm::mat4 trans;
				ConvertMat44(it->second->mTransformation, trans);
				light->SetTransformMatrix(trans);
			}

			scene->AddLight(light);
			*/
		}
	}

	void bwxGLSceneLoader::ProcessModels(std::shared_ptr<bwxGLScene> scene, bool animation)
	{
		aiNode* root = m_assimpScene->mRootNode;
		if (!root || root->mNumChildren == 0) return;  // Avoid empty scene

		std::span<aiNode*> children(root->mChildren, root->mNumChildren);
		for (aiNode* child : children)
		{
			this->ProcessAssimpNodes(scene, scene->GetRoot(), child, animation);
		}

		//if (this->generate_shaders) scene->GenerateModelShaders();
	}

	void bwxGLSceneLoader::ProcessAssimpNodes(std::shared_ptr<bwxGLScene> scene, std::shared_ptr<bwxGLNode> parent, aiNode* node, bool animation)
	{
		if (node->mNumMeshes <= 0) return;

		std::shared_ptr<bwxGLModel> model_tmp = std::make_shared<bwxGLModel>(bwxGL_MODEL_TYPE::MODEL_UNDEFINED);
		//model_tmp->SetName(node->mName.C_Str());
		//model_tmp->SetParent(parent);
		//model_tmp->SetKeepParentTransform(parent != nullptr);

		glm::mat4 trans;
		ConvertMat44(node->mTransformation, trans);
		if (m_convertBlenderCoords)
		{
			trans = glm::rotate(trans, glm::radians(90.0f), glm::vec3(1.0, 0, 0));
		}
		//model_tmp->SetTransformMatrix(trans);

		std::unordered_map<int, std::shared_ptr<bwxGLMaterial>> materialCache;

		for (unsigned int meshIdx : std::span(node->mMeshes, node->mNumMeshes))
		{
			aiMesh* ai_m = m_assimpScene->mMeshes[meshIdx];

			std::vector<bwxGLVertex> vertices;
			vertices.reserve(ai_m->mNumVertices);

			std::vector<GLuint> indices;
			indices.reserve(ai_m->mNumFaces * 3);

			int m_style = 0x00000000;
			if (ai_m->HasNormals()) m_style |= bwxGL_MESH_NORMAL;
			if (ai_m->HasTextureCoords(0)) m_style |= bwxGL_MESH_TEX_COORD;
			if (ai_m->HasTangentsAndBitangents()) m_style |= (bwxGL_MESH_TANGENT | bwxGL_MESH_BITANGENT);
			if (ai_m->HasVertexColors(0)) m_style |= bwxGL_MESH_COLOR;
			if ((bool)ai_m->GetNumUVChannels()) m_style |= bwxGL_MESH_UV;
			if (ai_m->HasFaces()) m_style |= bwxGL_MESH_INDICES;

			std::shared_ptr<bwxGLMesh> mesh = std::make_shared<bwxGLMesh>(m_style);
			//mesh->SetName(str::bwxFormatStd("%s_%d", ai_m->mName.C_Str(), meshIdx));
			//mesh->SetParent(model_tmp);

			// Loop for vertices
			for (unsigned int i = 0; i < ai_m->mNumVertices; i++)
			{
				bwxGLVertex v;
				v.position = glm::vec3(ai_m->mVertices[i].x, ai_m->mVertices[i].y, ai_m->mVertices[i].z);
				if (ai_m->HasNormals()) v.normal = glm::vec3(ai_m->mNormals[i].x, ai_m->mNormals[i].y, ai_m->mNormals[i].z);
				vertices.emplace_back(std::move(v));
			}

			// Loop for texture coordinates
			std::for_each(ai_m->mFaces, ai_m->mFaces + ai_m->mNumFaces, [&](const aiFace& face)
				{
					indices.insert(indices.end(), face.mIndices, face.mIndices + face.mNumIndices);
				});

			if (materialCache.find(ai_m->mMaterialIndex) == materialCache.end())
				materialCache[ai_m->mMaterialIndex] = ProcessMaterial(model_tmp, ai_m);

			//mesh->SetMaterial(materialCache[ai_m->mMaterialIndex]);
			model_tmp->AddMesh(mesh);
		}

		scene->AddModel(model_tmp);

		std::vector<aiNode*> children(node->mChildren, node->mChildren + node->mNumChildren);
		std::for_each(std::execution::par, children.begin(), children.end(),
			[this, scene, model_tmp, animation](aiNode* child)
			{
				//this->ProcessAssimpNodes(scene, model_tmp, child, animation);
			});
	}

	std::shared_ptr<bwxGLMaterial> bwxGLSceneLoader::ProcessMaterial(std::shared_ptr<bwxGLModel> model, aiMesh* ai_m)
	{
		aiMaterial* mat = m_assimpScene->mMaterials[ai_m->mMaterialIndex];
		std::shared_ptr<bwxGLMaterial> material = std::make_shared<bwxGLMaterial>();

		// Lambda to get color from Assimp material
		auto getColor = [&](const char* key, int type, int idx, void (bwxGLMaterial::* setFunc)(const glm::vec4&)) {
				aiColor4D color;
				if (aiGetMaterialColor(mat, key, type, idx, &color) == aiReturn_SUCCESS)
				{
					(material.get()->*setFunc)(glm::vec4(color.r, color.g, color.b, color.a));
				}
			};

		// Get colors
		getColor(AI_MATKEY_COLOR_AMBIENT, &bwxGLMaterial::SetAmbient);
		getColor(AI_MATKEY_COLOR_DIFFUSE, &bwxGLMaterial::SetDiffuse);
		getColor(AI_MATKEY_COLOR_SPECULAR, &bwxGLMaterial::SetSpecular);

		// Get shininess
		float shininess = 0.0f;
		if (aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess) == aiReturn_SUCCESS)
		{
			material->SetShininess(shininess);
		}

		// Texture map Assimp → BWX_SDK
		const std::vector<std::pair<aiTextureType, bwxGL_TEXTURE_TYPE>> textureMappings = {
			{aiTextureType_DIFFUSE,      bwxGL_TEXTURE_TYPE::TEXTURE_DIFFUSE},
			{aiTextureType_SPECULAR,     bwxGL_TEXTURE_TYPE::TEXTURE_SPECULAR},
			{aiTextureType_AMBIENT,      bwxGL_TEXTURE_TYPE::TEXTURE_AMBIENT},
			{aiTextureType_EMISSIVE,     bwxGL_TEXTURE_TYPE::TEXTURE_EMISSIVE},
			{aiTextureType_HEIGHT,       bwxGL_TEXTURE_TYPE::TEXTURE_HEIGHT},
			{aiTextureType_NORMALS,      bwxGL_TEXTURE_TYPE::TEXTURE_NORMALS},
			{aiTextureType_SHININESS,    bwxGL_TEXTURE_TYPE::TEXTURE_SHININESS},
			{aiTextureType_OPACITY,      bwxGL_TEXTURE_TYPE::TEXTURE_OPACITY},
			{aiTextureType_DISPLACEMENT, bwxGL_TEXTURE_TYPE::TEXTURE_DISPLACEMENT},
			{aiTextureType_LIGHTMAP,     bwxGL_TEXTURE_TYPE::TEXTURE_LIGHTMAP},
			{aiTextureType_REFLECTION,   bwxGL_TEXTURE_TYPE::TEXTURE_REFLECTION}
		};

		// Process textures
		for (const auto& [assimpType, glType] : textureMappings)
		{
			aiString path;
			int texIndex = 0;
			while (mat->GetTexture(assimpType, texIndex, &path) == AI_SUCCESS)
			{
				this->ProcessTexture(glType, material, model, path.data);
				texIndex++;
			}
		}

		return material;
	}

	void bwxGLSceneLoader::ProcessTexture(bwxGL_TEXTURE_TYPE type, std::shared_ptr<bwxGLMaterial> material, std::shared_ptr<bwxGLModel> model, const std::string& file)
	{
		bwxGLTexture2DData data;
		bool skip_texture = false;

		/*
		for (unsigned int i = 0; i < model->GetAllTexturesTmp()->size(); i++)
		{
			if (model->GetAllTexturesTmp()->operator[](i).path == file)
			{
				data = model->GetAllTexturesTmp()->operator[](i);
				data.type = type; // Jedna tektura może stanowić diffuse i specular..., dlatego kopiujemy bieżący typ
				material->AddTexture(data);
				skip_texture = true;
			}
		}
		if (!skip_texture)
		{
			data.path = file;
			data.type = type;
			data.id = Texture::CreateFromFile(file);
			material->AddTexture(data);
			model->GetAllTexturesTmp()->push_back(data);
		}
		*/
	}

}
