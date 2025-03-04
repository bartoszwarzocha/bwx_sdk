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
	namespace math {

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

		unsigned int bwxRoundUpToPower2(unsigned int x)
		{
			--x;
			x |= x >> 1;
			x |= x >> 2;
			x |= x >> 4;
			x |= x >> 8;
			x |= x >> 16;
			return ++x;
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

		float bwxDistance2D(float x1, float y1, float x2, float y2)
		{
			float dx = x2 - x1;
			float dy = y2 - y1;
			return std::sqrt(dx * dx + dy * dy);
		}

		float bwxFastSqrt(float number)
		{
			long i;
			float x, y;
			const float threehalfs = 1.5F;

			x = number * 0.5F;
			y = number;
			i = *(long*)&y;
			i = 0x5f3759df - (i >> 1);
			y = *(float*)&i;
			y = y * (threehalfs - (x * y * y));

			return 1.0f / y;
		}

	} // namespace math
} // namespace bwx_sdk
