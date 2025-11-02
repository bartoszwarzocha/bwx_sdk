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

#include <bwx_sdk/bwx_gl/bwx_gl_buffer_manager.h>

namespace bwx_sdk {

	void bwxGLBufferData::Bind() const {
		if (VBO) VBO->Bind();
		if (EBO) EBO->Bind();
		if (UBO) UBO->Bind();
		if (TBO) TBO->Bind();
		if (TFO) TFO->Bind();
	}

	void bwxGLBufferData::Unbind() const {
		if (VBO) VBO->Unbind();
		if (EBO) EBO->Unbind();
		if (UBO) UBO->Unbind();
		if (TBO) TBO->Unbind();
		if (TFO) TFO->Unbind();
	}

	void bwxGLBufferData::Release() {
		if (VBO) VBO->Release();
		if (EBO) EBO->Release();
		if (UBO) UBO->Release();
		if (TBO) TBO->Release();
		if (TFO) TFO->Release();
	}

	void bwxGLBufferData::Unload() {
		if (VBO) VBO->Unload();
		if (EBO) EBO->Unload();
		if (UBO) UBO->Unload();
		if (TBO) TBO->Unload();
		if (TFO) TFO->Unload();
	}

	void bwxGLBufferData::Delete() {
		if (VBO) VBO->Delete();
		if (EBO) EBO->Delete();
		if (UBO) UBO->Delete();
		if (TBO) TBO->Delete();
		if (TFO) TFO->Delete();
	}

    bwxGLBufferManager& bwxGLBufferManager::GetInstance() {
        static bwxGLBufferManager instance;
        return instance;
    }

    bwxGLBuffer* bwxGLBufferManager::GetOrCreateVBO(const std::string& key, const std::vector<float>& vertices) {
        auto it = m_resources.find(key);
        if (it != m_resources.end()) {
            it->second->refCount++;
            return it->second->VBO;
        }

        bwxGLBuffer* vbo = new bwxGLBuffer(GL_ARRAY_BUFFER);
        vbo->SetData(vertices.data(), vertices.size() * sizeof(float), GL_STATIC_DRAW);

		m_resources[key] = std::make_shared<bwxGLBufferData>(bwxGLBufferData { vbo, nullptr, nullptr, nullptr, nullptr });
        return vbo;
    }

    bwxGLBuffer* bwxGLBufferManager::GetOrCreateEBO(const std::string& key, const std::vector<unsigned int>& indices) {
        auto it = m_resources.find(key);
        if (it != m_resources.end()) {
            it->second->refCount++;
            return it->second->EBO;
        }

        bwxGLBuffer* ebo = new bwxGLBuffer(GL_ELEMENT_ARRAY_BUFFER);
        ebo->SetData(indices.data(), indices.size() * sizeof(unsigned int), GL_STATIC_DRAW);

		m_resources[key] = std::make_shared<bwxGLBufferData>(bwxGLBufferData { nullptr, ebo, nullptr, nullptr, nullptr });
        return ebo;
    }

	bwxGLBuffer* bwxGLBufferManager::GetOrCreateUBO(const std::string& key, const std::vector<float>& data) {
		auto it = m_resources.find(key);
		if (it != m_resources.end()) {
			it->second->refCount++;
			return it->second->UBO;
		}
		bwxGLBuffer* ubo = new bwxGLBuffer(GL_UNIFORM_BUFFER);
		ubo->SetData(data.data(), data.size() * sizeof(float), GL_STATIC_DRAW);

		m_resources[key] = std::make_shared<bwxGLBufferData>(bwxGLBufferData { nullptr, nullptr, ubo, nullptr, nullptr });
		return ubo;
	}

	bwxGLBuffer* bwxGLBufferManager::GetOrCreateTBO(const std::string& key, const std::vector<float>& data) {
		auto it = m_resources.find(key);
		if (it != m_resources.end()) {
			it->second->refCount++;
			return it->second->TBO;
		}
		bwxGLBuffer* tbo = new bwxGLBuffer(GL_TEXTURE_BUFFER);
		tbo->SetData(data.data(), data.size() * sizeof(float), GL_STATIC_DRAW);

		m_resources[key] = std::make_shared<bwxGLBufferData>(bwxGLBufferData { nullptr, nullptr, nullptr, tbo, nullptr });
		return tbo;
	}

	bwxGLBuffer* bwxGLBufferManager::GetOrCreateTFO(const std::string& key, const std::vector<float>& data) {
		auto it = m_resources.find(key);
		if (it != m_resources.end()) {
			it->second->refCount++;
			return it->second->TFO;
		}
		bwxGLBuffer* tfo = new bwxGLBuffer(GL_TRANSFORM_FEEDBACK_BUFFER);
		tfo->SetData(data.data(), data.size() * sizeof(float), GL_STATIC_DRAW);
		
		m_resources[key] = std::make_shared<bwxGLBufferData>(bwxGLBufferData { nullptr, nullptr, nullptr, nullptr, tfo });
		return tfo;
	}

    void bwxGLBufferManager::ReleaseBuffer(const std::string& key) {
        auto it = m_resources.find(key);
        if (it != m_resources.end()) {
            if (--it->second->refCount == 0) {
                if (it->second->VBO) {
                    it->second->VBO->Release();
                    delete it->second->VBO;
                }
                if (it->second->EBO) {
                    it->second->EBO->Release();
                    delete it->second->EBO;
                }
                if (it->second->UBO) {
					it->second->UBO->Release();
					delete it->second->UBO;
                }
				if (it->second->TBO) {
					it->second->TBO->Release();
					delete it->second->TBO;
				}
				if (it->second->TFO) {
					it->second->TFO->Release();
					delete it->second->TFO;
				}
                m_resources.erase(it);
            }
        }
    }

    void bwxGLBufferManager::Clear() {
        for (auto& [key, buffer] : m_resources) {
            if (buffer->VBO) {
                buffer->VBO->Release();
                delete buffer->VBO;
            }
            if (buffer->EBO) {
                buffer->EBO->Release();
                delete buffer->EBO;
            }
            if (buffer->UBO) {
                buffer->UBO->Release();
                delete buffer->UBO;
            }
            if (buffer->TBO) {
                buffer->TBO->Release();
                delete buffer->TBO;
            }
			if (buffer->TFO) {
				buffer->TFO->Release();
				delete buffer->TFO;
			}
        }
        m_resources.clear();
    }

    bwxGLBufferManager::~bwxGLBufferManager() {
        Clear();
    }

} // namespace bwx_sdk
