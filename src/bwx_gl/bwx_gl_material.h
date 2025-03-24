/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_material.h
// Purpose:     BWX_SDK Libarary; OpenGL Material
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_MATERIAL_H_
#define _BWX_GL_MATERIAL_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#define bwxGL_MATERIAL_IOR_AIR          1.000
#define bwxGL_MATERIAL_IOR_WATER        1.333
#define bwxGL_MATERIAL_IOR_DIAMOND      2.417
#define bwxGL_MATERIAL_IOR_ETHYL_ALC    1.360
#define bwxGL_MATERIAL_IOR_WHISKY       1.356
#define bwxGL_MATERIAL_IOR_VODKA        1.363
#define bwxGL_MATERIAL_IOR_AMBER        1.539
#define bwxGL_MATERIAL_IOR_AMETHYST     1.532
#define bwxGL_MATERIAL_IOR_GOLD         0.470

#include "bwx_gl_shader.h"
#include "bwx_gl_texture.h"
#include "bwx_gl_resource_manager.h"

#include <unordered_map>
#include <string>
#include <iostream>
#include <fstream>
#include <vector>
#include <memory>

namespace bwx_sdk {

    class bwxGLMaterial : public bwxGLResource {
    public:
        bwxGLMaterial();
        bwxGLMaterial(const std::string& name);
        ~bwxGLMaterial();

		void Clean();

        void ApplyToShader(bwxGLShaderProgram& shader) const;
        
		void Bind() const;
		void Unbind() const;
		void Release();
		void Unload();
		void Delete();

		void AddTexture(bwxGL_TEXTURE_TYPE type, const std::string& path);
        std::unordered_map<bwxGL_TEXTURE_TYPE, std::string>& GetTextures();
        bool HasTexture(bwxGL_TEXTURE_TYPE type) const;
        std::string GetTexturePath(bwxGL_TEXTURE_TYPE type) const;

		void SetAmbient(const glm::vec4& ambient);
		void SetDiffuse(const glm::vec4& diffuse);
		void SetSpecular(const glm::vec4& specular);
		void SetEmissive(const glm::vec4& emissive);
		void SetTransparent(const glm::vec4& transparent);
		void SetReflectivity(const glm::vec4& reflectivity);

		void SetShininess(GLfloat shininess);
		void SetReflection(GLfloat reflection);
		void SetRefraction(GLfloat refraction);
		void SetOpacity(GLfloat opacity);

		const glm::vec4& GetAmbient() const;
		const glm::vec4& GetDiffuse() const;
		const glm::vec4& GetSpecular() const;
		const glm::vec4& GetEmissive() const;
		const glm::vec4& GetTransparent() const;
		const glm::vec4& GetReflectivity() const;

		GLfloat GetShininess() const;
		GLfloat GetReflection() const;
		GLfloat GetRefraction() const;
		GLfloat GetOpacity() const;

		void SetName(const std::string& name) { m_name = name; }
		const std::string& GetName() const { return m_name; }

		void SetID(unsigned int id) { m_id = id; }
		unsigned int GetID() const { return m_id; }

		void SetTransparent(bool transparent) { m_isTransparent = transparent; }
		bool IsTransparent() const { return m_isTransparent; }
		void SetEmissive(bool emissive) { m_isEmissive = emissive; }
		bool IsEmissive() const { return m_isEmissive; }
		void SetReflection(bool reflection) { m_isReflection = reflection; }
		bool IsReflection() const { return m_isReflection; }
		void SetRefraction(bool refraction) { m_isRefraction = refraction; }
		bool IsRefraction() const { return m_isRefraction; }
		void SetTwoSided(bool twoSided) { m_isTwoSided = twoSided; }
		bool IsTwoSided() const { return m_isTwoSided; }

    private:
        glm::vec4 m_ambient;
        glm::vec4 m_diffuse;
        glm::vec4 m_specular;
        glm::vec4 m_emissive;
        glm::vec4 m_transparent;
        glm::vec4 m_reflectivity;

        GLfloat m_shininess;
        GLfloat m_reflection;
        GLfloat m_refraction;
        GLfloat m_opacity;

		std::string m_name;

        unsigned int m_id;

        bool m_isTransparent;
        bool m_isEmissive;
        bool m_isReflection;
        bool m_isRefraction;
        bool m_isTwoSided;

        std::unordered_map<bwxGL_TEXTURE_TYPE, std::string> m_textures;
    };

} // namespace bwx_sdk

#endif
