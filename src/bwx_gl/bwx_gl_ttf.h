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
#include "bwx_gl_buffer.h"

#include <string>
#include <map>
#include <vector>
#include <memory>

namespace bwx_sdk {

	/**
	 * @brief Class for loading and rendering True Type Fonts in OpenGL.
	 *
	 * This class is used to load a True Type Font from a file and render text
	 * using the loaded font. The font is loaded into a texture atlas, which
	 * is then used to render the text.
	 */
	class bwxGLTTF {
	public:
		/**
		 * @brief Structure representing a glyph in a True Type Font.
		 *
		 * This structure represents a glyph in a True Type Font. The glyph
		 * contains information about the size, bearing, advance, and UV
		 * coordinates of the glyph in the texture atlas.
		 */
		struct bwxGLTTFGlyph {
			glm::ivec2 size;         ///< Glyph size
			glm::ivec2 bearing;      ///< Position relative to the baseline
			GLuint advance;          ///< Offset to the next glyph
			glm::vec2 uvTopLeft;     ///< UV coordinates in the atlas
			glm::vec2 uvBottomRight; ///< UV coordinates in the atlas
		};

		/**
		 * @brief Loads a True Type Font from a file.
		 *
		 * This function loads a True Type Font from a file and creates a texture
		 * atlas containing all the glyphs from the font. The function returns
		 * true if the font was loaded successfully, and false otherwise.
		 *
		 * @param filepath Path to the font file.
		 * @param pixelHeight Height of the font in pixels.
		 * @return True if the font was loaded successfully, false otherwise.
		 */
		bool LoadFromFile(const std::string& filepath, int pixelHeight = 48);

		/**
		 * @brief Returns the texture atlas containing the font glyphs.
		 *
		 * This function returns the OpenGL texture ID of the texture atlas
		 * containing the font glyphs.
		 *
		 * @return OpenGL texture ID of the texture atlas.
		 */
		GLuint GetTextureAtlas() const;
		
		/**
		 * @brief Returns the glyph for a given character.
		 *
		 * This function returns the glyph for a given character. The glyph
		 * contains information about the size, bearing, advance, and UV
		 * coordinates of the glyph in the texture atlas.
		 *
		 * @param c Character for which to get the glyph.
		 * @return Glyph for the given character.
		 */
		bwxGLTTFGlyph& GetGlyph(wchar_t c);

		/**
		 * @brief Returns the map of glyphs for the font.
		 *
		 * This function returns the map of glyphs for the font. The map contains
		 * a glyph for each character in the font.
		 *
		 * @return Map of glyphs for the font.
		 */
		std::map<GLchar, bwxGLTTF::bwxGLTTFGlyph>& GetGlyphs();

		/**
		 * @brief Returns the width of a glyph.
		 *
		 * This function returns the width of a glyph for the specified character.
		 *
		 * @param c Character for which to get the glyph width.
		 * @return Width of the glyph.
		 */
		int GetGlyphHeight(wchar_t c) const;
		
		/**
		 * @brief Returns the height of a glyph.
		 *
		 * This function returns the height of a glyph for the specified character.
		 *
		 * @param c Character for which to get the glyph height.
		 * @return Height of the glyph.
		 */
		int GetGlyphWidth(wchar_t c) const;

		/**
		 * @brief Sets the charset for the font.
		 *
		 * This function sets the charset for the font. The charset is a string
		 * containing all the characters that should be included in the texture
		 * atlas. By default, the charset contains all printable ASCII characters.
		 *
		 * @param charset Charset to set.
		 */
		void SetCharset(const std::wstring& charset);

		/**
		 * @brief Sets the charset to the default charset of Polish language.
		 *
		 * This function sets the charset to the Polish language. The charset is 
		 * a string containing all the characters that should be included in the 
		 * texture atlas. By default, the charset contains all printable ASCII 
		 * characters.
		 * 
		 */
		void SetCharsetPL();

		/**
		 * @brief Sets the charset to the default charset of English language.
		 *
		 * This function sets the charset to the English language. The charset is
		 * a string containing all the characters that should be included in the
		 * texture atlas. By default, the charset contains all printable ASCII
		 * characters.
		 *
		 */
		void SetCharsetEN();

		/**
		 * @brief Sets the charset to the default charset of Russian language.
		 *
		 * This function sets the charset to the Russian language. The charset is
		 * a string containing all the characters that should be included in the
		 * texture atlas. By default, the charset contains all printable ASCII
		 * characters.
		 *
		 */
		void SetCharsetRU();

		/**
		 * @brief Sets the charset to the default charset of German language.
		 *
		 * This function sets the charset to the German language. The charset is
		 * a string containing all the characters that should be included in the
		 * texture atlas. By default, the charset contains all printable ASCII
		 * characters.
		 *
		 */
		void SetCharsetDE();

		/**
		 * @brief Sets the charset to the default charset of French language.
		 *
		 * This function sets the charset to the French language. The charset is
		 * a string containing all the characters that should be included in the
		 * texture atlas. By default, the charset contains all printable ASCII
		 * characters.
		 *
		 */
		void SetCharsetFR();

		/**
		 * @brief Sets the charset to the default charset of Spanish language.
		 *
		 * This function sets the charset to the Spanish language. The charset is
		 * a string containing all the characters that should be included in the
		 * texture atlas. By default, the charset contains all printable ASCII
		 * characters.
		 *
		 */
		void SetCharsetES();

		/**
		 * @brief Sets the charset to the default charset of Italian language.
		 *
		 * This function sets the charset to the Italian language. The charset is
		 * a string containing all the characters that should be included in the
		 * texture atlas. By default, the charset contains all printable ASCII
		 * characters.
		 *
		 */
		void SetCharsetIT();

	private:
		std::wstring m_charset =
			L" !\"#$%&'()*+,-./0123456789:;<=>?@"
			L"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; ///< Default charset

		GLuint m_textureAtlas; ///< OpenGL texture ID of the texture atlas
		std::map<GLchar, bwxGLTTFGlyph> m_glyphs; ///< Map of glyphs for each character
	};

	/**
	 * @brief Class for rendering text using a True Type Font.
	 *
	 * This class is used to render text using a True Type Font. The text is
	 * rendered using the font's texture atlas and a shader program.
	 */
	class bwxGLText {
	public:

		/**
		 * @brief Constructs a bwxGLText object.
		 *
		 * This constructor creates a bwxGLText object using the specified
		 * True Type Font.
		 *
		 * @param font True Type Font to use for rendering text.
		 */
		bwxGLText(bwxGLTTF& font);

		/**
		 * @brief Destroys the bwxGLText object.
		 *
		 * This destructor destroys the bwxGLText object.
		 */
		~bwxGLText();

		/**
		 * @brief Sets the True Type Font to use for rendering text.
		 *
		 * This function sets the True Type Font to use for rendering text.
		 *
		 * @param font True Type Font to use for rendering text.
		 */
		void SetFont(bwxGLTTF& font);

		/**
		 * @brief Returns the height of the font.
		 *
		 * This function returns the height of the font in pixels.
		 *
		 * @return Height of the font.
		 */
		int GetFontHeight();

		/**
		 * @brief Renders text at a specified position.
		 *
		 * This function renders the specified text at the specified position
		 * using the specified scale and color. The text is rendered using the
		 * font's texture atlas and the shader program set using SetShader().
		 *
		 * @param text Text to render.
		 * @param position Position at which to render the text.
		 * @param scale Scale at which to render the text.
		 * @param color Color in which to render the text.
		 */
		void Render(const std::wstring& text, const glm::mat4& orth, const glm::vec2& pos, GLfloat scale, const glm::vec4& color);

		/**
		 * @brief Sets the effect parameters for rendering text.
		 *
		 * This function sets the effect parameters for rendering text. The
		 * effect parameters are used to apply various effects to the text,
		 * such as outline, shadow, and glow.
		 *
		 * @param params Effect parameters to set.
		 */
		 //void SetEffectParams(const EffectParams& params);

		 /**
		  * @brief Sets the shader program to use for rendering text.
		  *
		  * This function sets the shader program to use for rendering text. The
		  * shader program should be a text shader program that supports rendering
		  * text using the font's texture atlas.
		  *
		  * @param shader Shader program to use for rendering text.
		  */
		void SetShaderProgram(std::shared_ptr<bwxGLShaderProgram> shader);

		/**
		 * @brief Sets the default shader program for rendering text.
		 *
		 * This function sets the default shader program for rendering text. The
		 * default shader program is a simple text shader program that supports
		 * rendering text using the font's texture atlas.
		 * Default shader program uses the following uniforms:
		 * projection - projection matrix
		 * textColor - text color
		 */
		void SetDefaultShaderProgram();

	private:
		bwxGLTTF& m_font; ///< True Type Font to use for rendering text
		std::shared_ptr<bwxGLShaderProgram> m_shaderProgram; ///< Shader program to use for rendering text
		std::shared_ptr<bwxGLBuffer> m_dynamicBuffer; ///< Dynamic buffer for rendering text
	};

} // namespace bwx_sdk

#endif