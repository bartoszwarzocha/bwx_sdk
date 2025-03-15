/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_renderable.h
// Purpose:     BWX_SDK Library; Base class for renderable objects
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_RENDERABLE_H_
#define _BWX_GL_RENDERABLE_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#define GLM_ENABLE_EXPERIMENTAL

#include <memory>

#include <glm/glm.hpp>

#include "bwx_gl_node.h"
#include "bwx_gl_material.h"
#include "bwx_gl_buffer.h"

namespace bwx_sdk {

    class bwxGLRenderable : public bwxGLNode {
    public:
        bwxGLRenderable();
        virtual ~bwxGLRenderable() = default;

        void SetMaterial(std::shared_ptr<bwxGLMaterial> material);
        std::shared_ptr<bwxGLMaterial> GetMaterial() const;

        void SetBuffer(std::shared_ptr<bwxGLBuffer> buffer);
        std::shared_ptr<bwxGLBuffer> GetBuffer() const;

        virtual void Render() = 0; // Ka¿dy renderowalny obiekt musi nadpisaæ Render()

    protected:
        std::shared_ptr<bwxGLMaterial> m_material;
        std::shared_ptr<bwxGLBuffer> m_buffer;
    };

} // namespace bwx_sdk

#endif
