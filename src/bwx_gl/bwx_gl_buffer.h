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

#include <vector>

#if defined(__APPLE__)
    #error OpenGL functionality is not available for macOS.
#else
    #include <GL/glew.h>
#endif

namespace bwx_sdk {

    enum class bwxGL_BUFFER_TYPE {
        VERTEX,
        INDEX,
        UNIFORM
    };

    class bwxGLBuffer {
    public:
        bwxGLBuffer(GLenum target);
        virtual ~bwxGLBuffer();

        void Bind() const;
        void Unbind() const;
        void Release();
        GLuint GetID() const { return m_bufferID; }

    protected:
        GLuint m_bufferID;
        GLenum m_target;
    };

    class bwxGLVertexBuffer : public bwxGLBuffer {
    public:
        bwxGLVertexBuffer();
        void UploadData(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);
    };

    class bwxGLIndexBuffer : public bwxGLBuffer {
    public:
        bwxGLIndexBuffer();
        void UploadData(const std::vector<GLuint>& indices, GLenum usage = GL_STATIC_DRAW);
        GLsizei GetCount() const { return m_count; }

    private:
        GLsizei m_count;
    };

    class bwxGLUniformBuffer : public bwxGLBuffer {
    public:
        bwxGLUniformBuffer();
        void UploadData(const void* data, GLsizeiptr size, GLenum usage = GL_DYNAMIC_DRAW);
        void BindToPoint(GLuint bindingPoint);
    };

} // namespace bwx_sdk

#endif
