/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_scene.cpp
// Purpose:     BWX_SDK Library; OpenGL Scene Implementation
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <iostream>
#include <memory>
#include <vector>

//#include "bwx_gl_camera.h"
//#include "bwx_gl_light.h"
#include "bwx_gl_mesh.h"

#include "bwx_gl_scene.h"

namespace bwx_sdk {

    bwxGLScene::bwxGLScene() {}

    bwxGLScene::~bwxGLScene() {
        //m_cameras.clear();
		//m_lights.clear();
        m_meshes.clear();
    }

    //void bwxGLScene::AddCamera(std::shared_ptr<bwxGLCamera> camera) {
    //    m_cameras.push_back(camera);
    //}

    void bwxGLScene::SetActiveCamera(int index) {
        //if (index >= 0 && index < static_cast<int>(m_cameras.size())) {
        //    m_activeCameraIndex = index;
        //}
        //else {
        //    std::cerr << "Invalid camera index: " << index << std::endl;
        //}
    }

    //std::shared_ptr<bwxGLCamera> bwxGLScene::GetActiveCamera() const {
    //    if (m_cameras.empty()) return nullptr;
    //    return m_cameras[m_activeCameraIndex];
    //}

    void bwxGLScene::AddMesh(std::shared_ptr<bwxGLMesh> mesh) {
        m_meshes.push_back(mesh);
    }

	std::shared_ptr<bwxGLMesh> bwxGLScene::GetMesh(int index) const {
		if (index >= 0 && index < static_cast<int>(m_meshes.size())) {
			return m_meshes[index];
		}
		return nullptr;
	}

	std::vector<std::shared_ptr<bwxGLMesh>> bwxGLScene::GetMeshes() const {
		return m_meshes;
	}

	//void bwxGLScene::AddLight(std::shared_ptr<bwxGLLight> light) {
	//	m_lights.push_back(light);
	//}

	//std::shared_ptr<bwxGLLight> bwxGLScene::GetLight(int index) const {
	//	if (index >= 0 && index < static_cast<int>(m_lights.size())) {
	//		return m_lights[index];
	//	}
	//	return nullptr;
	//}

	//std::vector<std::shared_ptr<bwxGLLight>> bwxGLScene::GetLights() const {
	//	return m_lights;
	//}

} // namespace bwx_sdk
