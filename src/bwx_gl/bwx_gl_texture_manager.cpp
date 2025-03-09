/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_texture_manager.cpp
// Purpose:     BWX_SDK Libarary; OpenGL Texture Manager
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

#include "bwx_gl_texture_manager.h"
#include <iostream>

namespace bwx_sdk {

    bwxGLTextureManager& bwxGLTextureManager::GetInstance() {
        static bwxGLTextureManager instance;
        return instance;
    }

    GLuint bwxGLTextureManager::LoadTexture(const std::string& filePath, bool generateMipmaps) {
        
        // Return ID if texture exists
        auto it = m_textureMap.find(filePath);
        if (it != m_textureMap.end()) {
            return it->second;
        }

        // If not then load texture with bwxGLImgLoader...
        bwxGLImgLoader image;
        if (!image.Load(filePath)) {
            std::cerr << "Error: Failed to load texture: " << filePath << std::endl;
            return 0;
        }

        GLuint textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        GLenum format = image.GetBytesPerPixel() == 4 ? GL_RGBA : GL_RGB;
        glTexImage2D(GL_TEXTURE_2D, 0, format, image.Width(), image.Height(), 0, format, GL_UNSIGNED_BYTE, image.Data().data());

        if (generateMipmaps) {
            glGenerateMipmap(GL_TEXTURE_2D);
        }

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, 0);

        // Add texture to map
        m_textureMap[filePath] = textureID;
        return textureID;
    }

    GLuint bwxGLTextureManager::GetTextureID(const std::string& filePath) const {
        auto it = m_textureMap.find(filePath);
        return (it != m_textureMap.end()) ? it->second : 0;
    }

    void bwxGLTextureManager::ReleaseTexture(const std::string& filePath) {
        auto it = m_textureMap.find(filePath);
        if (it != m_textureMap.end()) {
            glDeleteTextures(1, &it->second);
            m_textureMap.erase(it);
        }
    }

    void bwxGLTextureManager::Clear() {
        for (auto& pair : m_textureMap) {
            glDeleteTextures(1, &pair.second);
        }
        m_textureMap.clear();
    }

    bwxGLTextureManager::~bwxGLTextureManager() {
        Clear();
    }

} // namespace bwx_sdk
