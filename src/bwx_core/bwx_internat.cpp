/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_internat.cpp
// Purpose:     BWX_SDK Library; Internationalization support
// Author:      Bartosz Warzocha
// Created:     2015-02-23
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

/**
 * @file bwx_internat.cpp
 * @brief Implements the bwxInternat class for internationalization.
 *
 * The bwxInternat class extends wxLocale to provide convenient methods for
 * setting up languages, loading catalogs, and managing default application language.
 */

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include "bwx_internat.h"

namespace bwx_sdk {

	/**
	 * @brief Default constructor.
	 *
	 * Constructs a bwxInternat object using the default system language and
	 * enables the use of short catalog names.
	 */
	bwxInternat::bwxInternat() : wxLocale() {
		AddLanguageSystemDefault();
		UseShortCatalogNames();
	}

	/**
	 * @brief Constructs a bwxInternat object with a specified language.
	 *
	 * Adds the system default language, then adds the provided language, sets it as the default,
	 * and enables the use of short catalog names.
	 *
	 * @param l A bwxLanguage object representing the language to add.
	 */
	bwxInternat::bwxInternat(const bwxLanguage& l) : wxLocale() {
		AddLanguageSystemDefault();
		AddLanguage(l);
		SetDefaultAppLanguage(l);
		UseShortCatalogNames();
	}

	/**
	 * @brief Constructs a bwxInternat object with language parameters.
	 *
	 * Adds the system default language, then adds the language defined by the provided parameters,
	 * sets it as the default, and enables the use of short catalog names.
	 *
	 * @param short_name Short identifier for the language.
	 * @param name Full name of the language.
	 * @param uname Unique name for the language.
	 * @param wx_lang_code wxWidgets language code.
	 */
	bwxInternat::bwxInternat(const wxString& short_name, const wxString& name, const wxString& uname, wxLanguage wx_lang_code) : wxLocale() {
		AddLanguageSystemDefault();
		AddLanguage(short_name, name, uname, wx_lang_code);
		SetDefaultAppLanguage(short_name, name, uname, wx_lang_code);
		UseShortCatalogNames();
	}

	/**
	 * @brief Initializes the locale using a language short name.
	 *
	 * If the provided short name is empty, the system language is used. If the system language
	 * is unknown, the default language is used instead. Once the language code is determined,
	 * the locale is initialized and the language catalogs are loaded.
	 *
	 * @param short_name The short name of the language to initialize.
	 * @return True if initialization and catalog loading succeeded; false otherwise.
	 */
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

	/**
	 * @brief Initializes the locale by the full language name.
	 *
	 * Searches the internal language map for a language whose full name matches the provided name.
	 * If found, the locale is initialized using the corresponding short name.
	 *
	 * @param name The full name of the language.
	 * @return True if initialization succeeded; false otherwise.
	 */
	bool bwxInternat::InitByName(const wxString& name) {
		for (const auto& pair : lang_map) {
			if (pair.second.GetName().IsSameAs(name)) {
				return Init(pair.second.GetShortName());
			}
		}
		return false;
	}

	/**
	 * @brief Sets the default application language.
	 *
	 * Updates the default language used by the application.
	 *
	 * @param short_name Short identifier for the language.
	 * @param name Full name of the language.
	 * @param uname Unique name for the language.
	 * @param wx_lang_code wxWidgets language code.
	 */
	void bwxInternat::SetDefaultAppLanguage(const wxString& short_name, const wxString& name, const wxString& uname, wxLanguage wx_lang_code) noexcept {
		default_lang = bwxLanguage(short_name, name, uname, wx_lang_code);
	}

	/**
	 * @brief Adds a language to the internal language map.
	 *
	 * @param l A bwxLanguage object to add.
	 */
	void bwxInternat::AddLanguage(const bwxLanguage& l) {
		lang_map[l.GetShortName()] = l; // Replace emplace with direct assignment
	}

	/**
	 * @brief Adds a language to the internal language map using parameters.
	 *
	 * @param short_name Short identifier for the language.
	 * @param name Full name of the language.
	 * @param uname Unique name for the language.
	 * @param wx_lang_code wxWidgets language code.
	 */
	void bwxInternat::AddLanguage(const wxString& short_name, const wxString& name, const wxString& uname, wxLanguage wx_lang_code) {
		lang_map[short_name] = bwxLanguage(short_name, name, uname, wx_lang_code); // Assignment compatible with wxHashMap
	}

	/**
	 * @brief Adds the system default language.
	 *
	 * This method adds a language entry representing the system default.
	 */
	void bwxInternat::AddLanguageSystemDefault() {
		AddLanguage("", "System default", "System default", wxLANGUAGE_DEFAULT);
	}

	/**
	 * @brief Adds English to the language map.
	 */
	void bwxInternat::AddLanguageEnglish() {
		AddLanguage("en", "English", "English", wxLANGUAGE_ENGLISH);
	}

	/**
	 * @brief Adds Polish to the language map.
	 */
	void bwxInternat::AddLanguagePolish() {
		AddLanguage("pl", "Polski", "Polski", wxLANGUAGE_POLISH);
	}

	/**
	 * @brief Adds German to the language map.
	 */
	void bwxInternat::AddLanguageGerman() {
		AddLanguage("de", "Deutsch", "Deutsch", wxLANGUAGE_GERMAN);
	}

	/**
	 * @brief Retrieves the list of full language names.
	 *
	 * Iterates over the internal language map and returns an array of language names.
	 *
	 * @return A wxArrayString containing the names of all languages in the map.
	 */
	wxArrayString bwxInternat::GetLangNames() const {
		wxArrayString ret;
		for (const auto& pair : lang_map) {
			ret.Add(pair.second.GetName());
		}
		return ret;
	}

	/**
	 * @brief Resets the locale to the default language.
	 *
	 * Re-initializes the locale using the default language's short name.
	 */
	void bwxInternat::ResetToDefaultLanguage() {
		Init(default_lang.GetShortName());
	}

	/**
	 * @brief Loads language catalogs based on language information.
	 *
	 * This method constructs the catalog lookup path and attempts to load a series of catalogs
	 * (with or without short names) for the specified language.
	 *
	 * @param lang_info Pointer to wxLanguageInfo containing language details.
	 * @return True if all catalogs were loaded successfully; false if any catalog failed to load.
	 */
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
