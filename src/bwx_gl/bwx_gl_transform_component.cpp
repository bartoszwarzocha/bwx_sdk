/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_transform_component.cpp
// Purpose:     BWX_SDK Library; Transformation component (ECS)
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include "bwx_gl_transform_component.h"

namespace bwx_sdk {

	void bwxGLTransformComponent::RecalculateMatrix() {
		m_transform = glm::translate(m_position)
			* glm::mat4_cast(m_rotation)
			* glm::scale(m_scale);
		m_transformDirty = false;
	}

	glm::mat4 bwxGLTransformComponent::GetTransformMatrix() {
		if (m_transformDirty) {
			RecalculateMatrix();
		}
		return m_transform;
	}

} // namespace bwx_sdk