/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_renderable_component.h
// Purpose:     BWX_SDK Library; Base class for renderable objects (ECS)
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_RENDERABLE_COMPONENT_H_
#define _BWX_GL_RENDERABLE_COMPONENT_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <memory>
#include <iostream>

#include "bwx_gl_component.h"
#include "bwx_gl_material.h"
#include "bwx_gl_buffer.h"
#include "bwx_gl_render_system.h"

namespace bwx_sdk {

    /*
        // Tworzymy nowy wêze³ w scenie
        std::shared_ptr<bwxGLNode> node = std::make_shared<bwxGLNode>();

        // Tworzymy materia³ i bufor
        std::shared_ptr<bwxGLMaterial> material = std::make_shared<bwxGLMaterial>();
        std::shared_ptr<bwxGLBuffer> buffer = std::make_shared<bwxGLBuffer>(GL_ARRAY_BUFFER);

        // Dodajemy komponent renderowania do wêz³a
        std::shared_ptr<bwxGLRenderableComponent> renderComponent = node->AddComponent<bwxGLRenderableComponent>(material, buffer);

        // Aktualizacja i renderowanie
        node->Update(0.016f); // Przyk³adowe deltaTime
        node->Render(); // Renderuje obiekt, jeœli ma komponent RenderableComponent
    */

    class bwxGLRenderableComponent : public bwxGLComponent, public std::enable_shared_from_this<bwxGLRenderableComponent> {
    public:

		bwxGLRenderableComponent() = default;
        bwxGLRenderableComponent(std::shared_ptr<bwxGLMaterial> material, std::shared_ptr<bwxGLBuffer> buffer)
            : m_material(material), m_buffer(buffer) {
            bwxGLRenderSystem::GetInstance().RegisterRenderable(shared_from_this());
        }

        ~bwxGLRenderableComponent() {
            bwxGLRenderSystem::GetInstance().UnregisterRenderable(shared_from_this());
        }

        inline void SetMaterial(std::shared_ptr<bwxGLMaterial> material) { m_material = material; }
        inline std::shared_ptr<bwxGLMaterial> GetMaterial() const { return m_material; }

        inline void SetBuffer(std::shared_ptr<bwxGLBuffer> buffer) { m_buffer = buffer; }
        inline std::shared_ptr<bwxGLBuffer> GetBuffer() const { return m_buffer; }

        void Render() override;

    private:
        std::shared_ptr<bwxGLMaterial> m_material;
        std::shared_ptr<bwxGLBuffer> m_buffer;
    };

} // namespace bwx_sdk

#endif // _BWX_GL_RENDERABLE_COMPONENT_H_
