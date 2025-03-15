/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_node.cpp
// Purpose:     BWX_SDK Library; Base Node for Transformations
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include "bwx_gl_node.h"

namespace bwx_sdk {

    bwxGLNode::bwxGLNode()
        : m_position(0.0f), m_rotation(glm::quat()), m_scale(1.0f), m_localTransform(1.0f) {
    }

    void bwxGLNode::SetPosition(const glm::vec3& position) {
        m_position = position;
    }

    glm::vec3 bwxGLNode::GetPosition() const {
        return m_position;
    }

    void bwxGLNode::SetRotation(const glm::quat& rotation) {
        m_rotation = rotation;
    }

    void bwxGLNode::SetRotation(const glm::vec3& eulerAngles) {
        m_rotation = glm::quat(glm::radians(eulerAngles));
    }

    glm::quat bwxGLNode::GetRotation() const {
        return m_rotation;
    }

    glm::vec3 bwxGLNode::GetEulerAngles() const {
        return glm::degrees(glm::eulerAngles(m_rotation));
    }

    float bwxGLNode::GetYaw() const {
        return GetEulerAngles().y;
    }

    float bwxGLNode::GetPitch() const {
        return GetEulerAngles().x;
    }

    float bwxGLNode::GetRoll() const {
        return GetEulerAngles().z;
    }

    void bwxGLNode::SetScale(const glm::vec3& scale) {
        m_scale = scale;
    }

    glm::vec3 bwxGLNode::GetScale() const {
        return m_scale;
    }

    glm::mat4 bwxGLNode::GetTransformMatrix() const {
        glm::mat4 translation = glm::translate(glm::mat4(1.0f), m_position);
        glm::mat4 rotation = glm::mat4_cast(m_rotation);
        glm::mat4 scale = glm::scale(glm::mat4(1.0f), m_scale);
        return translation * rotation * scale;
    }

    void bwxGLNode::AddChild(std::shared_ptr<bwxGLNode> child) {
        if (child) {
            child->m_parent = shared_from_this();
            m_children.push_back(child);
        }
    }

    void bwxGLNode::RemoveChild(std::shared_ptr<bwxGLNode> child) {
        m_children.erase(std::remove(m_children.begin(), m_children.end(), child), m_children.end());
        if (child) {
            child->m_parent.reset();
        }
    }

    std::vector<std::shared_ptr<bwxGLNode>> bwxGLNode::GetChildren() const {
        return m_children;
    }

    void bwxGLNode::SetParent(std::shared_ptr<bwxGLNode> parent) {
        m_parent = parent;
    }

    std::shared_ptr<bwxGLNode> bwxGLNode::GetParent() const {
        return m_parent.lock();
    }

    void bwxGLNode::SetName(const std::string& name) {
        m_name = name;
    }

    std::string bwxGLNode::GetName() const {
        return m_name;
    }

} // namespace bwx_sdk
