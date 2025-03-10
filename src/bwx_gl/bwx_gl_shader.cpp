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

	bwxGLShader::bwxGLShader() : id(0) {}

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

		id = glCreateShader(bwxShaderTypeMap[type]);
		const char* src = shaderCode.c_str();
		glShaderSource(id, 1, &src, nullptr);
		glCompileShader(id);

		GLint success;
		glGetShaderiv(id, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			GLint length;
			glGetShaderiv(id, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> errorLog(length);
			glGetShaderInfoLog(id, length, &length, &errorLog[0]);
			std::cerr << "Shader compilation failed: " << &errorLog[0] << std::endl;
			return false;
		}
		return true;
	}

	void bwxGLShader::AttachToProgram(GLuint program)
	{
		if (id)
		{
			glAttachShader(program, id);
		}
	}

	void bwxGLShader::DeleteShader()
	{
		if (id)
		{
			glDeleteShader(id);
			id = 0;
		}
	}

	bwxGLShaderProgram::bwxGLShaderProgram() : program(glCreateProgram()) {}

	bwxGLShaderProgram::~bwxGLShaderProgram() {
		DeleteProgram();
	}

	void bwxGLShaderProgram::AttachShader(const bwxGLShader& shader)
	{
		glAttachShader(program, shader.GetID());
	}

	bool bwxGLShaderProgram::Link()
	{
		glLinkProgram(program);

		GLint success;
		glGetProgramiv(program, GL_LINK_STATUS, &success);
		if (!success)
		{
			GLint length;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &length);
			std::vector<char> errorLog(length);
			glGetProgramInfoLog(program, length, &length, &errorLog[0]);
			std::cerr << "Shader linking failed: " << &errorLog[0] << std::endl;

			glDeleteProgram(program);
			program = 0;
			return false;
		}
		return true;
	}

	void bwxGLShaderProgram::Use()
	{
		if (program)
		{
			glUseProgram(program);
		}
	}

	void bwxGLShaderProgram::DeleteProgram()
	{
		if (program)
		{
			glDeleteProgram(program);
			program = 0;
		}
	}

	GLint bwxGLShaderProgram::GetUniformLocation(const std::string& name)
	{
		auto it = uniformCache.find(name);
		if (it != uniformCache.end())
			return it->second;

		GLint location = glGetUniformLocation(program, name.c_str());
		uniformCache[name] = location;
		return location;
	}

} // namespace bwx_sdk
