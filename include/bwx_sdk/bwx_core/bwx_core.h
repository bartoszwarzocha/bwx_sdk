/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_core.h
// Purpose:     BWX_SDK Library; Basic core functions
// Author:      Bartosz Warzocha
// Created:     2015-09-03
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef _BWX_CORE_H_
#define _BWX_CORE_H_

#include "bwx_sdk/bwx_globals.h"

namespace bwx_sdk {

void bwxStdPathsInfo(void);

//----------------------------------------------------------------------------------------------
// Bitwise flag operations
//----------------------------------------------------------------------------------------------

inline void bwxSetByteFlag(int& var, bwxByteFlag flag) noexcept {
    var = flag;
}

inline void bwxAddByteFlag(int& var, bwxByteFlag flag) noexcept {
    var |= flag;
}

inline void bwxRemoveByteFlag(int& var, bwxByteFlag flag) noexcept {
    var &= ~flag;
}

inline bool bwxIsByteFlagSet(const int& var, bwxByteFlag flag) noexcept {
    return (var & flag) != 0;
}

//----------------------------------------------------------------------------------------------
// Multithreading (MT) error description functions
//----------------------------------------------------------------------------------------------

wxString bwxGetThreadErrorDescription(const wxThreadError& err, bool processLog = false);

wxString bwxGetSemaphoreErrorDescription(const wxSemaError& err, bool processLog = false);

wxString bwxGetMutexErrorDescritpion(const wxMutexError& err, bool processLog = false);

}  // namespace bwx_sdk

#endif
