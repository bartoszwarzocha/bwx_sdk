/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_light_component.cpp
// Purpose:     BWX_SDK Library; Light component for ECS node
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <bwx_sdk/bwx_gl/bwx_gl_light_component.h>

namespace bwx_sdk {

    bwxGLLightComponent::bwxGLLightComponent(bwxGL_LIGHT_TYPE type)
        : m_type(type), m_objectColor(1.0f), m_diffuse(1.0f), m_ambient(0.0f), m_specular(1.0f),
        m_power(1.0f), m_range(10.0f), m_constant(1.0f), m_linear(0.45f), m_quadratic(0.75f),
        m_innerCone(15.0f), m_outerCone(30.0f) {
    }

    void bwxGLLightComponent::SetObjectColor(const glm::vec3& color) {
        m_objectColor = color;
    }

    glm::vec3 bwxGLLightComponent::GetObjectColor() const {
        return m_objectColor;
    }

    void bwxGLLightComponent::SetLightColor(const glm::vec3& color) {
        m_diffuse = color * m_power;
    }

    glm::vec3 bwxGLLightComponent::GetLightColor() const {
        return m_diffuse;
    }

    void bwxGLLightComponent::SetDiffuse(const glm::vec3& color) {
        SetLightColor(color);
    }

    glm::vec3 bwxGLLightComponent::GetDiffuse() const {
        return m_diffuse;
    }

    void bwxGLLightComponent::SetAmbient(const glm::vec3& color) {
        m_ambient = color;
    }

    glm::vec3 bwxGLLightComponent::GetAmbient() const {
        return m_ambient;
    }

    void bwxGLLightComponent::SetSpecular(const glm::vec3& color) {
        m_specular = color;
    }

    glm::vec3 bwxGLLightComponent::GetSpecular() const {
        return m_specular;
    }

    void bwxGLLightComponent::SetPower(float power) {
        m_power = power;
        SetLightColor(m_diffuse);
    }

    float bwxGLLightComponent::GetPower() const {
        return m_power;
    }

    void bwxGLLightComponent::SetRange(float range) {
        m_range = range;
        m_linear = 4.5f / m_range;
        m_quadratic = 75.0f / (m_range * m_range);
        m_constant = 1.0f;
    }

    float bwxGLLightComponent::GetRange() const {
        return m_range;
    }

    void bwxGLLightComponent::SetAttenuationConstant(float value) {
        m_constant = value;
    }

    void bwxGLLightComponent::SetAttenuationLinear(float value) {
        m_linear = value;
    }

    void bwxGLLightComponent::SetAttenuationQuadratic(float value) {
        m_quadratic = value;
    }

    float bwxGLLightComponent::GetAttenuationConstant() const {
        return m_constant;
    }

    float bwxGLLightComponent::GetAttenuationLinear() const {
        return m_linear;
    }

    float bwxGLLightComponent::GetAttenuationQuadratic() const {
        return m_quadratic;
    }

    void bwxGLLightComponent::SetInnerCone(float angle) {
        m_innerCone = angle;
    }

    void bwxGLLightComponent::SetOuterCone(float angle) {
        m_outerCone = angle;
    }

    float bwxGLLightComponent::GetInnerCone() const {
        return m_innerCone;
    }

    float bwxGLLightComponent::GetOuterCone() const {
        return m_outerCone;
    }

    int bwxGLLightComponent::GetType() const {
        return static_cast<int>(m_type);
    }

} // namespace bwx_sdk