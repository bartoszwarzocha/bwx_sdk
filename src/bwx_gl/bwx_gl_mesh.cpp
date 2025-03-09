/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_mesh.cpp
// Purpose:     BWX_SDK Libarary; OpenGL Mesh
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include "bwx_gl_shader.h"
#include "bwx_gl_texture.h"
#include "bwx_gl_material.h"

#include "bwx_gl_mesh.h"

namespace bwx_sdk {

    bwxGLMesh::bwxGLMesh(int style) : m_inputDataFormat(style), m_VBO(), m_EBO(), m_VAO(0) {
        m_material = bwxGLMaterial();
        glGenVertexArrays(1, &m_VAO);
    }

    void bwxGLMesh::Render(bwxGLShaderProgram& shader, glm::mat4* model, bool transparent) {
        if (m_material.IsTransparent() != transparent) return;

        shader.SetUniform("model", *model);
        m_material.ApplyToShader(shader);

        glBindVertexArray(m_VAO);
        if (m_inputDataFormat & bwxGLMESH_INDICES) {
            glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(m_indices.size()), GL_UNSIGNED_INT, 0);
        }
        else {
            glDrawArrays(GL_TRIANGLES, 0, static_cast<GLsizei>(m_vertices.size()));
        }
        glBindVertexArray(0);
    }

    void bwxGLMesh::Delete() {
        m_vertices.clear();
        m_indices.clear();

        if (m_VAO) glDeleteVertexArrays(1, &m_VAO);
    }

    void bwxGLMesh::SetupMesh() {
        glBindVertexArray(m_VAO);

        // Send data to VBO
        m_VBO.UploadData(m_vertices.data(), m_vertices.size() * sizeof(bwxGLVertex));

        // Send data to EBO (if indexes exist)
        if (m_inputDataFormat & bwxGLMESH_INDICES) {
            m_EBO.UploadData(m_indices);
        }

        // Vertex attrs
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(bwxGLVertex), (GLvoid*)offsetof(bwxGLVertex, position));
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(bwxGLVertex), (GLvoid*)offsetof(bwxGLVertex, normal));
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(bwxGLVertex), (GLvoid*)offsetof(bwxGLVertex, tex_coord));
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(bwxGLVertex), (GLvoid*)offsetof(bwxGLVertex, tangent));
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(bwxGLVertex), (GLvoid*)offsetof(bwxGLVertex, bitangent));
        glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, sizeof(bwxGLVertex), (GLvoid*)offsetof(bwxGLVertex, color));

        // Enable attrs
        glEnableVertexAttribArray(0);
        if (m_inputDataFormat & bwxGLMESH_NORMAL)    glEnableVertexAttribArray(1);
        if (m_inputDataFormat & bwxGLMESH_TEX_COORD) glEnableVertexAttribArray(2);
        if (m_inputDataFormat & bwxGLMESH_TANGENT)   glEnableVertexAttribArray(3);
        if (m_inputDataFormat & bwxGLMESH_BITANGENT) glEnableVertexAttribArray(4);
        if (m_inputDataFormat & bwxGLMESH_COLOR)     glEnableVertexAttribArray(5);

        glBindVertexArray(0);
    }

} // namespace bwx_sdk
