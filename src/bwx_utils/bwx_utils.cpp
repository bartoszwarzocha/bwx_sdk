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

#include <bwx_sdk/bwx_core/bwx_math.h>

#include <bwx_sdk/bwx_utils/bwx_utils.h>

namespace bwx_sdk {
	namespace utils {

		//----------------------------------------------------------------------------------------------
		// Colour functions
		//----------------------------------------------------------------------------------------------
		wxColour bwxGetRandomColour()
		{
			//  return wxColour(R, G, B);
			return wxColour(math::bwxRand<int>(0, 255), math::bwxRand<int>(0, 255), math::bwxRand<int>(0, 255));
		}

		void bwxGetRandomColours(wxVector<wxColour>* v, int howMany, bool unique, const wxColour& first)
		{
			bool exists = false;
			wxColour tmp = first;

			for (int i = 0; i < howMany; i++)
			{
				if (i != 0)
				{
					tmp = bwxGetRandomColour();
				}

				if (!unique)
				{
					v->push_back(tmp);
				}
				else
				{
					if (i == 0)
					{
						v->push_back(tmp);
					}
					else
					{
						exists = false;
						for (int j = 0; j < (int)v->size(); j++)
						{
							if (v->operator[](j) == tmp)
							{
								exists = true;
								break;
							}
						}

						if (exists)
						{
							i -= 1;
							continue;
						}
						else
						{
							v->push_back(tmp);
						}
					}
				}
			}
		}

		wxColour bwxMixColours(const wxColour& col1, const wxColour& col2, double factor)
		{
			return wxColour(
				(col2.Red() * factor) + (col1.Red() * (1 - factor)),
				(col2.Green() * factor) + (col1.Green() * (1 - factor)),
				(col2.Blue() * factor) + (col1.Blue() * (1 - factor))
			);
		}

	}
}
