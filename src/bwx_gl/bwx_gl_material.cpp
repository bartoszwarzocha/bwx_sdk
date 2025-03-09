/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_material.cpp
// Purpose:     BWX_SDK Libarary; OpenGL Material
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
    #error OpenGL functionality is not available for macOS.
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <vector>

#include "bwx_gl_shader.h"
#include "bwx_gl_texture.h"
#include "bwx_gl_texture_manager.h"
#include "bwx_gl_material.h"

namespace bwx_sdk {

    bwxGLMaterial::bwxGLMaterial()
    {
        Clean();
    }
    
    bwxGLMaterial::~bwxGLMaterial()
    {
    }

    void bwxGLMaterial::Clean()
    {
        SetEmissive(false);
        SetTransparent(false);
        SetReflection(false);
        SetRefraction(false);
        SetTwoSided(false);

        m_ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
        m_diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
        m_specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
        m_emissive = glm::vec4(0.0f);
        m_transparent = glm::vec4(0.0f);
        m_reflectivity = glm::vec4(0.0f);
        m_shininess = 0.5f * 128;
        m_refraction = 0.0f;
        m_opacity = 1.0f;

        m_textures.clear();
    }

    void bwxGLMaterial::ApplyToShader(bwxGLShaderProgram& shader) const
    {
        shader.SetUniform("material.ambient", m_ambient);
        shader.SetUniform("material.diffuse", m_diffuse);
        shader.SetUniform("material.specular", m_specular);
        shader.SetUniform("material.emissive", m_emissive);
        shader.SetUniform("material.transparent", m_transparent);
        shader.SetUniform("material.reflectivity", m_reflectivity);
        shader.SetUniform("material.shininess", m_shininess);
        shader.SetUniform("material.refraction", m_refraction);
        shader.SetUniform("material.opacity", m_opacity);

        int textureIndex = 0;
        for (const auto& [type, texturePath] : m_textures)
        {
            GLuint textureID = bwxGLTextureManager::GetInstance().LoadTexture(texturePath);
            glActiveTexture(GL_TEXTURE0 + textureIndex);
            glBindTexture(GL_TEXTURE_2D, textureID);

            std::string uniformName;
            switch (type)
            {
            case TEXTURE_DIFFUSE: uniformName = "material.diffuseMap"; break;
            case TEXTURE_SPECULAR: uniformName = "material.specularMap"; break;
            case TEXTURE_NORMAL: uniformName = "material.normalMap"; break;
            case TEXTURE_HEIGHT: uniformName = "material.heightMap"; break;
            case TEXTURE_EMISSIVE: uniformName = "material.emissiveMap"; break;
            case TEXTURE_OPACITY: uniformName = "material.opacityMap"; break;
            default: continue;
            }

            shader.SetUniform(uniformName, textureIndex);
            textureIndex++;
        }
    }

    void bwxGLMaterial::AddTexture(bwxGL_TEXTURE_TYPE type, const std::string& texturePath)
    {
        m_textures[type] = texturePath;
    }

} // namespace bwx_sdk
