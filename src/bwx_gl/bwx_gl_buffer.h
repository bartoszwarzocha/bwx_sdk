/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_buffer.h
// Purpose:     BWX_SDK Libarary; OpenGL Buffer
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_BUFFER_H_
#define _BWX_GL_BUFFER_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <vector>
#include <iostream>

#include <GL/glew.h>
#include <glm/glm.hpp>

namespace bwx_sdk {

    enum class bwxGL_BUFFER_TYPE {
        VERTEX,
        INDEX,
        UNIFORM
    };

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

} // namespace bwx_sdk

#endif
