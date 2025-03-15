/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_movement.h
// Purpose:     BWXSDK; OpenGL, "silnik ruchu" oparty o zdarzenia wxWidgets
// Author:      Bartosz Warzocha
// Created:     2020-04-11
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		All rights reserved.
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "bwx_gl_movement.h"

namespace bwx_sdk
{

	bwxGLMovement::bwxGLMovement()
	{
		BindEvents();
	}

	void bwxGLMovement::BindEvents()
	{
		Bind(wxEVT_KEY_DOWN, &bwxGLMovement::OnKey, this);
		Bind(wxEVT_KEY_UP, &bwxGLMovement::OnKey, this);
	}

	void bwxGLMovement::UnbindEvents()
	{
		Unbind(wxEVT_KEY_DOWN, &bwxGLMovement::OnKey, this);
		Unbind(wxEVT_KEY_UP, &bwxGLMovement::OnKey, this);
	}

	void bwxGLMovement::OnKey(wxKeyEvent& event)
	{
		wxLogMessage(wxString::Format("%d", event.GetKeyCode()));
	}

}
