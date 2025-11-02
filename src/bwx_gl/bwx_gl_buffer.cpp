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

#include <bwx_sdk/bwx_gl/bwx_gl_buffer.h>

namespace bwx_sdk {

    bwxGLBuffer::bwxGLBuffer() : m_target(GL_ARRAY_BUFFER) {
        glGenBuffers(1, &m_bufferID);
    }

    bwxGLBuffer::bwxGLBuffer(GLenum target) : m_target(target) {
        glGenBuffers(1, &m_bufferID);
    }

    bwxGLBuffer::bwxGLBuffer(const std::vector<float>& data, GLsizei stride, const std::vector<GLint>& layout, GLenum target, GLenum usage) {
		Create(data, stride, layout, target, usage);
    }

	void bwxGLBuffer::Create(const std::vector<float>& data, GLsizei stride, const std::vector<GLint>& layout, GLenum target, GLenum usage) {
		m_target = target;

        glGenBuffers(1, &m_bufferID);
        glGenVertexArrays(1, &m_vaoID);

        glBindVertexArray(m_vaoID);

        Bind();
        glBufferData(m_target, data.size() * sizeof(float), data.data(), usage);

        GLsizei offset = 0;
        for (GLuint index = 0; index < layout.size(); ++index) {
            GLint size = layout[index];
            glVertexAttribPointer(index, size, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
            glEnableVertexAttribArray(index);
            offset += size;
        }

        glBindVertexArray(0);
        Unbind();
	}

    bwxGLBuffer::bwxGLBuffer(GLsizeiptr size, GLsizei stride, const std::vector<GLint>& layout, GLenum target, GLenum usage) {
		Create(size, stride, layout, target, usage);
    }

    void bwxGLBuffer::Create(GLsizeiptr size, GLsizei stride, const std::vector<GLint>& layout, GLenum target, GLenum usage) {
        m_target = target;

        glGenBuffers(1, &m_bufferID);
        glGenVertexArrays(1, &m_vaoID);

        glBindVertexArray(m_vaoID);

        Bind();
        glBufferData(m_target, size, nullptr, usage);

        GLsizei offset = 0;
        for (GLuint index = 0; index < layout.size(); ++index) {
            GLint sizeComp = layout[index];
            glVertexAttribPointer(index, sizeComp, GL_FLOAT, GL_FALSE, stride * sizeof(float), (void*)(offset * sizeof(float)));
            glEnableVertexAttribArray(index);
            offset += sizeComp;
        }

        glBindBuffer(m_target, 0);
        glBindVertexArray(0);
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
        if (m_vaoID) {
            glDeleteVertexArrays(1, &m_vaoID);
            m_vaoID = 0;
        }
    }

    void bwxGLBuffer::Unload() {
        Release();
    }

    void bwxGLBuffer::Delete() {
        Release();
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
