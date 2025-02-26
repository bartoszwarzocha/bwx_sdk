/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_utils.cpp
// Purpose:     BWXSDK Library - Utility functions
// Author:      Bartosz Warzocha
// Created:     2018-02-20
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     All rights reserved.
/////////////////////////////////////////////////////////////////////////////

/**
 * @file bwx_utils.cpp
 * @brief Implements utility functions for the BWXSDK library.
 */

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "bwx_sdk/bwx_core/bwx_math.h"

#include "bwx_utils.h"

namespace bwx_sdk {

	//----------------------------------------------------------------------------------------------
	// Colour functions
	//----------------------------------------------------------------------------------------------
	wxColour bwxMixColours(const wxColour& col1, const wxColour& col2, double factor)
	{
		return wxColour(
			(col2.Red() * factor) + (col1.Red() * (1 - factor)),
			(col2.Green() * factor) + (col1.Green() * (1 - factor)),
			(col2.Blue() * factor) + (col1.Blue() * (1 - factor))
		);
	}

}
