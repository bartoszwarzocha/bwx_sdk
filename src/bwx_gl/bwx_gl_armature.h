/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_armature.h
// Purpose:     BWX_SDK Libarary; OpenGL Armature
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_ARMATURE_H_
#define _BWX_GL_ARMATURE_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <vector>
#include <iostream>
#include <unordered_map>
#include <memory>

#include <GL/glew.h>
#include <glm/glm.hpp>

#include "bwx_gl_node.h"

namespace bwx_sdk {

	struct bwxGLBone {
		std::string name;
		glm::mat4 offset;
		glm::mat4 transform;
		glm::mat4 finalTransform;
		std::vector<std::shared_ptr<bwxGLBone>> children;
	};

	class bwxGLArmature : public bwxGLNode {
	public:
		bwxGLArmature();
		virtual ~bwxGLArmature();
		/* TODO: Implement the following methods
		void AddBone(std::shared_ptr<bwxGLBone> bone);
		void RemoveBone(std::shared_ptr<bwxGLBone> bone);
		std::vector<std::shared_ptr<bwxGLBone>> GetBones() const;
		void Update();
		void Render();
		void SetBoneTransform(const std::string& name, const glm::mat4& transform);
		glm::mat4 GetBoneTransform(const std::string& name);
		void SetBoneOffset(const std::string& name, const glm::mat4& offset);
		glm::mat4 GetBoneOffset(const std::string& name);
		void SetBoneFinalTransform(const std::string& name, const glm::mat4& finalTransform);
		glm::mat4 GetBoneFinalTransform(const std::string& name);
		void SetBoneChildren(const std::string& name, const std::vector<std::shared_ptr<bwxGLBone>>& children);
		std::vector<std::shared_ptr<bwxGLBone>> GetBoneChildren(const std::string& name);
		void SetBoneName(const std::string& name, const std::string& newName);
		std::string GetBoneName(const std::string& name);
		void SetBone(const std::string& name, std::shared_ptr<bwxGLBone> bone);
		std::shared_ptr<bwxGLBone> GetBone(const std::string& name);
		void SetBones(const std::vector<std::shared_ptr<bwxGLBone>>& bones);
		void SetBones(const std::unordered_map<std::string, std::shared_ptr<bwxGLBone>>& bones);
		void SetBones(const std::vector<std::string>& names, const std::vector<std::shared_ptr<bwxGLBone>>& bones);
		void SetBones(const std::vector<std::string>& names, const std::vector<glm::mat4>& offsets);
		*/
	private:
		std::vector<std::shared_ptr<bwxGLBone>> m_bones;
	};

} // namespace bwx_sdk

#endif