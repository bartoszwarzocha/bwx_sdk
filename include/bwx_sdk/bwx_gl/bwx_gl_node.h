/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_node.h
// Purpose:     BWX_SDK Library; Base Node for Transformations
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


#ifndef _BWX_GL_NODE_H_
#define _BWX_GL_NODE_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#define GLM_ENABLE_EXPERIMENTAL

#include <memory>
#include <vector>
#include <string>
#include <iostream>
#include <algorithm>
#include <functional>
#include <unordered_map>    
#include <typeindex>

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

#include "bwx_sdk/bwx_gl/bwx_gl_component.h"

namespace bwx_sdk {

    class bwxGLNode : public std::enable_shared_from_this<bwxGLNode> {
    public:
        bwxGLNode() = default;
        virtual ~bwxGLNode() = default;

        template <typename T, typename... Args>
        std::shared_ptr<T> AddComponent(Args&&... args) {
            static_assert(std::is_base_of<bwxGLComponent, T>::value, "T must derive from bwxGLComponent");
            auto component = std::make_shared<T>(std::forward<Args>(args)...);
            m_components[typeid(T)] = component;
            return component;
        }

        template <typename T>
        std::shared_ptr<T> GetComponent() {
            auto it = m_components.find(typeid(T));
            return (it != m_components.end()) ? std::static_pointer_cast<T>(it->second) : nullptr;
        }

        template <typename T>
		bool HasComponent() {
			return m_components.find(typeid(T)) != m_components.end();
		}

        template <typename T>
        void RemoveComponent() {
            m_components.erase(typeid(T));
        }

        void Update(float deltaTime) {
            for (auto& [type, component] : m_components) {
                component->Update(deltaTime);
            }
        }

        void Render() {
            for (auto& [type, component] : m_components) {
                component->Render();
            }
        }

    private:
        std::unordered_map<std::type_index, std::shared_ptr<bwxGLComponent>> m_components;
    };

} // namespace bwx_sdk

#endif // _BWX_GL_NODE_H_