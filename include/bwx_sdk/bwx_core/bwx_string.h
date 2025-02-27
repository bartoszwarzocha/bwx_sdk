/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_string.h
// Purpose:     BWX_SDK Library; String processing utilities
// Author:      Bartosz Warzocha
// Created:     2018-02-06
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef _BWX_STRING_H__
#define _BWX_STRING_H__

#include "bwx_sdk/bwx_globals.h"

namespace bwx_sdk {

wxArrayString bwxSimpleExplode(const wxString& s, const wxString& separator);

void bwxSimpleExplode(wxArrayString* a, const wxString& str, const wxString& separator);

}  // namespace bwx_sdk

#endif
