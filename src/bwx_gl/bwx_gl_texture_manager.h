/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_texture_manager.h
// Purpose:     BWX_SDK Libarary; OpenGL Texture Manager
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_TEXTURE_MANAGER_H_
#define _BWX_GL_TEXTURE_MANAGER_H_

#include <unordered_map>
#include <string>

#if defined(__APPLE__)
    #error OpenGL functionality is not available for macOS.
#else
    #include <GL/glew.h>
#endif

#include "bwx_gl_image_loader.h"

namespace bwx_sdk {

    class bwxGLTextureManager {
    public:
        static bwxGLTextureManager& GetInstance();

        GLuint LoadTexture(const std::string& filePath, bool generateMipmaps = true);
        GLuint GetTextureID(const std::string& filePath) const;
        void ReleaseTexture(const std::string& filePath);

        void Clear();

    private:
        bwxGLTextureManager() = default;
        ~bwxGLTextureManager();

        std::unordered_map<std::string, GLuint> m_textureMap;
    };

} // namespace bwx_sdk

#endif
