/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_material_manager.cpp
// Purpose:     BWX_SDK Libarary; OpenGL Material Manager
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <bwx_sdk/bwx_gl/bwx_gl_material_manager.h>

namespace bwx_sdk {

    // Singleton
    bwxGLMaterialManager& bwxGLMaterialManager::GetInstance() {
        static bwxGLMaterialManager instance;
        return instance;
    }

    std::shared_ptr<bwxGLMaterial> bwxGLMaterialManager::GetMaterial(const std::string& name) const {
        auto it = m_resources.find(name);
        if (it != m_resources.end()) {
			return it->second; // Return ptr to material if exists
        }
        return nullptr;
    }

	std::shared_ptr<bwxGLMaterial> bwxGLMaterialManager::GetMaterial(const unsigned int& id) const {
		for (auto it = m_resources.begin(); it != m_resources.end(); ++it) {
			if (it->second->GetID() == id) {
				return it->second;
			}
		}
		return nullptr;
	}

    std::shared_ptr<bwxGLMaterial> bwxGLMaterialManager::CreateMaterial(const std::string& name) {
        auto existingMaterial = GetMaterial(name);
        if (existingMaterial) {
			return existingMaterial; // If exists, return existing material
        }

        auto newMaterial = std::make_shared<bwxGLMaterial>(name);
        m_resources[name] = newMaterial;
        return newMaterial;
    }

    void bwxGLMaterialManager::ReleaseMaterial(const std::string& name) {
        auto it = m_resources.find(name);
        if (it != m_resources.end() && it->second) {
            m_resources.erase(it);
        }
    }

	void bwxGLMaterialManager::ReleaseMaterial(const int& id) {
		for (auto it = m_resources.begin(); it != m_resources.end(); ++it) {
			if (it->second->GetID() == id) {
				m_resources.erase(it);
				break;
			}
		}
	}

    void bwxGLMaterialManager::CleanupUnusedMaterials() {
        for (auto it = m_resources.begin(); it != m_resources.end();) {
            if (it->second) {
                it = m_resources.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    void bwxGLMaterialManager::ClearAllMaterials() {
        m_resources.clear();
    }

}