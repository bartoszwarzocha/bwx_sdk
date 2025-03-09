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

    // bwxGLBuffer (Base)
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

    // bwxGLVertexBuffer
    bwxGLVertexBuffer::bwxGLVertexBuffer() : bwxGLBuffer(GL_ARRAY_BUFFER) {}

    void bwxGLVertexBuffer::UploadData(const void* data, GLsizeiptr size, GLenum usage) {
        Bind();
        glBufferData(GL_ARRAY_BUFFER, size, data, usage);
        Unbind();
    }

    // bwxGLIndexBuffer
    bwxGLIndexBuffer::bwxGLIndexBuffer() : bwxGLBuffer(GL_ELEMENT_ARRAY_BUFFER), m_count(0) {}

    void bwxGLIndexBuffer::UploadData(const std::vector<GLuint>& indices, GLenum usage) {
        Bind();
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), usage);
        m_count = static_cast<GLsizei>(indices.size());
        Unbind();
    }

    // bwxGLUniformBuffer
    bwxGLUniformBuffer::bwxGLUniformBuffer() : bwxGLBuffer(GL_UNIFORM_BUFFER) {}

    void bwxGLUniformBuffer::UploadData(const void* data, GLsizeiptr size, GLenum usage) {
        Bind();
        glBufferData(GL_UNIFORM_BUFFER, size, data, usage);
        Unbind();
    }

    void bwxGLUniformBuffer::BindToPoint(GLuint bindingPoint) {
        glBindBufferBase(GL_UNIFORM_BUFFER, bindingPoint, m_bufferID);
    }

} // namespace bwx_sdk
