/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_model.cpp
// Purpose:     BWX_SDK Library; OpenGL Model
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include "bwx_gl_model.h"

namespace bwx_sdk {

	void bwxGLModel::Render(glm::mat4* model)
	{
		for (auto m : this->m_meshes) { /*m->Render(NULL, model, false);*/ }
	}

	void bwxGLModel::Render(bwxGLShader* sh, glm::mat4* model)
	{
		for (auto m : this->m_meshes) { /*m->Render(sh, model, false);*/ }
	}

	void bwxGLModel::Clean()
	{
		this->m_meshes.clear();
	}

}
