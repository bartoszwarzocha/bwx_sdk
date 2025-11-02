/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_render_system.cpp
// Purpose:     BWX_SDK Library; OpenGL Render system (ECS)
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <algorithm>

#include <bwx_sdk/bwx_gl/bwx_gl_render_system.h>
#include <bwx_sdk/bwx_gl/bwx_gl_renderable_component.h>
#include <bwx_sdk/bwx_gl/bwx_gl_camera_component.h>
#include <bwx_sdk/bwx_gl/bwx_gl_light_system.h>
#include <bwx_sdk/bwx_gl/bwx_gl_buffer_manager.h>

namespace bwx_sdk {

    void bwxGLRenderSystem::RegisterRenderable(std::shared_ptr<bwxGLRenderableComponent> renderable) {
        m_renderables.push_back(renderable);
    }

    void bwxGLRenderSystem::UnregisterRenderable(std::shared_ptr<bwxGLRenderableComponent> renderable) {
        m_renderables.erase(std::remove(m_renderables.begin(), m_renderables.end(), renderable), m_renderables.end());
    }

    void bwxGLRenderSystem::Clear() {
        //m_renderables.clear();
        //m_activeCamera.reset();
        //m_lightSystem.reset();
        //m_lightUBO = nullptr; // To consider...
    }

    void bwxGLRenderSystem::SetActiveCamera(std::shared_ptr<bwxGLCameraComponent> camera) {
        m_activeCamera = std::move(camera);
    }

    std::shared_ptr<bwxGLCameraComponent> bwxGLRenderSystem::GetActiveCamera() const {
        return m_activeCamera;
    }

    glm::mat4 bwxGLRenderSystem::GetViewMatrix() const {
        return m_activeCamera ? m_activeCamera->GetViewMatrix() : glm::mat4(1.0f);
    }

    glm::mat4 bwxGLRenderSystem::GetProjectionMatrix() const {
        return m_activeCamera ? m_activeCamera->GetProjectionMatrix() : glm::mat4(1.0f);
    }

    void bwxGLRenderSystem::SetLightSystem(std::shared_ptr<bwxGLLightSystem> system) {
        m_lightSystem = std::move(system);

        // Przy pierwszym przypisaniu zainicjalizuj UBO
        if (!m_lightUBO) {
            auto& bufferManager = bwxGLBufferManager::GetInstance();
            m_lightUBO = bufferManager.GetOrCreateUBO("LightsUBO", {});
            glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_lightUBO->GetID()); // binding 2
        }
    }

    std::shared_ptr<bwxGLLightSystem> bwxGLRenderSystem::GetLightSystem() const {
        return m_lightSystem;
    }

    void bwxGLRenderSystem::RenderAll() {
        if (!m_activeCamera) {
            wxLogWarning("RenderSystem: No active camera set.");
            return;
        }

        // Aktualizacja i przes³anie danych œwiate³ do UBO
        if (m_lightSystem && m_lightUBO) {
            m_lightSystem->Update(0.0f); // TODO: przekazaæ deltaTime jeœli potrzebne
            const auto& lights = m_lightSystem->GetPackedLights();

            if (!lights.empty()) {
                m_lightUBO->Bind();
                m_lightUBO->SetData(lights.data(), lights.size() * sizeof(bwxGLPackedLight), GL_DYNAMIC_DRAW);
                glBindBufferBase(GL_UNIFORM_BUFFER, 2, m_lightUBO->GetID());
                m_lightUBO->Unbind();
            }
        }

        glm::mat4 view = m_activeCamera->GetViewMatrix();
        glm::mat4 projection = m_activeCamera->GetProjectionMatrix();

        for (const auto& renderable : m_renderables) {
            if (!renderable) continue;
            renderable->Render();
        }
    }

} // namespace bwx_sdk