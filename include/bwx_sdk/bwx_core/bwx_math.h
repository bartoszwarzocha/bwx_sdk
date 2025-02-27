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

float bwxToRadians(float dgrs);

float bwxToDegrees(float rads);

bool bwxIsPower2(int x);

int bwxNextPower2(int x);

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

int bwxNextMultiple(int m, int value);

double bwxNextMultiple(double m, double value);

float bwxIntermediate(float a, float b, float x);

}  // namespace bwx_sdk

#endif
