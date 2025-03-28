/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_shader.h
// Purpose:     BWX_SDK Libarary; OpenGL Shader
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_SHADER_H_
#define _BWX_GL_SHADER_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <string>
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "bwx_gl_resource_manager.h"

#define bwxGL_SHADER_EMPTY 0
#define bwxGL_SHADER_PROGRAM_EMPTY 0

namespace bwx_sdk {

	enum bwxGL_SHADER_TYPE : GLenum {
		SHADER_VERTEX,
		SHADER_TESS_CONTROL,
		SHADER_TESS_EVALUATION,
		SHADER_GEOMETRY,
		SHADER_FRAGMENT,
		SHADER_COMPUTE
	};

	/**
	* @brief Class for handling OpenGL shaders
	*/
	class bwxGLShader : public bwxGLResource
	{
	public:

		bwxGLShader();
		bwxGLShader(bwxGL_SHADER_TYPE type, const std::string& source, bool fromFile = false);
		~bwxGLShader();

		bool LoadShader(bwxGL_SHADER_TYPE type, const std::string& source, bool fromFile = false);
		void AttachToProgram(GLuint program);
		
		void Bind() const override;
		void Unbind() const override;
		void Release() override;
		void Unload() override;
		void Delete() override;

		GLuint GetID() const { return m_id; }

	private:
		GLuint m_id;
	};
    
    // Helper functions for uniform handling
	inline void glUniformHelper(GLint location, GLfloat v0) { glUniform1f(location, v0); }
	inline void glUniformHelper(GLint location, GLfloat v0, GLfloat v1) { glUniform2f(location, v0, v1); }
	inline void glUniformHelper(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) { glUniform3f(location, v0, v1, v2); }
	inline void glUniformHelper(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) { glUniform4f(location, v0, v1, v2, v3); }
    
    inline void glUniformHelper(GLint location, const glm::vec2& v) { glUniform2f(location, v.x, v.y); }
    inline void glUniformHelper(GLint location, const glm::vec3& v) { glUniform3f(location, v.x, v.y, v.z); }
    inline void glUniformHelper(GLint location, const glm::vec4& v) { glUniform4f(location, v.x, v.y, v.z, v.w); }

	inline void glUniformHelper(GLint location, GLint v0) { glUniform1i(location, v0); }
	inline void glUniformHelper(GLint location, GLint v0, GLint v1) { glUniform2i(location, v0, v1); }
	inline void glUniformHelper(GLint location, GLint v0, GLint v1, GLint v2) { glUniform3i(location, v0, v1, v2); }
	inline void glUniformHelper(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) { glUniform4i(location, v0, v1, v2, v3); }

	inline void glUniformHelper(GLint location, const glm::ivec2& v) { glUniform2i(location, v.x, v.y); }
	inline void glUniformHelper(GLint location, const glm::ivec3& v) { glUniform3i(location, v.x, v.y, v.z); }
	inline void glUniformHelper(GLint location, const glm::ivec4& v) { glUniform4i(location, v.x, v.y, v.z, v.w); }
    
    inline void glUniformHelper(GLint location, const glm::mat4& mat) { glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat)); }

	// Helper functions for attribute handling
	inline void glVertexAttribHelper(GLint location, GLfloat v0) { glVertexAttrib1f(location, v0); }
	inline void glVertexAttribHelper(GLint location, GLfloat v0, GLfloat v1) { glVertexAttrib2f(location, v0, v1); }
	inline void glVertexAttribHelper(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) { glVertexAttrib3f(location, v0, v1, v2); }
	inline void glVertexAttribHelper(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) { glVertexAttrib4f(location, v0, v1, v2, v3); }

	inline void glVertexAttribHelper(GLint location, GLint v0) { glVertexAttribI1i(location, v0); }
	inline void glVertexAttribHelper(GLint location, GLint v0, GLint v1) { glVertexAttribI2i(location, v0, v1); }
	inline void glVertexAttribHelper(GLint location, GLint v0, GLint v1, GLint v2) { glVertexAttribI3i(location, v0, v1, v2); }
	inline void glVertexAttribHelper(GLint location, GLint v0, GLint v1, GLint v2, GLint v3) { glVertexAttribI4i(location, v0, v1, v2, v3); }

	/**
	* @brief Class for handling OpenGL shader programs
	*/
	class bwxGLShaderProgram : public bwxGLResource
	{
	public:
		bwxGLShaderProgram();
		~bwxGLShaderProgram();

		void AttachShader(const bwxGLShader& shader);
		void AttachShader(const GLuint& shaderId);
		
		bool Link();
		
		void Bind() const override;
		void Unbind() const override;
		void Delete() override;
		void Unload() override;
		void Release() override;

		GLuint GetProgram() const { return m_program; }

		void AddUniform(const std::string& name);
		void AddUniforms(const std::vector<std::string>& names);
		void AddAttribute(const std::string& name);
		void AddAttributes(const std::vector<std::string>& names);

		std::unordered_map<std::string, GLint> GetUniformCache() const { return m_uniformCache; }
		std::unordered_map<std::string, GLint> GetAttributeCache() const { return m_attributeCache; }

		std::unordered_map<std::string, GLint> GetProgramUniforms();
		std::unordered_map<std::string, GLint> GetProgramAttributes();

		template<typename... Args>
		void SetUniform(const std::string& name, Args... args)
		{
			GLint location = GetUniformLocation(name);
			if (location != -1) {
				if (m_program != 0) {
					glUniformHelper(location, args...);
				}
				else {
					std::cerr << "Error: Cannot set uniform '" << name << "' because no shader program is in use." << std::endl;
				}
			}
		}

		template<typename... Args>
		void SetUniforms(const std::unordered_map<std::string, Args...>& uniforms)
		{
			for (const auto& [name, value] : uniforms) {
				SetUniform(name, value);
			}
		}

		template<typename... Args>
		void SetAttribute(const std::string& name, Args... args)
		{
			GLint location = GetAttributeLocation(name);
			if (location != -1) {
				if (m_program != 0) {
					glVertexAttribHelper(location, args...);
				}
				else {
					std::cerr << "Error: Cannot set attribute '" << name << "' because no shader program is in use." << std::endl;
				}
			}
		}

		template<typename... Args>
		void SetAttributes(const std::unordered_map<std::string, Args...>& attributes)
		{
			for (const auto& [name, value] : attributes) {
				SetAttribute(name, value);
			}
		}

		GLuint CreateUBO(GLsizeiptr size, GLuint bindingPoint, const void* data = nullptr);

	private:
		GLint GetUniformLocation(const std::string& name);
		GLint GetAttributeLocation(const std::string& name);
		std::unordered_map<std::string, GLint> m_uniformCache;
		std::unordered_map<std::string, GLint> m_attributeCache;

		GLuint m_program;
	};

} // namespace bwx_sdk

#endif
