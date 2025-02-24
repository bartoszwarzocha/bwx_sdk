/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_cmd.cpp
// Purpose:     BWX_SDK Libarary; Easy to use cmd line parser
// Author:      Bartosz Warzocha
// Created:     2025-02-24
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

/**
 * @file bwx_cmd.cpp
 * @brief Implements easy to use command line parser derivating from wxCmdLineParser
 */

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "bwx_cmd.h"

namespace bwx_sdk {

	/**
	 * @brief Default constructor.
	 *
	 * Initializes the base wxCmdLineParser and calls the Init() method.
	 */
	bwxCmdLineParser::bwxCmdLineParser() : wxCmdLineParser() {
		this->Init();
	}

	/**
	 * @brief Constructor for ANSI and Unicode command line arguments.
	 *
	 * @param argc Number of arguments.
	 * @param argv Array of argument strings (char**).
	 */
	bwxCmdLineParser::bwxCmdLineParser(int argc, char** argv) : wxCmdLineParser(argc, argv) {
		this->Init();
	} // ANSI + Unicode

	/**
	 * @brief Constructor for wide-character command line arguments.
	 *
	 * @param argc Number of arguments.
	 * @param argv Array of argument strings (wchar_t**).
	 */
	bwxCmdLineParser::bwxCmdLineParser(int argc, wchar_t** argv) : wxCmdLineParser(argc, argv) {
		this->Init();
	}

	/**
	 * @brief Parses the command line arguments.
	 *
	 * If only one argument is provided, a help message is printed.
	 * The function then calls the base class parser and returns false if an error occurred
	 * or if help was requested.
	 *
	 * @return True if parsing was successful; false otherwise.
	 */
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

	/**
	 * @brief Initializes the command line parser.
	 *
	 * Sets the optional string and help information, and adds the default help switch
	 * and switch characters.
	 */
	void bwxCmdLineParser::Init()
	{
		this->SetOptionalString(wxT("(optional)"));
		this->SetHelpInfoString(wxT("For more information, run the application with the parameter -h, /h, or --help."));

		this->AddHelp();
		this->SetSwitches();
	}

	/**
	 * @brief Sets the characters used to identify switches.
	 *
	 * @param switch_chars String containing the switch characters.
	 */
	void bwxCmdLineParser::SetSwitches(const wxString& switch_chars)
	{
		this->SetSwitchChars(switch_chars);
	}

	/**
	 * @brief Adds a help switch to the parser.
	 *
	 * The help switch will display usage information.
	 *
	 * @param short_name Short name of the help switch.
	 * @param long_name Long name of the help switch.
	 * @param description Description of the help switch.
	 */
	void bwxCmdLineParser::AddHelp(const wxString& short_name, const wxString& long_name, const wxString& description)
	{
		this->AddSwitch(short_name, long_name, description, wxCMD_LINE_VAL_NONE | wxCMD_LINE_OPTION_HELP);
	}

	/**
	 * @brief Adds an optional switch.
	 *
	 * This method adds a command line switch that is optional. If not visible,
	 * the switch is hidden from the help output.
	 *
	 * @param short_name Short name of the switch.
	 * @param long_name Long name of the switch.
	 * @param description Description of the switch.
	 * @param visible Flag indicating whether the switch is visible.
	 */
	void bwxCmdLineParser::AddSwitchOptional(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		int flags = wxCMD_LINE_VAL_NONE | wxCMD_LINE_PARAM_OPTIONAL;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddSwitch(short_name, long_name, description + this->Optional(), flags);
	}

	/**
	 * @brief Adds a mandatory switch.
	 *
	 * This method adds a command line switch that is mandatory.
	 *
	 * @param short_name Short name of the switch.
	 * @param long_name Long name of the switch.
	 * @param description Description of the switch.
	 * @param visible Flag indicating whether the switch is visible.
	 */
	void bwxCmdLineParser::AddSwitchMandatory(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		int flags = wxCMD_LINE_VAL_NONE;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddSwitch(short_name, long_name, description, flags);
	}

	/**
	 * @brief Adds an optional string option.
	 *
	 * @param short_name Short name of the option.
	 * @param long_name Long name of the option.
	 * @param description Description of the option.
	 * @param visible Flag indicating whether the option is visible.
	 */
	void bwxCmdLineParser::AddStringOptional(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		this->AddOption(short_name, long_name, description + this->Optional(), wxCMD_LINE_VAL_STRING, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	/**
	 * @brief Adds a mandatory string option.
	 *
	 * @param short_name Short name of the option.
	 * @param long_name Long name of the option.
	 * @param description Description of the option.
	 * @param visible Flag indicating whether the option is visible.
	 */
	void bwxCmdLineParser::AddStringMandatory(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		int flags = wxCMD_LINE_OPTION_MANDATORY;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddOption(short_name, long_name, description, wxCMD_LINE_VAL_STRING, flags);
	}

	/**
	 * @brief Adds an optional numerical option.
	 *
	 * @param short_name Short name of the option.
	 * @param long_name Long name of the option.
	 * @param description Description of the option.
	 * @param visible Flag indicating whether the option is visible.
	 */
	void bwxCmdLineParser::AddNumberOptional(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		this->AddOption(short_name, long_name, description + this->Optional(), wxCMD_LINE_VAL_NUMBER, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	/**
	 * @brief Adds a mandatory numerical option.
	 *
	 * @param short_name Short name of the option.
	 * @param long_name Long name of the option.
	 * @param description Description of the option.
	 * @param visible Flag indicating whether the option is visible.
	 */
	void bwxCmdLineParser::AddNumberMandatory(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		int flags = wxCMD_LINE_OPTION_MANDATORY;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddOption(short_name, long_name, description, wxCMD_LINE_VAL_NUMBER, flags);
	}

	/**
	 * @brief Adds an optional double option.
	 *
	 * @param short_name Short name of the option.
	 * @param long_name Long name of the option.
	 * @param description Description of the option.
	 * @param visible Flag indicating whether the option is visible.
	 */
	void bwxCmdLineParser::AddDoubleOptional(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		this->AddOption(short_name, long_name, description + this->Optional(), wxCMD_LINE_VAL_DOUBLE, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	/**
	 * @brief Adds a mandatory double option.
	 *
	 * @param short_name Short name of the option.
	 * @param long_name Long name of the option.
	 * @param description Description of the option.
	 * @param visible Flag indicating whether the option is visible.
	 */
	void bwxCmdLineParser::AddDoubleMandatory(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		int flags = wxCMD_LINE_OPTION_MANDATORY;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddOption(short_name, long_name, description, wxCMD_LINE_VAL_DOUBLE, flags);
	}

	/**
	 * @brief Adds an optional date option.
	 *
	 * @param short_name Short name of the option.
	 * @param long_name Long name of the option.
	 * @param description Description of the option.
	 * @param visible Flag indicating whether the option is visible.
	 */
	void bwxCmdLineParser::AddDateOptional(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		this->AddOption(short_name, long_name, description + this->Optional(), wxCMD_LINE_VAL_DATE, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	/**
	 * @brief Adds a mandatory date option.
	 *
	 * @param short_name Short name of the option.
	 * @param long_name Long name of the option.
	 * @param description Description of the option.
	 * @param visible Flag indicating whether the option is visible.
	 */
	void bwxCmdLineParser::AddDateMandatory(const wxString& short_name, const wxString& long_name, const wxString& description, bool visible)
	{
		int flags = wxCMD_LINE_OPTION_MANDATORY;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddOption(short_name, long_name, description, wxCMD_LINE_VAL_DATE, flags);
	}

	/**
	 * @brief Adds an optional string parameter.
	 *
	 * This method adds a positional parameter for string values.
	 *
	 * @param name Name of the parameter.
	 * @param visible Flag indicating whether the parameter is visible.
	 */
	void bwxCmdLineParser::AddStringParamOptional(const wxString& name, bool visible)
	{
		int flags = wxCMD_LINE_PARAM_OPTIONAL;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddParam(name, wxCMD_LINE_VAL_STRING, flags);
	}

	/**
	 * @brief Adds a mandatory string parameter.
	 *
	 * @param name Name of the parameter.
	 * @param visible Flag indicating whether the parameter is visible.
	 */
	void bwxCmdLineParser::AddStringParamMandatory(const wxString& name, bool visible)
	{
		this->AddParam(name, wxCMD_LINE_VAL_STRING, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	/**
	 * @brief Adds an optional numerical parameter.
	 *
	 * @param name Name of the parameter.
	 * @param visible Flag indicating whether the parameter is visible.
	 */
	void bwxCmdLineParser::AddNumberParamOptional(const wxString& name, bool visible)
	{
		int flags = wxCMD_LINE_PARAM_OPTIONAL;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddParam(name, wxCMD_LINE_VAL_NUMBER, flags);
	}

	/**
	 * @brief Adds a mandatory numerical parameter.
	 *
	 * @param name Name of the parameter.
	 * @param visible Flag indicating whether the parameter is visible.
	 */
	void bwxCmdLineParser::AddNumberParamMandatory(const wxString& name, bool visible)
	{
		this->AddParam(name, wxCMD_LINE_VAL_NUMBER, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	/**
	 * @brief Adds an optional double parameter.
	 *
	 * @param name Name of the parameter.
	 * @param visible Flag indicating whether the parameter is visible.
	 */
	void bwxCmdLineParser::AddDoubleParamOptional(const wxString& name, bool visible)
	{
		int flags = wxCMD_LINE_PARAM_OPTIONAL;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddParam(name, wxCMD_LINE_VAL_DOUBLE, flags);
	}

	/**
	 * @brief Adds a mandatory double parameter.
	 *
	 * @param name Name of the parameter.
	 * @param visible Flag indicating whether the parameter is visible.
	 */
	void bwxCmdLineParser::AddDoubleParamMandatory(const wxString& name, bool visible)
	{
		this->AddParam(name, wxCMD_LINE_VAL_DOUBLE, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	/**
	 * @brief Adds an optional date parameter.
	 *
	 * @param name Name of the parameter.
	 * @param visible Flag indicating whether the parameter is visible.
	 */
	void bwxCmdLineParser::AddDateParamOptional(const wxString& name, bool visible)
	{
		int flags = wxCMD_LINE_PARAM_OPTIONAL;
		if (!visible) flags |= wxCMD_LINE_HIDDEN;
		this->AddParam(name, wxCMD_LINE_VAL_DATE, flags);
	}

	/**
	 * @brief Adds a mandatory date parameter.
	 *
	 * @param name Name of the parameter.
	 * @param visible Flag indicating whether the parameter is visible.
	 */
	void bwxCmdLineParser::AddDateParamMandatory(const wxString& name, bool visible)
	{
		this->AddParam(name, wxCMD_LINE_VAL_DATE, (visible) ? 0 : wxCMD_LINE_HIDDEN);
	}

	/**
	 * @brief Retrieves the state of a switch.
	 *
	 * @param name Name of the switch.
	 * @return The state of the switch.
	 */
	wxCmdLineSwitchState bwxCmdLineParser::GetSwitch(const wxString& name)
	{
		return this->FoundSwitch(name);
	}

	/**
	 * @brief Retrieves the state of a switch as a boolean.
	 *
	 * @param name Name of the switch.
	 * @return True if the switch is present; false otherwise.
	 */
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

	/**
	 * @brief Retrieves the string value of an option.
	 *
	 * @param name Name of the option.
	 * @param defaultValue Default value returned if the option is not found.
	 * @return The string value of the option.
	 */
	wxString bwxCmdLineParser::GetString(const wxString& name, const wxString& defaultValue)
	{
		wxString ret = defaultValue;
		this->Found(name, &ret);
		return ret;
	}

	/**
	 * @brief Retrieves the numerical value of an option.
	 *
	 * @param name Name of the option.
	 * @param defaultValue Default value returned if the option is not found.
	 * @return The numerical value of the option.
	 */
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

	/**
	 * @brief Retrieves the double precision value of an option.
	 *
	 * @param name Name of the option.
	 * @param defaultValue Default value returned if the option is not found.
	 * @return The double value of the option.
	 */
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

	/**
	 * @brief Retrieves the date value of an option.
	 *
	 * @param name Name of the option.
	 * @param defaultValue Default date returned if the option is not found or invalid.
	 * @return The date value of the option.
	 */
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

	/**
	 * @brief Retrieves the string value of a positional parameter.
	 *
	 * @param index Index of the parameter.
	 * @param defaultValue Default value returned if the parameter is not provided.
	 * @return The string value of the parameter.
	 */
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

	/**
	 * @brief Retrieves the numerical value of a positional parameter.
	 *
	 * @param index Index of the parameter.
	 * @param defaultValue Default value returned if the parameter is not provided.
	 * @return The numerical value of the parameter.
	 */
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

	/**
	 * @brief Retrieves the double precision value of a positional parameter.
	 *
	 * @param index Index of the parameter.
	 * @param defaultValue Default value returned if the parameter is not provided.
	 * @return The double value of the parameter.
	 */
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

	/**
	 * @brief Retrieves the date value of a positional parameter.
	 *
	 * @param index Index of the parameter.
	 * @param defaultValue Default date returned if the parameter is not provided or invalid.
	 * @return The date value of the parameter.
	 */
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
