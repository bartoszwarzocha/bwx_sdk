/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_string.cpp
// Purpose:     BWX_SDK Library; String processing utilities
// Author:      Bartosz Warzocha
// Created:     2018-02-06
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "bwx_string.h"

#include <string>
#include <locale>
#include <codecvt>

namespace bwx_sdk {
	namespace str {

		wxArrayString bwxSimpleExplode(const wxString& str, const wxString& separator)
		{
			wxArrayString result;
			if (separator.IsEmpty() || str.IsEmpty()) return result;

			size_t start = 0;
			int end = str.Find(separator);

			while (end != wxNOT_FOUND)
			{
				result.Add(str.Mid(start, end - start));
				start = end + separator.Length();
				end = str.find(separator, start);
			}

			if (start < str.Length()) result.Add(str.Mid(start));

			return result;
		}

		void bwxSimpleExplode(wxArrayString* result, const wxString& str, const wxString& separator)
		{
			if (!result || separator.IsEmpty() || str.IsEmpty()) return;

			result->Clear();
			size_t start = 0;
			int end = str.Find(separator);

			while (end != wxNOT_FOUND)
			{
				result->Add(str.Mid(start, end - start));
				start = end + separator.Length();
				end = str.find(separator, start);
			}

			if (start < str.Length()) result->Add(str.Mid(start));
		}

		wxString bwxSimpleJoin(const wxArrayString& array, const wxString& separator)
		{
			wxString result;
			for (size_t i = 0; i < array.GetCount(); ++i)
			{
				if (i > 0)
					result += separator;
				result += array[i];
			}
			return result;
		}

		wxString bwxSimpleJoin(const std::vector<std::string>& array, const wxString& separator)
		{
			wxString result;
			for (size_t i = 0; i < array.size(); ++i)
			{
				if (i > 0)
					result += separator;
				result += wxString::FromUTF8(array[i]);
			}
			return result;
		}

		wxString bwxTrim(const wxString& str, bool fromRight, bool fromLeft)
		{
			wxString result = str;
			if (fromLeft)  result.Trim(false);
			if (fromRight) result.Trim(true);
			return result;
		}

		wxString bwxReplaceAll(const wxString& str, const wxString& oldStr, const wxString& newStr)
		{
			wxString result = str;
			result.Replace(oldStr, newStr, true);
			return result;
		}

		wxString bwxReplaceAll(const wxString& str, const wxArrayString& oldStr, const wxArrayString& newStr)
		{
			wxString result = str;
			for (size_t i = 0; i < oldStr.GetCount(); i++)
			{
				result.Replace(oldStr[i], newStr[i], true);
			}
			return result;
		}

		wxString bwxRemoveChars(const wxString& str, const wxString& charsToRemove)
		{
			wxString result;
			for (wxUniChar ch : str)
			{
				if (!charsToRemove.Contains(ch))
				{
					result += ch;
				}
			}
			return result;
		}

		wxString bwxRemoveChars(const wxString& str, const wxArrayString& charsToRemove)
		{
			wxString result;
			for (wxUniChar ch : str)
			{
				bool found = false;
				for (const wxString& s : charsToRemove)
				{
					if (s.Contains(ch))
					{
						found = true;
						break;
					}
				}
				if (!found)
				{
					result += ch;
				}
			}
			return result;
		}

		wxString bwxReverse(const wxString& str)
		{
			wxString result;
			for (int i = str.Length() - 1; i >= 0; --i)
			{
				result += str[i];
			}
			return result;
		}

		wxString bwxToLowerCase(const wxString& str)
		{
			return str.Lower();
		}

		wxString bwxToUpperCase(const wxString& str)
		{
			return str.Upper();
		}

		bool bwxStartsWith(const wxString& str, const wxString& prefix)
		{
			return str.StartsWith(prefix);
		}

		bool bwxEndsWith(const wxString& str, const wxString& suffix)
		{
			return str.EndsWith(suffix);
		}

		wxString bwxFormatV(const char* format, va_list args)
		{
			wxString result;
			result.PrintfV(format, args);
			return result;
		}

		wxString bwxFormat(const char* format, ...)
		{
			va_list args;
			va_start(args, format);
			wxString result = bwxFormatV(format, args);
			va_end(args);
			return result;
		}

		std::string bwxFormatStd(const char* format, ...)
		{
			va_list args;
			va_start(args, format);
			std::string result;
			result = bwxFormat(format).ToStdString();
			va_end(args);
			return result;
		}

		std::wstring bwxStringToWstring(const std::string& str)
		{
			std::wstring_convert<std::codecvt_utf8_utf16<wchar_t>> converter;
			return converter.from_bytes(str);
		}

	} // namespace str
} // namespace bwx_sdk
