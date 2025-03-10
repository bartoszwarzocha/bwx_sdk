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

#include "bwx_gl_texture.h"
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

        // If not then load texture...
		bwxGLTexture2D texture(filePath);
		if (!texture.GetID()) {
			wxLogError("Failed to load texture: %s", filePath);
			std::cerr << "Failed to load texture: " << filePath << std::endl;
			return 0;
		}

		// ...and store it
		m_textureMap[filePath] = texture.GetID();
        return texture.GetID();
    }

	void bwxGLTextureManager::BindTexture(const std::string& filePath, int textureUnit) {
		auto it = m_textureMap.find(filePath);
		if (it != m_textureMap.end()) {
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, it->second);
		}
	}

	void bwxGLTextureManager::BindTexture(GLuint textureId, int textureUnit) {
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, textureId);
	}

	void bwxGLTextureManager::UnbindTexture(const std::string& filePath) {
		auto it = m_textureMap.find(filePath);
		if (it != m_textureMap.end()) {
			glDeleteTextures(1, &it->second);
			m_textureMap.erase(it);
		}
	}

	void bwxGLTextureManager::UnbindTexture(int textureUnit) {
		glActiveTexture(GL_TEXTURE0 + textureUnit);
		glBindTexture(GL_TEXTURE_2D, 0);
	}

    GLuint bwxGLTextureManager::GetTextureID(const std::string& filePath) const {
        auto it = m_textureMap.find(filePath);
        return (it != m_textureMap.end()) ? it->second : 0;
    }

    bwxGLTexture2DData bwxGLTextureManager::GetTexture(const std::string& filePath) const {
		auto it = m_textureMap.find(filePath);
		return bwxGLTexture2DData{ it->second, filePath };
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
