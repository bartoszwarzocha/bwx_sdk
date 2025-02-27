/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_math.cpp
// Purpose:     BWX_SDK Library; Mathematical functions and classes
// Author:      Bartosz Warzocha
// Created:     2018-02-27
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

/**
 * @file bwx_math.cpp
 * @brief Implements basic mathematical functions used within the BWX_SDK Library.
 */

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <random>
#include "bwx_math.h"

namespace bwx_sdk {

	float bwxToRadians(float dgrs)
	{
		return (dgrs * (float)bwxPI) / 180.0f;
	}

	float bwxToDegrees(float rads)
	{
		return (rads * 180.0f) / (float)bwxPI;
	}

	bool bwxIsPower2(int x)
	{
		return ((x > 0) && ((x & (x - 1)) == 0));
	}

	int bwxNextPower2(int x)
	{
		int i = x & (~x + 1);
		while (i < x)
		{
			i <<= 1;
		}
		return i;
	}

	int bwxNextMultiple(int m, int value)
	{
		return m * ((value + (m - 1)) / m);
	}

	double bwxNextMultiple(double m, double value)
	{
		return m * ((value + (m - 1)) / m);
	}

	float bwxIntermediate(float a, float b, float x)
	{
		if (x < a)
		{
			return 0.0f;
		}
		else if (x >= b)
		{
			return 1.0f;
		}
		else
		{
			x = (x - a) / (b - a);
			return x * x * (3.0f - 2.0f * x);
		}
	}

}
