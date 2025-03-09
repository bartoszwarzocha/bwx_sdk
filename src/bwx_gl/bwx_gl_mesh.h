/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_mesh.h
// Purpose:     BWX_SDK Libarary; OpenGL Mesh
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_MESH_H_
#define _BWX_GL_MESH_H_

#include <vector>

#include "bwx_gl_buffer.h"
#include "bwx_gl_material.h"
#include "bwx_gl_shader.h"

#if defined(__APPLE__)
    #error OpenGL functionality is not available for macOS.
#else
    #include <GL/glew.h>
    #include <glm/glm.hpp>
#endif

namespace bwx_sdk {

    #define bwxGLMESH_NORMAL       0x00000001
    #define bwxGLMESH_TEX_COORD    0x00000002
    #define bwxGLMESH_TANGENT      0x00000004
    #define bwxGLMESH_BITANGENT    0x00000008
    #define bwxGLMESH_COLOR        0x00000010
    #define bwxGLMESH_UV           0x00000020

    #define bwxGLMESH_INDICES      0x00001000
    #define bwxGLMESH_DEFAULT      bwxGLMESH_NORMAL | bwxGLMESH_TEX_COORD

    struct bwxGLVertex {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec2 tex_coord;
        glm::vec3 tangent;
        glm::vec3 bitangent;
        glm::vec3 color;
    };

    class bwxGLMesh {
    public:
        bwxGLMesh(int style);
        virtual ~bwxGLMesh() { Delete(); }

        void Render(bwxGLShaderProgram& shader, glm::mat4* model, bool transparent);
        void Delete();

        inline void AddVertex(const bwxGLVertex& v) { m_vertices.push_back(v); }
        inline void SetVertices(const std::vector<bwxGLVertex>& v) { m_vertices = v; }

        inline void AddIndice(GLuint i) { m_indices.push_back(i); }
        inline void SetIndices(const std::vector<GLuint>& i) { m_indices = i; }

        void ConvertVerticesTableToVector(GLfloat v[], GLuint size);
        void ConvertIndicesTableToVector(GLfloat i[], GLuint size);

        inline void SetMaterial(const bwxGLMaterial& m) { m_material = m; }
        inline bwxGLMaterial& GetMaterial() { return m_material; }

        inline void SetStyle(int style) { m_inputDataFormat = style; }
        inline int& GetStyle() { return m_inputDataFormat; }

        void SetupMesh();

    private:
        GLuint m_VAO;
        bwxGLVertexBuffer m_VBO;
        bwxGLIndexBuffer m_EBO;

        std::vector<bwxGLVertex> m_vertices;
        std::vector<GLuint> m_indices;

        int m_inputDataFormat;
        bwxGLMaterial m_material;
    };

} // namespace bwx_sdk

#endif
