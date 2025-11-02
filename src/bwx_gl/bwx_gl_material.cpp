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

#include <bwx_sdk/bwx_gl/bwx_gl_material.h>
#include <bwx_sdk/bwx_gl/bwx_gl_texture_manager.h>

namespace bwx_sdk {

	bwxGLMaterial::bwxGLMaterial() { Clean(); }
	bwxGLMaterial::bwxGLMaterial(const std::string& name) : m_name(name) { Clean(); }
	bwxGLMaterial::~bwxGLMaterial() {}

	void bwxGLMaterial::Clean()
	{
		m_isEmissive = false;
		m_isTransparent = false;

		m_ambient = glm::vec4(1.0f, 1.0f, 1.0f, 1.0f);
		m_diffuse = glm::vec4(0.8f, 0.8f, 0.8f, 1.0f);
		m_specular = glm::vec4(0.5f, 0.5f, 0.5f, 1.0f);
		m_emissive = glm::vec4(0.0f);
		m_transparent = glm::vec4(0.0f);
		m_shininess = 0.5f * 128;
		m_refraction = 0.0f;

		m_textures.clear();
	}

    void bwxGLMaterial::Bind() const {
        int textureUnit = 0;
        for (const auto& [type, texturePath] : m_textures) {
            auto id = bwxGLTextureManager::GetInstance().GetTextureID(texturePath);
            if (id == 0) continue;

            glActiveTexture(GL_TEXTURE0 + textureUnit);
            glBindTexture(GL_TEXTURE_2D, id);
            textureUnit++;
        }
    }

	void bwxGLMaterial::Unbind() const {
		int textureUnit = 0;
		for (const auto& [type, texturePath] : m_textures) {
			auto id = bwxGLTextureManager::GetInstance().GetTextureID(texturePath);
			if (id == 0) continue;
			glActiveTexture(GL_TEXTURE0 + textureUnit);
			glBindTexture(GL_TEXTURE_2D, 0);
			textureUnit++;
		}
	}

	void bwxGLMaterial::Release() {
		Unbind();
	}

	void bwxGLMaterial::Unload() {
		Delete();
	}

	void bwxGLMaterial::Delete() {
		for (const auto& [type, texturePath] : m_textures) {
			auto id = bwxGLTextureManager::GetInstance().GetTextureID(texturePath);
			if (id == 0) continue;
			glDeleteTextures(1, &id);
		}
	}

    void bwxGLMaterial::ApplyToShader(bwxGLShaderProgram& shader) const {
        shader.SetUniform("material.ambient", m_ambient);
        shader.SetUniform("material.diffuse", m_diffuse);
        shader.SetUniform("material.specular", m_specular);
        shader.SetUniform("material.emissive", m_emissive);
        shader.SetUniform("material.transparent", m_transparent);
        shader.SetUniform("material.reflectivity", m_reflectivity);
        shader.SetUniform("material.shininess", m_shininess);
        shader.SetUniform("material.reflection", m_reflection);
        shader.SetUniform("material.refraction", m_refraction);
        shader.SetUniform("material.opacity", m_opacity);
    }

	void bwxGLMaterial::AddTexture(bwxGL_TEXTURE_TYPE type, const std::string& path) {
		m_textures[type] = path;
	}

	std::unordered_map<bwxGL_TEXTURE_TYPE, std::string>& bwxGLMaterial::GetTextures() {
		return m_textures;
	}

    bool bwxGLMaterial::HasTexture(bwxGL_TEXTURE_TYPE type) const {
		return m_textures.find(type) != m_textures.end();
    }

	std::string bwxGLMaterial::GetTexturePath(bwxGL_TEXTURE_TYPE type) const {
		auto it = m_textures.find(type);
		if (it != m_textures.end()) {
			return it->second;
		}
		return "";
	}

	void bwxGLMaterial::SetAmbient(const glm::vec4& ambient) {
		m_ambient = ambient;
	}

	void bwxGLMaterial::SetDiffuse(const glm::vec4& diffuse) {
		m_diffuse = diffuse;
	}

	void bwxGLMaterial::SetSpecular(const glm::vec4& specular) {
		m_specular = specular;
	}

	void bwxGLMaterial::SetEmissive(const glm::vec4& emissive) {
		m_emissive = emissive;
	}

	void bwxGLMaterial::SetTransparent(const glm::vec4& transparent) {
		m_transparent = transparent;
	}

	void bwxGLMaterial::SetReflectivity(const glm::vec4& reflectivity) {
		m_reflectivity = reflectivity;
	}

	void bwxGLMaterial::SetShininess(GLfloat shininess) {
		m_shininess = shininess;
	}

	void bwxGLMaterial::SetReflection(GLfloat reflection) {
		m_reflection = reflection;
	}

	void bwxGLMaterial::SetRefraction(GLfloat refraction) {
		m_refraction = refraction;
	}

	void bwxGLMaterial::SetOpacity(GLfloat opacity) {
		m_opacity = opacity;
	}

	const glm::vec4& bwxGLMaterial::GetAmbient() const {
		return m_ambient;
	}

	const glm::vec4& bwxGLMaterial::GetDiffuse() const {
		return m_diffuse;
	}

	const glm::vec4& bwxGLMaterial::GetSpecular() const {
		return m_specular;
	}

	const glm::vec4& bwxGLMaterial::GetEmissive() const {
		return m_emissive;
	}

	const glm::vec4& bwxGLMaterial::GetTransparent() const {
		return m_transparent;
	}

	const glm::vec4& bwxGLMaterial::GetReflectivity() const {
		return m_reflectivity;
	}

	GLfloat bwxGLMaterial::GetShininess() const {
		return m_shininess;
	}

	GLfloat bwxGLMaterial::GetReflection() const {
		return m_reflection;
	}

	GLfloat bwxGLMaterial::GetRefraction() const {
		return m_refraction;
	}

	GLfloat bwxGLMaterial::GetOpacity() const {
		return m_opacity;
	}

} // namespace bwx_sdk
