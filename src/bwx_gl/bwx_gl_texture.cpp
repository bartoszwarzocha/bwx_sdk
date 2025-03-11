/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_texture.cpp
// Purpose:     BWX_SDK Libarary; OpenGL Texture
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
    #include <wx/wx.h>
#endif

#include "bwx_gl_texture.h"
#include <string>

#include "bwx_sdk/bwx_core/bwx_math.h"
#include "bwx_gl_image_loader.h"

namespace bwx_sdk {

    bwxGLTexture2D::bwxGLTexture2D(const wxString& file, GLint wrap, GLint filter, bool mipmaps, bool srgb)
    {
        Create(file, wrap, wrap, filter, filter, mipmaps, srgb);
    }

    bwxGLTexture2D::bwxGLTexture2D(const wxString& file, GLint wrapS, GLint wrapT, GLint filterMin, GLint filterMag, bool mipmaps, bool srgb)
    {
        Create(file, wrapS, wrapT, filterMin, filterMag, mipmaps, srgb);
    }

    bwxGLTexture2D::~bwxGLTexture2D()
    {
        Delete();
    }

    void bwxGLTexture2D::Create(const wxString& file, GLint wrapS, GLint wrapT, GLint filterMin, GLint filterMag, bool mipmaps, bool srgb)
    {
        Delete();  // Delete old texture (if exists)

        m_data.path = file;

        // Load image
        bwxGLImgLoader img;
        if (!img.Load(file)) {
            std::cerr << "Failed to load texture: " << file << std::endl;
            return;
        }

        glGenTextures(1, &m_data.textureID);
        glBindTexture(GL_TEXTURE_2D, m_data.textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);

        // Format
        GLenum format = img.HasAlpha() ? GL_RGBA : GL_RGB;
        GLenum internalFormat = srgb ? (img.HasAlpha() ? GL_SRGB8_ALPHA8 : GL_SRGB8) : format;

		// Texture data
        glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, img.Width(), img.Height(), 0, format, GL_UNSIGNED_BYTE, img.Data().data());

        if (mipmaps)
        {
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_BASE_LEVEL, 0);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAX_LEVEL, 4);
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        this->Unbind();
    }

    void bwxGLTexture2D::Bind(int index)
    {
        glActiveTexture(GL_TEXTURE0 + index);
        glBindTexture(GL_TEXTURE_2D, m_data.textureID);
    }

    void bwxGLTexture2D::Unbind()
    {
        glBindTexture(GL_TEXTURE_2D, 0);
    }

    void bwxGLTexture2D::Delete()
    {
        if (m_data.textureID)
        {
            glDeleteTextures(1, &m_data.textureID);
            m_data.textureID = 0;
        }
    }

} // namespace bwx_sdk
