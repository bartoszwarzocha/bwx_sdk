/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_shader_manager.h
// Purpose:     BWX_SDK Libarary; OpenGL Shader Manager
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_SHADER_MANAGER_H_
#define _BWX_GL_SHADER_MANAGER_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <unordered_map>
#include <string>
#include <memory>
#include <iostream>
#include <initializer_list>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "bwx_gl_shader.h"
#include "bwx_gl_resource_manager.h"

namespace bwx_sdk {

#define bwxGL_SHADER_VERTEX_SUFIX "_v" ///< Vertex shader sufix
#define bwxGL_SHADER_FRAGMENT_SUFIX "_f" ///< Fragment shader sufix
#define bwxGL_SHADER_GEOMETRY_SUFIX "_g" ///< Geometry shader sufix
#define bwxGL_SHADER_TESS_CONTROL_SUFIX "_tc" ///< Tessellation control shader sufix
#define bwxGL_SHADER_TESS_EVAL_SUFIX "_te" ///< Tessellation evaluation shader sufix
#define bwxGL_SHADER_COMPUTE_SUFIX "_c" ///< Compute shader sufix

	class bwxGLShaderManager : public bwxGLResourceManager<bwxGLShader> {
	public:
		static bwxGLShaderManager& GetInstance();

		void AddShader(const std::string& name, bwxGLShader& shader);

		GLuint LoadShader(const std::string& name, const std::string& source, bwxGL_SHADER_TYPE type, bool fromFile = false);

		/*
			shaderManager.LoadShaders("basic",
			{
				{bwx_sdk::bwxGL_SHADER_TYPE::SHADER_VERTEX, "vertex_shader.glsl"},
				{bwx_sdk::bwxGL_SHADER_TYPE::SHADER_FRAGMENT, "fragment_shader.glsl"}
			});
		*/
		GLuint LoadShaders(const std::string& name, const std::initializer_list<std::pair<bwxGL_SHADER_TYPE, std::string>>& shaders, bool fromFile = false);

		GLuint LoadShaders(const std::string& name, const std::string& vertex, const std::string& fragment);
		GLuint LoadShaders(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& geometry);
		GLuint LoadShaders(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval);
		GLuint LoadShaders(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval, const std::string& geometry);

		GLuint LoadShadersFromFiles(const std::string& name, const std::string& vertex, const std::string& fragment);
		GLuint LoadShadersFromFiles(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& geometry);
		GLuint LoadShadersFromFiles(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval);
		GLuint LoadShadersFromFiles(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval, const std::string& geometry);

		GLuint GetShaderID(const std::string& name) { return m_resources[name]->GetID(); }
		bwxGLShader GetShader(const std::string& name);
		std::shared_ptr<bwxGLShader> GetShaderPtr(const std::string& name);

		std::vector<GLuint> GetShaders(const std::string& name) const;
		std::vector<GLuint> GetAllShaders() const;
		std::vector<std::string> GetShaderNames() const;

		std::string GetShaderSuffix(bwxGL_SHADER_TYPE type);
		
		void Clear();

		/**
		* @brief Set overwrite flag - if true, the shader will be overwritten
		* 
		* @param overwrite - overwrite flag
		*/
		static void SetOverwrite(bool overwrite) { m_overwrite = overwrite; }

	private:
		bwxGLShaderManager() = default;
		~bwxGLShaderManager();

		std::string m_currentShader;

		static bool m_overwrite;
	};

	/**
	* @brief Class for handling OpenGL shader programs
	*/
	class bwxGLShaderProgramManager : public bwxGLResourceManager<bwxGLShaderProgram> {
	public:
		static bwxGLShaderProgramManager& GetInstance();

		void AddShaderProgram(const std::string& name, const bwxGLShaderProgram& program);

		/**
		* @brief Create shader program with bwxGLShaderManager shaders
		* 
		* @param programName - name of the shader program
		*/
		GLuint CreateShaderProgram(const std::string& programName);

		GLuint CreateShaderProgram(const std::string& programName, const std::initializer_list<std::pair<bwxGL_SHADER_TYPE, std::string>>& shaders, bool fromFile = false);

		GLuint CreateShaderProgram(const std::string& programName, const std::string& vertexName, const std::string& fragmentName);
		GLuint CreateShaderProgram(const std::string& programName, const std::string& vertexName, const std::string& fragmentName, const std::string& geometryName);
		GLuint CreateShaderProgram(const std::string& programName, const std::string& vertexName, const std::string& fragmentName, const std::string& tessControlName, const std::string& tessEvalName);
		GLuint CreateShaderProgram(const std::string& programName, const std::string& vertexName, const std::string& fragmentName, const std::string& tessControlName, const std::string& tessEvalName, const std::string& geometryName);

		GLuint CreateShaderProgramFromStrings(const std::string& programName, const std::string& vertex, const std::string& fragment, bool addToShaderManager = true);
		GLuint CreateShaderProgramFromStrings(const std::string& programName, const std::string& vertex, const std::string& fragment, const std::string& geometry, bool addToShaderManager = true);
		GLuint CreateShaderProgramFromStrings(const std::string& programName, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval, bool addToShaderManager = true);
		GLuint CreateShaderProgramFromStrings(const std::string& programName, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval, const std::string& geometry, bool addToShaderManager = true);

		GLuint CreateShaderProgramFromFiles(const std::string& programName, const std::string& vertex, const std::string& fragment, bool addToShaderManager = true);
		GLuint CreateShaderProgramFromFiles(const std::string& programName, const std::string& vertex, const std::string& fragment, const std::string& geometry, bool addToShaderManager = true);
		GLuint CreateShaderProgramFromFiles(const std::string& programName, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval, bool addToShaderManager = true);
		GLuint CreateShaderProgramFromFiles(const std::string& programName, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval, const std::string& geometry, bool addToShaderManager = true);
		
		GLuint GetShaderProgramID(const std::string& name) { return m_resources[name]->GetProgram(); }
		bwxGLShaderProgram GetShaderProgram(const std::string& name);
		std::shared_ptr<bwxGLShaderProgram> GetShaderProgramPtr(const std::string& name);

		std::vector<GLuint> GetAllShaderPrograms() const;
		std::vector<std::string> GetShaderProgramNames() const;

		void UseShaderProgram(const std::string& programName);
		void UnuseShaderProgram();
		void ReleaseShaderProgram(const std::string& programName);

		void Clear();

	private:
		bwxGLShaderProgramManager() = default;
		~bwxGLShaderProgramManager();

		std::string m_currentShaderProgram;
	};

}

#endif