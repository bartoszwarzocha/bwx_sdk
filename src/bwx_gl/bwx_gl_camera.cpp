/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_camera.cpp
// Purpose:     BWX_SDK Library; OpenGL Camera Implementation
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include "bwx_gl_camera.h"

namespace bwx_sdk {

    bwxGLCamera::bwxGLCamera(bwxGL_CAMERA_TYPE type)
        : m_cameraType(type),
        m_projectionType(bwxGL_CAMERA_LENS_TYPE::CAMERA_LENS_UNKNOWN),
        m_fov(45.0f), m_aspectRatio(1.777f),
        m_nearPlane(0.1f), m_farPlane(100.0f),
        m_movementSpeed(2.5f), m_sensitivity(0.1f),
        m_mouseControlEnabled(true), m_collisionDetectionEnabled(false) 
    {
        SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
        SetRotation(glm::vec3(0.0f, 0.0f, -1.0f));
        RecalculateViewMatrix();
        RecalculateProjectionMatrix();
    }

    bwxGLCamera::~bwxGLCamera() {}

    void bwxGLCamera::SetProjectionPerspective(float fov, float aspectRatio, float nearPlane, float farPlane) {
        m_projectionType = bwxGL_CAMERA_LENS_TYPE::CAMERA_LENS_PERSP;
        m_fov = fov;
        m_aspectRatio = aspectRatio;
        m_nearPlane = nearPlane;
        m_farPlane = farPlane;
        RecalculateProjectionMatrix();
    }

    void bwxGLCamera::SetProjectionOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane) {
        m_projectionType = bwxGL_CAMERA_LENS_TYPE::CAMERA_LENS_ORTHO;
        m_projectionMatrix = glm::ortho(left, right, bottom, top, nearPlane, farPlane);
    }

    void bwxGLCamera::Update() {
        RecalculateViewMatrix();
    }

    glm::mat4 bwxGLCamera::GetViewMatrix() const {
        glm::vec3 position = GetPosition();
        glm::vec3 direction = GetEulerAngles();
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
        return glm::lookAt(position, position + direction, up);
    }

    glm::mat4 bwxGLCamera::GetProjectionMatrix() const {
        return m_projectionMatrix;
    }

    void bwxGLCamera::ProcessMovement(bwxGL_CAMERA_MOVEMENT direction, float deltaTime) {
        float velocity = m_movementSpeed * deltaTime;
        glm::vec3 position = GetPosition();
        if (direction == bwxGL_CAMERA_MOVEMENT::CAMERA_MOVEMENT_FORWARD)
            position += GetEulerAngles() * velocity;
        if (direction == bwxGL_CAMERA_MOVEMENT::CAMERA_MOVEMENT_BACKWARD)
            position -= GetEulerAngles() * velocity;
        if (direction == bwxGL_CAMERA_MOVEMENT::CAMERA_MOVEMENT_LEFT)
            position -= glm::normalize(glm::cross(GetEulerAngles(), glm::vec3(0.0f, 1.0f, 0.0f))) * velocity;
        if (direction == bwxGL_CAMERA_MOVEMENT::CAMERA_MOVEMENT_RIGHT)
            position += glm::normalize(glm::cross(GetEulerAngles(), glm::vec3(0.0f, 1.0f, 0.0f))) * velocity;
        SetPosition(position);
    }

    void bwxGLCamera::ProcessRotation(float pitch, float yaw) {
        glm::vec3 direction;
        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        SetRotation(glm::normalize(direction));
    }

    void bwxGLCamera::ProcessZoom(float zoomAmount) {
        if (m_fov >= 1.0f && m_fov <= 45.0f)
            m_fov -= zoomAmount;
        if (m_fov < 1.0f)
            m_fov = 1.0f;
        if (m_fov > 45.0f)
            m_fov = 45.0f;
        RecalculateProjectionMatrix();
    }

    void bwxGLCamera::SetFocalLength(float focalLength) {
        m_fov = focalLength;
        RecalculateProjectionMatrix();
    }

    void bwxGLCamera::EnableMouseControl(bool enable) {
        m_mouseControlEnabled = enable;
    }

    void bwxGLCamera::EnableCollisionDetection(bool enable) {
        m_collisionDetectionEnabled = enable;
    }

    void bwxGLCamera::SetMovementSpeed(float speed) {
        m_movementSpeed = speed;
    }

    void bwxGLCamera::SetSensitivity(float sensitivity) {
        m_sensitivity = sensitivity;
    }

    bwxGL_CAMERA_TYPE bwxGLCamera::GetCameraType() const {
        return m_cameraType;
    }

    float bwxGLCamera::GetAspectRatio() const {
        return m_aspectRatio;
    }

    float bwxGLCamera::GetFOV() const {
        return m_fov;
    }

    void bwxGLCamera::RecalculateViewMatrix() {
        m_viewMatrix = GetViewMatrix();
    }

    void bwxGLCamera::RecalculateProjectionMatrix() {
        if (m_projectionType == bwxGL_CAMERA_LENS_TYPE::CAMERA_LENS_PERSP) {
            m_projectionMatrix = glm::perspective(glm::radians(m_fov), m_aspectRatio, m_nearPlane, m_farPlane);
        }
    }

} // namespace bwx_sdk
