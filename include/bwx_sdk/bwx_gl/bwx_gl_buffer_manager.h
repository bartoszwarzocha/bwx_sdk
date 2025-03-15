/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_buffer_manager.h
// Purpose:     BWX_SDK Library; OpenGL Shared Buffer Manager
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

#ifndef _BWX_GL_BUFFER_MANAGER_H_
#define _BWX_GL_BUFFER_MANAGER_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <string>
#include <unordered_map>
#include <vector>

#include "bwx_gl_buffer.h"

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

}  // namespace bwx_sdk

#endif
