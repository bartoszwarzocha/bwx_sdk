/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_renderer.cpp
// Purpose:     BWX_SDK Library; OpenGL Renderer Implementation
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <GL/glew.h>
#include <glm/gtc/type_ptr.hpp>

#include "bwx_gl_renderer.h"

#include <iostream>

namespace bwx_sdk {

    bwxGLRenderer& bwxGLRenderer::GetInstance() {
        static bwxGLRenderer instance;
        return instance;
    }

    void bwxGLRenderer::Render(const std::shared_ptr<bwxGLScene>& scene) {
        if (!scene) {
            std::cerr << "bwxGLRenderer: No scene to render!" << std::endl;
            return;
        }

        auto camera = scene->GetActiveCamera();
        if (!camera) {
            std::cerr << "bwxGLRenderer: No active camera in scene!" << std::endl;
            return;
        }

        // Ustawienie OpenGL (bufor g³êbi, czyszczenie ekranu)
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glEnable(GL_DEPTH_TEST);

        glm::mat4 viewMatrix = camera->GetViewMatrix();
        glm::mat4 projectionMatrix = camera->GetProjectionMatrix();

        for (const auto& mesh : scene->GetMeshes()) {
            if (!mesh) continue;

            // Pobranie materia³u i shadera
            auto material = mesh->GetMaterial();
            if (!material) continue;

            //auto shader = material->GetShader();
            //if (!shader) continue;

            //shader->Use();
            //shader->SetUniform("view", viewMatrix);
            //shader->SetUniform("projection", projectionMatrix);
            //shader->SetUniform("model", mesh->GetTransformMatrix());

            BindMaterial(*material);
            BindMesh(*mesh);

            // Renderowanie obiektu
            //glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mesh->GetIndices().size()), GL_UNSIGNED_INT, 0);
        }
    }

    void bwxGLRenderer::BindMaterial(const bwxGLMaterial& material) {
        material.Bind();
    }

    void bwxGLRenderer::BindMesh(const bwxGLMesh& mesh) {
        glBindVertexArray(mesh.GetVAO());
    }

} // namespace bwx_sdk
