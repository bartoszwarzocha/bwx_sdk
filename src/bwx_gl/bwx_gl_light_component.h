/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_light_component.h
// Purpose:     BWX_SDK Library; Light component for ECS node
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_LIGHT_COMPONENT_H_
#define _BWX_GL_LIGHT_COMPONENT_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <glm/glm.hpp>
#include "bwx_gl_component.h"

namespace bwx_sdk {

    enum class bwxGL_LIGHT_TYPE {
        LIGHT_UNDEFINED = 0,
        LIGHT_SUN,
        LIGHT_POINT,
        LIGHT_SPOT,
        LIGHT_HEMI,
        LIGHT_AREA,
        LIGHT_STANDARD
    };

    class bwxGLLightComponent : public bwxGLComponent {
    public:
        explicit bwxGLLightComponent(bwxGL_LIGHT_TYPE type = bwxGL_LIGHT_TYPE::LIGHT_SUN);

        void SetObjectColor(const glm::vec3& color);
        glm::vec3 GetObjectColor() const;

        void SetLightColor(const glm::vec3& color);
        glm::vec3 GetLightColor() const;

        void SetDiffuse(const glm::vec3& color);
        glm::vec3 GetDiffuse() const;

        void SetAmbient(const glm::vec3& color);
        glm::vec3 GetAmbient() const;

        void SetSpecular(const glm::vec3& color);
        glm::vec3 GetSpecular() const;

        void SetPower(float power);
        float GetPower() const;

        void SetRange(float range);
        float GetRange() const;

        void SetAttenuationConstant(float value);
        void SetAttenuationLinear(float value);
        void SetAttenuationQuadratic(float value);

        float GetAttenuationConstant() const;
        float GetAttenuationLinear() const;
        float GetAttenuationQuadratic() const;

        void SetInnerCone(float angle);
        void SetOuterCone(float angle);

        float GetInnerCone() const;
        float GetOuterCone() const;

        int GetType() const;

    private:
        bwxGL_LIGHT_TYPE m_type;

        glm::vec3 m_objectColor;
        glm::vec3 m_diffuse;
        glm::vec3 m_ambient;
        glm::vec3 m_specular;

        float m_power;

        float m_range;
        float m_constant;
        float m_linear;
        float m_quadratic;

        float m_innerCone;
        float m_outerCone;
    };

} // namespace bwx_sdk

#endif // _BWX_GL_LIGHT_COMPONENT_H_
