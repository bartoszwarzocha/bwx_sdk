/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_buffer_manager.h
// Purpose:     BWX_SDK Library; OpenGL Shared Buffer Manager
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_BUFFER_MANAGER_H_
#define _BWX_GL_BUFFER_MANAGER_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include "bwx_gl_buffer.h"

#include <unordered_map>
#include <string>
#include <vector>

namespace bwx_sdk {

    struct bwxGLBufferData {
        bwxGLBuffer* VBO;
        bwxGLBuffer* EBO;
        size_t refCount;
    };

    class bwxGLBufferManager {
    public:
        static bwxGLBufferManager& GetInstance();

        bwxGLBuffer* GetOrCreateVBO(const std::string& key, const std::vector<float>& vertices);
        bwxGLBuffer* GetOrCreateEBO(const std::string& key, const std::vector<unsigned int>& indices);
        void ReleaseBuffer(const std::string& key);
        void Clear();

    private:
        bwxGLBufferManager() = default;
        ~bwxGLBufferManager();

        std::unordered_map<std::string, bwxGLBufferData> m_buffers;
    };

} // namespace bwx_sdk

#endif
