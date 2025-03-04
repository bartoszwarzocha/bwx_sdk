/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_math.h
// Purpose:     BWX_SDK Library; String processing utilities
// Author:      Bartosz Warzocha
// Created:     2018-02-27
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef _BWX_MATH_H_
#define _BWX_MATH_H_

#include <math.h>

#include <cstdlib>
#include <ctime>
#include <type_traits>

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

template <typename T>
constexpr T bwxToRadians(T degrees) {
    return degrees * static_cast<T>(bwxPI) / static_cast<T>(180);
}

template <typename T>
constexpr T bwxToDegrees(T radians) {
    return radians * static_cast<T>(180) / static_cast<T>(bwxPI);
}

bool bwxIsPower2(int x);

int bwxNextPower2(int x);

unsigned int bwxRoundUpToPower2(unsigned int x);

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
    } else {
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

int bwxNextMultiple(int m, int value);

double bwxNextMultiple(double m, double value);

float bwxIntermediate(float a, float b, float x);

template <typename T>
T bwxClamp(T value, T minVal, T maxVal) {
    return (value < minVal) ? minVal : (value > maxVal ? maxVal : value);
}

template <typename T>
T bwxLerp(T a, T b, float t) {
    return a + t * (b - a);
}

template <typename T>
T bwxRoundToMultiple(T value, T multiple) {
    return multiple * std::round(value / multiple);
}

float bwxDistance2D(float x1, float y1, float x2, float y2);

float bwxFastSqrt(float number);

template <typename T>
constexpr T bwxNormalize(T x, T minVal, T maxVal) {
    if (minVal == maxVal) return static_cast<T>(0);  // Avoid division by 0
    return (x - minVal) / (maxVal - minVal);
}
}  // namespace bwx_sdk

#endif
