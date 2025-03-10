/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_light.h
// Purpose:     BWX_SDK Libarary; OpenGL Light
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_LIGHT_H_
#define _BWX_GL_LIGHT_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "bwx_gl_node.h"

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>

namespace bwx_sdk
{

	enum class bwxGL_LIGHT_TYPE
	{
		LIGHT_UNDEFINED = 0,
		LIGHT_SUN,
		LIGHT_POINT,
		LIGHT_SPOT,
		LIGHT_HEMI,
		LIGHT_AREA,
		LIGHT_STANDARD
	};

	class bwxGLLight : public bwxGLNode
	{
	public:
		bwxGLLight();
		bwxGLLight(const bwxGL_LIGHT_TYPE& t, glm::vec3 pos);

		void Init(const bwxGL_LIGHT_TYPE& t = bwxGL_LIGHT_TYPE::LIGHT_SUN);

		inline void SetObjectColor(glm::vec3 c) { m_objectColor = c; }
		inline glm::vec3 GetObjectColor() { return m_objectColor; }

		inline void SetLightColor(glm::vec3 c) { m_diffuse = c; }
		inline void SetLightColor(GLfloat r, GLfloat g, GLfloat b) { SetLightColor(glm::vec3(r * m_power, g * m_power, b * m_power)); }
		inline glm::vec3 GetLightColor() { return m_diffuse; }

		inline void SetDiffuse(glm::vec3 c) { SetLightColor(c); }
		inline void SetDiffuse(GLfloat r, GLfloat g, GLfloat b) { SetLightColor(r, g, b); };
		inline glm::vec3 GetDiffuse() { return m_diffuse; }

		inline void SetAmbient(glm::vec3 c) { m_ambient = c; }
		inline void SetAmbient(GLfloat r, GLfloat g, GLfloat b) { SetAmbient(glm::vec3(r, g, b)); }
		inline glm::vec3 GetAmbient() { return m_ambient; }

		inline void SetSpecular(glm::vec3 c) { m_specular = c; }
		inline void SetSpecular(GLfloat r, GLfloat g, GLfloat b) { SetSpecular(glm::vec3(r, g, b)); }
		inline glm::vec3 GetSpecular() { return m_specular; }

		inline void SetPower(GLfloat p)
		{
			m_power = p;
			SetLightColor(GetLightColor());
		}
		inline GLfloat GetPower() { return m_power; }

		inline void SetRange(GLfloat r)
		{
			m_range = r;
			m_linear = 4.5f / m_range;
			m_quadratic = 75.0f / (m_range * m_range);
			m_constant = 1.0f;
		}
		inline GLfloat GetRange() { return m_range; }

		inline void SetAttenuationLinear(GLfloat l) { m_linear = l; }
		inline void SetAttenuationQuadric(GLfloat q) { m_quadratic = q; }
		inline void SetAttenuationConstant(GLfloat c) { m_constant = c; }

		inline GLfloat GetAttenuationLinear() { return m_linear; }
		inline GLfloat GetAttenuationQuadric() { return m_quadratic; }
		inline GLfloat GetAttenuationConstant() { return m_constant; }

		// SPOT
		inline void SetInnerCone(GLfloat co) { m_innerCone = co; }
		inline GLfloat GetInnerCone() { return m_innerCone; }

		inline void SetOuterCone(GLfloat co) { m_outerCone = co; }
		inline GLfloat GetOuterCone() { return m_outerCone; }

		inline int GetType() { return (GLint)m_type; }

	private:
		bwxGLLight(const bwxGLLight& other);
		void operator=(const bwxGLLight& other);

		bwxGL_LIGHT_TYPE m_type;

		glm::vec3 m_objectColor;

		glm::vec3 m_diffuse;
		glm::vec3 m_ambient;
		glm::vec3 m_specular;

		GLfloat m_power;

		GLfloat m_range;
		GLfloat m_constant;
		GLfloat m_linear;
		GLfloat m_quadratic;

		GLfloat m_innerCone;
		GLfloat m_outerCone;
	};

} // namespace bwx_sdk

#endif
