/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_datetime.cpp
// Purpose:     BWX_SDK Library; Daye & Time operations
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-03-04
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <bwx_sdk/bwx_core/bwx_datetime.h>
#include <bwx_sdk/bwx_core/bwx_math.h>

namespace bwx_sdk {
    namespace dt {

        int bwxGetWeekDay(int d, int m, int y, int calendar)
        {
            int Y = y - (m < 3);
            int C = Y / 100, D = Y % 100;
            int M = (m + 9) % 12 + 1;

            int N = (d + (13 * M - 1) / 5 + D + D / 4 + C / 4 + 5 * C) % 7;
            if (calendar == 0) N = (N + 6) % 7;

            return (N == 0) ? 7 : N;
        }

        // Old implementation
        /*
        int bwxGetWeekDay(int d, int m, int y, int calendar)
        {
            int Y, C, M, N, D;
            M = 1 + (9 + m) % 12;
            Y = y - (M > 10); C = Y / 100; D = Y % 100;
            if (calendar != 0) N = ((13 * M - 1) / 5 + D + D / 4 + C / 4 + 5 * C + d) % 7;
            else N = ((13 * M - 1) / 5 + D + D / 4 + 6 * C + d + 5) % 7;
            int result = (7 + N) % 7;
            if (result == 0) result = 7;
            return result;
        }
        */

        int bwxGetWeekDay(wxDateTime date, int calendar = 1)
        {
            return bwxGetWeekDay(date.GetDay(), bwxDT2IntMonth(date.GetMonth()), date.GetYear(), calendar);
        }

        int bwxGetWeekNumber(int d, int m, int y, int wd)
        {
            int a[12] = { 0, 31, 59, 90, 120, 151, 181, 212, 243, 273, 304, 334 };
            int b[12] = { 0, 31, 60, 91, 121, 152, 182, 213, 244, 274, 305, 335 };

            int y_days = (int)wxDateTime::GetNumberOfDays(y);
            if (y_days == 365) d = d + a[m - 1];
            else if (y_days == 366) d = d + b[m - 1];
            else d = d + a[m - 1];
            int x = d - wd + 10;
            double z = x / 7;
            double result = 0;
            modf(z, &result);
            return (int)result;
        }

        int bwxGetWeekNumber(wxDateTime date, int wd)
        {
            return bwxGetWeekNumber(date.GetDay(), bwxDT2IntMonth(date.GetMonth()), date.GetYear(), wd);
        }

        int bwxGetWeekNumber(wxDateTime date)
        {
            return date.GetWeekOfYear();
        }

        wxDateSpan bwxGetDateDiff(wxDateTime d1, wxDateTime d2)
        {
            return d2.DiffAsDateSpan(d1);
        }

        // Old implementation
        /*
        wxDateSpan bwxGetDateDiff(wxDateTime d1, wxDateTime d2)
        {
            wxDateSpan s1;
            s1.SetYears(d1.GetYear());
            s1.SetMonths(d1.GetMonth());
            s1.SetDays(d1.GetDay());

            wxDateSpan s2;
            s2.SetYears(d2.GetYear());
            s2.SetMonths(d2.GetMonth());
            s2.SetDays(d2.GetDay());

            wxDateSpan span = s2 - s1;
            int y = span.GetYears();
            int m = span.GetMonths();
            int d = span.GetDays();

            if (d < 0)
            {
                m = m - 1;
                d = d + (int)(d1.GetLastMonthDay(d1.GetMonth(), d1.GetYear()).GetDay());
            }

            if (m < 0)
            {
                y = y - 1;
                m = m + 12;
            }

            wxDateSpan result(y, m, 0, d);
            return result;
        }
        */

        wxDateTime bwxAddToDate(wxDateTime date, int y, int m, int d)
        {
            wxDateSpan span;
            span.SetYears(y);
            span.SetMonths(m);
            span.SetDays(d);

            return date.Add(span);
        }

        wxDateTime bwxSubtractFromDate(wxDateTime date, int y, int m, int d)
        {
            wxDateSpan span;
            span.SetYears(y);
            span.SetMonths(m);
            span.SetDays(d);

            return date.Subtract(span);
        }

        wxTimeSpan bwxGetTimeDiff(wxDateTime t1, wxDateTime t2)
        {
            return t2 - t1;
        }

        // Old implementation
        /*
        wxTimeSpan bwxGetTimeDiff(wxDateTime t1, wxDateTime t2)
        {
            wxTimeSpan s1(t1.GetHour(), t1.GetMinute(), t1.GetSecond(), 0);
            wxTimeSpan s2(t2.GetHour(), t2.GetMinute(), t2.GetSecond(), 0);

            wxTimeSpan h24(24, 0, 0, 0);
            if (s1.IsEqualTo(s2)) s1.Subtract(h24);
            else if (s1.IsLongerThan(s2)) s1.Subtract(h24);

            wxTimeSpan span = s2 - s1;
            return span;
        }
        */

        int bwxDateSpanCompare(wxDateSpan s1, wxDateSpan s2, bool s1_conv, bool s2_conv)
        {
            int y_1 = s1.GetYears();
            int m_1 = s1.GetMonths();
            int d_1 = s1.GetDays();

            int y_2 = s2.GetYears();
            int m_2 = s2.GetMonths();
            int d_2 = s2.GetDays();

            if (s1_conv) {
                if (d_1 < 0) { m_1 = m_1 - 1; d_1 = d_1 + 30; }
                if (m_1 < 0) { y_1 = y_1 - 1; m_1 = m_1 + 12; }
            }
            if (s2_conv) {
                if (d_2 < 0) { m_2 = m_2 - 1;    d_2 = d_2 + 30; }
                if (m_2 < 0) { y_2 = y_2 - 1;    m_2 = m_2 + 12; }
            }

            if ((y_1 > y_2) || (y_1 == y_2 && m_1 > m_2) || (y_1 == y_2 && m_1 == m_2 && d_1 > d_2)) return 1;
            else if (y_1 == y_2 && m_1 == m_2 && d_1 == d_2) return 0;
            else return -1;
        }

        wxDateTime::Month bwxInt2DTMonth(int month)
        {
            if (month < 1 || month > 12) return wxDateTime::Inv_Month;
            else return static_cast<wxDateTime::Month>(month - 1);
        }

        int bwxDT2IntMonth(wxDateTime::Month month)
        {
            return static_cast<int>(month) + 1;
        }

        wxString bwxGetMonthName(int month, bool short_name, BWX_CASES c)
        {
            wxString mth = _ES_;

            if (c == BWX_NOMINATIVE)
            {
                switch (month)
                {
                case 1: mth = _("January"); break;
                case 2: mth = _("February"); break;
                case 3: mth = _("March"); break;
                case 4: mth = _("April"); break;
                case 5: mth = _("May"); break;
                case 6: mth = _("June"); break;
                case 7: mth = _("July"); break;
                case 8: mth = _("August"); break;
                case 9: mth = _("September"); break;
                case 10: mth = _("October"); break;
                case 11: mth = _("November"); break;
                case 12: mth = _("December"); break;
                }
            }
            else
            {
                switch (month)
                {
                case 1: mth = _("of January"); break;
                case 2: mth = _("of February"); break;
                case 3: mth = _("of March"); break;
                case 4: mth = _("of April"); break;
                case 5: mth = _("of May"); break;
                case 6: mth = _("of June"); break;
                case 7: mth = _("of July"); break;
                case 8: mth = _("of August"); break;
                case 9: mth = _("of September"); break;
                case 10: mth = _("of October"); break;
                case 11: mth = _("of November"); break;
                case 12: mth = _("of December"); break;
                }
            }

            if (short_name)
            {
                mth = mth.SubString(0, 2) + ".";
            }

            return mth;
        }

        wxString bwxGetWeekDayName(int week_day, bool short_name)
        {
            wxString wd = _ES_;
            switch (week_day)
            {
            case 1: (short_name) ? (wd = _("Mon")) : (wd = _("Monday")); break;
            case 2: (short_name) ? (wd = _("Tue")) : (wd = _("Tuesday")); break;
            case 3: (short_name) ? (wd = _("Wed")) : (wd = _("Wednesday")); break;
            case 4: (short_name) ? (wd = _("Thu")) : (wd = _("Thursday")); break;
            case 5: (short_name) ? (wd = _("Fri")) : (wd = _("Friday")); break;
            case 6: (short_name) ? (wd = _("Sat")) : (wd = _("Saturday")); break;
            case 7: (short_name) ? (wd = _("Sun")) : (wd = _("Sunday")); break;
            default: wd = ""; break;
            }

            return wd;
        }

        wxDateTime::WeekDay bwxInt2DTWeekDay(int wd)
        {
            switch (wd)
            {
            case  1: return wxDateTime::Mon; break;
            case  2: return wxDateTime::Tue; break;
            case  3: return wxDateTime::Wed; break;
            case  4: return wxDateTime::Thu; break;
            case  5: return wxDateTime::Fri; break;
            case  6: return wxDateTime::Sat; break;
            case  7: return wxDateTime::Sun; break;
            default: return wxDateTime::Inv_WeekDay; break;
            }
        }

        int bwxZodiac(wxDateTime date)
        {
            int dates[13][4] = {
                { 22, 12, 31, 12 }, // (1) for Capricorn at the end of the year
                { 1, 01, 19, 1 },   // (1) for Capricorn at the beginning of the year
                { 20, 1, 18, 2 },   // (2) Aquarius
                { 19, 2, 20, 3 },   // (3) Pisces
                { 21, 3, 19, 4 },   // (4) Aries
                { 20, 4, 22, 5 },   // (5) Taurus
                { 23, 5, 21, 6 },   // (6) Gemini
                { 22, 6, 22, 7 },   // (7) Cancer
                { 23, 7, 23, 8 },   // (8) Leo
                { 24, 8, 22, 9 },   // (9) Virgo
                { 23, 9, 22, 10 },  // (10) Libra
                { 23, 10, 21, 11 }, // (11) Scorpio
                { 22, 11, 21, 12 }  // (12) Sagittarius
            };

            int year = date.GetYear();

            int zodiac = -1;
            wxDateTime date1 = wxDateTime::Today();
            wxDateTime date2 = wxDateTime::Today();

            for (int i = 0; i < 13; i++)
            {
                date1.Set(dates[i][0], bwxInt2DTMonth(dates[i][1]), year);
                date2.Set(dates[i][2], bwxInt2DTMonth(dates[i][3]), year);
                if (date.IsBetween(date1, date2))
                {
                    zodiac = i;
                    break;
                }
            }

            if (zodiac == 0) zodiac = 1;
            return zodiac;
        }

        wxString bwxZodiacName(wxDateTime date)
        {
            wxString zodiac = _ES_;

            switch (bwxZodiac(date))
            {
            case 1: zodiac = _("Capricorn"); break;
            case 2: zodiac = _("Aquarius"); break;
            case 3: zodiac = _("Pisces"); break;
            case 4: zodiac = _("Aries"); break;
            case 5: zodiac = _("Taurus"); break;
            case 6: zodiac = _("Gemini"); break;
            case 7: zodiac = _("Cancer"); break;
            case 8: zodiac = _("Leo"); break;
            case 9: zodiac = _("Virgo"); break;
            case 10: zodiac = _("Libra"); break;
            case 11: zodiac = _("Scorpio"); break;
            case 12: zodiac = _("Sagittarius"); break;
            }

            return zodiac;
        }

        wxString bwxYStr(int y)
        {
            return wxString::Format(wxPLURAL("%d year", "%d years", y), y);
        }

        wxString bwxMStr(int m)
        {
            return wxString::Format(wxPLURAL("%d month", "%d months", m), m);
        }

        wxString bwxDStr(int d)
        {
            return wxString::Format(wxPLURAL("%d day", "%d days", d), d);
        }

        wxString bwxYMDStr(int y, int m, int d)
        {
            return bwxYStr(y) + wxT(", ") + bwxMStr(m) + _(" and ") + bwxDStr(d);
        }

        wxString bwxYMDStr(wxDateSpan span)
        {
            return bwxYStr(span.GetYears()) + wxT(", ") + bwxMStr(span.GetMonths()) + _(" and ") + bwxDStr(span.GetDays());
        }

        wxString bwxHourStr(int h)
        {
            return wxString::Format(wxPLURAL("%d hour", "%d hours", h), h);
        }

        wxString bwxMinuteStr(int m)
        {
            return wxString::Format(wxPLURAL("%d minute", "%d minutes", m), m);
        }

        wxString bwxSecondStr(int sec)
        {
            return wxString::Format(wxPLURAL("%d second", "%d seconds", sec), sec);
        }

        wxString bwxHMSStr(int h, int m, int s)
        {
            return bwxHourStr(h) + wxT(", ") + bwxMinuteStr(m) + _(" and ") + bwxSecondStr(s);
        }

        wxString bwxHMSStr(wxTimeSpan span)
        {
            return bwxHourStr(span.GetHours()) + wxT(", ") + bwxMinuteStr(span.GetMinutes()) + _(" and ") + bwxSecondStr((int)span.GetSeconds().ToLong());
        }

        wxString bwxFormatDateTime(const wxDateTime& dt, const wxString& format)
        {
            int Y = dt.GetYear();
            int M = bwxDT2IntMonth(dt.GetMonth());
            int D = dt.GetDay();
            int h = dt.GetHour();
            int m = dt.GetMinute();
            int s = dt.GetSecond();

            int tmp;

            wxString res = _ES_;
            wxString tmp_str = _ES_;
            wxChar c;

            wxString format_tmp = format;
            format_tmp += ("_____"); // Protection against reading beyond memory when i+n is out of string range
            for (size_t i = 0; i < format_tmp.Len(); i++)
            {
                c = format_tmp.GetChar(i);
                if (c == '$')
                {
                    switch ((char)format_tmp.GetChar((size_t)(i + 1)))
                    {
                    case 'Y': // YEAR
                        if (format_tmp.GetChar((size_t)(i + 2)) == 'Y' && format_tmp.GetChar((size_t)(i + 3)) == 'Y' && format_tmp.GetChar((size_t)(i + 4)) == 'Y') // YYYY
                        {
                            res += (_SF(_("A.D. %d"), Y));
                            i += 4;
                        }
                        else if (format_tmp.GetChar((size_t)(i + 2)) == 'Y' && format_tmp.GetChar((size_t)(i + 3)) == 'Y') // YYY
                        {
                            res += (_SF(_("%d y."), Y));
                            i += 3;
                        }
                        else if (format_tmp.GetChar((size_t)(i + 2)) == 'Y') // YY
                        {
                            res += (_SF(wxT("%d"), Y));
                            i += 2;
                        }
                        else // Y
                        {
                            res += (_SF(wxT("%d"), (Y % 1000)));
                            i += 1;
                        }
                        break;

                    case 'M': // MONTH
                        if (format_tmp.GetChar((size_t)(i + 2)) == 'M' && format_tmp.GetChar((size_t)(i + 3)) == 'M' && format_tmp.GetChar((size_t)(i + 4)) == 'M') // MMMM
                        {
                            tmp_str = bwxGetMonthName(M);
                            tmp_str.LowerCase();
                            res += tmp_str;
                            i += 4;
                        }
                        else if (format_tmp.GetChar((size_t)(i + 2)) == 'M' && format_tmp.GetChar((size_t)(i + 3)) == 'M') // MMM
                        {
                            tmp_str = bwxGetMonthName(M, false, BWX_ACCUSATIVE);
                            tmp_str.LowerCase();
                            res += tmp_str;
                            i += 3;
                        }
                        else if (format_tmp.GetChar((size_t)(i + 2)) == 'M') // MM
                        {
                            res += bwxGetMonthName(M, true);
                            i += 2;
                        }
                        else // M
                        {
                            if (M < 10) res += wxT("0");
                            res += (_SF(wxT("%d"), M));
                            i += 1;
                        }
                        break;

                    case 'D': // DAY
                        if (format_tmp.GetChar((size_t)(i + 2)) == 'D') // DD
                        {
                            tmp = dt.GetDayOfYear();
                            if (tmp < 10) res += wxT("00");
                            else if (tmp < 100) res += wxT("0");
                            res += (_SF(wxT("%d"), tmp));
                            i += 2;
                        }
                        else // D
                        {
                            if (D < 10) res += wxT("0");
                            res += (_SF(wxT("%d"), D));
                            i += 1;
                        }
                        break;

                    case 'W':
                        tmp = dt.GetWeekOfYear();
                        if (tmp < 10) res += wxT("0");
                        res += _SF(wxT("%d"), tmp);
                        i += 1;
                        break;

                    case 'w':
                        tmp = dt.GetWeekOfMonth();
                        if (tmp < 10) res += wxT("0");
                        res += _SF(wxT("%d"), tmp);
                        i += 1;
                        break;

                    case 'd':
                        if (format_tmp.GetChar((size_t)(i + 2)) == 'd') // dd
                        {
                            tmp = bwxGetWeekDay(dt);
                            tmp_str = bwxGetWeekDayName(tmp);
                            tmp_str.LowerCase();
                            res += tmp_str;
                            i += 2;
                        }
                        else // d
                        {
                            tmp = bwxGetWeekDay(dt);
                            tmp_str = bwxGetWeekDayName(tmp, true);
                            res += tmp_str;
                            i += 1;
                        }
                        break;

                    case 'h':
                        if (format_tmp.GetChar((size_t)(i + 2)) == 'h' && format_tmp.GetChar((size_t)(i + 3)) == 'h') // hhh
                        {
                            if (h > 12) tmp = h - 12;
                            else tmp = h;
                            if (tmp < 10) res += wxT("0");
                            res += (_SF(wxT("%d"), tmp) + ((h > 12) ? (wxT(" PM")) : (wxT(" AM"))));
                            i += 3;
                        }
                        else if (format_tmp.GetChar((size_t)(i + 2)) == 'h') // hh
                        {
                            if (h > 12) tmp = h - 12;
                            else tmp = h;
                            if (tmp < 10) res += wxT("0");
                            res += _SF(wxT("%d"), tmp);
                            i += 2;
                        }
                        else // h
                        {
                            if (h < 10) res += wxT("0");
                            res += _SF(wxT("%d"), h);
                            i += 1;
                        }
                        break;

                    case 'm':
                        if (m < 10) res += wxT("0");
                        res += _SF(wxT("%d"), m);
                        i += 1;
                        break;

                    case 's':
                        if (format_tmp.GetChar((size_t)(i + 2)) == 's') // ss
                        {
                            tmp = dt.GetMillisecond();
                            if (tmp < 10) res += wxT("00");
                            else if (tmp < 100) res += wxT("0");
                            res += _SF(wxT("%d"), tmp);
                            i += 2;
                        }
                        else // s
                        {
                            if (s < 10) res += wxT("0");
                            res += _SF(wxT("%d"), s);
                            i += 1;
                        }
                        break;

                    case 'Z':
                    case 'z':
                        res += bwxZodiacName(dt);
                        i += 1;
                        break;

                    default:
                        break;
                    }
                }
                else
                {
                    res += format_tmp.GetChar(i);
                }
            }

            return res.SubString(0, (res.Len() - 1) - 5);
        }

        std::string bwxToISO8601(wxDateTime date)
        {
            return std::string(date.FormatISOCombined().utf8_str());
        }

        int bwxCalculateAge(wxDateTime birthDate, wxDateTime currentDate)
        {
            wxDateSpan span = currentDate.DiffAsDateSpan(birthDate);
            return span.GetYears();
        }

        bool bwxIsValidDate(int d, int m, int y)
        {
            if (m < 1 || m > 12 || d < 1) return false;
            wxDateTime testDate;
            testDate.Set(d, bwxInt2DTMonth(m), y);
            return testDate.GetDay() == d;
        }

        wxDateTime bwxConvertToUTC(wxDateTime date)
        {
            return date.ToUTC();
        }

        time_t bwxToTimeT(wxDateTime date)
        {
            return date.GetTicks();
        }

        wxDateTime bwxFromTimeT(time_t timestamp)
        {
            return wxDateTime(timestamp);
        }

	} // namespace dt
} // namespace bwx_sdk
