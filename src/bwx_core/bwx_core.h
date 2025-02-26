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

/**
 * @file bwx_core.cpp
 * @brief Implements core functions for the BWXSDK.
 *
 * This file provides basic functions such as displaying standard system paths,
 * performing bit flag operations, and retrieving error descriptions for various
 * multithreading-related errors.
 */

namespace bwx_sdk {

	/**
	 * @brief Displays standard system paths in a message box.
	 *
	 * This function retrieves several common directories (such as application,
	 * configuration, data, and temporary directories) using wxStandardPaths
	 * and displays them in a message box.
	 */
	void bwxStdPathsInfo(void);

	//----------------------------------------------------------------------------------------------
	// Bitwise flag operations
	//----------------------------------------------------------------------------------------------
	
	/**
	 * @brief Adds a bit flag to a variable.
	 *
	 * This inline function sets the specified bit flag within the given variable.
	 *
	 * @param var Reference to the variable to modify.
	 * @param flag The bit flag to add.
	 */
	inline void bwxSetByteFlag(int& var, bwxByteFlag flag) noexcept;
	
	/**
	 * @brief Removes a bit flag from a variable.
	 *
	 * This inline function clears the specified bit flag from the given variable.
	 *
	 * @param var Reference to the variable to modify.
	 * @param flag The bit flag to remove.
	 */
	inline void bwxAddByteFlag(int& var, bwxByteFlag flag) noexcept;
	
	/**
	 * @brief Checks whether a bit flag is set in a variable.
	 *
	 * This inline function tests if the specified bit flag is present in the variable.
	 *
	 * @param var The variable to check.
	 * @param flag The bit flag to verify.
	 * @return true if the flag is set; false otherwise.
	 */
	inline void bwxRemoveByteFlag(int& var, bwxByteFlag flag) noexcept;
	
	/**
	 * @brief Overwrites a variable with a specific bit flag.
	 *
	 * This inline function sets the variable to exactly the given bit flag value,
	 * replacing any previous flags.
	 *
	 * @param var Reference to the variable to modify.
	 * @param flag The new bit flag value.
	 */
	inline bool bwxIsByteFlagSet(int& var, bwxByteFlag flag) noexcept;

	//----------------------------------------------------------------------------------------------
	// Multithreading (MT) error description functions
	//----------------------------------------------------------------------------------------------

	/**
	 * @brief Retrieves a description for a thread error.
	 *
	 * This function returns a descriptive message corresponding to the given
	 * wxThreadError error code. If a description exists, it formats the message,
	 * and optionally logs the error.
	 *
	 * @param err The wxThreadError error code.
	 * @param processLog If true, the error message will be logged.
	 * @return A wxString containing the thread error description.
	 */
	wxString bwxGetThreadErrorDescription(const wxThreadError& err, bool processLog = false);
	
	/**
	 * @brief Retrieves a description for a semaphore error.
	 *
	 * This function returns a descriptive message corresponding to the given
	 * wxSemaError error code. If a description exists, it formats the message,
	 * and optionally logs the error.
	 *
	 * @param err The wxSemaError error code.
	 * @param processLog If true, the error message will be logged.
	 * @return A wxString containing the semaphore error description.
	 */
	wxString bwxGetSemaphoreErrorDescription(const wxSemaError& err, bool processLog = false);
	
	/**
	 * @brief Retrieves a description for a mutex error.
	 *
	 * This function returns a descriptive message corresponding to the given
	 * wxMutexError error code. If a description exists, it formats the message,
	 * and optionally logs the error.
	 *
	 * @param err The wxMutexError error code.
	 * @param processLog If true, the error message will be logged.
	 * @return A wxString containing the mutex error description.
	 */
	wxString bwxGetMutexErrorDescritpion(const wxMutexError& err, bool processLog = false);

}

#endif
