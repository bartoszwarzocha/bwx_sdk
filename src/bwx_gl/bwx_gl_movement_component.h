/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_movement_component.h
// Purpose:     BWX_SDK Library; Movement component (ECS)
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_MOVEMENT_COMPONENT_H_
#define _BWX_GL_MOVEMENT_COMPONENT_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include <memory>

#include "bwx_gl_node.h"
#include "bwx_gl_component.h"

namespace bwx_sdk {

    /**
    * @brief Movement modes
    */
    enum class bwxGL_MOVEMENT_MODE {
        MODE_FREE,
        MODE_LOCKED
    };

    /**
    * @brief Movement types
    */
    enum class bwxGL_MOVEMENT_TYPE {
        FORWARD,
        BACKWARD,
        LEFT,
        RIGHT,
        UP,
        DOWN,
        ROTATE_LEFT,
        ROTATE_RIGHT,
        ZOOM_IN,
        ZOOM_OUT,
        JUMP
    };

    /**
    * @brief Interface for specific movement strategy
    */
    class bwxGLMovementStrategy {
    public:
        virtual ~bwxGLMovementStrategy() = default;
        virtual void ProcessMovement(bwxGLNode& node, const bwxGL_MOVEMENT_TYPE& type, float delta) = 0;
    };

    class bwxGLMovementComponent : public bwxGLComponent {
    public:
        using MovementCallback = std::function<void(bwxGLNode&, float)>;

        bwxGLMovementComponent();

        void ProcessMovement(bwxGL_MOVEMENT_TYPE type, float delta);

        void SetVelocity(const glm::vec3& velocity);
        glm::vec3 GetVelocity() const;

        void SetRotationSpeed(float speed);
        float GetRotationSpeed() const;

        void Translate(const glm::vec3& offset);
        void RotateEuler(const glm::vec3& eulerOffset);
        void RotateAroundAxis(const glm::vec3& axis, float angleDegrees);
        void RotateQuaternion(const glm::quat& rotation);
        void LookAt(const glm::vec3& target);
        void Zoom(float factor);

        void SetMovementCallback(bwxGL_MOVEMENT_TYPE type, MovementCallback callback);

        void SetMovementStrategy(std::unique_ptr<bwxGLMovementStrategy> strategy);
        bool HasMovementStrategy() const;

        void Update(float deltaTime) override;

    private:
        std::weak_ptr<bwxGLNode> m_node;
        glm::vec3 m_velocity;
        float m_rotationSpeed;

        std::unordered_map<bwxGL_MOVEMENT_TYPE, MovementCallback> m_movementCallbacks; ///< Callbacks for movement
        std::unique_ptr<bwxGLMovementStrategy> m_movementStrategy; ///< Movement strategy
    };

} // namespace bwx_sdk

#endif // _BWX_GL_MOVEMENT_COMPONENT_H_