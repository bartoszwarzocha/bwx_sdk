/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl.h
// Purpose:     BWX_SDK Library; OpenGL
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_H_
#define _BWX_GL_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/euler_angles.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/filename.h>
#include <wx/image.h>
#include <wx/mstream.h>

#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>
#include <fstream>

#include "bwx_gl_armature.h"
#include "bwx_gl_buffer.h"
#include "bwx_gl_buffer_manager.h"
#include "bwx_gl_camera.h"
#include "bwx_gl_image_loader.h"
#include "bwx_gl_light.h"
#include "bwx_gl_material.h"
#include "bwx_gl_material_library.h"
#include "bwx_gl_mesh.h"
#include "bwx_gl_model.h"
#include "bwx_gl_movement.h"
#include "bwx_gl_node.h"
#include "bwx_gl_renderable.h"
#include "bwx_gl_renderer.h"
#include "bwx_gl_scene.h"
#include "bwx_gl_shader.h"
#include "bwx_gl_skybox.h"
#include "bwx_gl_texture.h"
#include "bwx_gl_texture_manager.h"

#endif // _BWX_GL_H_
