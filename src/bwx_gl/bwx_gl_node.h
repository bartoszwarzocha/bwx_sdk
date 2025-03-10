/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_node.h
// Purpose:     BWX_SDK Library; Base Node for Transformations
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

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

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtx/transform.hpp>

namespace bwx_sdk {

    class bwxGLNode : public std::enable_shared_from_this<bwxGLNode> {
    public:
        bwxGLNode();
        virtual ~bwxGLNode() = default;

        void SetPosition(const glm::vec3& position);
        void SetPosition(float x, float y, float z) { SetPosition(glm::vec3(x, y, z)); }
        glm::vec3 GetPosition() const;

        void SetRotation(const glm::quat& rotation);
        void SetRotation(const glm::vec3& eulerAngles);
        void SetRotation(float pitch, float yaw, float roll) { SetRotation(glm::vec3(pitch, yaw, roll)); }
        glm::quat GetRotation() const;
        glm::vec3 GetEulerAngles() const;
        float GetYaw() const;
        float GetPitch() const;
        float GetRoll() const;

        void SetScale(const glm::vec3& scale);
        glm::vec3 GetScale() const;

        glm::mat4 GetTransformMatrix() const;

        void AddChild(std::shared_ptr<bwxGLNode> child);
        void RemoveChild(std::shared_ptr<bwxGLNode> child);
        std::vector<std::shared_ptr<bwxGLNode>> GetChildren() const;

        void SetParent(std::shared_ptr<bwxGLNode> parent);
        std::shared_ptr<bwxGLNode> GetParent() const;

        void SetName(const std::string& name);
        std::string GetName() const;

    protected:
        glm::vec3 m_position;
        glm::quat m_rotation;
        glm::vec3 m_scale;
        glm::mat4 m_localTransform;

        std::vector<std::shared_ptr<bwxGLNode>> m_children;
        std::weak_ptr<bwxGLNode> m_parent;
        std::string m_name;
    };

} // namespace bwx_sdk

#endif // _BWX_GL_NODE_H_