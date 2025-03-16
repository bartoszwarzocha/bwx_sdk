/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_material_library.cpp
// Purpose:     BWX_SDK Libarary; OpenGL Material Manager
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include "bwx_gl_material_library.h"

namespace bwx_sdk {

    // Singleton
    bwxGLMaterialLibrary& bwxGLMaterialLibrary::GetInstance() {
        static bwxGLMaterialLibrary instance;
        return instance;
    }

    std::shared_ptr<bwxGLMaterial> bwxGLMaterialLibrary::GetMaterial(const std::string& name) const {
        auto it = m_materials.find(name);
        if (it != m_materials.end()) {
			return it->second.lock(); // Return ptr to material if exists
        }
        return nullptr;
    }

	std::shared_ptr<bwxGLMaterial> bwxGLMaterialLibrary::GetMaterial(const unsigned int& id) const {
		for (auto it = m_materials.begin(); it != m_materials.end(); ++it) {
			if (it->second.lock()->GetID() == id) {
				return it->second.lock();
			}
		}
		return nullptr;
	}

    std::shared_ptr<bwxGLMaterial> bwxGLMaterialLibrary::CreateMaterial(const std::string& name) {
        auto existingMaterial = GetMaterial(name);
        if (existingMaterial) {
			return existingMaterial; // If exists, return existing material
        }

        auto newMaterial = std::make_shared<bwxGLMaterial>(name);
        m_materials[name] = newMaterial;
        return newMaterial;
    }

    void bwxGLMaterialLibrary::ReleaseMaterial(const std::string& name) {
        auto it = m_materials.find(name);
        if (it != m_materials.end() && it->second.expired()) {
            m_materials.erase(it);
        }
    }

	void bwxGLMaterialLibrary::ReleaseMaterial(const int& id) {
		for (auto it = m_materials.begin(); it != m_materials.end(); ++it) {
			if (it->second.lock()->GetID() == id) {
				m_materials.erase(it);
				break;
			}
		}
	}

    void bwxGLMaterialLibrary::CleanupUnusedMaterials() {
        for (auto it = m_materials.begin(); it != m_materials.end();) {
            if (it->second.expired()) {
                it = m_materials.erase(it);
            }
            else {
                ++it;
            }
        }
    }

    void bwxGLMaterialLibrary::ClearAllMaterials() {
        m_materials.clear();
    }

}