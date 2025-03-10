/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl.h
// Purpose:     BWX_SDK Library; OpenGL
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-09
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef _BWX_GL_H_
#define _BWX_GL_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#define GLM_ENABLE_EXPERIMENTAL

#include <GL/glew.h>
#include <wx/wxprec.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/euler_angles.hpp>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <wx/filename.h>
#include <wx/image.h>
#include <wx/mstream.h>

#include <fstream>
#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>

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

#endif  // _BWX_GL_H_
