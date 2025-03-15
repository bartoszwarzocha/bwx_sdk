/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_string.h
// Purpose:     BWX_SDK Library; String processing utilities
// Author:      Bartosz Warzocha
// Created:     2018-02-06
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef _BWX_STRING_H__
#define _BWX_STRING_H__

#include "bwx_sdk/bwx_globals.h"

namespace bwx_sdk {
namespace str {

wxArrayString bwxSimpleExplode(const wxString& s, const wxString& separator);

void bwxSimpleExplode(wxArrayString* a, const wxString& str, const wxString& separator);

wxString bwxSimpleJoin(const wxArrayString& array, const wxString& separator = "\n");

wxString bwxSimpleJoin(const std::vector<std::string>& array, const wxString& separator = "\n");

wxString bwxTrim(const wxString& str, bool fromRight = true, bool fromLeft = true);

wxString bwxReplaceAll(const wxString& str, const wxString& oldStr, const wxString& newStr);

wxString bwxReplaceAll(const wxString& str, const wxArrayString& oldStr, const wxArrayString& newStr);

wxString bwxRemoveChars(const wxString& str, const wxString& charsToRemove);

wxString bwxRemoveChars(const wxString& str, const wxArrayString& charsToRemove);

wxString bwxReverse(const wxString& str);

wxString bwxToLowerCase(const wxString& str);

wxString bwxToUpperCase(const wxString& str);

bool bwxStartsWith(const wxString& str, const wxString& prefix);

bool bwxEndsWith(const wxString& str, const wxString& suffix);

wxString bwxFormat(const char* format, ...);

wxString bwxFormat(const wxString& format, ...);

template <typename T>
wxString bwxToString(T number) {
    if constexpr (std::is_integral_v<T>) {
        return wxString::Format("%lld", static_cast<long long>(number));
    } else if constexpr (std::is_floating_point_v<T>) {
        return wxString::Format("%g", static_cast<double>(number));
    } else {
        static_assert(std::is_arithmetic_v<T>, "bwxToString supports only numeric types");
        return "";
    }
}

template <typename T>
T bwxFromString(const wxString& str) {
    if constexpr (std::is_integral_v<T>) {
        long long value = 0;
        if (str.ToLongLong(&value)) return static_cast<T>(value);
    } else if constexpr (std::is_floating_point_v<T>) {
        double value = 0.0;
        if (str.ToDouble(&value)) return static_cast<T>(value);
    }

    return static_cast<T>(0);  // 0 if error
}

std::wstring bwxStringToWstring(const std::string& str);

}  // namespace str
}  // namespace bwx_sdk

#endif
