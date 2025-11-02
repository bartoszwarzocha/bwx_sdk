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

#include <bwx_sdk/bwx_gl/bwx_gl_texture.h>
#include <bwx_sdk/bwx_gl/bwx_gl_texture_manager.h>
#include <iostream>

namespace bwx_sdk {

    bwxGLTextureManager& bwxGLTextureManager::GetInstance() {
        static bwxGLTextureManager instance;
        return instance;
    }

    GLuint bwxGLTextureManager::LoadTexture(const std::string& filePath, bool generateMipmaps) {
        
        // Return ID if texture exists
        auto it = m_resources.find(filePath);
        if (it != m_resources.end()) {
            return it->second->GetID();
        }

        // If not then load texture...
		std::shared_ptr<bwxGLTexture2D> texture = std::make_shared<bwxGLTexture2D>(filePath);
		if (!texture->GetID()) {
			wxLogError("Failed to load texture: %s", filePath);
			std::cerr << "Failed to load texture: " << filePath << std::endl;
			return 0;
		}

		// ...and store it
		m_resources[filePath] = texture;
        return texture->GetID();
    }

	void bwxGLTextureManager::BindTexture(const std::string& filePath, int textureUnit) {
		auto it = m_resources.find(filePath);
		if (it != m_resources.end()) {
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, it->second->GetID());
		}
	}

	void bwxGLTextureManager::UnbindTexture(int textureUnit) {
        glActiveTexture(GL_TEXTURE0 + textureUnit);
        glBindTexture(GL_TEXTURE_2D, 0);
	}

	void bwxGLTextureManager::DeleteTexture(const std::string& filePath) {
		auto it = m_resources.find(filePath);
		if (it != m_resources.end()) {
			it->second->Delete();
			m_resources.erase(it);
		}
	}

	std::shared_ptr<bwxGLTexture2D> bwxGLTextureManager::GetTexturePtr(const std::string& filePath) {
		auto it = m_resources.find(filePath);
		if (it != m_resources.end()) {
			return it->second;
		}
		return nullptr;
	}

	GLuint bwxGLTextureManager::GetTextureID(const std::string& filePath) {
		auto it = m_resources.find(filePath);
		if (it != m_resources.end()) {
			return it->second->GetID();
		}
		return 0;
	}

	void bwxGLTextureManager::Clear() {
		for (auto& texture : m_resources) {
			texture.second->Delete();
		}

		m_resources.clear();
	}

    bwxGLTextureManager::~bwxGLTextureManager() {
        Clear();
    }

} // namespace bwx_sdk
