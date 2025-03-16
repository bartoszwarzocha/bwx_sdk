/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_buffer.h
// Purpose:     BWX_SDK Libarary; OpenGL Buffer
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

#ifndef _BWX_GL_BUFFER_H_
#define _BWX_GL_BUFFER_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <iostream>
#include <vector>

namespace bwx_sdk {

enum class bwxGL_BUFFER_TYPE { VERTEX, INDEX, UNIFORM };

class bwxGLBuffer {
public:
    bwxGLBuffer() : m_bufferID(0), m_target(GL_ARRAY_BUFFER) {}
    bwxGLBuffer(GLenum target);
    virtual ~bwxGLBuffer();

    void Bind() const;
    void Unbind() const;
    void Release();
    GLuint GetID() const { return m_bufferID; }

    void SetData(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);
    void SetData(const std::vector<GLuint>& indices, GLenum usage = GL_STATIC_DRAW);

protected:
    GLuint m_bufferID;
    GLenum m_target;
};

}  // namespace bwx_sdk

#endif
