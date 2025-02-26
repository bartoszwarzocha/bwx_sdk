/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_string.cpp
// Purpose:     BWX_SDK Library; String processing utilities
// Author:      Bartosz Warzocha
// Created:     2018-02-06
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "bwx_string.h"

namespace bwx_sdk {

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
