/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_movement_component.cpp
// Purpose:     BWX_SDK Library; Movement component (ECS)
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <bwx_sdk/bwx_gl/bwx_gl_movement_component.h>
#include <bwx_sdk/bwx_gl/bwx_gl_transform_component.h>

namespace bwx_sdk {

	bwxGLMovementComponent::bwxGLMovementComponent()
		: m_velocity(0.0f), m_rotationSpeed(1.0f) {
	}

	void bwxGLMovementComponent::SetVelocity(const glm::vec3& velocity) {
		m_velocity = velocity;
	}

	glm::vec3 bwxGLMovementComponent::GetVelocity() const {
		return m_velocity;
	}

	void bwxGLMovementComponent::SetRotationSpeed(float speed) {
		m_rotationSpeed = speed;
	}

	float bwxGLMovementComponent::GetRotationSpeed() const {
		return m_rotationSpeed;
	}

	void bwxGLMovementComponent::Translate(const glm::vec3& offset) {
		if (auto node = m_node.lock()) {
			auto transform = node->GetComponent<bwxGLTransformComponent>();
			if (transform) {
				transform->SetPosition(transform->GetPosition() + offset);
			}
		}
	}

	void bwxGLMovementComponent::RotateEuler(const glm::vec3& eulerOffset) {
		if (auto node = m_node.lock()) {
			auto transform = node->GetComponent<bwxGLTransformComponent>();
			if (transform) {
				glm::vec3 current = transform->GetEulerAngles();
				transform->SetRotation(current + eulerOffset);
			}
		}
	}

	void bwxGLMovementComponent::RotateAroundAxis(const glm::vec3& axis, float angleDegrees) {
		if (auto node = m_node.lock()) {
			auto transform = node->GetComponent<bwxGLTransformComponent>();
			if (transform) {
				glm::quat q = glm::angleAxis(glm::radians(angleDegrees), glm::normalize(axis));
				glm::quat current = transform->GetRotation();
				transform->SetRotation(q * current);
			}
		}
	}

	void bwxGLMovementComponent::RotateQuaternion(const glm::quat& rotation) {
		if (auto node = m_node.lock()) {
			auto transform = node->GetComponent<bwxGLTransformComponent>();
			if (transform) {
				transform->SetRotation(rotation * transform->GetRotation());
			}
		}
	}

	void bwxGLMovementComponent::LookAt(const glm::vec3& target) {
		if (auto node = m_node.lock()) {
			auto transform = node->GetComponent<bwxGLTransformComponent>();
			if (transform) {
				glm::vec3 direction = glm::normalize(target - transform->GetPosition());
				glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
				glm::mat4 lookAtMatrix = glm::lookAt(transform->GetPosition(), target, up);
				transform->SetRotation(glm::quat_cast(glm::inverse(lookAtMatrix)));
			}
		}
	}

	void bwxGLMovementComponent::Zoom(float factor) {
		if (auto node = m_node.lock()) {
			auto transform = node->GetComponent<bwxGLTransformComponent>();
			if (transform) {
				glm::vec3 current = transform->GetScale();
				transform->SetScale(current * factor);
			}
		}
	}

	void bwxGLMovementComponent::SetMovementCallback(bwxGL_MOVEMENT_TYPE type, MovementCallback callback) {
		m_movementCallbacks[type] = std::move(callback);
	}

	void bwxGLMovementComponent::SetMovementStrategy(std::unique_ptr<bwxGLMovementStrategy> strategy) {
		m_movementStrategy = std::move(strategy);
	}

	bool bwxGLMovementComponent::HasMovementStrategy() const {
		return m_movementStrategy != nullptr;
	}

	void bwxGLMovementComponent::ProcessMovement(bwxGL_MOVEMENT_TYPE type, float delta) {
		bool handled = false;

		if (m_movementStrategy) {
			if (auto node = m_node.lock()) {
				m_movementStrategy->ProcessMovement(*node, type, delta);
				handled = true;
			}
		}

		if (!handled) {
			if (auto callbackIt = m_movementCallbacks.find(type); callbackIt != m_movementCallbacks.end()) {
				if (auto node = m_node.lock()) {
					callbackIt->second(*node, delta);
					handled = true;
				}
			}
		}

		if (!handled) {
			if (auto node = m_node.lock()) {
				auto transform = node->GetComponent<bwxGLTransformComponent>();
				if (!transform) return;

				switch (type) {
				case bwxGL_MOVEMENT_TYPE::FORWARD:
					Translate(glm::vec3(0, 0, -delta));
					break;
				case bwxGL_MOVEMENT_TYPE::BACKWARD:
					Translate(glm::vec3(0, 0, delta));
					break;
				case bwxGL_MOVEMENT_TYPE::LEFT:
					Translate(glm::vec3(-delta, 0, 0));
					break;
				case bwxGL_MOVEMENT_TYPE::RIGHT:
					Translate(glm::vec3(delta, 0, 0));
					break;
				case bwxGL_MOVEMENT_TYPE::UP:
					Translate(glm::vec3(0, delta, 0));
					break;
				case bwxGL_MOVEMENT_TYPE::DOWN:
					Translate(glm::vec3(0, -delta, 0));
					break;
				case bwxGL_MOVEMENT_TYPE::ROTATE_LEFT:
					RotateEuler(glm::vec3(0, -delta * m_rotationSpeed, 0));
					break;
				case bwxGL_MOVEMENT_TYPE::ROTATE_RIGHT:
					RotateEuler(glm::vec3(0, delta * m_rotationSpeed, 0));
					break;
				case bwxGL_MOVEMENT_TYPE::ZOOM_IN:
					Zoom(1.0f + delta);
					break;
				case bwxGL_MOVEMENT_TYPE::ZOOM_OUT:
					Zoom(1.0f / (1.0f + delta));
					break;
				case bwxGL_MOVEMENT_TYPE::JUMP:
					Translate(glm::vec3(0, delta * 5.0f, 0)); // Simple jump up
					break;
				}
			}
		}
	}

	void bwxGLMovementComponent::Update(float deltaTime) {
		if (glm::length(m_velocity) > 0.0f) {
			Translate(m_velocity * deltaTime);
		}
	}

} // namespace bwx_sdk
