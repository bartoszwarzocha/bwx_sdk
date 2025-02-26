/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_internat.h
// Purpose:     BWX_SDK Library; Internationalization support
// Author:      Bartosz Warzocha
// Created:     2015-02-23
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_INTERNAT_H_
#define _BWX_INTERNAT_H_

#include <wx/intl.h>
#include <wx/filename.h>
#include <wx/log.h>
#include <algorithm>
#include <vector>

#include "bwx_sdk/bwx_globals.h"

namespace bwx_sdk {

	constexpr auto bwxDEFAULT_LANG_FOLDER = "locale";

    class bwxLanguage {
    public:

        bwxLanguage() noexcept
            : wx_lang_code(wxLANGUAGE_DEFAULT), short_name(wxEmptyString), uname(wxEmptyString), name("System default") {}

        bwxLanguage(const wxString& sn, const wxString& n, const wxString& un, wxLanguage wxl) noexcept
            : wx_lang_code(wxl), short_name(sn), uname(un), name(n) {}

        void SetWxLangCode(wxLanguage l) noexcept { wx_lang_code = l; }

        wxLanguage GetWxLangCode() const noexcept { return wx_lang_code; }
        
		void SetShortName(const wxString& sn) noexcept { short_name = sn; }
        
		const wxString& GetShortName() const noexcept { return short_name; }
        
		void SetName(const wxString& n) noexcept { name = n; }
        
		const wxString& GetName() const noexcept { return name; }

        void SetUnicodeName(const wxString& n) noexcept { uname = n; }
        
		wxString GetUnicodeName() const noexcept { return uname.IsEmpty() ? name : uname; }

    private:
		wxLanguage wx_lang_code;
		wxString short_name;
        wxString uname;
		wxString name;
    };

	WX_DECLARE_STRING_HASH_MAP(bwxLanguage, LangMap);

    class bwxInternat : public wxLocale {
    public:
        
        bwxInternat();
        
		explicit bwxInternat(const bwxLanguage& l);

        bwxInternat(const wxString& short_name, const wxString& name, const wxString& uname, wxLanguage wx_lang_code);

        bool Init(const wxString& short_name = wxEmptyString);

        bool InitByName(const wxString& name);

        void SetDefaultAppLanguage(const bwxLanguage& l) noexcept { default_lang = l; }
        
		void SetDefaultAppLanguage(const wxString& short_name, const wxString& name, const wxString& uname, wxLanguage wx_lang_code) noexcept;

        bwxLanguage GetDefaultAppLanguage() const noexcept { return default_lang; }
        wxLanguage GetDefaultAppLanguageCode() const noexcept { return default_lang.GetWxLangCode(); }

        void AddLanguage(const bwxLanguage& l);

        void AddLanguage(const wxString& short_name, const wxString& name, const wxString& uname, wxLanguage wx_lang_code);
        
		void AddLanguageSystemDefault();

        void AddLanguageEnglish();

        void AddLanguagePolish();

        void AddLanguageGerman();

        wxArrayString GetLangNames() const;

        void SetLangFolderName(const wxString& name) noexcept { lang_folder = name; }
        const wxString& GetLangFolderName() const noexcept { return lang_folder; }

        void UseShortCatalogNames() noexcept { use_short_catalog_names = true; }
        void DontUseShortCatalogNames() noexcept { use_short_catalog_names = false; }

        void ResetToDefaultLanguage();

    private:
		bwxLanguage default_lang;
		LangMap lang_map;
		wxString lang_folder = bwxDEFAULT_LANG_FOLDER;
		bool use_short_catalog_names = true;

        bool LoadCatalogs(const wxLanguageInfo* lang_info);
    };

}

#endif