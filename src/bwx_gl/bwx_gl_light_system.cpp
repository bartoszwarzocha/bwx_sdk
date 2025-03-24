/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_light_system.cpp
// Purpose:     BWX_SDK Library; Light system to manage ECS light components
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include "bwx_gl_light_system.h"
#include "bwx_gl_transform_component.h"
#include "bwx_gl_light_component.h"
#include "bwx_gl_node.h"

namespace bwx_sdk {

    void bwxGLLightSystem::Register(std::shared_ptr<bwxGLNode> node) {
        if (node && node->HasComponent<bwxGLLightComponent>()) {
            m_registeredNodes.push_back(node);
        }
    }

    void bwxGLLightSystem::Unregister(std::shared_ptr<bwxGLNode> node) {
        m_registeredNodes.erase(std::remove_if(
            m_registeredNodes.begin(), m_registeredNodes.end(),
            [&](const std::weak_ptr<bwxGLNode>& weak)
            {
                auto shared = weak.lock();
                return !shared || shared == node;
            }),
            m_registeredNodes.end());
    }

    void bwxGLLightSystem::Clear() {
        m_registeredNodes.clear();
        m_lightData.clear();
    }

    void bwxGLLightSystem::Update(float /*deltaTime*/) {
        m_lightData.clear();

        for (auto it = m_registeredNodes.begin(); it != m_registeredNodes.end();) {
            auto node = it->lock();
            if (!node) {
                it = m_registeredNodes.erase(it);
                continue;
            }

            auto light = node->GetComponent<bwxGLLightComponent>();
            auto transform = node->GetComponent<bwxGLTransformComponent>();
            if (!light || !transform) {
                ++it;
                continue;
            }

            bwxGLPackedLight packed;

            packed.position = glm::vec4(transform->GetPosition(), static_cast<float>(light->GetType()));
            packed.direction = glm::vec4(transform->GetRotation() * glm::vec3(0, 0, -1), light->GetInnerCone());
            packed.diffuse = glm::vec4(light->GetDiffuse(), light->GetPower());
            packed.ambient = glm::vec4(light->GetAmbient(), light->GetRange());
            packed.specular = glm::vec4(light->GetSpecular(), light->GetOuterCone());
            packed.attenuation = glm::vec4(
                light->GetAttenuationConstant(),
                light->GetAttenuationLinear(),
                light->GetAttenuationQuadratic(),
                0.0f
            );

            m_lightData.push_back(packed);
            ++it;
        }
    }

    const std::vector<bwxGLPackedLight>& bwxGLLightSystem::GetPackedLights() const {
        return m_lightData;
    }

} // namespace bwx_sdk
