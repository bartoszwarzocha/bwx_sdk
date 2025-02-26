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

/**
 * @file bwx_math.h
 * @brief Provides mathematical utilities for the BWXSDK library.
 */

namespace bwx_sdk {

#define bwxPI M_PI                           ///< Pi constant.
#define bwxHPI bwxPI / 2.0f                  ///< Half-pi constant.
#define bwxQPI bwxPI / 4.0f                  ///< Quarter-pi constant.
#define bwxDPI bwxPI * 2.0f                  ///< Double-pi constant.
#define bwxSPI std::powf((float)bwxPI, 2.0f) ///< Square-pi constant.
#define bwxCPI std::powf((float)bwxPI, 3.0f) ///< Cube-pi constant.
#define bwxSQRT2 std::sqrtf(2.0f)            ///< Square root of 2 constant.
#define bwxEpsilon 1e-6f                     ///< Epsilon value for floating-point comparisons.

	/**
	 * @brief Converts an angle from degrees to radians.
	 *
	 * @param dgrs Angle in degrees.
	 * @return Angle in radians.
	 */
	float bwxToRadians(float dgrs);
	
	/**
	 * @brief Converts an angle from radians to degrees.
	 *
	 * @param rads Angle in radians.
	 * @return Angle in degrees.
	 */
	float bwxToDegrees(float rads);

	/**
	 * @brief Checks if an integer is a power of 2.
	 *
	 * @param x The number to check.
	 * @return true if x is a power of 2; false otherwise.
	 */
	bool bwxIsPower2(int x);
	
	/**
	 * @brief Returns the next power of 2 greater than or equal to the given number.
	 *
	 * @param x The number for which to find the next power of 2.
	 * @return The smallest power of 2 that is greater than or equal to x.
	 */
	int bwxNextPower2(int x);

	/**
	 * @brief Generates a random integer within the inclusive range [min, max].
	 *
	 * @param min The minimum value.
	 * @param max The maximum value.
	 * @return A random integer between min and max.
	 */
	int bwxRand(int min, int max);

	/**
	 * @brief Generates a random double within the inclusive range [min, max].
	 *
	 * @param min The minimum value.
	 * @param max The maximum value.
	 * @return A random double between min and max.
	 */
	double bwxRand(double min, double max);

	//template <class T> T bwxRand(T min, T max);

	/**
	 * @brief Returns the smallest multiple of m that is greater than or equal to value.
	 *
	 * @param m The base multiplier.
	 * @param value The value to round up.
	 * @return The smallest multiple of m not less than value.
	 */
	int bwxNextMultiple(int m, int value);

	/**
	 * @brief Returns the smallest multiple of m that is greater than or equal to value.
	 *
	 * @param m The base multiplier.
	 * @param value The value to round up.
	 * @return The smallest multiple of m not less than value (for double values).
	 */
	double bwxNextMultiple(double m, double value);

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
	float bwxIntermediate(float a, float b, float x);

}

#endif
