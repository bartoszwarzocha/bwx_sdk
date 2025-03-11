/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_ttf.h
// Purpose:     BWX_SDK Libarary; OpenGL True Type Font
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_TTF_H_
#define _BWX_GL_TTF_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "bwx_gl_shader.h"

#include <string>
#include <map>

namespace bwx_sdk {

	struct bwxGLTTFCharacter {
		GLuint    textureID;  ///< ID handle of the glyph texture
		glm::vec2 size;       ///< Size of glyph
		glm::vec2 bearing;    ///< Offset from baseline to left/top of glyph
		GLuint    advance;    ///< Offset to advance to next glyph
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

} // namespace bwx_sdk

#endif