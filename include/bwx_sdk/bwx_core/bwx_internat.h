/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_internat.h
// Purpose:     BWX_SDK Library; Internationalization support
// Author:      Bartosz Warzocha
// Created:     2015-02-23
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

// WARNING! This is a lightweight, automatically formatted version of the file.
// The file has been processed by clang-format and Python scripts of the project.
// (https://github.com/bartoszwarzocha/bwx_sdk/tree/master/scripts)
// Full versions of source code files, including hidden sections and Doxygen comments,
// can be found in the 'src' directory.

#ifndef _BWX_INTERNAT_H_
#define _BWX_INTERNAT_H_

#include <wx/filename.h>
#include <wx/intl.h>
#include <wx/log.h>

#include <algorithm>
#include <vector>

#include "bwx_sdk/bwx_globals.h"

namespace bwx_sdk {

constexpr auto bwxDEFAULT_LANG_FOLDER = "locale";

class bwxLanguage {
public:
    bwxLanguage() noexcept
        : m_wxLangCode(wxLANGUAGE_DEFAULT),
          m_shortName(wxEmptyString),
          m_uname(wxEmptyString),
          m_name("System default") {}

    bwxLanguage(const wxString& sn, const wxString& n, const wxString& un, wxLanguage wxl) noexcept
        : m_wxLangCode(wxl), m_shortName(sn), m_uname(un), m_name(n) {}

    void SetWxLangCode(wxLanguage l) noexcept { m_wxLangCode = l; }

    wxLanguage GetWxLangCode() const noexcept { return m_wxLangCode; }

    void SetShortName(const wxString& sn) noexcept { m_shortName = sn; }

    const wxString& GetShortName() const noexcept { return m_shortName; }

    void SetName(const wxString& n) noexcept { m_name = n; }

    const wxString& GetName() const noexcept { return m_name; }

    void SetUnicodeName(const wxString& n) noexcept { m_uname = n; }

    wxString GetUnicodeName() const noexcept { return m_uname.IsEmpty() ? m_name : m_uname; }

private:
    wxLanguage m_wxLangCode;
    wxString m_shortName;
    wxString m_uname;
    wxString m_name;
};

WX_DECLARE_STRING_HASH_MAP(bwxLanguage, LangMap);

class bwxInternat : public wxLocale {
public:
    bwxInternat();

    explicit bwxInternat(const bwxLanguage& l);

    bwxInternat(const wxString& shortName, const wxString& name, const wxString& uname, wxLanguage wxLangCode);

    bool Init(const wxString& shortName = wxEmptyString);

    bool InitByName(const wxString& name);

    void SetDefaultAppLanguage(const bwxLanguage& l) noexcept { m_defaultLang = l; }

    void SetDefaultAppLanguage(const wxString& shortName, const wxString& name, const wxString& uname,
                               wxLanguage wxLangCode) noexcept;

    bwxLanguage GetDefaultAppLanguage() const noexcept { return m_defaultLang; }
    wxLanguage GetDefaultAppLanguageCode() const noexcept { return m_defaultLang.GetWxLangCode(); }

    void AddLanguage(const bwxLanguage& l);

    void AddLanguage(const wxString& shortName, const wxString& name, const wxString& uname, wxLanguage wxLangCode);

    void AddLanguageSystemDefault();

    void AddLanguageEnglish();

    void AddLanguagePolish();

    void AddLanguageGerman();

    wxArrayString GetLangNames() const;

    void SetLangFolderName(const wxString& name) noexcept { m_langFolder = name; }
    const wxString& GetLangFolderName() const noexcept { return m_langFolder; }

    void UseShortCatalogNames() noexcept { m_useShortCatalogNames = true; }
    void DontUseShortCatalogNames() noexcept { m_useShortCatalogNames = false; }

    void ResetToDefaultLanguage();

private:
    bwxLanguage m_defaultLang;
    LangMap m_langMap;
    wxString m_langFolder = bwxDEFAULT_LANG_FOLDER;
    bool m_useShortCatalogNames = true;

    bool LoadCatalogs(const wxLanguageInfo* langInfo);
};

}  // namespace bwx_sdk

#endif
