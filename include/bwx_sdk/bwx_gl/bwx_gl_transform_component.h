/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_transform_component.h
// Purpose:     BWX_SDK Library; Transformation component (ECS)
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.


#ifndef _BWX_GL_TRANSFORM_COMPONENT_H_
#define _BWX_GL_TRANSFORM_COMPONENT_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#define GLM_ENABLE_EXPERIMENTAL

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "bwx_gl_component.h"

namespace bwx_sdk {

	class bwxGLTransformComponent : public bwxGLComponent {
	public:
		bwxGLTransformComponent()
			: m_position(glm::vec3(0.0f)), m_rotation(glm::quat()), m_scale(glm::vec3(1.0f)), m_transform(glm::mat4(0.0)), m_transformDirty(true)
		{
		}

		inline void SetPosition(const glm::vec3& pos) { m_position = pos; m_transformDirty = true; }
		inline void SetPosition(float x, float y, float z) { SetPosition(glm::vec3(x, y, z)); }
		
		inline void SetRotation(const glm::quat& rot) { m_rotation = rot; m_transformDirty = true; }
		inline void SetRotation(const glm::vec3& angles) { m_rotation = glm::quat(angles); m_transformDirty = true; }
		inline void SetRotation(float pitch, float yaw, float roll) { SetRotation(glm::vec3(pitch, yaw, roll)); }
		inline void SetYaw(float angle) { m_rotation = glm::rotate(m_rotation, angle, glm::vec3(0.0f, 1.0f, 0.0f)); m_transformDirty = true; }
		inline void SetPitch(float angle) { m_rotation = glm::rotate(m_rotation, angle, glm::vec3(1.0f, 0.0f, 0.0f)); m_transformDirty = true; }
		inline void SetRoll(float angle) { m_rotation = glm::rotate(m_rotation, angle, glm::vec3(0.0f, 0.0f, 1.0f)); m_transformDirty = true; }
		
		inline void SetScale(float scale) { SetScale(glm::vec3(scale)); }
		inline void SetScale(const glm::vec3& scale) { m_scale = scale; m_transformDirty = true; }

		inline glm::vec3 GetPosition() const { return m_position; }

		inline glm::quat GetRotation() const { return m_rotation; }
		inline glm::vec3 GetEulerAngles() const { return glm::eulerAngles(m_rotation); }
		inline float GetYaw() const { return glm::yaw(m_rotation); }
		inline float GetPitch() const { return glm::pitch(m_rotation); }
		inline float GetRoll() const { return glm::roll(m_rotation); }

		inline glm::vec3 GetScale() const { return m_scale; }

		glm::mat4 GetTransformMatrix();

	private:
		void RecalculateMatrix();

		glm::vec3 m_position;
		glm::quat m_rotation;
		glm::vec3 m_scale;
		glm::mat4 m_transform;

		bool m_transformDirty;
	};

} // namespace bwx_sdk

#endif // _BWX_GL_TRANSFORM_COMPONENT_H_