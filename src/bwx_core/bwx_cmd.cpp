/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_cmd.cpp
// Purpose:     BWX_SDK Libarary; Easy to use cmd line parser
// Author:      Bartosz Warzocha
// Created:     2025-02-24
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "bwx_cmd.h"

namespace bwx_sdk {

	bwxCmdLineParser::bwxCmdLineParser() : wxCmdLineParser() {
		this->Init();
	}

	bwxCmdLineParser::bwxCmdLineParser(int argc, char** argv) : wxCmdLineParser(argc, argv) {
		this->Init();
	} // ANSI + Unicode

	bwxCmdLineParser::bwxCmdLineParser(int argc, wchar_t** argv) : wxCmdLineParser(argc, argv) {
		this->Init();
	}

	bool bwxCmdLineParser::Parse()
	{
		if (argc == 1)
		{
			wxPrintf(wxT("\nFor more information, run the application with the parameter -h, /h, or --help.\n"));
			return false;
		}

		int result = wxCmdLineParser::Parse();
		if (result == -1 || result > 0) // (1 - help, -1 - error)
		{
			return false;
		}

		return true;
	}

	void bwxCmdLineParser::Init()
	{
		this->SetOptionalString(wxT("(optional)"));
		this->SetHelpInfoString(wxT("For more information, run the application with the parameter -h, /h, or --help."));

		this->AddHelp();
		this->SetSwitches();
	}

	void bwxCmdLineParser::SetSwitches(const wxString& switch_chars)
	{
		this->SetSwitchChars(switch_chars);
	}

	void bwxCmdLineParser::AddHelp(const wxString& short_name, const wxString& long_name, const wxString& description)
	{
		this->AddSwitch(short_name, long_name, description, wxCMD_LINE_VAL_NONE | wxCMD_LINE_OPTION_HELP);
	}

	void bwxCmdLineParser::AddSwitchOptional(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		int flags = wxCMD_LINE_VAL_NONE | wxCMD_LINE_PARAM_OPTIONAL;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddSwitch(short_name, long_name, description + this->Optional(), flags);
	}

	void bwxCmdLineParser::AddSwitchMandatory(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		int flags = wxCMD_LINE_VAL_NONE;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddSwitch(short_name, long_name, description, flags);
	}

	void bwxCmdLineParser::AddStringOptional(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		this->AddOption(short_name, long_name, description + this->Optional(), wxCMD_LINE_VAL_STRING, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	void bwxCmdLineParser::AddStringMandatory(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		int flags = wxCMD_LINE_OPTION_MANDATORY;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddOption(short_name, long_name, description, wxCMD_LINE_VAL_STRING, flags);
	}

	void bwxCmdLineParser::AddNumberOptional(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		this->AddOption(short_name, long_name, description + this->Optional(), wxCMD_LINE_VAL_NUMBER, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	void bwxCmdLineParser::AddNumberMandatory(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		int flags = wxCMD_LINE_OPTION_MANDATORY;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddOption(short_name, long_name, description, wxCMD_LINE_VAL_NUMBER, flags);
	}

	void bwxCmdLineParser::AddDoubleOptional(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		this->AddOption(short_name, long_name, description + this->Optional(), wxCMD_LINE_VAL_DOUBLE, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	void bwxCmdLineParser::AddDoubleMandatory(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		int flags = wxCMD_LINE_OPTION_MANDATORY;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddOption(short_name, long_name, description, wxCMD_LINE_VAL_DOUBLE, flags);
	}

	void bwxCmdLineParser::AddDateOptional(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		this->AddOption(short_name, long_name, description + this->Optional(), wxCMD_LINE_VAL_DATE, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	void bwxCmdLineParser::AddDateMandatory(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		int flags = wxCMD_LINE_OPTION_MANDATORY;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddOption(short_name, long_name, description, wxCMD_LINE_VAL_DATE, flags);
	}

	void bwxCmdLineParser::AddStringParamOptional(const wxString& name, bool visible)
	{
		int flags = wxCMD_LINE_PARAM_OPTIONAL;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddParam(name, wxCMD_LINE_VAL_STRING, flags);
	}

	void bwxCmdLineParser::AddStringParamMandatory(const wxString& name, bool visible)
	{
		this->AddParam(name, wxCMD_LINE_VAL_STRING, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	void bwxCmdLineParser::AddNumberParamOptional(const wxString& name, bool visible)
	{
		int flags = wxCMD_LINE_PARAM_OPTIONAL;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddParam(name, wxCMD_LINE_VAL_NUMBER, flags);
	}

	void bwxCmdLineParser::AddNumberParamMandatory(const wxString& name, bool visible)
	{
		this->AddParam(name, wxCMD_LINE_VAL_NUMBER, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	void bwxCmdLineParser::AddDoubleParamOptional(const wxString& name, bool visible)
	{
		int flags = wxCMD_LINE_PARAM_OPTIONAL;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddParam(name, wxCMD_LINE_VAL_DOUBLE, flags);
	}

	void bwxCmdLineParser::AddDoubleParamMandatory(const wxString& name, bool visible)
	{
		this->AddParam(name, wxCMD_LINE_VAL_DOUBLE, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	void bwxCmdLineParser::AddDateParamOptional(const wxString& name, bool visible)
	{
		int flags = wxCMD_LINE_PARAM_OPTIONAL;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddParam(name, wxCMD_LINE_VAL_DATE, flags);
	}

	void bwxCmdLineParser::AddDateParamMandatory(const wxString& name, bool visible)
	{
		this->AddParam(name, wxCMD_LINE_VAL_DATE, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	wxCmdLineSwitchState bwxCmdLineParser::GetSwitch(const wxString& name)
	{
		return this->FoundSwitch(name);
	}

	bool bwxCmdLineParser::GetSwitchAsBool(const wxString& name)
	{
		bool ret = false;

		wxCmdLineSwitchState tmp = this->GetSwitch(name);

		if (tmp)
		{
			ret = true;
		}

		return ret;
	}

	wxString bwxCmdLineParser::GetString(const wxString& name, const wxString& defaultValue)
	{
		wxString ret = defaultValue;
		this->Found(name, &ret);
		return ret;
	}

	long bwxCmdLineParser::GetNumber(const wxString& name, const long& defaultValue)
	{
		wxString tmp = wxEmptyString;
		long ret = defaultValue;
		this->Found(name, &tmp);
		if (!tmp.IsEmpty())
		{
			tmp.ToLong(&ret);
		}
		return ret;
	}

	double bwxCmdLineParser::GetDouble(const wxString& name, const double& defaultValue)
	{
		wxString tmp = wxEmptyString;
		double ret = defaultValue;
		this->Found(name, &tmp);
		if (!tmp.IsEmpty())
		{
			tmp.ToDouble(&ret);
		}
		return ret;
	}

	wxDateTime bwxCmdLineParser::GetDate(const wxString& name, const wxDateTime& defaultValue)
	{
		wxString tmp = wxEmptyString;
		wxDateTime tmpdt = wxDefaultDateTime;
		wxDateTime ret = defaultValue;
		this->Found(name, &tmp);
		if (!tmp.IsEmpty())
		{
			if (tmpdt.ParseISOCombined(tmp))
			{
				if (tmpdt.IsValid())
				{
					ret = tmpdt;
				}
			}
		}
		return ret;
	}

	wxString bwxCmdLineParser::GetStringParam(size_t index, const wxString& defaultValue)
	{
		wxString ret = defaultValue;
		wxString tmp = this->GetParam(index);
		if (!tmp.IsEmpty())
		{
			ret = tmp;
		}
		return ret;
	}

	long bwxCmdLineParser::GetNumberParam(size_t index, const long& defaultValue)
	{
		long ret = defaultValue;
		wxString tmp = this->GetParam(index);
		if (!tmp.IsEmpty())
		{
			tmp.ToLong(&ret);
		}
		return ret;
	}

	double bwxCmdLineParser::GetDoubleParam(size_t index, const double& defaultValue)
	{
		double ret = defaultValue;
		wxString tmp = this->GetParam(index);
		if (!tmp.IsEmpty())
		{
			tmp.ToDouble(&ret);
		}
		return ret;
	}

	wxDateTime bwxCmdLineParser::GetDateParam(size_t index, const wxDateTime& defaultValue)
	{
		wxDateTime tmpdt = wxDefaultDateTime;
		wxDateTime ret = defaultValue;
		wxString tmp = this->GetParam(index);
		if (!tmp.IsEmpty())
		{
			if (tmpdt.ParseISOCombined(tmp))
			{
				if (tmpdt.IsValid())
				{
					ret = tmpdt;
				}
			}
		}
		return ret;
	}

}
