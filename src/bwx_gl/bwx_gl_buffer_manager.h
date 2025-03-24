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

    class bwxGLBufferData : public bwxGLResource {

    public:
		bwxGLBufferData() : VBO(nullptr), EBO(nullptr), UBO(nullptr), TBO(nullptr), TFO(nullptr), refCount(0) {}

		bwxGLBufferData(bwxGLBuffer* vbo, bwxGLBuffer* ebo, bwxGLBuffer* ubo, bwxGLBuffer* tbo, bwxGLBuffer* tfo)
			: VBO(vbo), EBO(ebo), UBO(ubo), TBO(tbo), TFO(tfo), refCount(1) {}

		bwxGLBuffer* VBO = nullptr; ///< Vertex Buffer Object
		bwxGLBuffer* EBO = nullptr; ///< Element Buffer Object
		bwxGLBuffer* UBO = nullptr; ///< Uniform Buffer Object
		bwxGLBuffer* TBO = nullptr; ///< Texture Buffer Object
		bwxGLBuffer* TFO = nullptr; ///< Transform Feedback Object
        size_t refCount;

        void Bind() const override;
		void Unbind() const override;
		void Release() override;
		void Unload() override;
		void Delete() override;
    };

    class bwxGLBufferManager : public bwxGLResourceManager<bwxGLBufferData> 
    {
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

} // namespace bwx_sdk

#endif
