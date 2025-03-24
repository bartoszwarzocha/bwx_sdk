/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_component.h
// Purpose:     BWX_SDK Libarary; OpenGL Component (ECS)
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_COMPONENT_H_
#define _BWX_GL_COMPONENT_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <vector>
#include <iostream>
#include <unordered_map>
#include <memory>

namespace bwx_sdk {

    class bwxGLNode; // Forward declaration

    class bwxGLComponent {
    public:
        virtual ~bwxGLComponent() = default;
		virtual void Update(float deltaTime) {} // Optional update
		virtual void Render() {} // Optional render

		void SetNode(std::shared_ptr<bwxGLNode> node) { m_node = node; }
		std::shared_ptr<bwxGLNode> GetNode() const { return m_node.lock(); }

    protected:
        bwxGLComponent() = default;
		std::weak_ptr<bwxGLNode> m_node;
    };

} // namespace bwx_sdk

#endif // _BWX_GL_COMPONENT_H_