/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_camera_component.cpp
// Purpose:     BWX_SDK Library; Camera component for ECS node
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <bwx_sdk/bwx_gl/bwx_gl_camera_component.h>
#include <bwx_sdk/bwx_gl/bwx_gl_transform_component.h>
#include <bwx_sdk/bwx_gl/bwx_gl_node.h>

namespace bwx_sdk {

    bwxGLCameraComponent::bwxGLCameraComponent(bwxGL_CAMERA_TYPE type)
        : m_cameraType(type), m_projectionType(bwxGL_CAMERA_LENS_TYPE::CAMERA_LENS_UNKNOWN),
        m_fov(45.0f), m_aspectRatio(1.0f), m_nearPlane(0.1f), m_farPlane(100.0f),
        m_sensitivity(0.1f), m_mouseControlEnabled(false), m_collisionDetectionEnabled(false),
        m_viewMatrix(1.0f), m_projectionMatrix(1.0f) {
    }

    void bwxGLCameraComponent::SetProjectionPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
        m_projectionType = bwxGL_CAMERA_LENS_TYPE::CAMERA_LENS_PERSP;
        m_fov = fov;
        m_aspectRatio = aspectRatio;
        m_nearPlane = nearPlane;
        m_farPlane = farPlane;
        RecalculateProjectionMatrix();
    }

    void bwxGLCameraComponent::SetProjectionOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
        m_projectionType = bwxGL_CAMERA_LENS_TYPE::CAMERA_LENS_ORTHO;
        m_projectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
    }

    glm::mat4 bwxGLCameraComponent::GetViewMatrix() const {
        return m_viewMatrix;
    }

    glm::mat4 bwxGLCameraComponent::GetProjectionMatrix() const {
        return m_projectionMatrix;
    }

    void bwxGLCameraComponent::SetFocalLength(float focalLength) {
        m_fov = focalLength;
        RecalculateProjectionMatrix();
    }

    void bwxGLCameraComponent::EnableMouseControl(bool enable) {
        m_mouseControlEnabled = enable;
    }

    void bwxGLCameraComponent::EnableCollisionDetection(bool enable) {
        m_collisionDetectionEnabled = enable;
    }

    void bwxGLCameraComponent::SetSensitivity(float sensitivity) {
        m_sensitivity = sensitivity;
    }

    bwxGL_CAMERA_TYPE bwxGLCameraComponent::GetCameraType() const {
        return m_cameraType;
    }

    float bwxGLCameraComponent::GetAspectRatio() const {
        return m_aspectRatio;
    }

    float bwxGLCameraComponent::GetFOV() const {
        return m_fov;
    }

    void bwxGLCameraComponent::Update(float /*deltaTime*/) {
        RecalculateViewMatrix();
    }

    void bwxGLCameraComponent::RecalculateViewMatrix() {
        auto node = GetNode();
        if (!node) return;
        auto transform = node->GetComponent<bwxGLTransformComponent>();
        if (!transform) return;

        glm::vec3 position = transform->GetPosition();
        glm::vec3 forward = transform->GetRotation() * glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 up = transform->GetRotation() * glm::vec3(0.0f, 1.0f, 0.0f);

        m_viewMatrix = glm::lookAt(position, position + forward, up);
    }

    void bwxGLCameraComponent::RecalculateProjectionMatrix() {
        if (m_projectionType == bwxGL_CAMERA_LENS_TYPE::CAMERA_LENS_PERSP) {
            m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
        }
    }

} // namespace bwx_sdk
