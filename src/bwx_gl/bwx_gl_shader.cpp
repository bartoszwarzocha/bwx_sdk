/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_shader.cpp
// Purpose:     BWX_SDK Libarary; OpenGL Shader
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <iostream>
#include <fstream>
#include <vector>

#include "bwx_gl_shader.h"

namespace bwx_sdk {

	std::unordered_map<bwxGL_SHADER_TYPE, GLenum> bwxShaderTypeMap = {
		{ SHADER_VERTEX, GL_VERTEX_SHADER },
		{ SHADER_TESS_CONTROL, GL_TESS_CONTROL_SHADER },
		{ SHADER_TESS_EVALUATION, GL_TESS_EVALUATION_SHADER },
		{ SHADER_GEOMETRY, GL_GEOMETRY_SHADER },
		{ SHADER_FRAGMENT, GL_FRAGMENT_SHADER },
		{ SHADER_COMPUTE, GL_COMPUTE_SHADER }
	};

	bwxGLShader::bwxGLShader() : m_id(bwxGL_SHADER_EMPTY) {}

	bwxGLShader::bwxGLShader(bwxGL_SHADER_TYPE type, const std::string& source, bool fromFile)
	{
		LoadShader(type, source, fromFile);
	}

	bwxGLShader::~bwxGLShader() {
		DeleteShader();
	}

	bool bwxGLShader::LoadShader(bwxGL_SHADER_TYPE type, const std::string& source, bool fromFile)
	{
		std::string shaderCode = source;
		if (fromFile)
		{
			std::ifstream file(source);
			if (!file.is_open())
			{
				std::cerr << "Shader file load error: " << source << std::endl;
				return false;
			}
			shaderCode = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
			file.close();
		}

		m_id = glCreateShader(bwxShaderTypeMap[type]);
		const char* src = shaderCode.c_str();
		glShaderSource(m_id, 1, &src, nullptr);
		glCompileShader(m_id);

		GLint success;
		glGetShaderiv(m_id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLint length;
			glGetShaderiv(m_id, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> errorLog(length);
			glGetShaderInfoLog(m_id, length, &length, &errorLog[0]);
			std::cerr << "Shader compilation failed: " << &errorLog[0] << std::endl;
			return false;
		}
		return true;
	}

	void bwxGLShader::AttachToProgram(GLuint program)
	{
		if (m_id)
		{
			glAttachShader(program, m_id);
		}
	}

	void bwxGLShader::DeleteShader()
	{
		if (m_id)
		{
			glDeleteShader(m_id);
			m_id = bwxGL_SHADER_EMPTY;
		}
	}

	bwxGLShaderProgram::bwxGLShaderProgram() : m_program(glCreateProgram()) {}

	bwxGLShaderProgram::~bwxGLShaderProgram() {
		Delete();
	}

	void bwxGLShaderProgram::AttachShader(const bwxGLShader& shader)
	{
		glAttachShader(m_program, shader.GetID());
	}

	void bwxGLShaderProgram::AttachShader(const GLuint& shaderId)
	{
		glAttachShader(m_program, shaderId);
	}

	bool bwxGLShaderProgram::Link()
	{
		glLinkProgram(m_program);

		GLint success;
		glGetProgramiv(m_program, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLint length;
			glGetProgramiv(m_program, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> errorLog(length);
			glGetProgramInfoLog(m_program, length, &length, &errorLog[0]);
			std::cerr << "Shader linking failed: " << &errorLog[0] << std::endl;

			glDeleteProgram(m_program);
			m_program = bwxGL_SHADER_PROGRAM_EMPTY;
			return false;
		}
		return true;
	}

	void bwxGLShaderProgram::Bind()
	{
		if (m_program)
		{
			glUseProgram(m_program);
		}
	}

	void bwxGLShaderProgram::Unbind()
	{
		glUseProgram(0);
	}

	void bwxGLShaderProgram::Delete()
	{
		if (m_program)
		{
			glDeleteProgram(m_program);
			m_program = bwxGL_SHADER_PROGRAM_EMPTY;
		}
	}

	void bwxGLShaderProgram::AddUniform(const std::string& name)
	{
		GetUniformLocation(name);
	}

	void bwxGLShaderProgram::AddUniforms(const std::vector<std::string>& names)
	{
		for (const auto& name : names)
		{
			AddUniform(name);
		}
	}

	void bwxGLShaderProgram::AddAttribute(const std::string& name)
	{
		GetAttributeLocation(name);
	}

	void bwxGLShaderProgram::AddAttributes(const std::vector<std::string>& names)
	{
		for (const auto& name : names)
		{
			AddAttribute(name);
		}
	}

	std::unordered_map<std::string, GLint> bwxGLShaderProgram::GetProgramUniforms()
	{
		std::unordered_map<std::string, GLint> uniforms;
		GLint numUniforms;
		glGetProgramiv(m_program, GL_ACTIVE_UNIFORMS, &numUniforms);
		for (int i = 0; i < numUniforms; i++)
		{
			GLchar name[256];
			GLsizei length;
			GLint size;
			GLenum type;
			glGetActiveUniform(m_program, i, 256, &length, &size, &type, name);
			uniforms[name] = glGetUniformLocation(m_program, name);
		}
		return uniforms;
	}

	std::unordered_map<std::string, GLint> bwxGLShaderProgram::GetProgramAttributes()
	{
		std::unordered_map<std::string, GLint> attributes;
		GLint numAttributes;
		glGetProgramiv(m_program, GL_ACTIVE_ATTRIBUTES, &numAttributes);
		for (int i = 0; i < numAttributes; i++)
		{
			GLchar name[256];
			GLsizei length;
			GLint size;
			GLenum type;
			glGetActiveAttrib(m_program, i, 256, &length, &size, &type, name);
			attributes[name] = glGetAttribLocation(m_program, name);
		}
		return attributes;
	}

	GLint bwxGLShaderProgram::GetUniformLocation(const std::string& name)
	{
		auto it = m_uniformCache.find(name);
		if (it != m_uniformCache.end()) {
			return it->second;
		}

		GLint location = glGetUniformLocation(m_program, name.c_str());
		if (location == -1) {
			std::cerr << "Warning: Uniform '" << name << "' not found in shader program " << m_program << std::endl;
		}

		m_uniformCache[name] = location;
		return location;
	}

	GLint bwxGLShaderProgram::GetAttributeLocation(const std::string& name)
	{
		auto it = m_attributeCache.find(name);
		if (it != m_attributeCache.end()) {
			return it->second;
		}
		GLint location = glGetAttribLocation(m_program, name.c_str());
		if (location == -1) {
			std::cerr << "Warning: Attribute '" << name << "' not found in shader program " << m_program << std::endl;
		}
		m_attributeCache[name] = location;
		return location;
	}

	GLuint bwxGLShaderProgram::CreateUBO(GLsizeiptr size, GLuint bindingPoint, const void* data)
	{
		GLuint ubo;
		glGenBuffers(1, &ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, ubo);
		glBufferData(GL_UNIFORM_BUFFER, size, data, GL_DYNAMIC_DRAW);
		glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, ubo);
		glBindBuffer(GL_UNIFORM_BUFFER, 0);
		return ubo;
	}

} // namespace bwx_sdk
