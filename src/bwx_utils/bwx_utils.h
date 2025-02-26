/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_utils.h
// Purpose:     BWXSDK
// Author:      Bartosz Warzocha
// Created:     2018-02-20
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_UTILS_H_
#define _BWX_UTILS_H_

#include "bwx_sdk/bwx_globals.h"

namespace bwx_sdk {

	/**
	 * @brief Mixes two colors based on a given factor.
	 *
	 * This function returns a new color which is the result of mixing the two given colors
	 * using the provided factor. The factor specifies the influence of the second color on
	 * the resulting color. A factor of 0.0 yields the first color, while a factor of 1.0 yields
	 * the second color.
	 *
	 * @param col1 The first color to mix.
	 * @param col2 The second color to mix.
	 * @param factor The mixing factor in the range [0.0, 1.0]. Values outside this range may yield unexpected results.
	 * @return A new wxColour resulting from the mix of col1 and col2.
	 *
	 * @example
	 * @code
	 * wxColour colour1(255, 0, 0);  // Red
	 * wxColour colour2(0, 0, 255);  // Blue
	 * double factor = 0.5;          // Equal mix
	 * wxColour mixedColour = bwxMixColours(colour1, colour2, factor);
	 * // mixedColour will be purple (127, 0, 127)
	 * @endcode
	 */
	wxColour bwxMixColours(const wxColour& col1, const wxColour& col2, double factor);

}

#endif
