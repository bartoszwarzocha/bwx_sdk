/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_renderable_component.cpp
// Purpose:     BWX_SDK Library; Base class for renderable objects (ECS)
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <bwx_sdk/bwx_gl/bwx_gl_renderable_component.h>

namespace bwx_sdk {

    void bwxGLRenderableComponent::Render() {
        if (m_material && m_buffer) {
            m_material->Bind();
            m_buffer->Bind();

            // Rendering
            //m_buffer->Draw();
            m_buffer->Unbind();
            m_material->Unbind();

			// Odwo³anie siê do komponentu transformaci
            //auto transform = m_node.lock()->GetComponent<bwxGLTransformComponent>();
            //glm::mat4 modelMatrix = transform ? transform->GetTransformMatrix() : glm::mat4(1.0f);
            //m_material->SetUniform("u_Model", modelMatrix);
        }
    }

} // namespace bwx_sdk