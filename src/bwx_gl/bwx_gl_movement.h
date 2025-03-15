/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_movement.h
// Purpose:     BWXSDK; OpenGL, "silnik ruchu" oparty o zdarzenia wxWidgets
// Author:      Bartosz Warzocha
// Created:     2020-04-11
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_MOVEMENT_H_
#define _BWX_GL_MOVEMENT_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

namespace bwx_sdk {

	class bwxGLMovement : public wxEvtHandler
	{
	public:
		bwxGLMovement();

		void BindEvents();
		void UnbindEvents();

		void OnKey(wxKeyEvent& event);

	private:
	};

} // namespace bwx_sdk

#endif