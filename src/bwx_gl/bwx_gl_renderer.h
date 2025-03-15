/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_renderer.h
// Purpose:     BWX_SDK Library; OpenGL Renderer
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_RENDERER_H_
#define _BWX_GL_RENDERER_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <iostream>
#include <memory>

#include "bwx_gl_scene.h"
#include "bwx_gl_material.h"
#include "bwx_gl_mesh.h"

namespace bwx_sdk {

    class bwxGLRenderer {
    public:
        static bwxGLRenderer& GetInstance();

        void Render(const std::shared_ptr<bwxGLScene>& scene);

    private:
        bwxGLRenderer() = default;
        ~bwxGLRenderer() = default;

        void BindMaterial(const bwxGLMaterial& material);
        void BindMesh(const bwxGLMesh& mesh);
    };

} // namespace bwx_sdk

#endif // _BWX_GL_RENDERER_H_
