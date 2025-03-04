/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_string.h
// Purpose:     BWX_SDK Library; String processing utilities
// Author:      Bartosz Warzocha
// Created:     2018-02-06
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_STRING_H__
#define _BWX_STRING_H__

#include "bwx_sdk/bwx_globals.h"

/**
 * @file bwx_string.h
 * @brief Provides string processing utilities for the BWXSDK library.
 */

namespace bwx_sdk {
	namespace str {

		/**
		 * @brief Splits a string into parts using a specified separator.
		 *
		 * This function splits the given string based on the provided separator and returns
		 * the result as a wxArrayString.
		 *
		 * @param str The string to be split.
		 * @param separator The separator that defines the split points.
		 * @return wxArrayString containing the parts resulting from the split.
		 *
		 * @example
		 * @code
		 * wxString text = "one,two,three";
		 * wxString sep = ",";
		 * wxArrayString result = bwxSimpleExplode(text, sep);
		 * // result contains ["one", "two", "three"]
		 * @endcode
		 */
		wxArrayString bwxSimpleExplode(const wxString& s, const wxString& separator);

		/**
		 * @brief Splits a string into parts using a specified separator.
		 *
		 * This function splits the given string based on the provided separator and returns
		 * the result as a wxArrayString.
		 *
		 * @param a Pointer to the wxArrayString that will store the result.
		 * @param str The string to be split.
		 * @param separator The separator that defines the split points.
		 *
		 * @example
		 * @code
		 * wxArrayString a;
		 * wxString text = "one,two,three";
		 * wxString sep = ",";
		 * bwxSimpleExplode(&a, text, sep);
		 * // a contains ["one", "two", "three"]
		 * @endcode
		 */
		void bwxSimpleExplode(wxArrayString* a, const wxString& str, const wxString& separator);

		/**
		* @brief Joins an array of strings into a single string using a specified separator.
		*
		* @param array The array of strings to be joined.
		* @param separator The separator that will be inserted between the strings.
		* @return The resulting string.
		*/
		wxString bwxSimpleJoin(const wxArrayString& array, const wxString& separator);

		/**
		 * @brief Trims the whitespace from the beginning and/or end of a string.
		 *
		 * @param str The string to be trimmed.
		 * @param fromRight If true, the whitespace from the right end of the string will be removed.
		 * @param fromLeft If true, the whitespace from the left end of the string will be removed.
		 * @return The trimmed string.
		 */
		wxString bwxTrim(const wxString& str, bool fromRight = true, bool fromLeft = true);

		/**
		 * @brief Replaces all occurrences of a substring in a string with another substring.
		 *
		 * @param str The string in which the replacement will be made.
		 * @param oldStr The substring to be replaced.
		 * @param newStr The substring that will replace the old substring.
		 * @return The resulting string.
		 */
		wxString bwxReplaceAll(const wxString& str, const wxString& oldStr, const wxString& newStr);

		/**
		 * @brief Replaces all occurrences of multiple substrings in a string with other substrings.
		 *
		 * This function replaces all occurrences of substrings in the oldStr array with the corresponding
		 * substrings in the newStr array.
		 *
		 * @param str The string in which the replacements will be made.
		 * @param oldStr The array of substrings to be replaced.
		 * @param newStr The array of substrings that will replace the old substrings.
		 * @return The resulting string.
		 */
		wxString bwxReplaceAll(const wxString& str, const wxArrayString& oldStr, const wxArrayString& newStr);

		/**
		 * @brief Removes all occurrences of specified characters from a string.
		 *
		 * @param str The string from which the characters will be removed.
		 * @param charsToRemove The characters that will be removed.
		 * @return The resulting string.
		 */
		wxString bwxRemoveChars(const wxString& str, const wxString& charsToRemove);

		/**
		 * @brief Removes all occurrences of specified characters from a string.
		 *
		 * @param str The string from which the characters will be removed.
		 * @param charsToRemove The characters that will be removed.
		 * @return The resulting string.
		 */
		wxString bwxRemoveChars(const wxString& str, const wxArrayString& charsToRemove);

		/**
		 * @brief Reverses the order of characters in a string.
		 *
		 * @param str The string to be reversed.
		 * @return The reversed string.
		 */
		wxString bwxReverse(const wxString& str);

		/**
		 * @brief Converts a string to lowercase.
		 *
		 * @param str The string to be converted.
		 * @return The resulting string.
		 */
		wxString bwxToLowerCase(const wxString& str);

		/**
		 * @brief Converts a string to uppercase.
		 *
		 * @param str The string to be converted.
		 * @return The resulting string.
		 */
		wxString bwxToUpperCase(const wxString& str);

		/**
		 * @brief Checks if a string starts with a specified prefix.
		 *
		 * @param str The string to be checked.
		 * @param prefix The prefix to be checked for.
		 * @return True if the string starts with the prefix, false otherwise.
		 */
		bool bwxStartsWith(const wxString& str, const wxString& prefix);

		/**
		 * @brief Checks if a string ends with a specified suffix.
		 *
		 * @param str The string to be checked.
		 * @param suffix The suffix to be checked for.
		 * @return True if the string ends with the suffix, false otherwise.
		 */
		bool bwxEndsWith(const wxString& str, const wxString& suffix);

		/**
		 * @brief Formats a string using printf-style formatting.
		 *
		 * This function is a wrapper around wxString::PrintfV that allows for printf-style formatting
		 * of strings.
		 *
		 * @param format The format string.
		 * @param ... The arguments to be formatted.
		 * @return The formatted string.
		 */
		wxString bwxFormat(const char* format, ...);

		/**
		 * @brief Formats a string using printf-style formatting.
		 *
		 * This function is a wrapper around wxString::PrintfV that allows for printf-style formatting
		 * of strings.
		 *
		 * @param format The format string.
		 * @param ... The arguments to be formatted.
		 * @return The formatted string.
		 */
		wxString bwxFormat(const wxString& format, ...);

		/**
		 * @brief Converts a numeric value to a string.
		 *
		 * This function converts a numeric value to a string using wxString::Format.
		 *
		 * @param number The numeric value to be converted.
		 * @return The resulting string.
		 */
		template <typename T>
		wxString bwxToString(T number)
		{
			if constexpr (std::is_integral_v<T>)
			{
				return wxString::Format("%lld", static_cast<long long>(number));
			}
			else if constexpr (std::is_floating_point_v<T>)
			{
				return wxString::Format("%g", static_cast<double>(number));
			}
			else
			{
				static_assert(std::is_arithmetic_v<T>, "bwxToString supports only numeric types");
				return "";
			}
		}

		/**
		 * @brief Converts a string to a numeric value.
		 *
		 * This function converts a string to a numeric value using wxString::ToLongLong or wxString::ToDouble.
		 *
		 * @param str The string to be converted.
		 * @return The resulting numeric value.
		 */
		template <typename T>
		T bwxFromString(const wxString& str)
		{
			if constexpr (std::is_integral_v<T>)
			{
				long long value = 0;
				if (str.ToLongLong(&value))
					return static_cast<T>(value);
			}
			else if constexpr (std::is_floating_point_v<T>)
			{
				double value = 0.0;
				if (str.ToDouble(&value))
					return static_cast<T>(value);
			}

			return static_cast<T>(0); // 0 if error
		}

	}
}

#endif
