/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_shader_manager.cpp
// Purpose:     BWX_SDK Libarary; OpenGL Shader Manager
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include "bwx_gl_shader_manager.h"

#include <iostream>

namespace bwx_sdk {

	bool bwxGLShaderManager::m_overwrite = false;

	bwxGLShaderManager& bwxGLShaderManager::GetInstance() {
		static bwxGLShaderManager instance;
		return instance;
	}

	bwxGLShaderManager::~bwxGLShaderManager() {
		
	}

	void bwxGLShaderManager::AddShader(const std::string& name, const bwxGLShader& shader) {
		m_shaderMap[name] = std::make_shared<bwxGLShader>(shader);
	}

	GLuint bwxGLShaderManager::LoadShader(const std::string& name, const std::string& source, bwxGL_SHADER_TYPE type, bool fromFile) 
	{
		std::shared_ptr<bwxGLShader> shader = std::make_shared<bwxGLShader>();
		if (shader->LoadShader(type, source, fromFile)) 
		{
			if (m_overwrite || m_shaderMap.find(name) == m_shaderMap.end())
			{
				m_shaderMap[name] = shader;
			}
		}
		return shader->GetID();
	}

	GLuint bwxGLShaderManager::LoadShaders(const std::string& name, const std::initializer_list<std::pair<bwxGL_SHADER_TYPE, std::string>>& shaders, bool fromFile) {
		GLuint ret = bwxGL_SHADER_EMPTY;
		for (const auto& shader : shaders) {
			ret = LoadShader(name + GetShaderSuffix(shader.first), shader.second, shader.first, fromFile);
			if (ret == bwxGL_SHADER_EMPTY) return ret;
		}
		return ret;
	}

	GLuint bwxGLShaderManager::LoadShaders(const std::string& name, const std::string& vertex, const std::string& fragment) 
	{
		GLuint ret = bwxGL_SHADER_EMPTY;
		ret = LoadShader(name + bwxGL_SHADER_VERTEX_SUFIX, vertex, SHADER_VERTEX);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_FRAGMENT_SUFIX, fragment, SHADER_FRAGMENT);
		return ret;
	}

	GLuint bwxGLShaderManager::LoadShaders(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& geometry)
	{
		GLuint ret = bwxGL_SHADER_EMPTY;
		ret = LoadShader(name + bwxGL_SHADER_VERTEX_SUFIX, vertex, SHADER_VERTEX);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_FRAGMENT_SUFIX, fragment, SHADER_FRAGMENT);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_GEOMETRY_SUFIX, geometry, SHADER_GEOMETRY);
		return ret;
	}

	GLuint bwxGLShaderManager::LoadShaders(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval)
	{
		GLuint ret = bwxGL_SHADER_EMPTY;
		ret = LoadShader(name + bwxGL_SHADER_VERTEX_SUFIX, vertex, SHADER_VERTEX);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_FRAGMENT_SUFIX, fragment, SHADER_FRAGMENT);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_TESS_CONTROL_SUFIX, tessControl, SHADER_TESS_CONTROL);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_TESS_EVAL_SUFIX, tessEval, SHADER_TESS_EVALUATION);
		return ret;
	}

	GLuint bwxGLShaderManager::LoadShaders(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval, const std::string& geometry)
	{
		GLuint ret = bwxGL_SHADER_EMPTY;
		ret = LoadShader(name + bwxGL_SHADER_VERTEX_SUFIX, vertex, SHADER_VERTEX);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_FRAGMENT_SUFIX, fragment, SHADER_FRAGMENT);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_TESS_CONTROL_SUFIX, tessControl, SHADER_TESS_CONTROL);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_TESS_EVAL_SUFIX, tessEval, SHADER_TESS_EVALUATION);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_GEOMETRY_SUFIX, geometry, SHADER_GEOMETRY);
		return ret;
	}

	GLuint bwxGLShaderManager::LoadShadersFromFiles(const std::string& name, const std::string& vertex, const std::string& fragment)
	{
		GLuint ret = bwxGL_SHADER_EMPTY;
		ret = LoadShader(name + bwxGL_SHADER_VERTEX_SUFIX, vertex, SHADER_VERTEX, true);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_FRAGMENT_SUFIX, fragment, SHADER_FRAGMENT, true);
		return ret;
	}

	GLuint bwxGLShaderManager::LoadShadersFromFiles(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& geometry)
	{
		GLuint ret = bwxGL_SHADER_EMPTY;
		ret = LoadShader(name + bwxGL_SHADER_VERTEX_SUFIX, vertex, SHADER_VERTEX, true);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_FRAGMENT_SUFIX, fragment, SHADER_FRAGMENT, true);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_GEOMETRY_SUFIX, geometry, SHADER_GEOMETRY, true);
		return ret;
	}

	GLuint bwxGLShaderManager::LoadShadersFromFiles(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval)
	{
		GLuint ret = bwxGL_SHADER_EMPTY;
		ret = LoadShader(name + bwxGL_SHADER_VERTEX_SUFIX, vertex, SHADER_VERTEX, true);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_FRAGMENT_SUFIX, fragment, SHADER_FRAGMENT, true);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_TESS_CONTROL_SUFIX, tessControl, SHADER_TESS_CONTROL, true);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_TESS_EVAL_SUFIX, tessEval, SHADER_TESS_EVALUATION, true);
		return ret;
	}

	GLuint bwxGLShaderManager::LoadShadersFromFiles(const std::string& name, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval, const std::string& geometry)
	{
		GLuint ret = bwxGL_SHADER_EMPTY;
		ret = LoadShader(name + bwxGL_SHADER_VERTEX_SUFIX, vertex, SHADER_VERTEX, true);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_FRAGMENT_SUFIX, fragment, SHADER_FRAGMENT, true);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_TESS_CONTROL_SUFIX, tessControl, SHADER_TESS_CONTROL, true);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_TESS_EVAL_SUFIX, tessEval, SHADER_TESS_EVALUATION, true);
		if (ret != bwxGL_SHADER_EMPTY) ret = LoadShader(name + bwxGL_SHADER_GEOMETRY_SUFIX, geometry, SHADER_GEOMETRY, true);
		return ret;
	}

	bwxGLShader bwxGLShaderManager::GetShader(const std::string& name) {
		auto it = m_shaderMap.find(name);
		if (it != m_shaderMap.end()) {
			return *it->second.get();
		}
		return bwxGLShader();
	}

	std::shared_ptr<bwxGLShader> bwxGLShaderManager::GetShaderPtr(const std::string& name) {
		auto it = m_shaderMap.find(name);
		if (it != m_shaderMap.end()) {
			return it->second;
		}
		return nullptr;
	}

    std::vector<GLuint> bwxGLShaderManager::GetShaders(const std::string& name) const
    {
        std::vector<GLuint> shaders;
        std::vector<std::string> suffixes = {
            bwxGL_SHADER_VERTEX_SUFIX,
            bwxGL_SHADER_FRAGMENT_SUFIX,
            bwxGL_SHADER_GEOMETRY_SUFIX,
            bwxGL_SHADER_TESS_CONTROL_SUFIX,
            bwxGL_SHADER_TESS_EVAL_SUFIX
        };

        for (const auto& suffix : suffixes)
        {
            auto it = m_shaderMap.find(name + suffix);
            if (it != m_shaderMap.end())
            {
                shaders.push_back(it->second->GetID());
            }
        }

        return shaders;
    }

	std::vector<GLuint> bwxGLShaderManager::GetAllShaders() const
	{
		std::vector<GLuint> shaders;
		for (const auto& shader : m_shaderMap)
		{
			shaders.push_back(shader.second->GetID());
		}
		return shaders;
	}

	std::vector<std::string> bwxGLShaderManager::GetShaderNames() const
	{
		std::vector<std::string> names;
		for (const auto& shader : m_shaderMap)
		{
			names.push_back(shader.first);
		}
		return names;
	}

	std::string bwxGLShaderManager::GetShaderSuffix(bwxGL_SHADER_TYPE type) {
		switch (type) {
		case SHADER_VERTEX: return bwxGL_SHADER_VERTEX_SUFIX;
		case SHADER_FRAGMENT: return bwxGL_SHADER_FRAGMENT_SUFIX;
		case SHADER_GEOMETRY: return bwxGL_SHADER_GEOMETRY_SUFIX;
		case SHADER_TESS_CONTROL: return bwxGL_SHADER_TESS_CONTROL_SUFIX;
		case SHADER_TESS_EVALUATION: return bwxGL_SHADER_TESS_EVAL_SUFIX;
		case SHADER_COMPUTE: return bwxGL_SHADER_COMPUTE_SUFIX;
		default: return "";
		}
	}

	void bwxGLShaderManager::Clear() {
		m_shaderMap.clear();
	}
	
	// ---------------------------------------------------------------------

	bwxGLShaderProgramManager& bwxGLShaderProgramManager::GetInstance() {
		static bwxGLShaderProgramManager instance;
		return instance;
	}

	bwxGLShaderProgramManager::~bwxGLShaderProgramManager() {
		//for (const auto& program : m_shaderProgramMap)
		//	glDeleteProgram(m_shaderProgramMap.begin()->second);
	}

	void bwxGLShaderProgramManager::AddShaderProgram(const std::string& name, const bwxGLShaderProgram& program) {
		m_shaderProgramMap[name] = std::make_shared<bwxGLShaderProgram>(program);
	}

	GLuint bwxGLShaderProgramManager::CreateShaderProgram(const std::string& programName, const std::initializer_list<std::pair<bwxGL_SHADER_TYPE, std::string>>& shaders, bool fromFile)
	{
		GLuint ret = bwxGL_SHADER_PROGRAM_EMPTY;
		std::shared_ptr<bwxGLShaderProgram> program = std::make_shared<bwxGLShaderProgram>();

		for (const auto& shader : shaders)
		{
			ret = bwxGLShaderManager::GetInstance().LoadShader(programName + bwxGLShaderManager::GetInstance().GetShaderSuffix(shader.first), shader.second, shader.first, fromFile);
			if (ret == bwxGL_SHADER_EMPTY) return ret;
			program->AttachShader(ret);
		}

		if (program->Link())
		{
			m_shaderProgramMap[programName] = program;
			return program->GetProgram();
		}

		return bwxGL_SHADER_PROGRAM_EMPTY;
	}

	GLuint bwxGLShaderProgramManager::CreateShaderProgram(const std::string& programName)
	{
		std::shared_ptr<bwxGLShaderProgram> program = std::make_shared<bwxGLShaderProgram>();
		auto shaders = bwxGLShaderManager::GetInstance().GetShaders(programName);

		for (const auto& shader : shaders)
		{
			program->AttachShader(shader);
		}

		if (program->Link())
		{
			m_shaderProgramMap[programName] = program;
			return program->GetProgram();
		}

		return bwxGL_SHADER_PROGRAM_EMPTY;
	}

	GLuint bwxGLShaderProgramManager::CreateShaderProgram(const std::string& programName, const std::string& vertexName, const std::string& fragmentName)
	{
		std::shared_ptr<bwxGLShaderProgram> program = std::make_shared<bwxGLShaderProgram>();
		program->AttachShader(bwxGLShaderManager::GetInstance().GetShader(vertexName));
		program->AttachShader(bwxGLShaderManager::GetInstance().GetShader(fragmentName));
		if (program->Link())
		{
			m_shaderProgramMap[programName] = program;
			return program->GetProgram();
		}
		return bwxGL_SHADER_PROGRAM_EMPTY;
	}

	GLuint bwxGLShaderProgramManager::CreateShaderProgram(const std::string& programName, const std::string& vertexName, const std::string& fragmentName, const std::string& geometryName)
	{
		std::shared_ptr<bwxGLShaderProgram> program = std::make_shared<bwxGLShaderProgram>();
		program->AttachShader(bwxGLShaderManager::GetInstance().GetShader(vertexName));
		program->AttachShader(bwxGLShaderManager::GetInstance().GetShader(fragmentName));
		program->AttachShader(bwxGLShaderManager::GetInstance().GetShader(geometryName));
		if (program->Link())
		{
			m_shaderProgramMap[programName] = program;
			return program->GetProgram();
		}
		return bwxGL_SHADER_PROGRAM_EMPTY;
	}

	GLuint bwxGLShaderProgramManager::CreateShaderProgram(const std::string& programName, const std::string& vertexName, const std::string& fragmentName, const std::string& tessControlName, const std::string& tessEvalName)
	{
		std::shared_ptr<bwxGLShaderProgram> program = std::make_shared<bwxGLShaderProgram>();
		program->AttachShader(bwxGLShaderManager::GetInstance().GetShader(vertexName));
		program->AttachShader(bwxGLShaderManager::GetInstance().GetShader(fragmentName));
		program->AttachShader(bwxGLShaderManager::GetInstance().GetShader(tessControlName));
		program->AttachShader(bwxGLShaderManager::GetInstance().GetShader(tessEvalName));
		if (program->Link())
		{
			m_shaderProgramMap[programName] = program;
			return program->GetProgram();
		}
		return bwxGL_SHADER_PROGRAM_EMPTY;
	}

	GLuint bwxGLShaderProgramManager::CreateShaderProgram(const std::string& programName, const std::string& vertexName, const std::string& fragmentName, const std::string& tessControlName, const std::string& tessEvalName, const std::string& geometryName)
	{
		std::shared_ptr<bwxGLShaderProgram> program = std::make_shared<bwxGLShaderProgram>();
		program->AttachShader(bwxGLShaderManager::GetInstance().GetShader(vertexName));
		program->AttachShader(bwxGLShaderManager::GetInstance().GetShader(fragmentName));
		program->AttachShader(bwxGLShaderManager::GetInstance().GetShader(tessControlName));
		program->AttachShader(bwxGLShaderManager::GetInstance().GetShader(tessEvalName));
		program->AttachShader(bwxGLShaderManager::GetInstance().GetShader(geometryName));
		if (program->Link())
		{
			m_shaderProgramMap[programName] = program;
			return program->GetProgram();
		}
		return bwxGL_SHADER_PROGRAM_EMPTY;
	}

	GLuint bwxGLShaderProgramManager::CreateShaderProgramFromStrings(const std::string& programName, const std::string& vertex, const std::string& fragment, bool addToShaderManager)
	{
		std::shared_ptr<bwxGLShaderProgram> program = std::make_shared<bwxGLShaderProgram>();
		bwxGLShader vertexShader;
		bwxGLShader fragmentShader;
		if (vertexShader.LoadShader(SHADER_VERTEX, vertex) && 
			fragmentShader.LoadShader(SHADER_FRAGMENT, fragment))
		{
			if (addToShaderManager)
			{
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_VERTEX_SUFIX, vertexShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_FRAGMENT_SUFIX, fragmentShader);
			}

			program->AttachShader(vertexShader);
			program->AttachShader(fragmentShader);
			if (program->Link())
			{
				m_shaderProgramMap[programName] = program;
				return program->GetProgram();
			}
		}
		return bwxGL_SHADER_PROGRAM_EMPTY;
	}

	GLuint bwxGLShaderProgramManager::CreateShaderProgramFromStrings(const std::string& programName, const std::string& vertex, const std::string& fragment, const std::string& geometry, bool addToShaderManager)
	{
		std::shared_ptr<bwxGLShaderProgram> program = std::make_shared<bwxGLShaderProgram>();
		bwxGLShader vertexShader;
		bwxGLShader fragmentShader;
		bwxGLShader geometryShader;
		if (vertexShader.LoadShader(SHADER_VERTEX, vertex) && 
			fragmentShader.LoadShader(SHADER_FRAGMENT, fragment) && 
			geometryShader.LoadShader(SHADER_GEOMETRY, geometry))
		{
			if (addToShaderManager)
			{
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_VERTEX_SUFIX, vertexShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_FRAGMENT_SUFIX, fragmentShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_GEOMETRY_SUFIX, geometryShader);
			}

			program->AttachShader(vertexShader);
			program->AttachShader(fragmentShader);
			program->AttachShader(geometryShader);
			if (program->Link())
			{
				m_shaderProgramMap[programName] = program;
				return program->GetProgram();
			}
		}
		return bwxGL_SHADER_PROGRAM_EMPTY;
	}

	GLuint bwxGLShaderProgramManager::CreateShaderProgramFromStrings(const std::string& programName, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval, bool addToShaderManager)
	{
		std::shared_ptr<bwxGLShaderProgram> program = std::make_shared<bwxGLShaderProgram>();
		bwxGLShader vertexShader;
		bwxGLShader fragmentShader;
		bwxGLShader tessControlShader;
		bwxGLShader tessEvalShader;
		if (vertexShader.LoadShader(SHADER_VERTEX, vertex) && 
			fragmentShader.LoadShader(SHADER_FRAGMENT, fragment) && 
			tessControlShader.LoadShader(SHADER_TESS_CONTROL, tessControl) && 
			tessEvalShader.LoadShader(SHADER_TESS_EVALUATION, tessEval))
		{
			if (addToShaderManager)
			{
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_VERTEX_SUFIX, vertexShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_FRAGMENT_SUFIX, fragmentShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_TESS_CONTROL_SUFIX, tessControlShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_TESS_EVAL_SUFIX, tessEvalShader);
			}

			program->AttachShader(vertexShader);
			program->AttachShader(fragmentShader);
			program->AttachShader(tessControlShader);
			program->AttachShader(tessEvalShader);
			if (program->Link())
			{
				m_shaderProgramMap[programName] = program;
				return program->GetProgram();
			}
		}
		return bwxGL_SHADER_PROGRAM_EMPTY;
	}

	GLuint bwxGLShaderProgramManager::CreateShaderProgramFromStrings(const std::string& programName, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval, const std::string& geometry, bool addToShaderManager)
	{
		std::shared_ptr<bwxGLShaderProgram> program = std::make_shared<bwxGLShaderProgram>();
		bwxGLShader vertexShader;
		bwxGLShader fragmentShader;
		bwxGLShader tessControlShader;
		bwxGLShader tessEvalShader;
		bwxGLShader geometryShader;
		if (vertexShader.LoadShader(SHADER_VERTEX, vertex) && 
			fragmentShader.LoadShader(SHADER_FRAGMENT, fragment) && 
			tessControlShader.LoadShader(SHADER_TESS_CONTROL, tessControl) && 
			tessEvalShader.LoadShader(SHADER_TESS_EVALUATION, tessEval) && 
			geometryShader.LoadShader(SHADER_GEOMETRY, geometry))
		{
			if (addToShaderManager)
			{
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_VERTEX_SUFIX, vertexShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_FRAGMENT_SUFIX, fragmentShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_TESS_CONTROL_SUFIX, tessControlShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_TESS_EVAL_SUFIX, tessEvalShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_GEOMETRY_SUFIX, geometryShader);
			}

			program->AttachShader(vertexShader);
			program->AttachShader(fragmentShader);
			program->AttachShader(tessControlShader);
			program->AttachShader(tessEvalShader);
			program->AttachShader(geometryShader);
			if (program->Link())
			{
				m_shaderProgramMap[programName] = program;
				return program->GetProgram();
			}
		}
		return bwxGL_SHADER_PROGRAM_EMPTY;
	}

	GLuint bwxGLShaderProgramManager::CreateShaderProgramFromFiles(const std::string& programName, const std::string& vertex, const std::string& fragment, bool addToShaderManager)
	{
		std::shared_ptr<bwxGLShaderProgram> program = std::make_shared<bwxGLShaderProgram>();
		bwxGLShader vertexShader;
		bwxGLShader fragmentShader;
		if (vertexShader.LoadShader(SHADER_VERTEX, vertex, true) &&
			fragmentShader.LoadShader(SHADER_FRAGMENT, fragment, true))
		{
			if (addToShaderManager)
			{
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_VERTEX_SUFIX, vertexShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_FRAGMENT_SUFIX, fragmentShader);
			}

			program->AttachShader(vertexShader);
			program->AttachShader(fragmentShader);
			if (program->Link())
			{
				m_shaderProgramMap[programName] = program;
				return program->GetProgram();
			}
		}
		return bwxGL_SHADER_PROGRAM_EMPTY;
	}

	GLuint bwxGLShaderProgramManager::CreateShaderProgramFromFiles(const std::string& programName, const std::string& vertex, const std::string& fragment, const std::string& geometry, bool addToShaderManager)
	{
		std::shared_ptr<bwxGLShaderProgram> program = std::make_shared<bwxGLShaderProgram>();
		bwxGLShader vertexShader;
		bwxGLShader fragmentShader;
		bwxGLShader geometryShader;
		if (vertexShader.LoadShader(SHADER_VERTEX, vertex, true) &&
			fragmentShader.LoadShader(SHADER_FRAGMENT, fragment, true) &&
			geometryShader.LoadShader(SHADER_GEOMETRY, geometry, true))
		{
			if (addToShaderManager)
			{
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_VERTEX_SUFIX, vertexShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_FRAGMENT_SUFIX, fragmentShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_GEOMETRY_SUFIX, geometryShader);
			}

			program->AttachShader(vertexShader);
			program->AttachShader(fragmentShader);
			program->AttachShader(geometryShader);
			if (program->Link())
			{
				m_shaderProgramMap[programName] = program;
				return program->GetProgram();
			}
		}
		return bwxGL_SHADER_PROGRAM_EMPTY;
	}

	GLuint bwxGLShaderProgramManager::CreateShaderProgramFromFiles(const std::string& programName, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval, bool addToShaderManager)
	{
		std::shared_ptr<bwxGLShaderProgram> program = std::make_shared<bwxGLShaderProgram>();
		bwxGLShader vertexShader;
		bwxGLShader fragmentShader;
		bwxGLShader tessControlShader;
		bwxGLShader tessEvalShader;
		if (vertexShader.LoadShader(SHADER_VERTEX, vertex, true) &&
			fragmentShader.LoadShader(SHADER_FRAGMENT, fragment, true) &&
			tessControlShader.LoadShader(SHADER_TESS_CONTROL, tessControl, true) &&
			tessEvalShader.LoadShader(SHADER_TESS_EVALUATION, tessEval, true))
		{
			if (addToShaderManager)
			{
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_VERTEX_SUFIX, vertexShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_FRAGMENT_SUFIX, fragmentShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_TESS_CONTROL_SUFIX, tessControlShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_TESS_EVAL_SUFIX, tessEvalShader);
			}

			program->AttachShader(vertexShader);
			program->AttachShader(fragmentShader);
			program->AttachShader(tessControlShader);
			program->AttachShader(tessEvalShader);
			if (program->Link())
			{
				m_shaderProgramMap[programName] = program;
				return program->GetProgram();
			}
		}
		return bwxGL_SHADER_PROGRAM_EMPTY;
	}

	GLuint bwxGLShaderProgramManager::CreateShaderProgramFromFiles(const std::string& programName, const std::string& vertex, const std::string& fragment, const std::string& tessControl, const std::string& tessEval, const std::string& geometry, bool addToShaderManager)
	{
		std::shared_ptr<bwxGLShaderProgram> program = std::make_shared<bwxGLShaderProgram>();
		bwxGLShader vertexShader;
		bwxGLShader fragmentShader;
		bwxGLShader tessControlShader;
		bwxGLShader tessEvalShader;
		bwxGLShader geometryShader;
		if (vertexShader.LoadShader(SHADER_VERTEX, vertex, true) &&
			fragmentShader.LoadShader(SHADER_FRAGMENT, fragment, true) &&
			tessControlShader.LoadShader(SHADER_TESS_CONTROL, tessControl, true) &&
			tessEvalShader.LoadShader(SHADER_TESS_EVALUATION, tessEval, true) &&
			geometryShader.LoadShader(SHADER_GEOMETRY, geometry, true))
		{
			if (addToShaderManager)
			{
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_VERTEX_SUFIX, vertexShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_FRAGMENT_SUFIX, fragmentShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_TESS_CONTROL_SUFIX, tessControlShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_TESS_EVAL_SUFIX, tessEvalShader);
				bwxGLShaderManager::GetInstance().AddShader(programName + bwxGL_SHADER_GEOMETRY_SUFIX, geometryShader);
			}

			program->AttachShader(vertexShader);
			program->AttachShader(fragmentShader);
			program->AttachShader(tessControlShader);
			program->AttachShader(tessEvalShader);
			program->AttachShader(geometryShader);
			if (program->Link())
			{
				m_shaderProgramMap[programName] = program;
				return program->GetProgram();
			}
		}
		return bwxGL_SHADER_PROGRAM_EMPTY;
	}

	bwxGLShaderProgram bwxGLShaderProgramManager::GetShaderProgram(const std::string& programName) {
		auto it = m_shaderProgramMap.find(programName);
		if (it != m_shaderProgramMap.end()) {
			return *it->second.get();
		}
		return bwxGLShaderProgram();
	}

	std::shared_ptr<bwxGLShaderProgram> bwxGLShaderProgramManager::GetShaderProgramPtr(const std::string& programName) {
		auto it = m_shaderProgramMap.find(programName);
		if (it != m_shaderProgramMap.end()) {
			return it->second;
		}
		return nullptr;
	}

	std::vector<GLuint> bwxGLShaderProgramManager::GetAllShaderPrograms() const
	{
		std::vector<GLuint> programs;
		for (const auto& program : m_shaderProgramMap)
		{
			programs.push_back(program.second->GetProgram());
		}
		return programs;
	}

	std::vector<std::string> bwxGLShaderProgramManager::GetShaderProgramNames() const
	{
		std::vector<std::string> names;
		for (const auto& program : m_shaderProgramMap)
		{
			names.push_back(program.first);
		}
		return names;
	}

	void bwxGLShaderProgramManager::UseShaderProgram(const std::string& programName)
	{
		auto it = m_shaderProgramMap.find(programName);
		if (it != m_shaderProgramMap.end())
		{
			it->second->Use();
		}
	}

	void bwxGLShaderProgramManager::UnuseShaderProgram()
	{
		glUseProgram(0);
	}

	void bwxGLShaderProgramManager::ReleaseShaderProgram(const std::string& programName)
	{
		auto it = m_shaderProgramMap.find(programName);
		if (it != m_shaderProgramMap.end())
		{
			glDeleteProgram(it->second->GetProgram());
			m_shaderProgramMap.erase(it);
		}
	}

	void bwxGLShaderProgramManager::Clear() {
		m_shaderProgramMap.clear();
	}

} // namespace bwx_sdk