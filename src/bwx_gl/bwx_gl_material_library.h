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

#include "bwx_gl_material.h"

namespace bwx_sdk {

    class bwxGLMaterialLibrary {
    public:
        static bwxGLMaterialLibrary& GetInstance();
        std::shared_ptr<bwxGLMaterial> GetMaterial(const std::string& name) const;
		std::shared_ptr<bwxGLMaterial> GetMaterial(const int& id) const;
        std::shared_ptr<bwxGLMaterial> CreateMaterial(const std::string& name);
        void ReleaseMaterial(const std::string& name);
		void ReleaseMaterial(const int& id);
        void CleanupUnusedMaterials();
        void ClearAllMaterials();

    private:
        bwxGLMaterialLibrary() = default;
        ~bwxGLMaterialLibrary() = default;

        bwxGLMaterialLibrary(const bwxGLMaterialLibrary&) = delete;
        bwxGLMaterialLibrary& operator=(const bwxGLMaterialLibrary&) = delete;

        std::unordered_map<std::string, std::weak_ptr<bwxGLMaterial>> m_materials;
    };

} // namespace bwx_sdk

#endif // BWX_GL_MATERIAL_MANAGER_H
