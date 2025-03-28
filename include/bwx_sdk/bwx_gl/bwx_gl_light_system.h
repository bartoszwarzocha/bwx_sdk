/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_light_system.h
// Purpose:     BWX_SDK Library; Light system to manage ECS light components
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.


#ifndef _BWX_GL_LIGHT_SYSTEM_H_
#define _BWX_GL_LIGHT_SYSTEM_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <vector>
#include <memory>
#include <glm/glm.hpp>

#include "bwx_gl_light_component.h"
#include "bwx_gl_node.h"
#include "bwx_gl_buffer.h"

namespace bwx_sdk {

    struct bwxGLPackedLight {
        glm::vec4 position;     // xyz = position, w = type
        glm::vec4 direction;    // xyz = direction, w = innerCone
        glm::vec4 diffuse;      // xyz = color, w = power
        glm::vec4 ambient;      // xyz = ambient, w = range
        glm::vec4 specular;     // xyz = specular, w = outerCone
        glm::vec4 attenuation;  // x = constant, y = linear, z = quadratic, w = unused
    };

    class bwxGLLightSystem {
    public:
        static bwxGLLightSystem& GetInstance() {
            static bwxGLLightSystem instance;
            return instance;
        }

        void Register(std::shared_ptr<bwxGLNode> node);
        void Unregister(std::shared_ptr<bwxGLNode> node);
        void Clear();

        void Update(float deltaTime);

        const std::vector<bwxGLPackedLight>& GetPackedLights() const;

    private:
        bwxGLLightSystem() = default;
        ~bwxGLLightSystem() = default;

        std::vector<std::weak_ptr<bwxGLNode>> m_registeredNodes;
        std::vector<bwxGLPackedLight> m_lightData;
        bwxGLBuffer* m_lightUBO = nullptr;
    };

} // namespace bwx_sdk

#endif // _BWX_GL_LIGHT_SYSTEM_H_
