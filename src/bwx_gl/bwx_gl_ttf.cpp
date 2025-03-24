/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_ttf.cpp
// Purpose:     BWX_SDK Libarary; OpenGL True Type Font
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <ft2build.h>
#include FT_FREETYPE_H

#include <memory>

#include "bwx_gl_ttf.h"
#include "bwx_gl_shader.h"
#include "bwx_gl_shader_generator.h"
#include "bwx_gl_buffer.h"

namespace bwx_sdk {

	void bwxGLTTF::SetCharset(const std::wstring& charset)
	{
		m_charset = charset;
	}

    void bwxGLTTF::SetCharsetPL()
    {
        m_charset =
            L" !\"#$%&'()*+,-./0123456789:;<=>?@"
            L"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
            L"ĄĆĘŁŃÓŚŹŻąćęłńóśźż"; ///< Default charset (with Polish characters)
    }

	void bwxGLTTF::SetCharsetEN()
	{
		m_charset =
			L" !\"#$%&'()*+,-./0123456789:;<=>?@"
			L"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"; ///< Default charset (with English characters)
	}

	void bwxGLTTF::SetCharsetRU()
	{
		m_charset =
			L" !\"#$%&'()*+,-./0123456789:;<=>?@"
			L"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
			L"АБВГДЕЁЖЗИЙКЛМНОПРСТУФХЦЧШЩЪЫЬЭЮЯ"
			L"абвгдеёжзийклмнопрстуфхцчшщъыьэюя"; ///< Default charset (with Russian characters)
	}

	void bwxGLTTF::SetCharsetDE()
	{
		m_charset =
			L" !\"#$%&'()*+,-./0123456789:;<=>?@"
			L"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
			L"ÄÖÜäöüß"; ///< Default charset (with German characters)
	}

	void bwxGLTTF::SetCharsetFR()
	{
		m_charset =
			L" !\"#$%&'()*+,-./0123456789:;<=>?@"
			L"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
			L"ÀÂÆÇÉÈÊËÎÏÔŒÙÛÜŸàâæçéèêëîïôœùûüÿ"; ///< Default charset (with French characters)
	}

	void bwxGLTTF::SetCharsetES()
	{
		m_charset =
			L" !\"#$%&'()*+,-./0123456789:;<=>?@"
			L"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
			L"ÁÉÍÓÚÜÑáéíóúüñ"; ///< Default charset (with Spanish characters)
	}

	void bwxGLTTF::SetCharsetIT()
	{
		m_charset =
			L" !\"#$%&'()*+,-./0123456789:;<=>?@"
			L"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"
			L"ÀÈÉÌÒÓÙàèéìòóù"; ///< Default charset (with Italian characters)
	}

    bool bwxGLTTF::LoadFromFile(const std::string& filepath, int pixelHeight)
    {
        FT_Library ft;
        if (FT_Init_FreeType(&ft))
        {
            std::cerr << "Failed to initialize FreeType." << std::endl;
            return false;
        }

        FT_Face face;
        if (FT_New_Face(ft, filepath.c_str(), 0, &face))
        {
            std::cerr << "Failed to load font face." << std::endl;
            FT_Done_FreeType(ft);
            return false;
        }

        FT_Set_Pixel_Sizes(face, 0, pixelHeight);

        glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
        glGenTextures(1, &m_textureAtlas);
        glBindTexture(GL_TEXTURE_2D, m_textureAtlas);

        int atlasWidth = 0;
        int atlasHeight = 0;
        for (wchar_t c : m_charset)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                continue;

            atlasWidth += face->glyph->bitmap.width;
            atlasHeight = std::max(atlasHeight, static_cast<int>(face->glyph->bitmap.rows));
        }

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

        int offsetX = 0;
        for (wchar_t c : m_charset)
        {
            if (FT_Load_Char(face, c, FT_LOAD_RENDER))
                continue;

            FT_Bitmap& bitmap = face->glyph->bitmap;

            glTexSubImage2D(GL_TEXTURE_2D, 0, offsetX, 0, bitmap.width, bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, bitmap.buffer);

            bwxGLTTFGlyph glyph;
            glyph.size = { bitmap.width, bitmap.rows };
            glyph.bearing = { face->glyph->bitmap_left, face->glyph->bitmap_top };
            glyph.advance = face->glyph->advance.x >> 6;
            glyph.uvTopLeft = glm::vec2(offsetX / static_cast<float>(atlasWidth), bitmap.rows / static_cast<float>(atlasHeight));
            glyph.uvBottomRight = glm::vec2((offsetX + bitmap.width) / static_cast<float>(atlasWidth), 0);

            m_glyphs[c] = glyph;

            offsetX += bitmap.width;
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        FT_Done_Face(face);
        FT_Done_FreeType(ft);

        return true;
    }

	GLuint bwxGLTTF::GetTextureAtlas() const
	{
		return m_textureAtlas;
	}

	bwxGLTTF::bwxGLTTFGlyph& bwxGLTTF::GetGlyph(wchar_t c)
	{
		return m_glyphs.at(c);
	}

    std::map<GLchar, bwxGLTTF::bwxGLTTFGlyph>& bwxGLTTF::GetGlyphs() { return m_glyphs; }

    int bwxGLTTF::GetGlyphHeight(wchar_t c) const { return m_glyphs.at(c).size.y; }

    int bwxGLTTF::GetGlyphWidth(wchar_t c) const { return m_glyphs.at(c).size.x; }
    
    bwxGLText::bwxGLText(bwxGLTTF& font) : m_font(font)
	{
		GLsizeiptr bufferSize = sizeof(GLfloat) * 6 * 4;
        m_dynamicBuffer = std::make_shared<bwxGLBuffer>();
        m_dynamicBuffer->Create(bufferSize, 4, { 4 }, GL_ARRAY_BUFFER, GL_DYNAMIC_DRAW);
		SetDefaultShaderProgram();
    }

	bwxGLText::~bwxGLText()
	{
		m_dynamicBuffer->Release();
		m_dynamicBuffer.reset();
	}

	void bwxGLText::SetShaderProgram(std::shared_ptr<bwxGLShaderProgram> shader)
	{
		m_shaderProgram = shader;
	}

	void bwxGLText::SetDefaultShaderProgram()
	{
		m_shaderProgram = std::make_shared<bwxGLShaderProgram>();
		m_shaderProgram->AttachShader(bwxGLShader(SHADER_VERTEX, bwxGLShaderGenerator::GetDefaultTTFVertexShader()));
		m_shaderProgram->AttachShader(bwxGLShader(SHADER_FRAGMENT, bwxGLShaderGenerator::GetDefaultTTFFragmentShader()));
        if (m_shaderProgram->Link())
        {
            m_shaderProgram->Bind();
			m_shaderProgram->AddUniform("projection"); // Add to uniform cache if exists in program
			m_shaderProgram->AddUniform("textColor"); // Add to uniform cache if exists in program
            m_shaderProgram->Unbind();
        }
	}

    void bwxGLText::SetFont(bwxGLTTF& font) {
        m_font = font;
    }

	int bwxGLText::GetFontHeight() {
		return m_font.GetGlyph('H').size.y;
	}

    void bwxGLText::Render(const std::wstring& text, const glm::mat4& orth, const glm::vec2& pos, GLfloat scale, const glm::vec4& color)
    {
        // Activate corresponding render state
        m_shaderProgram->Bind();
        m_shaderProgram->SetUniform("projection", orth);
        m_shaderProgram->SetUniform("textColor", color[0], color[1], color[2], color[3]);

        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_font.GetTextureAtlas());

        // Initialize VAO/VBO once (ideally this should be done during class initialization)
        glBindVertexArray(m_dynamicBuffer->GetVAO());

        GLfloat x = pos.x;
        GLfloat y = pos.y;

        for (auto c : text)
        {
            const auto& ch = m_font.GetGlyph(c);

            GLfloat xpos = x + ch.bearing.x * scale;
            GLfloat ypos = y - (ch.size.y - ch.bearing.y) * scale;

            GLfloat w = ch.size.x * scale;
            GLfloat h = ch.size.y * scale;

            GLfloat vertices[6][4] = {
                { xpos,     ypos + h,   ch.uvTopLeft.x,     ch.uvBottomRight.y },
                { xpos,     ypos,       ch.uvTopLeft.x,     ch.uvTopLeft.y },
                { xpos + w, ypos,       ch.uvBottomRight.x, ch.uvTopLeft.y },

                { xpos,     ypos + h,   ch.uvTopLeft.x,     ch.uvBottomRight.y },
                { xpos + w, ypos,       ch.uvBottomRight.x, ch.uvTopLeft.y },
                { xpos + w, ypos + h,   ch.uvBottomRight.x, ch.uvBottomRight.y }
            };

            glBindBuffer(GL_ARRAY_BUFFER, m_dynamicBuffer->GetID());
            glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);

            glDrawArrays(GL_TRIANGLES, 0, 6);

            x += (ch.advance) * scale;
        }

        glBindVertexArray(0);
        glBindTexture(GL_TEXTURE_2D, 0);
        m_shaderProgram->Unbind();
    }

} // namespace bwx_sdk