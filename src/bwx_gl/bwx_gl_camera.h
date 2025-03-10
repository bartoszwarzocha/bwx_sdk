/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_camera.h
// Purpose:     BWX_SDK Library; OpenGL Camera (dziedziczy po bwxGLNode)
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_CAMERA_H_
#define _BWX_GL_CAMERA_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "bwx_gl_node.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>

namespace bwx_sdk {

    enum class bwxGL_CAMERA_TYPE {
        CAMERA_TYPE_FPP,
        CAMERA_TYPE_SPECTATOR,
        CAMERA_TYPE_ORBIT,
        CAMERA_TYPE_FLIGHT,
        CAMERA_TYPE_TPP,
        CAMERA_TYPE_TPP_STRINGS
    };

    enum class bwxGL_CAMERA_LENS_TYPE {
        CAMERA_LENS_ORTHO,
        CAMERA_LENS_PERSP,
        CAMERA_LENS_UNKNOWN
    };

    enum class bwxGL_CAMERA_MOVEMENT {
        CAMERA_MOVEMENT_FORWARD,
        CAMERA_MOVEMENT_BACKWARD,
        CAMERA_MOVEMENT_LEFT,
        CAMERA_MOVEMENT_RIGHT
    };

    class bwxGLCamera : public bwxGLNode {
    public:
        bwxGLCamera(bwxGL_CAMERA_TYPE type);
        ~bwxGLCamera();

        // Konfiguracja kamery
        void SetProjectionPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);
        void SetProjectionOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);

        // Obs³uga macierzy
        void Update();
        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;

        // Sterowanie kamer¹
        void ProcessMovement(bwxGL_CAMERA_MOVEMENT direction, float deltaTime);
        void ProcessRotation(float pitch, float yaw);
        void ProcessZoom(float zoomAmount);
        void SetFocalLength(float focalLength);

        // Ustawienia specyficzne dla trybu
        void EnableMouseControl(bool enable);
        void EnableCollisionDetection(bool enable);
        void SetMovementSpeed(float speed);
        void SetSensitivity(float sensitivity);

        // Pobieranie danych kamery
        bwxGL_CAMERA_TYPE GetCameraType() const;
        float GetAspectRatio() const;
        float GetFOV() const;

    private:
        // Wewnêtrzne funkcje obliczeniowe
        void RecalculateViewMatrix();
        void RecalculateProjectionMatrix();

        // Parametry kamery
        bwxGL_CAMERA_TYPE m_cameraType;
        bwxGL_CAMERA_LENS_TYPE m_projectionType;
        float m_fov;
        float m_aspectRatio;
        float m_nearPlane;
        float m_farPlane;
        float m_movementSpeed;
        float m_sensitivity;
        bool m_mouseControlEnabled;
        bool m_collisionDetectionEnabled;

        // Macierze OpenGL
        glm::mat4 m_viewMatrix;
        glm::mat4 m_projectionMatrix;
    };

} // namespace bwx_sdk

#endif // _BWX_GL_CAMERA_H_
