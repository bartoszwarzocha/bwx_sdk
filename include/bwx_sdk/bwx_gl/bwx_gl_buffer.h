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

#include <initializer_list>
#include <vector>

#include "bwx_gl_resource_manager.h"

namespace bwx_sdk {

class bwxGLBuffer : public bwxGLResource {
public:
    bwxGLBuffer();
    bwxGLBuffer(GLenum target);
    bwxGLBuffer(const std::vector<float>& data, GLsizei stride, const std::vector<GLint>& layout,
                GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW);
    bwxGLBuffer(GLsizeiptr size, GLsizei stride, const std::vector<GLint>& layout, GLenum target = GL_ARRAY_BUFFER,
                GLenum usage = GL_DYNAMIC_DRAW);

    void Create(const std::vector<float>& data, GLsizei stride, const std::vector<GLint>& layout,
                GLenum target = GL_ARRAY_BUFFER, GLenum usage = GL_STATIC_DRAW);
    void Create(GLsizeiptr size, GLsizei stride, const std::vector<GLint>& layout, GLenum target = GL_ARRAY_BUFFER,
                GLenum usage = GL_DYNAMIC_DRAW);

    virtual ~bwxGLBuffer();

    void Bind() const override;
    void Unbind() const override;
    void Release() override;
    void Unload() override;
    void Delete() override;

    inline GLuint GetID() const { return m_bufferID; }
    inline GLuint GetVAO() const { return m_vaoID; }

    void SetData(const void* data, GLsizeiptr size, GLenum usage = GL_STATIC_DRAW);
    void SetData(const std::vector<GLuint>& indices, GLenum usage = GL_STATIC_DRAW);

protected:
    GLuint m_bufferID = 0;
    GLuint m_vaoID = 0;
    GLenum m_target = GL_ARRAY_BUFFER;
};

}  // namespace bwx_sdk

#endif
