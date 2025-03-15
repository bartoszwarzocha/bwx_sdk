/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_texture.h
// Purpose:     BWX_SDK Libarary; OpenGL Texture
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_TEXTURE_H_
#define _BWX_GL_TEXTURE_H_

#include <wx/string.h>

#if defined(__APPLE__)
    #error OpenGL functionality is not available for macOS.
#else
    #include <GL/glew.h>
#endif

namespace bwx_sdk {

    enum bwxGL_TEXTURE_TYPE {
        TEXTURE_DIFFUSE,
        TEXTURE_SPECULAR,
        TEXTURE_NORMAL,
        TEXTURE_HEIGHT,
        TEXTURE_EMISSIVE,
        TEXTURE_OPACITY,
        TEXTURE_UNKNOWN
    };

    struct bwxGLTexture2DData {
        GLuint textureID = 0;
        wxString name = wxEmptyString;
        wxString path;
    };

    class bwxGLTexture2D {
    public:
        bwxGLTexture2D() = default;
        bwxGLTexture2D(const wxString& file, GLint wrap = GL_REPEAT, GLint filter = GL_LINEAR, bool mipmaps = true, bool srgb = false);
        bwxGLTexture2D(const wxString& file, GLint wrapS, GLint wrapT, GLint filterMin, GLint filterMag, bool mipmaps = true, bool srgb = false);
        ~bwxGLTexture2D();

        void Create(const wxString& file, GLint wrapS, GLint wrapT, GLint filterMin, GLint filterMag, bool mipmaps = true, bool srgb = false);

        void Bind(int index = 0);
        void Unbind();
        void Delete();

		inline void SetName(const wxString& name) { m_data.name = name; }

        inline GLuint GetID() const { return m_data.textureID; }
        inline const wxString& GetPath() const { return m_data.path; }
		inline const wxString& GetName() const { return m_data.name; }
		inline bwxGLTexture2DData GetData() const { return m_data; }

    private:
        bwxGLTexture2DData m_data;
    };

} // namespace bwx_sdk

#endif
