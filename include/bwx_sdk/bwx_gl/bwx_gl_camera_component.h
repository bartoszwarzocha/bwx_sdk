/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_camera_component.h
// Purpose:     BWX_SDK Library; Camera component for ECS node
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.


#ifndef _BWX_GL_CAMERA_COMPONENT_H_
#define _BWX_GL_CAMERA_COMPONENT_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "bwx_gl_component.h"

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

    class bwxGLCameraComponent : public bwxGLComponent {
    public:
        bwxGLCameraComponent(bwxGL_CAMERA_TYPE type);

        void SetProjectionPerspective(float fov, float aspectRatio, float nearPlane, float farPlane);
        void SetProjectionOrthographic(float left, float right, float bottom, float top, float nearPlane, float farPlane);

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;

        void SetFocalLength(float focalLength);
        void EnableMouseControl(bool enable);
        void EnableCollisionDetection(bool enable);
        void SetSensitivity(float sensitivity);

        bwxGL_CAMERA_TYPE GetCameraType() const;
        float GetAspectRatio() const;
        float GetFOV() const;

        void Update(float deltaTime) override;

    private:
        void RecalculateViewMatrix();
        void RecalculateProjectionMatrix();

        bwxGL_CAMERA_TYPE m_cameraType;
        bwxGL_CAMERA_LENS_TYPE m_projectionType;

        float m_fov;
        float m_aspectRatio;
        float m_nearPlane;
        float m_farPlane;
        float m_sensitivity;

        bool m_mouseControlEnabled;
        bool m_collisionDetectionEnabled;

        glm::mat4 m_viewMatrix;
        glm::mat4 m_projectionMatrix;
    };

} // namespace bwx_sdk

#endif // _BWX_GL_CAMERA_COMPONENT_H_
