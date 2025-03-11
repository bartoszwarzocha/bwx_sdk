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

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	class bwxGLShader
	{
	public:

		bwxGLShader();
		~bwxGLShader();

		bool LoadShader(bwxGL_SHADER_TYPE type, const std::string& source, bool fromFile = false);
		void AttachToProgram(GLuint program);
		void DeleteShader();

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

	/**
	* @brief Class for handling OpenGL shader programs
	*/
	class bwxGLShaderProgram
	{
	public:
		bwxGLShaderProgram();
		~bwxGLShaderProgram();

		void AttachShader(const bwxGLShader& shader);
		void AttachShader(const GLuint& shaderId);
		bool Link();
		void Use();
		void Release();
		void Delete();

		GLuint GetProgram() const { return m_program; }

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

		/*
			shaderProgram.SetUniforms({
				{"lightPosition", glm::vec3(1.0f, 1.5f, 2.0f)},
				{"ambientColor", glm::vec3(0.1f, 0.1f, 0.1f)},
				{"diffuseIntensity", 1.0f},
				{"shadowMapIndex", 2}
			});
		*/
		template<typename T>
		void SetUniforms(const std::unordered_map<std::string, T>& uniforms)
		{
			for (const auto& [name, value] : uniforms) {
				SetUniform(name, value);
			}
		}

		GLuint CreateUBO(GLsizeiptr size, GLuint bindingPoint, const void* data = nullptr);

	private:
		GLint GetUniformLocation(const std::string& name);
		std::unordered_map<std::string, GLint> m_uniformCache;

		GLuint m_program;
	};

} // namespace bwx_sdk

#endif
