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
#include <cstdlib>
#include <ctime>
#include <type_traits>

#include "bwx_sdk/bwx_globals.h"

/**
 * @file bwx_math.h
 * @brief Provides mathematical utilities for the BWXSDK library.
 */

namespace bwx_sdk {
	namespace math {

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
		template <typename T>
		constexpr T bwxToRadians(T degrees)
		{
			return degrees * static_cast<T>(bwxPI) / static_cast<T>(180);
		}

		/**
		 * @brief Converts an angle from radians to degrees.
		 *
		 * @param rads Angle in radians.
		 * @return Angle in degrees.
		 */
		template <typename T>
		constexpr T bwxToDegrees(T radians)
		{
			return radians * static_cast<T>(180) / static_cast<T>(bwxPI);
		}

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
		* @brief Rounds up a number to the next power of 2.
		*
		* This function takes an unsigned integer and rounds it up to the next power of 2.
		* If the input number is already a power of 2, it returns the same number.
		*
		* @param x The number to round up.
		* @return The next power of 2 greater than or equal to x.
		*/
		unsigned int bwxRoundUpToPower2(unsigned int x);

		/**
		 * @brief Returns the previous power of 2 less than or equal to the given number.
		 *
		 * @param x The number for which to find the previous power of 2.
		 * @return The largest power of 2 that is less than or equal to x.
		 */
		template <typename T>
		T bwxRand(T min, T max) {
			static_assert(std::is_arithmetic<T>::value, "bwxRand requires a numeric type.");

			static bool initialized = false;
			if (!initialized) {
				std::srand(static_cast<unsigned int>(std::time(0)));
				initialized = true;
			}

			if (std::is_integral<T>::value) {
				return min + std::rand() % ((max - min) + 1);
			}
			else {
				return min + static_cast<T>(std::rand()) / (static_cast<T>(RAND_MAX / (max - min)));
			}
		}

		/*
		template <typename T>
		T bwxRandom(T minVal, T maxVal)
		{
			static std::random_device rd;
			static std::mt19937 gen(rd());

			if constexpr (std::is_integral_v<T>)
			{
				std::uniform_int_distribution<T> dist(minVal, maxVal);
				return dist(gen);
			}
			else
			{
				std::uniform_real_distribution<T> dist(minVal, maxVal);
				return dist(gen);
			}
		}
		*/

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

		/**
		 * @brief Clamps a value to a specified range.
		 *
		 * @param value The value to clamp.
		 * @param minVal The minimum value.
		 * @param maxVal The maximum value.
		 * @return The clamped value.
		 */
		template <typename T>
		T bwxClamp(T value, T minVal, T maxVal)
		{
			return (value < minVal) ? minVal : (value > maxVal ? maxVal : value);
		}

		/**
		 * @brief Linearly interpolates between two values.
		 *
		 * @param a The start value.
		 * @param b The end value.
		 * @param t The interpolation factor (0.0 to 1.0).
		 * @return The interpolated value.
		 */
		template <typename T>
		T bwxLerp(T a, T b, float t)
		{
			return a + t * (b - a);
		}

		/**
		 * @brief Rounds a value to the nearest multiple of another value.
		 *
		 * @param value The value to round.
		 * @param multiple The base multiplier.
		 * @return The rounded value.
		 */
		template <typename T>
		T bwxRoundToMultiple(T value, T multiple)
		{
			return multiple * std::round(value / multiple);
		}


		/**
		* @brief Calculates the Euclidean distance between two points in 2D space.
		*
		* This function computes the distance between two points (x1, y1) and (x2, y2)
		* using the Euclidean distance formula.
		*
		* @param x1 The x-coordinate of the first point.
		* @param y1 The y-coordinate of the first point.
		* @param x2 The x-coordinate of the second point.
		* @param y2 The y-coordinate of the second point.
		* @return The Euclidean distance between the two points.
		*/
		float bwxDistance2D(float x1, float y1, float x2, float y2);

		/**
		 * @brief Calculates the square root of a number using the fast inverse square root method.
		 *
		 * This function approximates the square root of a number using the fast inverse square root method.
		 * The method is based on the famous Quake III Arena algorithm.
		 *
		 * @param number The number to calculate the square root of.
		 * @return The square root of the number.
		 */
		float bwxFastSqrt(float number);

		/**
		 * @brief Normalizes a value to the range [0, 1] based on the given interval.
		 *
		 * This function normalizes a value x to the range [0, 1] based on the interval [minVal, maxVal].
		 * If minVal is equal to maxVal, the function returns 0 to avoid division by zero.
		 *
		 * @param x The value to normalize.
		 * @param minVal The minimum value of the interval.
		 * @param maxVal The maximum value of the interval.
		 * @return The normalized value in the range [0, 1].
		 */
		template <typename T>
		constexpr T bwxNormalize(T x, T minVal, T maxVal)
		{
			if (minVal == maxVal) return static_cast<T>(0); // Avoid division by 0
			return (x - minVal) / (maxVal - minVal);
		}

	}
}

#endif
