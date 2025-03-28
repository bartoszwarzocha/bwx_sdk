/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_render_system.h
// Purpose:     BWX_SDK Library; OpenGL Render system (ECS)
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.


#ifndef _BWX_GL_RENDER_SYSTEM_H_
#define _BWX_GL_RENDER_SYSTEM_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <vector>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "bwx_gl_camera_component.h"
#include "bwx_gl_light_system.h"
#include "bwx_gl_buffer.h"

namespace bwx_sdk {

	class bwxGLRenderableComponent; // Forward declaration
	class bwxGLCameraComponent; // Forward declaration

    class bwxGLRenderSystem {
    public:
        static bwxGLRenderSystem& GetInstance() {
            static bwxGLRenderSystem instance;
            return instance;
        }

        void RegisterRenderable(std::shared_ptr<bwxGLRenderableComponent> renderable);
        void UnregisterRenderable(std::shared_ptr<bwxGLRenderableComponent> renderable);

		void Clear();

        void RenderAll();

        void SetActiveCamera(std::shared_ptr<bwxGLCameraComponent> m_activeCamera);
        std::shared_ptr<bwxGLCameraComponent> GetActiveCamera() const;

        void SetLightSystem(std::shared_ptr<bwxGLLightSystem> system);
        std::shared_ptr<bwxGLLightSystem> GetLightSystem() const;

        glm::mat4 GetViewMatrix() const;
        glm::mat4 GetProjectionMatrix() const;

    private:
        bwxGLRenderSystem() = default;
        ~bwxGLRenderSystem() = default;

        std::vector<std::shared_ptr<bwxGLRenderableComponent>> m_renderables;
		std::shared_ptr<bwxGLCameraComponent> m_activeCamera;
        std::shared_ptr<bwxGLLightSystem> m_lightSystem;
		bwxGLBuffer* m_lightUBO;
    };

} // namespace bwx_sdk

#endif // _BWX_GL_RENDER_SYSTEM_H_