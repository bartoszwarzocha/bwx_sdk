/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_renderable.cpp
// Purpose:     BWX_SDK Library; Base class for renderable objects
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include "bwx_gl_renderable.h"

namespace bwx_sdk {

    bwxGLRenderable::bwxGLRenderable()
        : m_material(nullptr), m_buffer(nullptr) {
    }

    void bwxGLRenderable::SetMaterial(std::shared_ptr<bwxGLMaterial> material) {
        m_material = material;
    }

    std::shared_ptr<bwxGLMaterial> bwxGLRenderable::GetMaterial() const {
        return m_material;
    }

    void bwxGLRenderable::SetBuffer(std::shared_ptr<bwxGLBuffer> buffer) {
        m_buffer = buffer;
    }

    std::shared_ptr<bwxGLBuffer> bwxGLRenderable::GetBuffer() const {
        return m_buffer;
    }

} // namespace bwx_sdk
