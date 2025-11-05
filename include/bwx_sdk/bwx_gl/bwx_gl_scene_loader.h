/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_scene_loader.h
// Purpose:     BWX_SDK Libarary; OpenGL Scene Loader
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

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

#define bwxGL_SCENE_LOADER_CAMERAS 0x00000001 ///< Load cameras
#define bwxGL_SCENE_LOAER_LIGHTS 0x00000002 ///< Load lights
#define bwxGL_SCENE_LOADER_MODELS 0x00000004 ///< Load models
#define bwxGL_SCENE_LOADER_ANIMATIONS 0x00000008 ///< Load animations

#define bwxGL_SCENE_LOADER_GEN_SHADERS 0x00080000 ///< Generate shaders
#define bwxGL_SCENE_LOADER_CONV_BLENDER_COORDS 0x80000000 ///< Convert Blender coordinates

	/**
	 * @brief OpenGL Scene Loader
	 */
	class bwxGLSceneLoader
	{
	public:

		/**
		 * @brief Construct a new bwxGLSceneLoader object
		 */
		bwxGLSceneLoader();

		/**
		 * @brief Destroy the bwxGLSceneLoader object
		 */
		virtual ~bwxGLSceneLoader() {}

		/**
		 * @brief Load scene from file
		 *
		 * @param file File path
		 * @param scene Scene object
		 * @param flags Flags
		 * @return true if successful
		 * @return false if failed
		 */
		bool Load(const std::string& file, std::shared_ptr<bwxGLScene> scene, unsigned int flags = bwxGL_SCENE_LOADER_MODELS);
		
		/**
		 * @brief Load cameras from file
		 *
		 * @param file File path
		 * @param scene Scene object
		 * @return true if successful
		 * @return false if failed
		 */
		bool LoadCameras(const std::string& file, std::shared_ptr<bwxGLScene> scene);

		/**
		 * @brief Load lights from file
		 *
		 * @param file File path
		 * @param scene Scene object
		 * @return true if successful
		 * @return false if failed
		 */
		bool LoadLights(const std::string& file, std::shared_ptr<bwxGLScene> scene);

		/**
		 * @brief Load models from file
		 *
		 * @param file File path
		 * @param scene Scene object
		 * @return true if successful
		 * @return false if failed
		 */
		bool LoadModels(const std::string& file, std::shared_ptr<bwxGLScene> scene);

		/**
		 * @brief Load models with animations from file
		 *
		 * @param file File path
		 * @param scene Scene object
		 * @return true if successful
		 * @return false if failed
		 */
		bool LoadModelsWithAnimations(const std::string& file, std::shared_ptr<bwxGLScene> scene);


		/**
		 * @brief Set Assimp flags
		 */
		void SetAssimpFlags(unsigned int flags) { this->m_assimpFlags = flags; }

		/**
		 * @brief Get Assimp flags
		 */
		unsigned int GetAssimpFlags() { return m_assimpFlags; }

		/**
		 * @brief Get Assimp scene
		 */
		const aiScene* GetAssimpScene() { return m_assimpScene; }

		
		/**
		 * @brief Convert Assimp matrix to glm::mat3
		 */
		void ConvertMat33(const aiMatrix3x3& from, glm::mat3& to);

		/**
		 * @brief Convert Assimp matrix to glm::mat4
		 */
		void ConvertMat44(const aiMatrix4x4& from, glm::mat4& to);


		/**
		 * @brief Set generate shaders flag
		 */
		void SetGenerateShaders(bool generate) { this->m_generateShaders = generate; }

		/**
		 * @brief Get generate shaders flag
		 */
		bool GetGenerateShaders() { return m_generateShaders; }

		/**
		 * @brief Set convert Blender coordinates flag
		 */
		void SetConvertBlenderCoords(bool convert) { this->m_convertBlenderCoords = convert; }
		
		/**
		 * @brief Get convert Blender coordinates flag
		 */
		bool GetConvertBlenderCoords() { return m_convertBlenderCoords; }

		// Assimp flags
		inline void SetCalcTangentSpace(bool calc) { this->m_assimpFlags |= aiProcess_CalcTangentSpace; } ///< Generate tangents and bitangents
		inline void SetJoinIdenticalVertices(bool join) { this->m_assimpFlags |= aiProcess_JoinIdenticalVertices; } ///< Join identical vertices/ optimize indexing
		inline void SetMakeLeftHanded(bool left) { this->m_assimpFlags |= aiProcess_MakeLeftHanded; } ///< Convert everything to D3D left handed space
		inline void SetTriangulate(bool tri) { this->m_assimpFlags |= aiProcess_Triangulate; } ///< Make sure we're triangles
		inline void SetRemoveComponent(bool remove) { this->m_assimpFlags |= aiProcess_RemoveComponent; } ///< Remove some parts of the data
		inline void SetGenNormals(bool gen) { this->m_assimpFlags |= aiProcess_GenNormals; } ///< Generate normals if there are none
		inline void SetGenSmoothNormals(bool gen) { this->m_assimpFlags |= aiProcess_GenSmoothNormals; } ///< Generate smooth normals
		inline void SetSplitLargeMeshes(bool split) { this->m_assimpFlags |= aiProcess_SplitLargeMeshes; } ///< Split large, unrenderable meshes into submeshes
		inline void SetPreTransformVertices(bool pre) { this->m_assimpFlags |= aiProcess_PreTransformVertices; } ///< Transforms scene hierarchy into one root with geometry
		inline void SetLimitBoneWeights(bool limit) { this->m_assimpFlags |= aiProcess_LimitBoneWeights; } ///< Limit bone weights to 4 per vertex
		inline void SetValidateDataStructure(bool validate) { this->m_assimpFlags |= aiProcess_ValidateDataStructure; } ///< Validate data structure
		inline void SetImproveCacheLocality(bool improve) { this->m_assimpFlags |= aiProcess_ImproveCacheLocality; } ///< Improve cache locality of the output vertices
		inline void SetRemoveRedundantMaterials(bool remove) { this->m_assimpFlags |= aiProcess_RemoveRedundantMaterials; } ///< Remove redundant materials
		inline void SetFixInfacingNormals(bool fix) { this->m_assimpFlags |= aiProcess_FixInfacingNormals; } ///< Fix infacing normals
		inline void SetPopulateArmatureData(bool populate) { this->m_assimpFlags |= aiProcess_PopulateArmatureData; } ///< Populate armature data
		inline void SetSortByPType(bool sort) { this->m_assimpFlags |= aiProcess_SortByPType; } ///< Sort meshes by primitive type
		inline void SetFindDegenerates(bool find) { this->m_assimpFlags |= aiProcess_FindDegenerates; } ///< Find degenerate primitives and remove them
		inline void SetFindInvalidData(bool find) { this->m_assimpFlags |= aiProcess_FindInvalidData; } ///< Find invalid data (e.g. invalid normal vector)
		inline void SetGenUVCoords(bool gen) { this->m_assimpFlags |= aiProcess_GenUVCoords; } ///< Convert spherical, cylindrical, box and planar mapping to proper
		inline void SetTransformUVCoords(bool transform) { this->m_assimpFlags |= aiProcess_TransformUVCoords; } ///< Preprocess UV transformations (scaling, translation ...)
		inline void SetFindInstances(bool find) { this->m_assimpFlags |= aiProcess_FindInstances; } ///< Detect and join identical vertex data sets
		inline void SetOptimizeMeshes(bool optimize) { this->m_assimpFlags |= aiProcess_OptimizeMeshes; } ///< Optimize mesh data
		inline void SetOptimizeGraph(bool optimize) { this->m_assimpFlags |= aiProcess_OptimizeGraph; } ///< Optimize the scene hierarchy
		inline void SetFlipUVs(bool flip) { this->m_assimpFlags |= aiProcess_FlipUVs; } ///< Flip UV coordinates along the y-axis
		inline void SetFlipWindingOrder(bool flip) { this->m_assimpFlags |= aiProcess_FlipWindingOrder; } ///< Flip face winding order
		inline void SetSplitByBoneCount(bool split) { this->m_assimpFlags |= aiProcess_SplitByBoneCount; } ///< Split meshes with too many bones
		inline void SetDebone(bool debone) { this->m_assimpFlags |= aiProcess_Debone; } ///< Remove bones
		inline void SetGlobalScale(bool scale) { this->m_assimpFlags |= aiProcess_GlobalScale; } ///< Scale the whole scene
		inline void SetEmbedTextures(bool embed) { this->m_assimpFlags |= aiProcess_EmbedTextures; } ///< Embed textures into the model
		inline void SetForceGenNormals(bool force) { this->m_assimpFlags |= aiProcess_ForceGenNormals; } ///< Force generate normals
		inline void SetDropNormals(bool drop) { this->m_assimpFlags |= aiProcess_DropNormals; } ///< Drop normals
		inline void SetGenBoundingBoxes(bool gen) { this->m_assimpFlags |= aiProcess_GenBoundingBoxes; } ///< Generate bounding boxes

		inline void ResetAssimpFlags() { this->m_assimpFlags = 0; } ///< Reset Assimp flags

		// Assimp presets
		inline void SetPresetTargetRealtimeFast() { this->m_assimpFlags |= aiProcessPreset_TargetRealtime_Fast; } ///< Default postprocess configuration optimizing the data for real-time rendering.
		inline void SetPresetTargetRealtimeQuality() { this->m_assimpFlags |= aiProcessPreset_TargetRealtime_Quality; } ///< Default postprocess configuration optimizing the data for real-time rendering.
		inline void SetPresetTargetRealtimeMaxQuality() { this->m_assimpFlags |= aiProcessPreset_TargetRealtime_MaxQuality; } ///< Default postprocess configuration optimizing the data for real-time rendering.

	private:
		/**
		 * @brief Process cameras
		 *
		 * @param scene Scene object
		 */
		void ProcessCameras(std::shared_ptr<bwxGLScene> scene);

		/**
		 * @brief Process lights
		 *
		 * @param scene Scene object
		 */
		void ProcessLights(std::shared_ptr<bwxGLScene> scene);

		/**
		 * @brief Process models
		 *
		 * @param scene Scene object
		 * @param animation Animation flag
		 */
		void ProcessModels(std::shared_ptr<bwxGLScene> scene, bool animation);

		/**
		 * @brief Process Assimp nodes
		 *
		 * @param scene Scene object
		 * @param parent Parent node
		 * @param node Assimp node
		 * @param animation Animation flag
		 */
		void ProcessAssimpNodes(std::shared_ptr<bwxGLScene> scene, std::shared_ptr<bwxGLNode> parent, aiNode* node, bool animation);

		/**
		 * @brief Process Assimp meshes
		 *
		 * @param model Model object
		 * @param ai_m Assimp mesh
		 * @return std::shared_ptr<bwxGLMesh> Mesh object
		 */
		std::shared_ptr<bwxGLMaterial> ProcessMaterial(std::shared_ptr<bwxGLModel> model, aiMesh* ai_m);

		/**
		 * @brief Process Assimp textures
		 *
		 * @param type Texture type
		 * @param material Material object
		 * @param model Model object
		 * @param file File path
		 */
		void ProcessTexture(bwxGL_TEXTURE_TYPE type, std::shared_ptr<bwxGLMaterial> material, std::shared_ptr<bwxGLModel> model, const std::string& file);

		const aiScene* m_assimpScene; ///< Assimp scene
		unsigned int m_assimpFlags; ///< Assimp flags

		bool m_generateShaders; ///< Generate shaders flag
		bool m_convertBlenderCoords; ///< Convert Blender coordinates flag
	};
}

#endif // _BWX_GL_SCENE_LOADER_H_
