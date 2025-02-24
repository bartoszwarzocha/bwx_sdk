/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_string.cpp
// Purpose:     BWX_SDK Library; String processing utilities
// Author:      Bartosz Warzocha
// Created:     2018-02-06
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

/**
 * @file bwx_string.cpp
 * @brief Implements basic string functions used within the BWX_SDK Library.
 */

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "bwx_string.h"

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
	wxArrayString bwxSimpleExplode(const wxString& str, const wxString& separator)
	{
		wxArrayString a;
		a.Clear();

		wxString s = str + separator; // Append separator at the end
		int l = s.Length();
		wxString temp;

		for (int i = 0; i < l; ++i)
		{
			if (s[i] != separator[0]) // Compare directly with the first character of the separator
			{
				temp += s[i];
			}
			else
			{
				if (!temp.IsEmpty())
				{
					a.Add(temp);
					temp.Clear();
				}
			}
		}

		return a;
	}

	/**
	 * @brief Splits a string into parts using a specified separator and stores the results in a provided wxArrayString.
	 *
	 * This function splits the given string based on the provided separator and appends the resulting parts
	 * to the wxArrayString pointed to by the provided pointer.
	 *
	 * @param a Pointer to the wxArrayString that will hold the split parts.
	 * @param str The string to be split.
	 * @param separator The separator that defines the split points.
	 *
	 * @example
	 * @code
	 * wxString text = "one,two,three";
	 * wxString sep = ",";
	 * wxArrayString result;
	 * bwxSimpleExplode(&result, text, sep);
	 * // result contains ["one", "two", "three"]
	 * @endcode
	 */
	void bwxSimpleExplode(wxArrayString* a, const wxString& str, const wxString& separator)
	{
		a->Clear();

		wxString s = str + separator; // Append separator at the end
		int l = s.Length();
		wxString temp;

		for (int i = 0; i < l; ++i)
		{
			if (s[i] != separator[0]) // Compare directly with the first character of the separator
			{
				temp += s[i];
			}
			else
			{
				if (!temp.IsEmpty())
				{
					a->Add(temp);
					temp.Clear();
				}
			}
		}
	}

}
