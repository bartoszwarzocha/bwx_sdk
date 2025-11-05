/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_resource_manager.h
// Purpose:     BWX_SDK Library; OpenGL Resource Manager Base Class 
//              (Dual Ownership Mode + Timed Expiry + Access Reset 
//              + Auto Reload + Hot Reload)
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Updated:     2025-03-23
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_RESOURCE_MANAGER_H_
#define _BWX_GL_RESOURCE_MANAGER_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <vector>
#include <unordered_map>
#include <memory>
#include <string>
#include <chrono>
#include <filesystem>

namespace bwx_sdk {

	/**
	 * @brief Base class for OpenGL resources.
	 */
    class bwxGLResource {
    public:
        virtual ~bwxGLResource() {}
        virtual void Unload() = 0;
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;
        virtual void Release() = 0;
        virtual void Delete() = 0;
        virtual bool LoadFromFile(const std::string& filename) { return false; } // optional override
    };

    template <typename bwxGLResourceType>
    class bwxGLResourceManager {
    public:
        using bwxGLResourcePtr = std::shared_ptr<bwxGLResourceType>;
        using Clock = std::chrono::steady_clock;
        using TimePoint = std::chrono::time_point<Clock>;

        bwxGLResourceManager(bool useSharedOwnership = true, int autoExpireSeconds = -1, bool resetOnAccess = false)
            : m_useSharedOwnership(useSharedOwnership), m_autoExpireSeconds(autoExpireSeconds), m_resetOnAccess(resetOnAccess), m_enableHotReload(false) {
        }

        virtual ~bwxGLResourceManager() {
            Clear();
        }

        void EnableHotReload(bool enabled) {
            m_enableHotReload = enabled;
        }

        void CheckHotReload() {
            if (!m_enableHotReload) return;

            for (const auto& [name, path] : m_filenames) {
                auto lastWriteTime = std::filesystem::last_write_time(path);
                if (m_fileWriteTimes[name] != lastWriteTime) {
                    m_fileWriteTimes[name] = lastWriteTime;
                    Reload(name);
                    wxLogMessage("[ResourceManager] Hot reloaded resource: %s", name);
                }
            }
        }

        bwxGLResourcePtr Load(const std::string& name, const std::string& filename) {
            CleanUp();

            if (auto existing = Get(name)) {
                return existing;
            }

            bwxGLResourcePtr resource = std::make_shared<bwxGLResourceType>();
            if (!resource->LoadFromFile(filename)) {
                wxLogError("[ResourceManager] Failed to load resource: %s", filename);
                return nullptr;
            }

            if (m_useSharedOwnership || m_autoExpireSeconds >= 0) {
                m_resources[name] = resource;
                m_timestamps[name] = Clock::now();
                m_filenames[name] = filename;
                m_fileWriteTimes[name] = std::filesystem::last_write_time(filename);
            }

            return resource;
        }

        bwxGLResourcePtr GetOrLoad(const std::string& name, const std::string& filename) {
            if (auto res = Get(name)) {
                return res;
            }
            return Load(name, filename);
        }

        bool Reload(const std::string& name) {
            auto it = m_filenames.find(name);
            if (it == m_filenames.end()) return false;

            bwxGLResourcePtr resource = std::make_shared<bwxGLResourceType>();
            if (!resource->LoadFromFile(it->second)) {
                wxLogError("[ResourceManager] Failed to reload resource: %s", name);
                return false;
            }

            m_resources[name] = resource;
            m_timestamps[name] = Clock::now();
            m_fileWriteTimes[name] = std::filesystem::last_write_time(it->second);
            return true;
        }

        void ReloadAll() {
            for (const auto& [name, filename] : m_filenames) {
                Reload(name);
            }
        }

        bwxGLResourcePtr Acquire(const std::string& name) {
            CleanUp();
            auto it = m_resources.find(name);
            if (it != m_resources.end()) {
                if (m_resetOnAccess) {
                    m_timestamps[name] = Clock::now();
                }
                return it->second;
            }
            return nullptr;
        }

        bwxGLResourcePtr Get(const std::string& name) {
            return Acquire(name);
        }

        bool Has(const std::string& name) const {
            return m_resources.find(name) != m_resources.end();
        }

        bool IsAlive(const std::string& name) const {
            return Has(name);
        }

        void KeepAlive(const std::string& name, bwxGLResourcePtr resource) {
            m_resources[name] = resource;
            m_timestamps[name] = Clock::now();
        }

        void Bind(const std::string& name) {
            if (auto resource = Get(name)) {
                resource->Bind();
            }
        }

        void Unbind() {
            for (auto& [_, resource] : m_resources) {
                if (resource) {
                    resource->Unbind();
                }
            }
        }

        void Release(const std::string& name) {
            if (auto resource = Get(name)) {
                resource->Release();
            }
        }

        void Delete(const std::string& name) {
            auto it = m_resources.find(name);
            if (it != m_resources.end()) {
                if (it->second) {
                    it->second->Unload();
                }
                m_resources.erase(it);
                m_timestamps.erase(name);
                m_filenames.erase(name);
                m_fileWriteTimes.erase(name);
            }
        }

        void Clear() {
            for (auto& [_, resource] : m_resources) {
                if (resource) {
                    resource->Unload();
                }
            }
            m_resources.clear();
            m_timestamps.clear();
            m_filenames.clear();
            m_fileWriteTimes.clear();
        }

        void Dump() const {
            wxLogMessage("== Resource Dump (%zu entries) ==", m_resources.size());
            for (const auto& [name, ptr] : m_resources) {
                wxLogMessage(" - %s : %s", name, ptr ? "active" : "nullptr");
            }
        }

        int GetCount() const {
            return static_cast<int>(m_resources.size());
        }

        void SetOwnershipMode(bool shared) {
            m_useSharedOwnership = shared;
        }

        bool IsSharedOwnership() const {
            return m_useSharedOwnership;
        }

        void SetAutoExpireSeconds(int seconds) {
            m_autoExpireSeconds = seconds;
        }

        int GetAutoExpireSeconds() const {
            return m_autoExpireSeconds;
        }

        void SetResetOnAccess(bool reset) {
            m_resetOnAccess = reset;
        }

        bool IsResetOnAccessEnabled() const {
            return m_resetOnAccess;
        }

    protected:
        void CleanUp() {
            auto now = Clock::now();

            for (auto it = m_resources.begin(); it != m_resources.end(); ) {
                const std::string& name = it->first;
                auto& res = it->second;

                bool remove = false;

                if (!m_useSharedOwnership && (!res || res.use_count() == 1)) {
                    remove = true;
                }

                if (m_autoExpireSeconds >= 0) {
                    auto tsIt = m_timestamps.find(name);
                    if (tsIt != m_timestamps.end()) {
                        auto age = std::chrono::duration_cast<std::chrono::seconds>(now - tsIt->second).count();
                        if (age >= m_autoExpireSeconds) {
                            remove = true;
                        }
                    }
                }

                if (remove) {
                    it = m_resources.erase(it);
                    m_timestamps.erase(name);
                    m_filenames.erase(name);
                    m_fileWriteTimes.erase(name);
                }
                else {
                    ++it;
                }
            }
        }

        std::unordered_map<std::string, bwxGLResourcePtr> m_resources;
        std::unordered_map<std::string, TimePoint> m_timestamps;
        std::unordered_map<std::string, std::string> m_filenames;
        std::unordered_map<std::string, std::filesystem::file_time_type> m_fileWriteTimes;
        bool m_useSharedOwnership;
        int m_autoExpireSeconds;
        bool m_resetOnAccess;
        bool m_enableHotReload;
    };

} // namespace bwx_sdk

#endif // _BWX_GL_RESOURCE_MANAGER_H_
