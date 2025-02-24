/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_utils.h
// Purpose:     BWXSDK
// Author:      Bartosz Warzocha
// Created:     2018-02-20
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_UTILS_H_
#define _BWX_UTILS_H_

#include "bwx_sdk/bwx_globals.h"

namespace bwx_sdk {

	wxColour bwxMixColours(const wxColour& col1, const wxColour& col2, double factor);

}

#endif
