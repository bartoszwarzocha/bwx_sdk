/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_shader.h
// Purpose:     BWX_SDK Libarary; OpenGL Shader
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

#ifndef _BWX_GL_SHADER_H_
#define _BWX_GL_SHADER_H_

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#else
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#endif

namespace bwx_sdk {

enum bwxGL_SHADER_TYPE : GLenum {
    SHADER_VERTEX,
    SHADER_TESS_CONTROL,
    SHADER_TESS_EVALUATION,
    SHADER_GEOMETRY,
    SHADER_FRAGMENT,
    SHADER_COMPUTE
};

class bwxGLShader {
public:
    bwxGLShader();
    ~bwxGLShader();

    bool LoadShader(bwxGL_SHADER_TYPE type, const std::string& source, bool fromFile = false);
    void AttachToProgram(GLuint program);
    void DeleteShader();

    GLuint GetID() const { return id; }

private:
    GLuint id;
};

// Helper functions for uniform handling
inline void glUniformHelper(GLint location, GLfloat v0) { glUniform1f(location, v0); }
inline void glUniformHelper(GLint location, GLfloat v0, GLfloat v1) { glUniform2f(location, v0, v1); }
inline void glUniformHelper(GLint location, GLfloat v0, GLfloat v1, GLfloat v2) { glUniform3f(location, v0, v1, v2); }
inline void glUniformHelper(GLint location, GLfloat v0, GLfloat v1, GLfloat v2, GLfloat v3) {
    glUniform4f(location, v0, v1, v2, v3);
}

inline void glUniformHelper(GLint location, const glm::vec2& v) { glUniform2f(location, v.x, v.y); }
inline void glUniformHelper(GLint location, const glm::vec3& v) { glUniform3f(location, v.x, v.y, v.z); }
inline void glUniformHelper(GLint location, const glm::vec4& v) { glUniform4f(location, v.x, v.y, v.z, v.w); }

inline void glUniformHelper(GLint location, const glm::mat4& mat) {
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
}

class bwxGLShaderProgram {
public:
    bwxGLShaderProgram();
    ~bwxGLShaderProgram();

    void AttachShader(const bwxGLShader& shader);
    bool Link();
    void Use();
    void DeleteProgram();

    template <typename... Args>
    void SetUniform(const std::string& name, Args... args) {
        GLint location = GetUniformLocation(name);
        if (location != -1) {
            glUniformHelper(location, args...);
        }
    }

private:
    GLint GetUniformLocation(const std::string& name);
    std::unordered_map<std::string, GLint> uniformCache;

    GLuint program;
};

}  // namespace bwx_sdk

#endif
