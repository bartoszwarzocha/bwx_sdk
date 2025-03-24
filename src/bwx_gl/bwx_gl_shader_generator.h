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
#include <unordered_map>

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

		// LIGHTS
		static std::string GetLightStructBlock();
		static std::string GetLightCalculationFunction();

	private:
		static std::unordered_map<std::string, std::string> m_shaderCache;
		static std::string GenerateVertexShader(bool useNormals, bool useTexCoords, bool useLighting);
		static std::string GenerateFragmentShader(bool useTextures, bool useLighting);
	};

} // namespace bwx_sdk

#endif // _BWX_GL_SHADER_GENERATOR_H_