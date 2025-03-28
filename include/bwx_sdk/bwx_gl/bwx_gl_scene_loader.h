/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_scene_loader.h
// Purpose:     BWX_SDK Libarary; OpenGL Scene Loader
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.


#ifndef _BWX_GL_SCENE_LOADER_H_
#define _BWX_GL_SCENE_LOADER_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <vector>
#include <iostream>
#include <string>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "bwx_gl_model.h"
#include "bwx_gl_mesh.h"
//#include "bwx_gl_camera.h"
//#include "bwx_gl_light.h"
#include "bwx_gl_scene.h"
#include "bwx_gl_material.h"

namespace bwx_sdk
{

#define bwxGL_SCENE_LOADER_CAMERAS 0x00000001
#define bwxGL_SCENE_LOAER_LIGHTS 0x00000002
#define bwxGL_SCENE_LOADER_MODELS 0x00000004
#define bwxGL_SCENE_LOADER_ANIMATIONS 0x00000008

#define bwxGL_SCENE_LOADER_GEN_SHADERS 0x00080000
#define bwxGL_SCENE_LOADER_CONV_BLENDER_COORDS 0x80000000

	class bwxGLSceneLoader
	{
	public:

		bwxGLSceneLoader();

		virtual ~bwxGLSceneLoader() {}

		bool Load(const std::string& file, std::shared_ptr<bwxGLScene> scene, unsigned int flags = bwxGL_SCENE_LOADER_MODELS);
		
		bool LoadCameras(const std::string& file, std::shared_ptr<bwxGLScene> scene);

		bool LoadLights(const std::string& file, std::shared_ptr<bwxGLScene> scene);

		bool LoadModels(const std::string& file, std::shared_ptr<bwxGLScene> scene);

		bool LoadModelsWithAnimations(const std::string& file, std::shared_ptr<bwxGLScene> scene);

		void SetAssimpFlags(unsigned int flags) { this->m_assimpFlags = flags; }

		unsigned int GetAssimpFlags() { return m_assimpFlags; }

		const aiScene* GetAssimpScene() { return m_assimpScene; }

		void ConvertMat33(const aiMatrix3x3& from, glm::mat3& to);

		void ConvertMat44(const aiMatrix4x4& from, glm::mat4& to);

		void SetGenerateShaders(bool generate) { this->m_generateShaders = generate; }

		bool GetGenerateShaders() { return m_generateShaders; }

		void SetConvertBlenderCoords(bool convert) { this->m_convertBlenderCoords = convert; }
		
		bool GetConvertBlenderCoords() { return m_convertBlenderCoords; }

		// Assimp flags
		inline void SetCalcTangentSpace(bool calc) { this->m_assimpFlags |= aiProcess_CalcTangentSpace; }
		inline void SetJoinIdenticalVertices(bool join) { this->m_assimpFlags |= aiProcess_JoinIdenticalVertices; }
		inline void SetMakeLeftHanded(bool left) { this->m_assimpFlags |= aiProcess_MakeLeftHanded; }
		inline void SetTriangulate(bool tri) { this->m_assimpFlags |= aiProcess_Triangulate; }
		inline void SetRemoveComponent(bool remove) { this->m_assimpFlags |= aiProcess_RemoveComponent; }
		inline void SetGenNormals(bool gen) { this->m_assimpFlags |= aiProcess_GenNormals; }
		inline void SetGenSmoothNormals(bool gen) { this->m_assimpFlags |= aiProcess_GenSmoothNormals; }
		inline void SetSplitLargeMeshes(bool split) { this->m_assimpFlags |= aiProcess_SplitLargeMeshes; }
		inline void SetPreTransformVertices(bool pre) { this->m_assimpFlags |= aiProcess_PreTransformVertices; }
		inline void SetLimitBoneWeights(bool limit) { this->m_assimpFlags |= aiProcess_LimitBoneWeights; }
		inline void SetValidateDataStructure(bool validate) { this->m_assimpFlags |= aiProcess_ValidateDataStructure; }
		inline void SetImproveCacheLocality(bool improve) { this->m_assimpFlags |= aiProcess_ImproveCacheLocality; }
		inline void SetRemoveRedundantMaterials(bool remove) { this->m_assimpFlags |= aiProcess_RemoveRedundantMaterials; }
		inline void SetFixInfacingNormals(bool fix) { this->m_assimpFlags |= aiProcess_FixInfacingNormals; }
		inline void SetPopulateArmatureData(bool populate) { this->m_assimpFlags |= aiProcess_PopulateArmatureData; }
		inline void SetSortByPType(bool sort) { this->m_assimpFlags |= aiProcess_SortByPType; }
		inline void SetFindDegenerates(bool find) { this->m_assimpFlags |= aiProcess_FindDegenerates; }
		inline void SetFindInvalidData(bool find) { this->m_assimpFlags |= aiProcess_FindInvalidData; }
		inline void SetGenUVCoords(bool gen) { this->m_assimpFlags |= aiProcess_GenUVCoords; }
		inline void SetTransformUVCoords(bool transform) { this->m_assimpFlags |= aiProcess_TransformUVCoords; }
		inline void SetFindInstances(bool find) { this->m_assimpFlags |= aiProcess_FindInstances; }
		inline void SetOptimizeMeshes(bool optimize) { this->m_assimpFlags |= aiProcess_OptimizeMeshes; }
		inline void SetOptimizeGraph(bool optimize) { this->m_assimpFlags |= aiProcess_OptimizeGraph; }
		inline void SetFlipUVs(bool flip) { this->m_assimpFlags |= aiProcess_FlipUVs; }
		inline void SetFlipWindingOrder(bool flip) { this->m_assimpFlags |= aiProcess_FlipWindingOrder; }
		inline void SetSplitByBoneCount(bool split) { this->m_assimpFlags |= aiProcess_SplitByBoneCount; }
		inline void SetDebone(bool debone) { this->m_assimpFlags |= aiProcess_Debone; }
		inline void SetGlobalScale(bool scale) { this->m_assimpFlags |= aiProcess_GlobalScale; }
		inline void SetEmbedTextures(bool embed) { this->m_assimpFlags |= aiProcess_EmbedTextures; }
		inline void SetForceGenNormals(bool force) { this->m_assimpFlags |= aiProcess_ForceGenNormals; }
		inline void SetDropNormals(bool drop) { this->m_assimpFlags |= aiProcess_DropNormals; }
		inline void SetGenBoundingBoxes(bool gen) { this->m_assimpFlags |= aiProcess_GenBoundingBoxes; }

		inline void ResetAssimpFlags() { this->m_assimpFlags = 0; }

		// Assimp presets
		inline void SetPresetTargetRealtimeFast() { this->m_assimpFlags |= aiProcessPreset_TargetRealtime_Fast; }
		inline void SetPresetTargetRealtimeQuality() { this->m_assimpFlags |= aiProcessPreset_TargetRealtime_Quality; }
		inline void SetPresetTargetRealtimeMaxQuality() { this->m_assimpFlags |= aiProcessPreset_TargetRealtime_MaxQuality; }

	private:
		void ProcessCameras(std::shared_ptr<bwxGLScene> scene);

		void ProcessLights(std::shared_ptr<bwxGLScene> scene);

		void ProcessModels(std::shared_ptr<bwxGLScene> scene, bool animation);

		void ProcessAssimpNodes(std::shared_ptr<bwxGLScene> scene, std::shared_ptr<bwxGLNode> parent, aiNode* node, bool animation);

		std::shared_ptr<bwxGLMaterial> ProcessMaterial(std::shared_ptr<bwxGLModel> model, aiMesh* ai_m);

		void ProcessTexture(bwxGL_TEXTURE_TYPE type, std::shared_ptr<bwxGLMaterial> material, std::shared_ptr<bwxGLModel> model, const std::string& file);

		const aiScene* m_assimpScene;
		unsigned int m_assimpFlags;

		bool m_generateShaders;
		bool m_convertBlenderCoords;
	};
}

#endif // _BWX_GL_SCENE_LOADER_H_
