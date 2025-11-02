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

#include <bwx_sdk/bwx_core/bwx_internat.h>

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

	bwxInternat::bwxInternat(const wxString& shortName, const wxString& name, const wxString& uname, wxLanguage wxLangCode) : wxLocale() {
		AddLanguageSystemDefault();
		AddLanguage(shortName, name, uname, wxLangCode);
		SetDefaultAppLanguage(shortName, name, uname, wxLangCode);
		UseShortCatalogNames();
	}

	bool bwxInternat::Init(const wxString& shortName) {
		wxLanguage tmp_lang = wxLANGUAGE_UNKNOWN;

		if (shortName.IsEmpty()) {
			tmp_lang = static_cast<wxLanguage>(wxLocale::GetSystemLanguage());
			if (tmp_lang == wxLANGUAGE_UNKNOWN)
				tmp_lang = m_defaultLang.GetWxLangCode();
		}
		else {
			auto it = m_langMap.find(shortName);
			if (it != m_langMap.end())
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
		for (const auto& pair : m_langMap) {
			if (pair.second.GetName().IsSameAs(name)) {
				return Init(pair.second.GetShortName());
			}
		}
		return false;
	}

	void bwxInternat::SetDefaultAppLanguage(const wxString& shortName, const wxString& name, const wxString& uname, wxLanguage wxLangCode) noexcept {
		m_defaultLang = bwxLanguage(shortName, name, uname, wxLangCode);
	}

	void bwxInternat::AddLanguage(const bwxLanguage& l) {
		m_langMap[l.GetShortName()] = l; // Replace emplace with direct assignment
	}

	void bwxInternat::AddLanguage(const wxString& shortName, const wxString& name, const wxString& uname, wxLanguage wxLangCode) {
		m_langMap[shortName] = bwxLanguage(shortName, name, uname, wxLangCode); // Assignment compatible with wxHashMap
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
		for (const auto& pair : m_langMap) {
			ret.Add(pair.second.GetName());
		}
		return ret;
	}

	void bwxInternat::ResetToDefaultLanguage() {
		Init(m_defaultLang.GetShortName());
	}

	bool bwxInternat::LoadCatalogs(const wxLanguageInfo* langInfo) {
		wxFileName localePath(wxGetCwd(), m_langFolder);
		AddCatalogLookupPathPrefix(localePath.GetFullPath());

		wxString locale_name = langInfo->CanonicalName.SubString(0, 1);
		std::vector<wxString> catalogs = {
			locale_name,
			locale_name + (m_useShortCatalogNames ? "-s" : "_system"),
			locale_name + (m_useShortCatalogNames ? "-b" : "_bwx")
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

}
