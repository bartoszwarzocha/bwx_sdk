/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_scene.h
// Purpose:     BWX_SDK Library; OpenGL Scene
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_SCENE_H_
#define _BWX_GL_SCENE_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

//#include "bwx_gl_camera.h"
//#include "bwx_gl_light.h"
#include "bwx_gl_mesh.h"
#include "bwx_gl_model.h"
#include "bwx_gl_node.h"

#include <iostream>
#include <memory>
#include <vector>

namespace bwx_sdk {

    class bwxGLScene {
    public:
        bwxGLScene();
        ~bwxGLScene();

        // Zarz¹dzanie kamerami
        //void AddCamera(std::shared_ptr<bwxGLCamera> camera);
        void SetActiveCamera(int index);
        //std::shared_ptr<bwxGLCamera> GetActiveCamera() const;

        void AddMesh(std::shared_ptr<bwxGLMesh> mesh);
		std::shared_ptr<bwxGLMesh> GetMesh(int index) const;
		std::vector<std::shared_ptr<bwxGLMesh>> GetMeshes() const;

		void AddModel(std::shared_ptr<bwxGLModel> model);
		std::shared_ptr<bwxGLModel> GetModel(int index) const;
		std::vector<std::shared_ptr<bwxGLModel>> GetModels() const;

		//void AddLight(std::shared_ptr<bwxGLLight> light);
		//std::shared_ptr<bwxGLLight> GetLight(int index) const;
		//std::vector<std::shared_ptr<bwxGLLight>> GetLights() const;

		inline void SetRoot(std::shared_ptr<bwxGLNode> root) { m_root = root; }
		inline std::shared_ptr<bwxGLNode> GetRoot() const { return m_root; }

    private:
        //std::vector<std::shared_ptr<bwxGLCamera>> m_cameras;
		//std::vector<std::shared_ptr<bwxGLLight>> m_lights;
		std::vector<std::shared_ptr<bwxGLModel>> m_models;
        std::vector<std::shared_ptr<bwxGLMesh>> m_meshes;

        int m_activeCameraIndex = 0;

		std::shared_ptr<bwxGLNode> m_root;
    };

} // namespace bwx_sdk

#endif // _BWX_GL_SCENE_H_
