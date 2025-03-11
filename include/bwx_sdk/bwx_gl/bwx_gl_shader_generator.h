/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_shader_generator.h
// Purpose:     BWX_SDK Libarary; OpenGL Shader Generator
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

#ifndef _BWX_GL_SHADER_GENERATOR_H_
#define _BWX_GL_SHADER_GENERATOR_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <fstream>
#include <iostream>
#include <string>
#include <vector>

namespace bwx_sdk {

class bwxGLShaderGenerator {
public:
    static std::string GenerateVertexShader(bool useNormals = true, bool useTexCoords = true, bool useLighting = true);
    static std::string GenerateFragmentShader(bool useTextures = true, bool useLighting = true);
    static std::string GenerateSkyboxVertexShader();
    static std::string GenerateSkyboxFragmentShader();
};

}  // namespace bwx_sdk

#endif  // _BWX_GL_SHADER_GENERATOR_H_
