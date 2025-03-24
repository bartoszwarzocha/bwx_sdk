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

class bwxGLBufferData : public bwxGLResource {
public:
    bwxGLBufferData() : VBO(nullptr), EBO(nullptr), UBO(nullptr), TBO(nullptr), TFO(nullptr), refCount(0) {}

    bwxGLBufferData(bwxGLBuffer* vbo, bwxGLBuffer* ebo, bwxGLBuffer* ubo, bwxGLBuffer* tbo, bwxGLBuffer* tfo)
        : VBO(vbo), EBO(ebo), UBO(ubo), TBO(tbo), TFO(tfo), refCount(1) {}

    bwxGLBuffer* VBO = nullptr;
    bwxGLBuffer* EBO = nullptr;
    bwxGLBuffer* UBO = nullptr;
    bwxGLBuffer* TBO = nullptr;
    bwxGLBuffer* TFO = nullptr;
    size_t refCount;

    void Bind() const override;
    void Unbind() const override;
    void Release() override;
    void Unload() override;
    void Delete() override;
};

class bwxGLBufferManager : public bwxGLResourceManager<bwxGLBufferData> {
public:
    static bwxGLBufferManager& GetInstance();

    bwxGLBuffer* GetOrCreateVBO(const std::string& key, const std::vector<float>& vertices);
    bwxGLBuffer* GetOrCreateEBO(const std::string& key, const std::vector<unsigned int>& indices);
    bwxGLBuffer* GetOrCreateUBO(const std::string& key, const std::vector<float>& data);
    bwxGLBuffer* GetOrCreateTBO(const std::string& key, const std::vector<float>& data);
    bwxGLBuffer* GetOrCreateTFO(const std::string& key, const std::vector<float>& data);

    void ReleaseBuffer(const std::string& key);

    void Clear();

private:
    bwxGLBufferManager() = default;
    ~bwxGLBufferManager();
};

}  // namespace bwx_sdk

#endif
