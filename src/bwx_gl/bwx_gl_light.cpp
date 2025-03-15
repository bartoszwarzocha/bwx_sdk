/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_light.cpp
// Purpose:     BWX_SDK Libarary; OpenGL Light
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include "bwx_gl_light.h"

namespace bwx_sdk
{

    bwxGLLight::bwxGLLight() : bwxGLNode()
    {
        Init();
    }

    bwxGLLight::bwxGLLight(const bwxGL_LIGHT_TYPE& t, glm::vec3 pos) : bwxGLNode()
    {
        Init(t);
        SetPosition(pos);
    }

    bwxGLLight::bwxGLLight(const bwxGLLight& other) {
        *this = other;
    }

    void bwxGLLight::operator=(const bwxGLLight& other) {
		m_type = other.m_type;
		m_objectColor = other.m_objectColor;
		m_diffuse = other.m_diffuse;
		m_ambient = other.m_ambient;
		m_specular = other.m_specular;
		m_power = other.m_power;
		m_range = other.m_range;
		m_constant = other.m_constant;
		m_linear = other.m_linear;
		m_quadratic = other.m_quadratic;
		m_innerCone = other.m_innerCone;
		m_outerCone = other.m_outerCone;
    }

    void bwxGLLight::Init(const bwxGL_LIGHT_TYPE& t)
    {
        m_type = t;

        m_power = 1.0f;

        SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
        SetRotation(glm::vec3(0.0f, 0.0f, 1.0f));

        m_objectColor = glm::vec3(1.0f, 1.0f, 1.0f);

        m_diffuse = glm::vec3(1.0f, 1.0f, 1.0f);
        m_ambient = glm::vec3(0.5f, 0.5f, 0.5f);
        m_specular = glm::vec3(1.0f, 1.0f, 1.0f);

        m_innerCone = 14.5f;
        m_outerCone = 15.5f;

        SetRange(100);
    }

}