/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_ttf.h
// Purpose:     BWX_SDK Libarary; OpenGL True Type Font
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

#ifndef _BWX_GL_TTF_H_
#define _BWX_GL_TTF_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <map>
#include <string>

#include "bwx_gl_shader.h"

namespace bwx_sdk {

struct bwxGLTTFCharacter {
    GLuint textureID;
    glm::vec2 size;
    glm::vec2 bearing;
    GLuint advance;
};

class bwxGLTTF {
public:
    bwxGLTTF();
    bwxGLTTF(const int& first, const int& last);
    bwxGLTTF(const glm::vec2& char_range);
    ~bwxGLTTF();

    void Init();

    void Render(const std::string& text, const glm::mat4& ortho, GLfloat x, GLfloat y, GLfloat scale, glm::vec3 color);

    static const char* DefaultTTFVertexShader();
    static const char* DefaultTTFFragmentShader();

    void UseDefaultTTFShader();

private:
    int m_firstChar;
    int m_lastChar;
    GLuint m_VAO, m_VBO;
    std::map<GLchar, bwxGLTTFCharacter> chars;
    bwxGLShader* shader;
};

}  // namespace bwx_sdk

#endif
