/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_shader_generator.h
// Purpose:     BWX_SDK Libarary; OpenGL Shader Generator
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_SHADER_GENERATOR_H_
#define _BWX_GL_SHADER_GENERATOR_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <string>
#include <iostream>
#include <fstream>
#include <vector>

namespace bwx_sdk {

	class bwxGLShaderGenerator
	{
	public:
		static std::string GetVertexShader(bool useNormals = true, bool useTexCoords = true, bool useLighting = true);
		static std::string GetFragmentShader(bool useTextures = true, bool useLighting = true);
		static std::string GetDefaultSkyboxVertexShader();
		static std::string GetDefaultSkyboxFragmentShader();
		static std::string GetDefaultTTFVertexShader();
		static std::string GetDefaultTTFFragmentShader();
	};

} // namespace bwx_sdk

#endif // _BWX_GL_SHADER_GENERATOR_H_