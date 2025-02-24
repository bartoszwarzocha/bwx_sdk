/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_math.h
// Purpose:     BWX_SDK Library; String processing utilities
// Author:      Bartosz Warzocha
// Created:     2018-02-27
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_MATH_H_
#define _BWX_MATH_H_

#include <math.h>

#include "bwx_sdk/bwx_globals.h"

namespace bwx_sdk {

#define bwxPI M_PI
#define bwxHPI bwxPI / 2.0f
#define bwxQPI bwxPI / 4.0f
#define bwxDPI bwxPI * 2.0f
#define bwxSPI std::powf((float)bwxPI, 2.0f)
#define bwxCPI std::powf((float)bwxPI, 3.0f)
#define bwxSQRT2 std::sqrtf(2.0f)
#define bwxEpsilon 1e-6f

	float BWX_EXPORT bwxToRadians(float dgrs);
	float BWX_EXPORT bwxToDegrees(float rads);

	bool BWX_EXPORT bwxIsPower2(int x);
	int BWX_EXPORT bwxNextPower2(int x);

	int BWX_EXPORT bwxRand(int min, int max);
	double BWX_EXPORT bwxRand(double min, double max);
	//template <class T> T bwxRand(T min, T max);

	int BWX_EXPORT bwxNextMultiple(int m, int value);
	double BWX_EXPORT bwxNextMultiple(double m, double value);

	float BWX_EXPORT bwxIntermediate(float a, float b, float x);

}

#endif
