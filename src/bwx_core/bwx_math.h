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

#define bwxPI M_PI ///< Pi constant.
#define bwxHPI bwxPI / 2.0f ///< Half-pi constant.
#define bwxQPI bwxPI / 4.0f ///< Quarter-pi constant.
#define bwxDPI bwxPI * 2.0f ///< Double-pi constant.
#define bwxSPI std::powf((float)bwxPI, 2.0f) ///< Square-pi constant.
#define bwxCPI std::powf((float)bwxPI, 3.0f) ///< Cube-pi constant.
#define bwxSQRT2 std::sqrtf(2.0f) ///< Square root of 2 constant.
#define bwxEpsilon 1e-6f ///< Epsilon value for floating-point comparisons.

	/**
	* @brief Converts degrees to radians.
	*/
	float bwxToRadians(float dgrs);
	
	/**
	* @brief Converts radians to degrees.
	*/
	float bwxToDegrees(float rads);

	/**
	* @brief Check if value is a power of 2.
	*/
	bool bwxIsPower2(int x);
	
	/**
	* @brief Rounds a value to the nearest power of 2.
	*/
	int bwxNextPower2(int x);

	/**
	* @brief Generates a random integer in the range [min, max].
	*/
	int bwxRand(int min, int max);

	/**
	* @brief Generates a random double in the range [min, max].
	*/
	double bwxRand(double min, double max);

	//template <class T> T bwxRand(T min, T max);

	/**
	* @brief Clamps a value to the specified range.
	*/
	int bwxNextMultiple(int m, int value);

	/**
	* @brief Clamps a value to the specified range.
	*/
	double bwxNextMultiple(double m, double value);

	/**
	* @brief Clamps a value to the specified range.
	*/
	float bwxIntermediate(float a, float b, float x);

}

#endif
