/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_internat.cpp
// Purpose:     BWX_SDK Library; Internationalization support
// Author:      Bartosz Warzocha
// Created:     2015-02-23
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "bwx_internat.h"

namespace bwx_sdk {

	
	bwxInternat::bwxInternat() : wxLocale() {
		AddLanguageSystemDefault();
		UseShortCatalogNames();
	}

	bwxInternat::bwxInternat(const bwxLanguage& l) : wxLocale() {
		AddLanguageSystemDefault();
		AddLanguage(l);
		SetDefaultAppLanguage(l);
		UseShortCatalogNames();
	}

	bwxInternat::bwxInternat(const wxString& short_name, const wxString& name, const wxString& uname, wxLanguage wx_lang_code) : wxLocale() {
		AddLanguageSystemDefault();
		AddLanguage(short_name, name, uname, wx_lang_code);
		SetDefaultAppLanguage(short_name, name, uname, wx_lang_code);
		UseShortCatalogNames();
	}

	bool bwxInternat::Init(const wxString& short_name) {
		wxLanguage tmp_lang = wxLANGUAGE_UNKNOWN;

		if (short_name.IsEmpty()) {
			tmp_lang = static_cast<wxLanguage>(wxLocale::GetSystemLanguage());
			if (tmp_lang == wxLANGUAGE_UNKNOWN)
				tmp_lang = default_lang.GetWxLangCode();
		}
		else {
			auto it = lang_map.find(short_name);
			if (it != lang_map.end())
				tmp_lang = it->second.GetWxLangCode();
		}

		if (!wxLocale::Init(tmp_lang, wxLOCALE_LOAD_DEFAULT)) {
			wxLogError("Failed to initialize locale for language code: %d", tmp_lang);
			return false;
		}

		const wxLanguageInfo* lang_info = wxLocale::GetLanguageInfo(tmp_lang);
		if (!lang_info) {
			wxLogError("No language info found for code: %d", tmp_lang);
			return false;
		}

		return LoadCatalogs(lang_info);
	}

	bool bwxInternat::InitByName(const wxString& name) {
		for (const auto& pair : lang_map) {
			if (pair.second.GetName().IsSameAs(name)) {
				return Init(pair.second.GetShortName());
			}
		}
		return false;
	}

	void bwxInternat::SetDefaultAppLanguage(const wxString& short_name, const wxString& name, const wxString& uname, wxLanguage wx_lang_code) noexcept {
		default_lang = bwxLanguage(short_name, name, uname, wx_lang_code);
	}

	void bwxInternat::AddLanguage(const bwxLanguage& l) {
		lang_map[l.GetShortName()] = l; // Replace emplace with direct assignment
	}

	void bwxInternat::AddLanguage(const wxString& short_name, const wxString& name, const wxString& uname, wxLanguage wx_lang_code) {
		lang_map[short_name] = bwxLanguage(short_name, name, uname, wx_lang_code); // Assignment compatible with wxHashMap
	}

	void bwxInternat::AddLanguageSystemDefault() {
		AddLanguage("", "System default", "System default", wxLANGUAGE_DEFAULT);
	}

	void bwxInternat::AddLanguageEnglish() {
		AddLanguage("en", "English", "English", wxLANGUAGE_ENGLISH);
	}

	void bwxInternat::AddLanguagePolish() {
		AddLanguage("pl", "Polski", "Polski", wxLANGUAGE_POLISH);
	}

	void bwxInternat::AddLanguageGerman() {
		AddLanguage("de", "Deutsch", "Deutsch", wxLANGUAGE_GERMAN);
	}

	wxArrayString bwxInternat::GetLangNames() const {
		wxArrayString ret;
		for (const auto& pair : lang_map) {
			ret.Add(pair.second.GetName());
		}
		return ret;
	}

	void bwxInternat::ResetToDefaultLanguage() {
		Init(default_lang.GetShortName());
	}

	bool bwxInternat::LoadCatalogs(const wxLanguageInfo* lang_info) {
		wxFileName localePath(wxGetCwd(), lang_folder);
		AddCatalogLookupPathPrefix(localePath.GetFullPath());

		wxString locale_name = lang_info->CanonicalName.SubString(0, 1);
		std::vector<wxString> catalogs = {
			locale_name,
			locale_name + (use_short_catalog_names ? "-s" : "_system"),
			locale_name + (use_short_catalog_names ? "-b" : "_bwx")
		};

		bool success = true;
		for (const auto& cat : catalogs) {
			if (!AddCatalog(cat)) {
				wxLogWarning("Failed to add catalog: %s", cat);
				success = false;
			}
		}
		return success;
	}

} // namespace bwx_sdk
