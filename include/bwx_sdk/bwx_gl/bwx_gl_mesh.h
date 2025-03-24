/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_mesh.h
// Purpose:     BWX_SDK Library; OpenGL Mesh
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

#ifndef _BWX_GL_MESH_H_
#define _BWX_GL_MESH_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

#include "bwx_gl_buffer.h"

namespace bwx_sdk {

#define bwxGL_MESH_NORMAL 0x00000001
#define bwxGL_MESH_TEX_COORD 0x00000002
#define bwxGL_MESH_TANGENT 0x00000004
#define bwxGL_MESH_BITANGENT 0x00000008
#define bwxGL_MESH_COLOR 0x00000010
#define bwxGL_MESH_UV 0x00000020

#define bwxGL_MESH_INDICES 0x00001000
#define bwxGL_MESH_DEFAULT bwxGL_MESH_NORMAL | bwxGL_MESH_TEX_COORD

struct bwxGLVertex {
    glm::vec3 position;
    glm::vec3 normal;
    glm::vec2 texCoord;
    glm::vec3 tangent;
    glm::vec3 bitangent;
    glm::vec3 color;
    glm::vec3 uv;
};

class bwxGLMesh {
public:
    bwxGLMesh(int style);
    virtual ~bwxGLMesh();

    void Render();
    void Delete();

    inline void AddVertex(const bwxGLVertex& v) { m_vertices.push_back(v); }
    inline void SetVertices(const std::vector<bwxGLVertex>& v) { m_vertices = v; }

    inline void AddIndice(GLuint i) { m_indices.push_back(i); }
    inline void SetIndices(const std::vector<GLuint>& i) { m_indices = i; }

    void ConvertVerticesTableToVector(GLfloat v[], GLuint size);
    void ConvertIndicesTableToVector(GLfloat i[], GLuint size);

    void SetupMesh();

    inline bwxGLBuffer* GetVBO() { return m_vbo; }
    inline bwxGLBuffer* GetEBO() { return m_ebo; }
    inline GLuint GetVAO() const { return m_vao; }

private:
    std::vector<bwxGLVertex> m_vertices;
    std::vector<GLuint> m_indices;
    int m_inputDataFormat;

    bwxGLBuffer* m_vbo = nullptr;
    bwxGLBuffer* m_ebo = nullptr;
    GLuint m_vao = 0;
};

}  // namespace bwx_sdk

#endif
