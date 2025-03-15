/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_buffer_manager.cpp
// Purpose:     BWX_SDK Library; OpenGL Shared Buffer Manager Implementation
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include "bwx_gl_buffer_manager.h"

namespace bwx_sdk {

    bwxGLBufferManager& bwxGLBufferManager::GetInstance() {
        static bwxGLBufferManager instance;
        return instance;
    }

    bwxGLBuffer* bwxGLBufferManager::GetOrCreateVBO(const std::string& key, const std::vector<float>& vertices) {
        auto it = m_buffers.find(key);
        if (it != m_buffers.end()) {
            it->second.refCount++;
            return it->second.VBO;
        }

        bwxGLBuffer* vbo = new bwxGLBuffer(GL_ARRAY_BUFFER);
        vbo->SetData(vertices.data(), vertices.size() * sizeof(float), GL_STATIC_DRAW);

        bwxGLBufferData bufferData{ vbo, nullptr, 1 };
        m_buffers[key] = bufferData;
        return vbo;
    }

    bwxGLBuffer* bwxGLBufferManager::GetOrCreateEBO(const std::string& key, const std::vector<unsigned int>& indices) {
        auto it = m_buffers.find(key);
        if (it != m_buffers.end()) {
            it->second.refCount++;
            return it->second.EBO;
        }

        bwxGLBuffer* ebo = new bwxGLBuffer(GL_ELEMENT_ARRAY_BUFFER);
        ebo->SetData(indices.data(), indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);

        bwxGLBufferData bufferData{ nullptr, ebo, 1 };
        m_buffers[key] = bufferData;
        return ebo;
    }

    void bwxGLBufferManager::ReleaseBuffer(const std::string& key) {
        auto it = m_buffers.find(key);
        if (it != m_buffers.end()) {
            if (--it->second.refCount == 0) {
                if (it->second.VBO) {
                    it->second.VBO->Release();
                    delete it->second.VBO;
                }
                if (it->second.EBO) {
                    it->second.EBO->Release();
                    delete it->second.EBO;
                }
                m_buffers.erase(it);
            }
        }
    }

    void bwxGLBufferManager::Clear() {
        for (auto& [key, buffer] : m_buffers) {
            if (buffer.VBO) {
                buffer.VBO->Release();
                delete buffer.VBO;
            }
            if (buffer.EBO) {
                buffer.EBO->Release();
                delete buffer.EBO;
            }
        }
        m_buffers.clear();
    }

    bwxGLBufferManager::~bwxGLBufferManager() {
        Clear();
    }

} // namespace bwx_sdk
