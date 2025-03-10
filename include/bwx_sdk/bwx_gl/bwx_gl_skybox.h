/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_skybox.h
// Purpose:     BWXSDK; OpenGL, SkyBox
// Author:      Bartosz Warzocha
// Created:     2020-04-10
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		All rights reserved.
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef _BWX_GL_SKYBOX_H_
#define _BWX_GL_SKYBOX_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <GL/glew.h>

#include <glm/glm.hpp>

#include "bwx_gl_shader.h"

namespace bwx_sdk {

class bwxGLSkyBox {
public:
    bwxGLSkyBox();
    bwxGLSkyBox(int size, std::vector<std::string> faces);
    bwxGLSkyBox(int size, const std::string& pos_x, const std::string& neg_x, const std::string& pos_y,
                const std::string& neg_y, const std::string& pos_z, const std::string& neg_z);
    virtual ~bwxGLSkyBox();

    void Init(GLfloat size);
    void LoadCube(std::vector<std::string> faces);

    void Render(glm::mat4* PV);

    static const char* DefaultSkyBoxVertexShader();
    static const char* DefaultSkyBoxFragmentShader();

    void UseDefaultSkyBoxShader();

private:
    GLuint VAO, VBO;
    GLuint textureID;
    // bwxGLShader* shader;
};

}  // namespace bwx_sdk

#endif
