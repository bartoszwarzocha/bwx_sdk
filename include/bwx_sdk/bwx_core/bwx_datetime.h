/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_datetime.h
// Purpose:     My wxWidgets app
// Author:      Name Surname <name.surname@email.org>
// Created:     2025-03-04
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef _BWX_DATETIME_H_
#define _BWX_DATETIME_H_

#include "bwx_sdk/bwx_globals.h"

namespace bwx_sdk {

int bwxGetWeekDay(int d, int m, int y, int s = 1);

int bwxGetWeekDay(wxDateTime date, int s);

int bwxGetWeekNumber(int d, int m, int y, int wd);

int bwxGetWeekNumber(wxDateTime date, int wd);

int bwxGetWeekNumber(wxDateTime date);

wxDateSpan bwxGetDateDiff(wxDateTime d1, wxDateTime d2);

wxDateTime bwxAddToDate(wxDateTime date, int y, int m, int d);

wxDateTime bwxSubtractFromDate(wxDateTime date, int y, int m, int d);

wxTimeSpan bwxGetTimeDiff(wxDateTime t1, wxDateTime t2);

int bwxDateSpanCompare(wxDateSpan s1, wxDateSpan s2, bool s1Conv, bool s2Conv);

wxDateTime::Month bwxInt2DTMonth(int month);

int bwxDT2IntMonth(wxDateTime::Month month);

wxDateTime::WeekDay bwxInt2DTWeekDay(int wd);

wxString bwxGetMonthName(int month, bool shortName = false, BWX_CASES c = BWX_NOMINATIVE);

wxString bwxGetWeekDayName(int weekDay, bool shortName = false);

int bwxZodiac(wxDateTime date);

wxString bwxZodiacName(wxDateTime date);

wxString bwxYStr(int y);

wxString bwxMStr(int m);

wxString bwxDStr(int d);

wxString bwxYMDStr(int y, int m, int d);

wxString bwxYMDStr(wxDateSpan span);

wxString bwxHourStr(int h);

wxString bwxMinuteStr(int m);

wxString bwxSecondStr(int s);

wxString bwxHMSStr(int h, int m, int s);

wxString bwxHMSStr(wxTimeSpan span);

wxString bwxFormatDateTime(const wxDateTime& dt, const wxString& format = wxT("$YY-$M-$D $h:$m:$s"));

std::string bwxToISO8601(wxDateTime date);

int bwxCalculateAge(wxDateTime birthDate, wxDateTime currentDate = wxDateTime::Today());

bool bwxIsValidDate(int d, int m, int y);

wxDateTime bwxConvertToUTC(wxDateTime date);

time_t bwxToTimeT(wxDateTime date);

wxDateTime bwxFromTimeT(time_t timestamp);

}  // namespace bwx_sdk

#endif
