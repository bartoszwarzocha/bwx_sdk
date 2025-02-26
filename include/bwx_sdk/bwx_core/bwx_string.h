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

/**
 * @file bwx_string.h
 * @brief Provides string processing utilities for the BWXSDK library.
 */

namespace bwx_sdk {

	/**
	 * @brief Splits a string into parts using a specified separator.
	 *
	 * This function splits the given string based on the provided separator and returns
	 * the result as a wxArrayString.
	 *
	 * @param str The string to be split.
	 * @param separator The separator that defines the split points.
	 * @return wxArrayString containing the parts resulting from the split.
	 *
	 * @example
	 * @code
	 * wxString text = "one,two,three";
	 * wxString sep = ",";
	 * wxArrayString result = bwxSimpleExplode(text, sep);
	 * // result contains ["one", "two", "three"]
	 * @endcode
	 */
	wxArrayString bwxSimpleExplode(const wxString& s, const wxString& separator);

	/**
	 * @brief Splits a string into parts using a specified separator.
	 *
	 * This function splits the given string based on the provided separator and returns
	 * the result as a wxArrayString.
	 *
	 * @param a Pointer to the wxArrayString that will store the result.
	 * @param str The string to be split.
	 * @param separator The separator that defines the split points.
	 *
	 * @example
	 * @code
	 * wxArrayString a;
	 * wxString text = "one,two,three";
	 * wxString sep = ",";
	 * bwxSimpleExplode(&a, text, sep);
	 * // a contains ["one", "two", "three"]
	 * @endcode
	 */
	void bwxSimpleExplode(wxArrayString* a, const wxString& str, const wxString& separator);

}

#endif
