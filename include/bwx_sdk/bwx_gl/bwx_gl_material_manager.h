/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_material_manager.h
// Purpose:     BWX_SDK Libarary; OpenGL Material Manager
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_MATERIAL_MANAGER_H_
#define _BWX_GL_MATERIAL_MANAGER_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <unordered_map>
#include <memory>
#include <string>
#include <iostream>

#include "bwx_gl_material.h"
#include "bwx_gl_resource_manager.h"

namespace bwx_sdk {

	class bwxGLMaterialManager : public bwxGLResourceManager<bwxGLMaterial>
    {
    public:
        static bwxGLMaterialManager& GetInstance();
        std::shared_ptr<bwxGLMaterial> GetMaterial(const std::string& name) const;
		std::shared_ptr<bwxGLMaterial> GetMaterial(const unsigned int& id) const;
        std::shared_ptr<bwxGLMaterial> CreateMaterial(const std::string& name);
        void ReleaseMaterial(const std::string& name);
		void ReleaseMaterial(const int& id);
        void CleanupUnusedMaterials();
        void ClearAllMaterials();

    private:
        bwxGLMaterialManager() = default;
        ~bwxGLMaterialManager() = default;

        bwxGLMaterialManager(const bwxGLMaterialManager&) = delete;
        bwxGLMaterialManager& operator=(const bwxGLMaterialManager&) = delete;

        //std::unordered_map<std::string, std::weak_ptr<bwxGLMaterial>> m_materials;
    };

} // namespace bwx_sdk

#endif // BWX_GL_MATERIAL_MANAGER_H
