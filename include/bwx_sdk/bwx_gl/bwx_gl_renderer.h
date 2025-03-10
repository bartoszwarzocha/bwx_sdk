/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_renderer.h
// Purpose:     BWX_SDK Library; OpenGL Renderer
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

#ifndef _BWX_GL_RENDERER_H_
#define _BWX_GL_RENDERER_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <iostream>
#include <memory>

#include "bwx_gl_material.h"
#include "bwx_gl_mesh.h"
#include "bwx_gl_scene.h"

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

}  // namespace bwx_sdk

#endif  // _BWX_GL_RENDERER_H_
