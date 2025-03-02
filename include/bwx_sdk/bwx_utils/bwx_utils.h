/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_utils.h
// Purpose:     BWXSDK
// Author:      Bartosz Warzocha
// Created:     2018-02-20
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef _BWX_UTILS_H_
#define _BWX_UTILS_H_

#include "bwx_sdk/bwx_globals.h"

namespace bwx_sdk {

wxColour bwxGetRandomColour();

void bwxGetRandomColours(wxVector<wxColour>* v, int howMany, bool unique = true,
                         const wxColour& first = wxColour(255, 0, 255));

wxColour bwxMixColours(const wxColour& col1, const wxColour& col2, double factor);

}  // namespace bwx_sdk

#endif
