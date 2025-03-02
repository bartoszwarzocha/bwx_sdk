/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_config_utils.h
// Purpose:     BWX_SDK Library; Basic config wrapper
// Author:      Bartosz Warzocha
// Created:     2015-09-03
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef __BWXCONFIGUTILS_H__
#define __BWXCONFIGUTILS_H__

#include <wx/confbase.h>
#include <bwx_sdk/bwx_globals.h>
#include <bwx_oop.h>

namespace bwx_sdk {

    /**
     * @brief Macro to initialize wxFileConfig with default settings.
     *
     * This macro initializes a wxFileConfig instance with the default configuration file
     * path and enables automatic saving and default recording.
     */
#define bwxSetFileConf() wxFileConfig config(_ES_, _ES_, wxStandardPaths::Get().GetUserDataDir() + "\\config.conf", _ES_, wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_GLOBAL_FILE); \
    config.EnableAutoSave(); \
    config.SetRecordDefaults()

     /**
      * @brief Macro to read a configuration entry.
      *
      * @param key Configuration key to read.
      * @param var Variable to store the read value.
      * @param def Default value if the key is not found.
      */
#define bwxConfR(key, var, def) config.Read(key, &var, def)

      /**
       * @brief Macro to write a configuration entry.
       *
       * @param key Configuration key to write.
       * @param val Value to store in the configuration file.
       */
#define bwxConfW(key, val) config.Write(key, val)

       /**
        * @brief Enumeration for configuration entry types.
        */
    enum bwxConfigEntryType
    {
        CONFIG_ENTRY_TYPE_STRING = 0, ///< String type configuration entry.
        CONFIG_ENTRY_TYPE_INT,        ///< Integer type configuration entry.
        CONFIG_ENTRY_TYPE_FLOAT,      ///< Float type configuration entry.
        CONFIG_ENTRY_TYPE_DOUBLE,     ///< Double type configuration entry.
        CONFIG_ENTRY_TYPE_BOOLEAN     ///< Boolean type configuration entry.
    };

    /**
     * @brief Represents a configuration entry.
     *
     * This class stores a single configuration entry including its type, path, default value,
     * and current value.
     */
    class bwxConfigEntry
    {
        _inline(bwxConfigEntryType, type, Type); ///< Type of the configuration entry.
        _inline(wxString, path, Path); ///< Path of the configuration entry.
        _inline(wxAny, defaultValue, DefaultValue); ///< Default value of the configuration entry.
        _inline(wxAny, value, Value); ///< Current value of the configuration entry.

    public:
        /**
         * @brief Default constructor initializing an empty configuration entry.
         */
        bwxConfigEntry() : m_type(CONFIG_ENTRY_TYPE_STRING), m_path(""), m_defaultValue(wxAny()), m_value(wxAny()) {}

        /**
         * @brief Constructor initializing the configuration entry with given parameters.
         * @param t Type of the configuration entry.
         * @param p Path of the configuration entry.
         * @param d Default value of the configuration entry.
         */
        bwxConfigEntry(const bwxConfigEntryType& t, const wxString& p, const wxAny& d) : m_type(t), m_path(p), m_defaultValue(d) {}
    };

    /**
     * @brief Utility class for managing application configuration.
     *
     * This class provides methods to read, write, and manage configuration entries
     * using wxFileConfig.
     */
    class bwxConfigUtils
    {
    private:
        static bwxPropertyMap<wxString, bwxConfigEntry> m_configEntries; ///< Storage for configuration entries.

    public:
        /**
         * @brief Loads configuration entries from the file into memory.
         */
        static void LoadConfig();

        /**
         * @brief Saves configuration entries from memory to the file.
         */
        static void SaveConfig();

        /**
         * @brief Clears all stored configuration entries.
         */
        static void ClearEntries();

        /**
         * @brief Adds a new string configuration entry.
         * @param field The configuration key.
         * @param defaultValue The default value for this entry.
         */
        static void AddStringEntry(const wxString& field, const wxString& defaultValue);

        /**
         * @brief Adds a new integer configuration entry.
         * @param field The configuration key.
         * @param defaultValue The default value for this entry.
         */
        static void AddIntEntry(const wxString& field, const int& defaultValue);

        /**
         * @brief Adds a new float configuration entry.
         * @param field The configuration key.
         * @param defaultValue The default value for this entry.
         */
        static void AddFloatEntry(const wxString& field, const float& defaultValue);

        /**
         * @brief Adds a new double configuration entry.
         * @param field The configuration key.
         * @param defaultValue The default value for this entry.
         */
        static void AddDoubleEntry(const wxString& field, const double& defaultValue);

        /**
         * @brief Adds a new boolean configuration entry.
         * @param field The configuration key.
         * @param defaultValue The default value for this entry.
         */
        static void AddBooleanEntry(const wxString& field, const bool& defaultValue);

        /**
         * @brief Sets a configuration value.
         * @param key Configuration key.
         * @param val Value to set.
         */
        static void Set(const wxString& key, wxAny val);

        /**
         * @brief Retrieves a configuration value as a wxString.
         */
        static void Get(const wxString& key, wxString* val);

        /**
         * @brief Retrieves a configuration value as an integer.
         */
        static void Get(const wxString& key, int* val);

        /**
         * @brief Retrieves a configuration value as a float.
         */
        static void Get(const wxString& key, float* val);

        /**
         * @brief Retrieves a configuration value as a boolean.
         */
        static void Get(const wxString& key, bool* val);

        /**
         * @brief Retrieves a configuration value as a wxColour.
         */
        static bool Get(const wxString& key, wxColour* c);

        /**
         * @brief Logs the current configuration entries.
         */
        static void ShowInLog();
    };

}

#endif
