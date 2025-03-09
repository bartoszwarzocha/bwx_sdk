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

#include <wx/string.h>
#include <vector>
#include <unordered_map>

#include "bwx_gl_texture.h"
#include "bwx_gl_shader.h"

#if defined(__APPLE__)
    #error OpenGL functionality is not available for macOS.
#else
    #include <GL/glew.h>
    #include <glm/glm.hpp>
#endif

namespace bwx_sdk {

    enum bwxGL_TEXTURE_TYPE {
        TEXTURE_DIFFUSE,
        TEXTURE_SPECULAR,
        TEXTURE_NORMAL,
        TEXTURE_HEIGHT,
        TEXTURE_EMISSIVE,
        TEXTURE_OPACITY,
        TEXTURE_UNKNOWN
    };

    class bwxGLMaterial {
    public:
        bwxGLMaterial();
        ~bwxGLMaterial();

        void Clean();
        void ApplyToShader(bwxGLShaderProgram& shader) const;

        // Ustawianie w³aœciwoœci materia³u
        inline void SetAmbient(glm::vec4 a) { m_ambient = a; }
        inline void SetDiffuse(glm::vec4 d) { m_diffuse = d; }
        inline void SetSpecular(glm::vec4 s) { m_specular = s; }
        inline void SetEmissive(glm::vec4 e) { m_emissive = e; }
        inline void SetTransparent(glm::vec4 t) { m_transparent = t; }
        inline void SetReflectivity(glm::vec4 r) { m_reflectivity = r; }
        inline void SetShininess(GLfloat s) { m_shininess = s; }
        inline void SetReflection(GLfloat r) { m_reflection = r; }
        inline void SetRefraction(GLfloat r) { m_refraction = r; }
        inline void SetOpacity(GLfloat o) { m_opacity = o; }

        inline glm::vec4 GetAmbient() const { return m_ambient; }
        inline glm::vec4 GetDiffuse() const { return m_diffuse; }
        inline glm::vec4 GetSpecular() const { return m_specular; }
        inline glm::vec4 GetEmissive() const { return m_emissive; }
        inline glm::vec4 GetTransparent() const { return m_transparent; }
        inline glm::vec4 GetReflectivity() const { return m_reflectivity; }
        inline GLfloat GetShininess() const { return m_shininess; }
        inline GLfloat GetReflection() const { return m_reflection; }
        inline GLfloat GetRefraction() const { return m_refraction; }
        inline GLfloat GetOpacity() const { return m_opacity; }

        // Tekstury
        void AddTexture(bwxGL_TEXTURE_TYPE type, const std::string& texturePath);
        inline std::unordered_map<bwxGL_TEXTURE_TYPE, std::string>& GetTextures() { return m_textures; }
        inline bool HasTexture(bwxGL_TEXTURE_TYPE type) const { return m_textures.find(type) != m_textures.end(); }
        inline std::string GetTexturePath(bwxGL_TEXTURE_TYPE type) const { return HasTexture(type) ? m_textures.at(type) : ""; }

        // Identyfikacja materia³u
        inline void SetName(const std::string& n) { m_name = n; }
        inline std::string GetName() const { return m_name; }

        inline void SetId(unsigned int d) { m_id = d; }
        inline unsigned int GetId() const { return m_id; }

        // Flagowanie materia³u
        inline void SetEmissive(bool e = true) { m_isEmissive = e; }
        inline void SetTransparent(bool o = true) { m_isTransparent = o; }
        inline void SetReflection(bool r = true) { m_isReflection = r; }
        inline void SetRefraction(bool r = true) { m_isRefraction = r; }
        inline void SetTwoSided(bool ts = true) { m_isTwoSided = ts; }

        inline bool IsEmissive() const { return m_isEmissive; }
        inline bool IsTransparent() const { return m_isTransparent; }
        inline bool IsReflection() const { return m_isReflection; }
        inline bool IsRefraction() const { return m_isRefraction; }
        inline bool IsTwoSided() const { return m_isTwoSided; }

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

        std::unordered_map<bwxGL_TEXTURE_TYPE, std::string> m_textures; // Mapowanie tekstur

        std::string m_name;
        unsigned int m_id;

        bool m_isTransparent;
        bool m_isEmissive;
        bool m_isReflection;
        bool m_isRefraction;
        bool m_isTwoSided;
    };

} // namespace bwx_sdk

#endif
