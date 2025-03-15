/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_buffer.cpp
// Purpose:     BWX_SDK Libarary; OpenGL Buffer
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include "bwx_gl_buffer.h"

namespace bwx_sdk {

    bwxGLBuffer::bwxGLBuffer(GLenum target) : m_target(target) {
        glGenBuffers(1, &m_bufferID);
    }

    bwxGLBuffer::~bwxGLBuffer() {
        Release();
    }

    void bwxGLBuffer::Bind() const {
        glBindBuffer(m_target, m_bufferID);
    }

    void bwxGLBuffer::Unbind() const {
        glBindBuffer(m_target, 0);
    }

    void bwxGLBuffer::Release() {
        if (m_bufferID) {
            glDeleteBuffers(1, &m_bufferID);
            m_bufferID = 0;
        }
    }

    void bwxGLBuffer::SetData(const void* data, GLsizeiptr size, GLenum usage) {
        Bind();
        glBufferData(m_target, size, data, usage);
        Unbind();
    }

    void bwxGLBuffer::SetData(const std::vector<GLuint>& indices, GLenum usage) {
        SetData(indices.data(), indices.size() * sizeof(GLuint), usage);
    }

} // namespace bwx_sdk
