/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_datetime.h
// Purpose:     BWX_SDK Library; Daye & Time operations
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-03-04
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_DATETIME_H_
#define _BWX_DATETIME_H_

#include "bwx_sdk/bwx_globals.h"

namespace bwx_sdk {
    namespace dt {

        /**
         * @brief Determines the day of the week based on the date (Zeller's algorithm).
         *
         * @param d Day of the month.
         * @param m Month (1-12).
         * @param y Year.
         * @param s 0 for Julian calendar, non-0 for Gregorian calendar.
         * @return Day of the week (1 = Monday, 7 = Sunday).
         */
        int bwxGetWeekDay(int d, int m, int y, int s = 1);

        /**
         * @brief Determines the day of the week based on the date (Zeller's algorithm).
         *
         * @param date wxDateTime object representing the date.
         * @param s 0 for Julian calendar, non-0 for Gregorian calendar.
         * @return Day of the week (1 = Monday, 7 = Sunday).
         */
        int bwxGetWeekDay(wxDateTime date, int s);

        /**
         * @brief Calculates the week number in the year based on the date.
         *
         * @param d Day of the month.
         * @param m Month (1-12).
         * @param y Year.
         * @param wd Day of the week (1 = Monday, 7 = Sunday).
         * @return Week number in the year.
         */
        int bwxGetWeekNumber(int d, int m, int y, int wd);

        /**
         * @brief Calculates the week number in the year based on the date.
         *
         * @param date wxDateTime object representing the date.
         * @param wd Day of the week (1 = Monday, 7 = Sunday).
         * @return Week number in the year.
         */
        int bwxGetWeekNumber(wxDateTime date, int wd);

        /**
         * @brief Calculates the week number in the year based on the date.
         *
         * @param date wxDateTime object representing the date.
         * @return Week number in the year.
         */
        int bwxGetWeekNumber(wxDateTime date);

        /**
         * @brief Calculates the time interval between two dates.
         *
         * @param d1 First date.
         * @param d2 Second date.
         * @return wxDateSpan object representing the difference between the dates.
         */
        wxDateSpan bwxGetDateDiff(wxDateTime d1, wxDateTime d2);

        /**
         * @brief Adds a specified number of years, months, and days to a date.
         *
         * @param date Initial date.
         * @param y Number of years to add.
         * @param m Number of months to add.
         * @param d Number of days to add.
         * @return New date after adding the specified values.
         */
        wxDateTime bwxAddToDate(wxDateTime date, int y, int m, int d);

        /**
         * @brief Subtracts a specified number of years, months, and days from a date.
         *
         * @param date Initial date.
         * @param y Number of years to subtract.
         * @param m Number of months to subtract.
         * @param d Number of days to subtract.
         * @return New date after subtracting the specified values.
         */
        wxDateTime bwxSubtractFromDate(wxDateTime date, int y, int m, int d);

        /**
         * @brief Calculates the time interval between two times (accurate to seconds).
         *
         * @param t1 First time.
         * @param t2 Second time.
         * @return wxTimeSpan object representing the difference between the times.
         */
        wxTimeSpan bwxGetTimeDiff(wxDateTime t1, wxDateTime t2);

        /**
         * @brief Compares two wxDateSpan objects.
         *
         * @param s1 First wxDateSpan object.
         * @param s2 Second wxDateSpan object.
         * @param s1Conv Whether to convert days and months for the first object.
         * @param s2Conv Whether to convert days and months for the second object.
         * @return -1 if s1 is less than s2, 0 if they are equal, 1 if s1 is greater than s2.
         */
        int bwxDateSpanCompare(wxDateSpan s1, wxDateSpan s2, bool s1Conv, bool s2Conv);

        /**
         * @brief Converts the given integer to the wxDateTime month enumeration value.
         *
         * @param month Month number (1-12).
         * @return Corresponding wxDateTime month enumeration value.
         */
        wxDateTime::Month bwxInt2DTMonth(int month);

        /**
         * @brief Converts the given wxDateTime month enumeration value to the corresponding integer.
         *
         * @param month Month as wxDateTime enumeration value.
         * @return Month number (1-12).
         */
        int bwxDT2IntMonth(wxDateTime::Month month);

        /**
         * @brief Converts the given integer to the wxDateTime weekday enumeration value.
         *
         * @param wd Day of the week number (1-7).
         * @return Corresponding wxDateTime weekday enumeration value.
         */
        wxDateTime::WeekDay bwxInt2DTWeekDay(int wd);

        /**
         * @brief Returns the name of the month based on the given month number (1 = January).
         *
         * @param month Month number (1-12).
         * @param shortName Whether to return the short name of the month.
         * @param c Grammatical form of the month name (e.g., nominative, genitive).
         * @return Month name as wxString.
         */
        wxString bwxGetMonthName(int month, bool shortName = false, BWX_CASES c = BWX_NOMINATIVE);

        /**
         * @brief Returns the name of the weekday based on the given weekday number (1 = Monday).
         *
         * @param weekDay Day of the week number (1 = Monday, 7 = Sunday).
         * @param shortName Whether to return the short name of the weekday.
         * @return Weekday name as wxString.
         */
        wxString bwxGetWeekDayName(int weekDay, bool shortName = false);

        /**
         * @brief Returns the zodiac sign index for the given date.
         *
         * @param date wxDateTime object representing the date.
         * @return Zodiac sign index (1-12).
         */
        int bwxZodiac(wxDateTime date);

        /**
         * @brief Returns the name of the zodiac sign for the given date.
         *
         * @param date wxDateTime object representing the date.
         * @return Zodiac sign name as wxString.
         */
        wxString bwxZodiacName(wxDateTime date);

        /**
         * @brief Returns formatted year strings based on their quantity.
         *
         * @param y Number of years.
         * @return Formatted year string.
         */
        wxString bwxYStr(int y);

        /**
         * @brief Returns formatted month strings based on their quantity.
         *
         * @param m Number of months.
         * @return Formatted month string.
         */
        wxString bwxMStr(int m);

        /**
         * @brief Returns formatted day strings based on their quantity.
         *
         * @param d Number of days.
         * @return Formatted day string.
         */
        wxString bwxDStr(int d);

        /**
         * @brief Returns formatted year, month, and day strings based on their quantity.
         *
         * @param y Number of years.
         * @param m Number of months.
         * @param d Number of days.
         * @return Formatted year, month, and day string.
         */
        wxString bwxYMDStr(int y, int m, int d);

        /**
         * @brief Returns formatted year, month, and day strings based on a wxDateSpan object.
         *
         * @param span wxDateSpan object representing the difference between dates.
         * @return Formatted year, month, and day string.
         */
        wxString bwxYMDStr(wxDateSpan span);

        /**
         * @brief Returns formatted hour strings based on their quantity.
         *
         * @param h Number of hours.
         * @return Formatted hour string.
         */
        wxString bwxHourStr(int h);

        /**
         * @brief Returns formatted minute strings based on their quantity.
         *
         * @param m Number of minutes.
         * @return Formatted minute string.
         */
        wxString bwxMinuteStr(int m);

        /**
         * @brief Returns formatted second strings based on their quantity.
         *
         * @param s Number of seconds.
         * @return Formatted second string.
         */
        wxString bwxSecondStr(int s);

        /**
         * @brief Returns formatted hour, minute, and second strings based on their quantity.
         *
         * @param h Number of hours.
         * @param m Number of minutes.
         * @param s Number of seconds.
         * @return Formatted hour, minute, and second string.
         */
        wxString bwxHMSStr(int h, int m, int s);

        /**
         * @brief Returns formatted hour, minute, and second strings based on a wxTimeSpan object.
         *
         * @param span wxTimeSpan object representing the difference between times.
         * @return Formatted hour, minute, and second string.
         */
        wxString bwxHMSStr(wxTimeSpan span);

        /**
         * @brief Formats the date and time based on the given format.
         *
         * @param dt wxDateTime object representing the date and time.
         * @param format Format in which the text should be returned.
         * @return Formatted date and time string.
         *
         * Formatting:
         * - $YYYY - Year in the format 'YYYY year'
         * - $YYY - Year in the format 'YYYY y.'
         * - $YY - Year in the format 'YYYY'
         * - $Y - Year in the format 'YY'
         * - $MMMM - Month name in nominative
         * - $MMM - Month name in genitive
         * - $MM - Short month name
         * - $M - Month number
         * - $DD - Day number in the year
         * - $D - Day number in the month
         * - $W - Week number in the year
         * - $w - Week number in the month
         * - $dd - Full weekday name
         * - $d - Short weekday name
         * - $hhh - Hours in 12-hour format with AM/PM specifier
         * - $hh - Hours in 12-hour format without specifier
         * - $h - Hours in 24-hour format
         * - $m - Minutes
         * - $s - Seconds
         * - $ss - Milliseconds
         * - $Z, $z - Zodiac sign name for the given date
         */
        wxString bwxFormatDateTime(const wxDateTime& dt, const wxString& format = wxT("$YY-$M-$D $h:$m:$s"));

        /**
         * @brief Converts a wxDateTime object to a string in ISO 8601 format.
         *
         * @param date wxDateTime object representing the date and time.
         * @return Date and time string in ISO 8601 format.
         */
        std::string bwxToISO8601(wxDateTime date);

        /**
         * @brief Calculates age basing on current date.
         *
         * @param date wxDateTime object representing the current date.
         * @return Age in years.
         */
        int bwxCalculateAge(wxDateTime birthDate, wxDateTime currentDate = wxDateTime::Today());

        /**
         * @brief Checks if the given date is valid.
         *
         * @param d Day of the month.
         * @param m Month (1-12).
         * @param y Year.
         * @return true if the date is valid; false otherwise.
         */
        bool bwxIsValidDate(int d, int m, int y);

        /**
         * @brief Converts date and time to UTC.
         *
         * @param date wxDateTime object representing the date and time.
         * @return UTC date and time.
         */
        wxDateTime bwxConvertToUTC(wxDateTime date);

        /**
         * @brief Converts date and time to time_t.
         *
         * @param date wxDateTime object representing the date and time.
         * @return Time_t value.
         */
        time_t bwxToTimeT(wxDateTime date);

        /**
        * @brief Converts time_t to wxDateTime.
        *
        * @param timestamp Time_t value.
        * @return wxDateTime object representing the date and time.
        */
        wxDateTime bwxFromTimeT(time_t timestamp);

    }
}

#endif
