/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_texture_manager.h
// Purpose:     BWX_SDK Libarary; OpenGL Texture Manager
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef _BWX_GL_TEXTURE_MANAGER_H_
#define _BWX_GL_TEXTURE_MANAGER_H_

#include <iostream>
#include <memory>
#include <string>
#include <unordered_map>

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#else
#include <GL/glew.h>
#endif

#include "bwx_gl_image_loader.h"
#include "bwx_gl_texture.h"

namespace bwx_sdk {

class bwxGLTextureManager {
public:
    static bwxGLTextureManager& GetInstance();

    GLuint LoadTexture(const std::string& filePath, bool generateMipmaps = true);

    void BindTexture(const std::string& filePath, int textureUnit = 0);
    void UnbindTexture(int textureUnit = 0);

    void DeleteTexture(const std::string& filePath);

    std::shared_ptr<bwxGLTexture2D> GetTexturePtr(const std::string& filePath);
    GLuint GetTextureID(const std::string& filePath);

    void Clear();

private:
    bwxGLTextureManager() = default;
    ~bwxGLTextureManager();

    std::unordered_map<std::string, std::shared_ptr<bwxGLTexture2D>> m_textureMap;
};

}  // namespace bwx_sdk

#endif
