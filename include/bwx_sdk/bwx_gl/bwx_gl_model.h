/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_model.h
// Purpose:     BWX_SDK Library; OpenGL Model
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


#ifndef _BWX_GL_MODEL_H_
#define _BWX_GL_MODEL_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <vector>
#include <memory>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "bwx_gl_mesh.h"
#include "bwx_gl_shader.h"

namespace bwx_sdk
{

	enum class bwxGL_MODEL_TYPE
	{
		MODEL_UNDEFINED,
		MODEL_STATIC,
		MODEL_STATIC_ANIMATED,
		MODEL_DYNAMIC,
		MODEL_DYNAMIC_ANIMATED
	};

	class bwxGLModel
	{
	public:
		bwxGLModel(bwxGL_MODEL_TYPE type = bwxGL_MODEL_TYPE::MODEL_STATIC) : m_type(type) { Clean(); }
		virtual ~bwxGLModel() { Clean(); }

		void Clean();

		inline void AddMesh(std::shared_ptr<bwxGLMesh> mesh) { this->m_meshes.push_back(mesh); }

		// To verify
		void Render(glm::mat4* model);
		void Render(bwxGLShader* sh, glm::mat4* model);

		inline void SetType(bwxGL_MODEL_TYPE type) { m_type = type; }
		inline bwxGL_MODEL_TYPE GetType() { return m_type; }

		inline int GetMeshesCount() { return m_meshes.size(); }

	private:
		bwxGL_MODEL_TYPE m_type;
		std::vector<std::shared_ptr<bwxGLMesh>> m_meshes;
	};

}
#endif