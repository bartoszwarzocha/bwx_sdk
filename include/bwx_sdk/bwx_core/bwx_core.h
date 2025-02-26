/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_core.h
// Purpose:     BWX_SDK Library; Basic core functions
// Author:      Bartosz Warzocha
// Created:     2015-09-03
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_CORE_H_
#define _BWX_CORE_H_

#include "bwx_sdk/bwx_globals.h"

namespace bwx_sdk {

	void bwxStdPathsInfo(void);

	//----------------------------------------------------------------------------------------------
	// Bitwise flag operations
	//----------------------------------------------------------------------------------------------
	
	inline void bwxSetByteFlag(int& var, bwxByteFlag flag) noexcept;
	
	inline void bwxAddByteFlag(int& var, bwxByteFlag flag) noexcept;
	
	inline void bwxRemoveByteFlag(int& var, bwxByteFlag flag) noexcept;
	
	inline bool bwxIsByteFlagSet(int& var, bwxByteFlag flag) noexcept;

	//----------------------------------------------------------------------------------------------
	// Multithreading (MT) error description functions
	//----------------------------------------------------------------------------------------------

	wxString bwxGetThreadErrorDescription(const wxThreadError& err, bool processLog = false);
	
	wxString bwxGetSemaphoreErrorDescription(const wxSemaError& err, bool processLog = false);
	
	wxString bwxGetMutexErrorDescritpion(const wxMutexError& err, bool processLog = false);

}

#endif
