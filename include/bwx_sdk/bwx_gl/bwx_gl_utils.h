/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_utils.h
// Purpose:     BWX_SDK Libarary; OpenGL Utils
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

#ifndef _BWX_GL_UTILS_H_
#define _BWX_GL_UTILS_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <GL/glew.h>
#include <wx/glcanvas.h>

#include <chrono>
#include <glm/glm.hpp>

namespace bwx_sdk {

class bwxGLUtils {
public:
    static glm::vec2 GetWindowCoordinates(wxGLCanvas* canvas, glm::vec2 pos);

    static std::string GetVersion();

    static std::string GetVendor();

    static std::string GetRenderer();

    static wxGLContextAttrs GetDefaultContextAttrs(int major = 3, int minor = 3);

    static wxGLAttributes GetDefaultCanvasAttrs(int depth = 24);

    static std::string GetErrorString(int err);

    static void SetDefaultClearColor(GLfloat r = 0.2f, GLfloat g = 0.3f, GLfloat b = 0.3f, GLfloat a = 1.0f);
};

class bwxGLFPSMonitor {
public:
    bwxGLFPSMonitor();

    GLfloat GetFPS(int refresh_ms = 500);

    std::string GetFPSStr(int refresh_ms = 500);

    GLfloat GetDelta();

    GLfloat GetElapsedTime();

private:
    std::chrono::steady_clock::time_point m_lastUpdate;
    std::chrono::steady_clock::time_point m_lastFrame;

    GLint m_frames;
    GLfloat m_fps;
};

}  // namespace bwx_sdk

#endif  // _BWX_GL_UTILS_H_
