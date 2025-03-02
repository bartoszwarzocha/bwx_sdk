/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_config_utils.h
// Purpose:     BWX_SDK Library; Basic config wrapper
// Author:      Bartosz Warzocha
// Created:     2015-09-03
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef __BWXCONFIGUTILS_H__
#define __BWXCONFIGUTILS_H__

#include <bwx_oop.h>
#include <bwx_sdk/bwx_globals.h>
#include <wx/confbase.h>

namespace bwx_sdk {

#define bwxSetFileConf()                                                                             \
    wxFileConfig config(_ES_, _ES_, wxStandardPaths::Get().GetUserDataDir() + "\\config.conf", _ES_, \
                        wxCONFIG_USE_LOCAL_FILE | wxCONFIG_USE_GLOBAL_FILE);                         \
    config.EnableAutoSave();                                                                         \
    config.SetRecordDefaults()

#define bwxConfR(key, var, def) config.Read(key, &var, def)

#define bwxConfW(key, val) config.Write(key, val)

enum bwxConfigEntryType {
    CONFIG_ENTRY_TYPE_STRING = 0,
    CONFIG_ENTRY_TYPE_INT,
    CONFIG_ENTRY_TYPE_FLOAT,
    CONFIG_ENTRY_TYPE_DOUBLE,
    CONFIG_ENTRY_TYPE_BOOLEAN
};

class bwxConfigEntry {
    _inline(bwxConfigEntryType, type, Type);
    _inline(wxString, path, Path);
    _inline(wxAny, defaultValue, DefaultValue);
    _inline(wxAny, value, Value);

public:
    bwxConfigEntry() : m_type(CONFIG_ENTRY_TYPE_STRING), m_path(""), m_defaultValue(wxAny()), m_value(wxAny()) {}

    bwxConfigEntry(const bwxConfigEntryType& t, const wxString& p, const wxAny& d)
        : m_type(t), m_path(p), m_defaultValue(d) {}
};

class bwxConfigUtils {
private:
    static bwxPropertyMap<wxString, bwxConfigEntry> m_configEntries;

public:
    static void LoadConfig();

    static void SaveConfig();

    static void ClearEntries();

    static void AddStringEntry(const wxString& field, const wxString& defaultValue);

    static void AddIntEntry(const wxString& field, const int& defaultValue);

    static void AddFloatEntry(const wxString& field, const float& defaultValue);

    static void AddDoubleEntry(const wxString& field, const double& defaultValue);

    static void AddBooleanEntry(const wxString& field, const bool& defaultValue);

    static void Set(const wxString& key, wxAny val);

    static void Get(const wxString& key, wxString* val);

    static void Get(const wxString& key, int* val);

    static void Get(const wxString& key, float* val);

    static void Get(const wxString& key, bool* val);

    static bool Get(const wxString& key, wxColour* c);

    static void ShowInLog();
};

}  // namespace bwx_sdk

#endif
