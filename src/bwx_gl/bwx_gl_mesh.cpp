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

#include "bwx_gl_buffer_manager.h"
#include "bwx_gl_mesh.h"

namespace bwx_sdk {

    bwxGLMesh::bwxGLMesh(int style) : m_inputDataFormat(style) {}

    void bwxGLMesh::SetupMesh() {
        //if (!m_buffer) {
        //    m_buffer = std::make_shared<bwxGLBuffer>();
        //}

        std::vector<float> vertexData;
        for (const auto& v : m_vertices) {
            vertexData.push_back(v.position.x);
            vertexData.push_back(v.position.y);
            vertexData.push_back(v.position.z);

            if (m_inputDataFormat & bwxGL_MESH_NORMAL) {
                vertexData.push_back(v.normal.x);
                vertexData.push_back(v.normal.y);
                vertexData.push_back(v.normal.z);
            }
            if (m_inputDataFormat & bwxGL_MESH_TEX_COORD) {
                vertexData.push_back(v.texCoord.x);
                vertexData.push_back(v.texCoord.y);
            }
            if (m_inputDataFormat & bwxGL_MESH_TANGENT) {
                vertexData.push_back(v.tangent.x);
                vertexData.push_back(v.tangent.y);
                vertexData.push_back(v.tangent.z);
            }
            if (m_inputDataFormat & bwxGL_MESH_BITANGENT) {
                vertexData.push_back(v.bitangent.x);
                vertexData.push_back(v.bitangent.y);
                vertexData.push_back(v.bitangent.z);
            }
            if (m_inputDataFormat & bwxGL_MESH_COLOR) {
                vertexData.push_back(v.color.r);
                vertexData.push_back(v.color.g);
                vertexData.push_back(v.color.b);
            }
        }

        m_vbo = bwxGLBufferManager::GetInstance().GetOrCreateVBO("mesh_vbo", vertexData);
        m_ebo = bwxGLBufferManager::GetInstance().GetOrCreateEBO("mesh_ebo", m_indices);

        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);
        glBindBuffer(GL_ARRAY_BUFFER, m_vbo->GetID());
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ebo->GetID());

        int stride = 3;
        if (m_inputDataFormat & bwxGL_MESH_NORMAL) stride += 3;
        if (m_inputDataFormat & bwxGL_MESH_TEX_COORD) stride += 2;
        if (m_inputDataFormat & bwxGL_MESH_TANGENT) stride += 3;
        if (m_inputDataFormat & bwxGL_MESH_BITANGENT) stride += 3;
        if (m_inputDataFormat & bwxGL_MESH_COLOR) stride += 3;
        stride *= sizeof(float);

        int offset = 0;
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(static_cast<intptr_t>(offset)));
        glEnableVertexAttribArray(0);
        offset += 3 * sizeof(float);

        if (m_inputDataFormat & bwxGL_MESH_NORMAL) {
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(static_cast<intptr_t>(offset)));
            glEnableVertexAttribArray(1);
            offset += 3 * sizeof(float);
        }
        if (m_inputDataFormat & bwxGL_MESH_TEX_COORD) {
            glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(static_cast<intptr_t>(offset)));
            glEnableVertexAttribArray(2);
            offset += 2 * sizeof(float);
        }
        if (m_inputDataFormat & bwxGL_MESH_TANGENT) {
            glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(static_cast<intptr_t>(offset)));
            glEnableVertexAttribArray(3);
            offset += 3 * sizeof(float);
        }
        if (m_inputDataFormat & bwxGL_MESH_BITANGENT) {
            glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(static_cast<intptr_t>(offset)));
            glEnableVertexAttribArray(4);
            offset += 3 * sizeof(float);
        }
        if (m_inputDataFormat & bwxGL_MESH_COLOR) {
            glVertexAttribPointer(5, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(static_cast<intptr_t>(offset)));
            glEnableVertexAttribArray(5);
        }

        glBindVertexArray(0);
    }

    bwxGLMesh::~bwxGLMesh() {
        bwxGLBufferManager::GetInstance().ReleaseBuffer("mesh_vbo");
        bwxGLBufferManager::GetInstance().ReleaseBuffer("mesh_ebo");
        glDeleteVertexArrays(1, &m_vao);
		Delete();
    }

    void bwxGLMesh::Render() {
        //if (!m_material || !m_buffer) return;
        //m_material->Bind();
        glBindVertexArray(m_vao);

        if (m_inputDataFormat & bwxGL_MESH_INDICES) {
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
    }

    void bwxGLMesh::ConvertVerticesTableToVector(GLfloat v[], GLuint size)
    {
        if (m_vertices.size() > 0) m_vertices.clear();

        int offset = 0;

        for (unsigned int i = 0; i < size;)
        {
            bwxGLVertex tmp;

            tmp.position = glm::vec3(v[i], v[i + 1], v[i + 2]);
            offset += 3;

            if (m_inputDataFormat & bwxGL_MESH_NORMAL)
            {
                tmp.normal = glm::vec3(v[i + offset], v[i + offset + 1], v[i + offset + 2]);
                offset += 3;
            }
            else
            {
                tmp.normal = glm::vec3(0.0f, 0.0f, 0.0f);
            }

            if (m_inputDataFormat & bwxGL_MESH_TEX_COORD)
            {
                tmp.texCoord = glm::vec2(v[i + offset], v[i + offset + 1]);
                offset += 2;
            }
            else
            {
                tmp.texCoord = glm::vec2(0.0f, 0.0f);
            }

            if (m_inputDataFormat & bwxGL_MESH_TANGENT)
            {
                tmp.tangent = glm::vec3(v[i + offset], v[i + offset + 1], v[i + offset + 1]);
                offset += 3;
            }
            else
            {
                tmp.tangent = glm::vec3(0.0f, 0.0f, 0.0f);
            }

            if (m_inputDataFormat & bwxGL_MESH_BITANGENT)
            {
                tmp.bitangent = glm::vec3(v[i + offset], v[i + offset + 1], v[i + offset + 2]);
                offset += 3;
            }
            else
            {
                tmp.bitangent = glm::vec3(0.0f, 0.0f, 0.0f);
            }

            if (m_inputDataFormat & bwxGL_MESH_COLOR)
            {
                tmp.color = glm::vec3(v[i + offset], v[i + offset + 1], v[i + offset + 2]);
                offset += 3;
            }
            else
            {
                tmp.color = glm::vec3(0.0f, 0.0f, 0.0f);
            }

            if (m_inputDataFormat & bwxGL_MESH_UV)
            {
                tmp.uv = glm::vec3(v[i + offset], v[i + offset + 1], v[i + offset + 2]);
                offset += 3;
            }
            else
            {
                tmp.uv = glm::vec3(0.0f, 0.0f, 0.0f);
            }

            m_vertices.push_back(tmp);

            i += offset;
            offset = 0;
        }
    }

    void bwxGLMesh::ConvertIndicesTableToVector(GLfloat i[], GLuint size)
    {
        if (m_indices.size() > 0) m_indices.clear();

        for (unsigned int j = 0; j < size; j++)
        {
            m_indices.push_back(i[j]);
        }
    }

} // namespace bwx_sdk
