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

#include <bwx_sdk/bwx_gl/bwx_gl_buffer.h>
#include <glm/glm.hpp>
#include <map>
#include <memory>
#include <string>
#include <vector>

#include "bwx_gl_shader.h"

namespace bwx_sdk {

class bwxGLTTF {
public:
    struct bwxGLTTFGlyph {
        glm::ivec2 size;
        glm::ivec2 bearing;
        GLuint advance;
        glm::vec2 uvTopLeft;
        glm::vec2 uvBottomRight;
    };

    bool LoadFromFile(const std::string& filepath, int pixelHeight = 48);

    GLuint GetTextureAtlas() const;

    bwxGLTTFGlyph& GetGlyph(wchar_t c);

    std::map<GLchar, bwxGLTTF::bwxGLTTFGlyph>& GetGlyphs();

    int GetGlyphHeight(wchar_t c) const;

    int GetGlyphWidth(wchar_t c) const;

    void SetCharset(const std::wstring& charset);

    void SetCharsetPL();

    void SetCharsetEN();

    void SetCharsetRU();

    void SetCharsetDE();

    void SetCharsetFR();

    void SetCharsetES();

    void SetCharsetIT();

private:
    std::wstring m_charset =
        L" !\"#$%&'()*+,-./0123456789:;<=>?@"
        L"ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~";

    GLuint m_textureAtlas;
    std::map<GLchar, bwxGLTTFGlyph> m_glyphs;
};

class bwxGLText {
public:
    bwxGLText(bwxGLTTF& font);

    ~bwxGLText();

    void SetFont(bwxGLTTF& font);

    int GetFontHeight();

    void Render(const std::wstring& text, const glm::mat4& orth, const glm::vec2& pos, GLfloat scale,
                const glm::vec4& color);

    // void SetEffectParams(const EffectParams& params);

    void SetShaderProgram(std::shared_ptr<bwxGLShaderProgram> shader);

    void SetDefaultShaderProgram();

private:
    bwxGLTTF& m_font;
    std::shared_ptr<bwxGLShaderProgram> m_shaderProgram;
    std::shared_ptr<bwxGLBuffer> m_dynamicBuffer;
}

}  // namespace bwx_sdk

#endif
