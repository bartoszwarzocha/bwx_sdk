/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_string.h
// Purpose:     BWX_SDK Library; String processing utilities
// Author:      Bartosz Warzocha
// Created:     2018-02-06
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_STRING_H__
#define _BWX_STRING_H__

#include "bwx_sdk/bwx_globals.h"

namespace bwx_sdk {

	wxArrayString bwxSimpleExplode(const wxString& s, const wxString& separator);

	void bwxSimpleExplode(wxArrayString* a, const wxString& str, const wxString& separator);

}

#endif
