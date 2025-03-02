/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_cmd.h
// Purpose:     BWX_SDK Libarary; Easy to use cmd line parser
// Author:      Bartosz Warzocha
// Created:     2025-02-24
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __BWXCMDLINEPARSER_H__
#define __BWXCMDLINEPARSER_H__

#include <wx/cmdline.h>

#include "bwx_sdk/bwx_globals.h"

/**
 * @file bwx_cmd.h
 * @brief Provides a simple command line parser for the BWXSDK library.
 */

namespace bwx_sdk {

	/**
	 * @class bwxCmdLineParser
	 * @brief A command line parser class for the BWXSDK library.
	 *
	 * The `bwxCmdLineParser` class extends the functionality of `wxCmdLineParser`
	 * and provides easy-to-use methods for adding and processing command line options.
	 *
	 * @details
	 * This class allows you to add various types of options, such as string options,
	 * numerical options, double precision options, and date options. You can also add
	 * mandatory and optional options, as well as positional parameters.
	 *
	 * Example usage:
	 * @code
	 * bwxCmdLineParser parser(argc, argv);
	 * parser.AddHelp("h", "help", "Displays help");
	 * parser.AddStringOptional("o", "output", "Path to the output file");
	 * if (!parser.Parse()) {
	 *     return 1;
	 * }
	 * wxString outputPath = parser.GetString("output", "default.txt");
	 * @endcode
	 */
	class bwxCmdLineParser : public wxCmdLineParser
	{
	public:
		/**
		 * @brief Default constructor.
		 *
		 * Initializes the base wxCmdLineParser and calls the Init() method.
		 */
		bwxCmdLineParser();

		/**
		 * @brief Constructor for ANSI and Unicode command line arguments.
		 *
		 * @param argc Number of arguments.
		 * @param argv Array of argument strings (char**).
		 */
		bwxCmdLineParser(int argc, char** argv); // Unicode (ANSI + Unicode -> char** argv)
		
		/**
		 * @brief Constructor for wide-character command line arguments.
		 *
		 * @param argc Number of arguments.
		 * @param argv Array of argument strings (wchar_t**).
		 */
		bwxCmdLineParser(int argc, wchar_t** argv);

		/**
		 * @brief Parses the command line arguments.
		 *
		 * If only one argument is provided, a help message is printed.
		 * The function then calls the base class parser and returns false if an error occurred
		 * or if help was requested.
		 *
		 * @return True if parsing was successful; false otherwise.
		 */
		bool Parse();

		/* Sets & Gets */

		/**
		 * @brief Sets the optional string.
		 *
		 * @param s Optional string.
		 */
		void SetOptionalString(const wxString& s) { this->m_optionalStr = s; }

		/**
		 * @brief Gets the optional string.
		 *
		 * @return Optional string.
		 */
		wxString GetOptionalString() { return this->m_optionalStr; }

		
		void SetHelpInfoString(const wxString& s) { this->m_helpInfoStr = wxString::Format("\n%s\n", s); }
		wxString GetHelpInfoString() { return this->m_helpInfoStr; }

		/* General settings */

		/**
		 * @brief Sets the characters used to identify switches.
		 *
		 * @param switchChars String containing the switch characters.
		 */
		void SetSwitches(const wxString& switchChars = "/-");

		/**
		 * @brief Adds a help switch to the parser.
		 *
		 * The help switch will display usage information.
		 *
		 * @param shortName Short name of the help switch.
		 * @param longName Long name of the help switch.
		 * @param description Description of the help switch.
		 */
		void AddHelp(const wxString& shortName = "h", const wxString& longName = "help", const wxString& description = wxT("Shows this help"));

		/* Options with names */

		/**
		 * @brief Adds an optional switch.
		 *
		 * This method adds a command line switch that is optional. If not visible,
		 * the switch is hidden from the help output.
		 *
		 * @param shortName Short name of the switch.
		 * @param longName Long name of the switch.
		 * @param description Description of the switch.
		 * @param visible Flag indicating whether the switch is visible.
		 */
		void AddSwitchOptional(const wxString& shortName, const wxString& longName, const wxString& description, bool visible = true);
		
		/**
		 * @brief Adds a mandatory switch.
		 *
		 * This method adds a command line switch that is mandatory.
		 *
		 * @param shortName Short name of the switch.
		 * @param longName Long name of the switch.
		 * @param description Description of the switch.
		 * @param visible Flag indicating whether the switch is visible.
		 */
		void AddSwitchMandatory(const wxString& shortName, const wxString& longName, const wxString& description, bool visible = true);
		
		/**
		 * @brief Adds an optional string option.
		 *
		 * @param shortName Short name of the option.
		 * @param longName Long name of the option.
		 * @param description Description of the option.
		 * @param visible Flag indicating whether the option is visible.
		 */
		void AddStringOptional(const wxString& shortName, const wxString& longName, const wxString& description, bool visible = true);
		
		/**
		 * @brief Adds a mandatory string option.
		 *
		 * @param shortName Short name of the option.
		 * @param longName Long name of the option.
		 * @param description Description of the option.
		 * @param visible Flag indicating whether the option is visible.
		 */
		void AddStringMandatory(const wxString& shortName, const wxString& longName, const wxString& description, bool visible = true);
		
		/**
		 * @brief Adds an optional numerical option.
		 *
		 * @param shortName Short name of the option.
		 * @param longName Long name of the option.
		 * @param description Description of the option.
		 * @param visible Flag indicating whether the option is visible.
		 */
		void AddNumberOptional(const wxString& shortName, const wxString& longName, const wxString& description, bool visible = true);
		
		/**
		 * @brief Adds a mandatory numerical option.
		 *
		 * @param shortName Short name of the option.
		 * @param longName Long name of the option.
		 * @param description Description of the option.
		 * @param visible Flag indicating whether the option is visible.
		 */
		void AddNumberMandatory(const wxString& shortName, const wxString& longName, const wxString& description, bool visible = true);
		
		/**
		 * @brief Adds an optional double precision option.
		 *
		 * @param shortName Short name of the option.
		 * @param longName Long name of the option.
		 * @param description Description of the option.
		 * @param visible Flag indicating whether the option is visible.
		 */
		void AddDoubleOptional(const wxString& shortName, const wxString& longName, const wxString& description, bool visible = true);
		
		/**
		 * @brief Adds a mandatory double precision option.
		 *
		 * @param shortName Short name of the option.
		 * @param longName Long name of the option.
		 * @param description Description of the option.
		 * @param visible Flag indicating whether the option is visible.
		 */
		void AddDoubleMandatory(const wxString& shortName, const wxString& longName, const wxString& description, bool visible = true);
		
		/**
		 * @brief Adds an optional date option.
		 *
		 * @param shortName Short name of the option.
		 * @param longName Long name of the option.
		 * @param description Description of the option.
		 * @param visible Flag indicating whether the option is visible.
		 */
		void AddDateOptional(const wxString& shortName, const wxString& longName, const wxString& description, bool visible = true);
		
		/**
		 * @brief Adds a mandatory date option.
		 *
		 * @param shortName Short name of the option.
		 * @param longName Long name of the option.
		 * @param description Description of the option.
		 * @param visible Flag indicating whether the option is visible.
		 */
		void AddDateMandatory(const wxString& shortName, const wxString& longName, const wxString& description, bool visible = true);

		/* Parameters */

		/**
		 * @brief Adds an optional string parameter.
		 *
		 * This method adds a positional parameter for string values.
		 *
		 * @param name Name of the parameter.
		 * @param visible Flag indicating whether the parameter is visible.
		 */
		void AddStringParamOptional(const wxString& name, bool visible = true);
		
		/**
		 * @brief Adds a mandatory string parameter.
		 *
		 * This method adds a positional parameter for string values.
		 *
		 * @param name Name of the parameter.
		 * @param visible Flag indicating whether the parameter is visible.
		 */
		void AddStringParamMandatory(const wxString& name, bool visible = true);
		
		/**
		 * @brief Adds an optional numerical parameter.
		 *
		 * This method adds a positional parameter for numerical values.
		 *
		 * @param name Name of the parameter.
		 * @param visible Flag indicating whether the parameter is visible.
		 */
		void AddNumberParamOptional(const wxString& name, bool visible = true);
		
		/**
		 * @brief Adds a mandatory numerical parameter.
		 *
		 * This method adds a positional parameter for numerical values.
		 *
		 * @param name Name of the parameter.
		 * @param visible Flag indicating whether the parameter is visible.
		 */
		void AddNumberParamMandatory(const wxString& name, bool visible = true);
		
		/**
		 * @brief Adds an optional double precision parameter.
		 *
		 * This method adds a positional parameter for double precision values.
		 *
		 * @param name Name of the parameter.
		 * @param visible Flag indicating whether the parameter is visible.
		 */
		void AddDoubleParamOptional(const wxString& name, bool visible = true);
		
		/**
		 * @brief Adds a mandatory double precision parameter.
		 *
		 * This method adds a positional parameter for double precision values.
		 *
		 * @param name Name of the parameter.
		 * @param visible Flag indicating whether the parameter is visible.
		 */
		void AddDoubleParamMandatory(const wxString& name, bool visible = true);
		
		/**
		 * @brief Adds an optional date parameter.
		 *
		 * This method adds a positional parameter for date values.
		 *
		 * @param name Name of the parameter.
		 * @param visible Flag indicating whether the parameter is visible.
		 */
		void AddDateParamOptional(const wxString& name, bool visible = true);
		
		/**
		 * @brief Adds a mandatory date parameter.
		 *
		 * This method adds a positional parameter for date values.
		 *
		 * @param name Name of the parameter.
		 * @param visible Flag indicating whether the parameter is visible.
		 */
		void AddDateParamMandatory(const wxString& name, bool visible = true);

		/* Data process */

		/**
		 * @brief Retrieves the state of a switch.
		 *
		 * @param name Name of the switch.
		 * @return The state of the switch.
		 */
		wxCmdLineSwitchState GetSwitch(const wxString& name);

		/**
		 * @brief Retrieves the state of a switch as a boolean.
		 *
		 * @param name Name of the switch.
		 * @return True if the switch is present; false otherwise.
		 */
		bool GetSwitchAsBool(const wxString& name);

		/**
		 * @brief Retrieves a string value.
		 *
		 * @param name Name of the value.
		 * @param defaultValue Default value.
		 * @return The string value.
		 */
		wxString GetString(const wxString& name, const wxString& defaultValue = _ES_);
		
		/**
		 * @brief Retrieves a numerical value.
		 *
		 * @param name Name of the value.
		 * @param defaultValue Default value.
		 * @return The numerical value.
		 */
		long GetNumber(const wxString& name, const long& defaultValue = -1);
		
		/**
		 * @brief Retrieves a double precision value.
		 *
		 * @param name Name of the value.
		 * @param defaultValue Default value.
		 * @return The double precision value.
		 */
		double GetDouble(const wxString& name, const double& defaultValue = -1);
		
		/**
		 * @brief Retrieves the date value of an option.
		 *
		 * @param name Name of the option.
		 * @param defaultValue Default date returned if the option is not found or invalid.
		 * @return The date value of the option.
		 */
		wxDateTime GetDate(const wxString& name, const wxDateTime& defaultValue = _DT_);

		/**
		 * @brief Retrieves the string value of a positional parameter.
		 *
		 * @param index Index of the parameter.
		 * @param defaultValue Default value returned if the parameter is not provided.
		 * @return The string value of the parameter.
		 */
		wxString GetStringParam(size_t index = 0, const wxString& defaultValue = _ES_);
		
		/**
		 * @brief Retrieves the numerical value of a positional parameter.
		 *
		 * @param index Index of the parameter.
		 * @param defaultValue Default value returned if the parameter is not provided.
		 * @return The numerical value of the parameter.
		 */
		long GetNumberParam(size_t index = 0, const long& defaultValue = -1);
		
		/**
		 * @brief Retrieves the double precision value of a positional parameter.
		 *
		 * @param index Index of the parameter.
		 * @param defaultValue Default value returned if the parameter is not provided.
		 * @return The double precision value of the parameter.
		 */
		double GetDoubleParam(size_t index = 0, const double& defaultValue = -1);
		
		/**
		 * @brief Retrieves the date value of a positional parameter.
		 *
		 * @param index Index of the parameter.
		 * @param defaultValue Default date returned if the parameter is not found or invalid.
		 * @return The date value of the parameter.
		 */
		wxDateTime GetDateParam(size_t index = 0, const wxDateTime& defaultValue = _DT_);

	private:
		/**
		 * @brief Initializes the command line parser.
		 *
		 * Sets the optional string and help information, and adds the default help switch
		 * and switch characters.
		 */
		void Init();

		wxString m_optionalStr; ///< Optional string.
		wxString m_helpInfoStr; ///< Help information string.

		/**
		 * @brief Returns the optional string.
		 *
		 * @return Optional string.
		 */
		wxString Optional() { return " " + this->m_optionalStr; }

		int m_argc; ///< Number of arguments.
	};

}

#endif

