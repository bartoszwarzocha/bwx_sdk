/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_cmd.h
// Purpose:     BWX_SDK Libarary; Easy to use cmd line parser
// Author:      Bartosz Warzocha
// Created:     2025-02-24
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef __BWXCMDLINEPARSER_H__
#define __BWXCMDLINEPARSER_H__

#include <wx/cmdline.h>

#include "bwx_sdk/bwx_globals.h"

namespace bwx_sdk {

class bwxCmdLineParser : public wxCmdLineParser {
public:
    bwxCmdLineParser();

    bwxCmdLineParser(int argc, char** argv);  // Unicode (ANSI + Unicode -> char** argv)

    bwxCmdLineParser(int argc, wchar_t** argv);

    bool Parse();

    /* Sets & Gets */

    void SetOptionalString(const wxString& s) { this->optional_str = s; }

    wxString GetOptionalString() { return this->optional_str; }

    void SetHelpInfoString(const wxString& s) { this->help_info_str = wxString::Format(wxT("\n%s\n"), s); }
    wxString GetHelpInfoString() { return this->help_info_str; }

    /* General settings */

    void SetSwitches(const wxString& switch_chars = wxT("/-"));

    void AddHelp(const wxString& short_name = wxT("h"), const wxString& long_name = wxT("help"),
                 const wxString& description = wxT("Shows this help"));

    /* Options with names */

    void AddSwitchOptional(const wxString& short_name, const wxString& long_name, const wxString& description,
                           bool visible = true);

    void AddSwitchMandatory(const wxString& short_name, const wxString& long_name, const wxString& description,
                            bool visible = true);

    void AddStringOptional(const wxString& short_name, const wxString& long_name, const wxString& description,
                           bool visible = true);

    void AddStringMandatory(const wxString& short_name, const wxString& long_name, const wxString& description,
                            bool visible = true);

    void AddNumberOptional(const wxString& short_name, const wxString& long_name, const wxString& description,
                           bool visible = true);

    void AddNumberMandatory(const wxString& short_name, const wxString& long_name, const wxString& description,
                            bool visible = true);

    void AddDoubleOptional(const wxString& short_name, const wxString& long_name, const wxString& description,
                           bool visible = true);

    void AddDoubleMandatory(const wxString& short_name, const wxString& long_name, const wxString& description,
                            bool visible = true);

    void AddDateOptional(const wxString& short_name, const wxString& long_name, const wxString& description,
                         bool visible = true);

    void AddDateMandatory(const wxString& short_name, const wxString& long_name, const wxString& description,
                          bool visible = true);

    /* Parameters */

    void AddStringParamOptional(const wxString& name, bool visible = true);

    void AddStringParamMandatory(const wxString& name, bool visible = true);

    void AddNumberParamOptional(const wxString& name, bool visible = true);

    void AddNumberParamMandatory(const wxString& name, bool visible = true);

    void AddDoubleParamOptional(const wxString& name, bool visible = true);

    void AddDoubleParamMandatory(const wxString& name, bool visible = true);

    void AddDateParamOptional(const wxString& name, bool visible = true);

    void AddDateParamMandatory(const wxString& name, bool visible = true);

    /* Data process */

    wxCmdLineSwitchState GetSwitch(const wxString& name);

    bool GetSwitchAsBool(const wxString& name);

    wxString GetString(const wxString& name, const wxString& defaultValue = _ES_);

    long GetNumber(const wxString& name, const long& defaultValue = -1);

    double GetDouble(const wxString& name, const double& defaultValue = -1);

    wxDateTime GetDate(const wxString& name, const wxDateTime& defaultValue = _DT_);

    wxString GetStringParam(size_t index = 0, const wxString& defaultValue = _ES_);

    long GetNumberParam(size_t index = 0, const long& defaultValue = -1);

    double GetDoubleParam(size_t index = 0, const double& defaultValue = -1);

    wxDateTime GetDateParam(size_t index = 0, const wxDateTime& defaultValue = _DT_);

private:
    void Init();

    wxString optional_str;
    wxString help_info_str;

    wxString Optional() { return " " + this->optional_str; }

    int argc;
};

}  // namespace bwx_sdk

#endif
