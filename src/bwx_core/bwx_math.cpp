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

	/**
	 * @brief Converts an angle from degrees to radians.
	 *
	 * @param dgrs Angle in degrees.
	 * @return Angle in radians.
	 */
	float bwxToRadians(float dgrs)
	{
		return (dgrs * (float)bwxPI) / 180.0f;
	}

	/**
	 * @brief Converts an angle from radians to degrees.
	 *
	 * @param rads Angle in radians.
	 * @return Angle in degrees.
	 */
	float bwxToDegrees(float rads)
	{
		return (rads * 180.0f) / (float)bwxPI;
	}

	/**
	 * @brief Checks if an integer is a power of 2.
	 *
	 * @param x The number to check.
	 * @return true if x is a power of 2; false otherwise.
	 */
	bool bwxIsPower2(int x)
	{
		return ((x > 0) && ((x & (x - 1)) == 0));
	}

	/**
	 * @brief Returns the next power of 2 greater than or equal to the given number.
	 *
	 * @param x The number for which to find the next power of 2.
	 * @return The smallest power of 2 that is greater than or equal to x.
	 */
	int bwxNextPower2(int x)
	{
		int i = x & (~x + 1);
		while (i < x)
		{
			i <<= 1;
		}
		return i;
	}

	/**
	 * @brief Generates a random integer within the inclusive range [min, max].
	 *
	 * @param min The minimum value.
	 * @param max The maximum value.
	 * @return A random integer between min and max.
	 */
	int bwxRand(int min, int max)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_int_distribution<> dis(min, max);
		return dis(gen);
	}

	/**
	 * @brief Generates a random double within the inclusive range [min, max].
	 *
	 * @param min The minimum value.
	 * @param max The maximum value.
	 * @return A random double between min and max.
	 */
	double bwxRand(double min, double max)
	{
		std::random_device rd;
		std::mt19937 gen(rd());
		std::uniform_real_distribution<> dis(min, max);
		return dis(gen);
	}

	/**
	 * @brief Returns the smallest multiple of m that is greater than or equal to value.
	 *
	 * @param m The base multiplier.
	 * @param value The value to round up.
	 * @return The smallest multiple of m not less than value.
	 */
	int bwxNextMultiple(int m, int value)
	{
		return m * ((value + (m - 1)) / m);
	}

	/**
	 * @brief Returns the smallest multiple of m that is greater than or equal to value.
	 *
	 * @param m The base multiplier.
	 * @param value The value to round up.
	 * @return The smallest multiple of m not less than value (for double values).
	 */
	double bwxNextMultiple(double m, double value)
	{
		return m * ((value + (m - 1)) / m);
	}

	/**
	 * @brief Returns a normalized value in the range [0, 1] based on x within the interval [a, b].
	 *
	 * This function performs a smooth interpolation (using a smoothstep function)
	 * where the result is 0.0 if x is less than a, 1.0 if x is greater than or equal to b,
	 * and smoothly interpolates between 0 and 1 for values between a and b.
	 *
	 * @param a The start of the interval.
	 * @param b The end of the interval.
	 * @param x The value to interpolate.
	 * @return A normalized value between 0.0 and 1.0.
	 */
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
