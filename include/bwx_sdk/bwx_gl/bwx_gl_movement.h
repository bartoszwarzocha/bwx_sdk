/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_movement.h
// Purpose:     BWXSDK; OpenGL, "silnik ruchu" oparty o zdarzenia wxWidgets
// Author:      Bartosz Warzocha
// Created:     2020-04-11
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		All rights reserved.
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef _BWX_GL_MOVEMENT_H_
#define _BWX_GL_MOVEMENT_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

namespace bwx_sdk {

class bwxGLMovement : public wxEvtHandler {
public:
    bwxGLMovement();

    void BindEvents();
    void UnbindEvents();

    void OnKey(wxKeyEvent& event);

private:
};

}  // namespace bwx_sdk

#endif
