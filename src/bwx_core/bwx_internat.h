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

/**
 * @file bwx_internat.h
 * @brief Provides internationalization support for the BWXSDK library.
 */

namespace bwx_sdk {

	constexpr auto bwxDEFAULT_LANG_FOLDER = "locale"; ///< Default folder name for language catalogs.

	/**
	* @brief Class representing a language.
	*/
    class bwxLanguage {
    public:

		/**
		 * @brief Default constructor.
		 *
		 * Initializes the language with default values.
		 */
        bwxLanguage() noexcept
            : wx_lang_code(wxLANGUAGE_DEFAULT), short_name(wxEmptyString), uname(wxEmptyString), name("System default") {}

		/**
		 * @brief Constructs a bwxLanguage object with specified parameters.
		 *
		 * @param sn Short identifier for the language.
		 * @param n Full name of the language.
		 * @param un Unique name for the language.
		 * @param wxl wxWidgets language code.
		 */
        bwxLanguage(const wxString& sn, const wxString& n, const wxString& un, wxLanguage wxl) noexcept
            : wx_lang_code(wxl), short_name(sn), uname(un), name(n) {}

		/**
		 * @brief Sets the wxWidgets language code.
		 *
		 * @param l The wxWidgets language code to set.
		 */
        void SetWxLangCode(wxLanguage l) noexcept { wx_lang_code = l; }

		/**
		* @brief Retrieves the wxWidgets language code.
		*/
        wxLanguage GetWxLangCode() const noexcept { return wx_lang_code; }
        
		/**
		* @brief Sets the short name for the language.
		*/
		void SetShortName(const wxString& sn) noexcept { short_name = sn; }
        
		/**
		* @brief Retrieves the short name for the language.
		*/
		const wxString& GetShortName() const noexcept { return short_name; }
        
		/**
		* @brief Sets the full name for the language.
		*/
		void SetName(const wxString& n) noexcept { name = n; }
		
		/**
		* @brief Retrieves the full name for the language.
		*/
		void SetName(const wxString& n) noexcept { name = n; }
        
		/**
		* @brief Retrieves the Unicode name for the language.
		*/
		const wxString& GetName() const noexcept { return name; }

		/**
		* @brief Sets the Unicode name for the language.
		*/
        void SetUnicodeName(const wxString& n) noexcept { uname = n; }
        
		/**
		* @brief Retrieves the Unicode name for the language.
		*/
		wxString GetUnicodeName() const noexcept { return uname.IsEmpty() ? name : uname; }

    private:
		wxLanguage wx_lang_code; ///< wxWidgets language code
		wxString short_name; ///< Short name
        wxString uname; ///< Unicode name
		wxString name; ///< Full name
    };

	WX_DECLARE_STRING_HASH_MAP(bwxLanguage, LangMap); ///< Hash map for language storage

	/**
	 * @brief Class for internationalization support.
	 *
	 * This class provides internationalization support for the BWXSDK library.
	 */
    class bwxInternat : public wxLocale {
    public:
        
		/**
		 * @brief Default constructor.
		 *
		 * Initializes the locale with the system default language and enables short catalog names.
		 */
        bwxInternat();
        
		/**
		 * @brief Constructs a bwxInternat object with a specified language.
		 *
		 * Adds the system default language, then adds the provided language, sets it as the default,
		 * and enables the use of short catalog names.
		 *
		 * @param l A bwxLanguage object representing the language to add.
		 */
		explicit bwxInternat(const bwxLanguage& l);

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
        bwxInternat(const wxString& short_name, const wxString& name, const wxString& uname, wxLanguage wx_lang_code);

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
        bool Init(const wxString& short_name = wxEmptyString);

		/**
		 * @brief Initializes the locale by the full language name.
		 *
		 * Searches the internal language map for a language whose full name matches the provided name.
		 * If found, the locale is initialized using the corresponding short name.
		 *
		 * @param name The full name of the language.
		 * @return True if initialization succeeded; false otherwise.
		 */
        bool InitByName(const wxString& name);

		/**
		*  @brief Sets the default application language.
		* 
		*  Updates the default language used by the application.
		* 
		*  @param l A bwxLanguage object representing the language to set.
		*/
        void SetDefaultAppLanguage(const bwxLanguage& l) noexcept { default_lang = l; }
        
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
		void SetDefaultAppLanguage(const wxString& short_name, const wxString& name, const wxString& uname, wxLanguage wx_lang_code) noexcept;

        bwxLanguage GetDefaultAppLanguage() const noexcept { return default_lang; }
        wxLanguage GetDefaultAppLanguageCode() const noexcept { return default_lang.GetWxLangCode(); }

		/**
		 * @brief Adds a language to the internal language map.
		 *
		 * @param l A bwxLanguage object to add.
		 */
        void AddLanguage(const bwxLanguage& l);

		/**
		 * @brief Adds a language to the internal language map using parameters.
		 *
		 * @param short_name Short identifier for the language.
		 * @param name Full name of the language.
		 * @param uname Unique name for the language.
		 * @param wx_lang_code wxWidgets language code.
		 */
        void AddLanguage(const wxString& short_name, const wxString& name, const wxString& uname, wxLanguage wx_lang_code);
        
		/**
		 * @brief Adds the system default language.
		 *
		 * This method adds a language entry representing the system default.
		 */
		void AddLanguageSystemDefault();

		/**
		 * @brief Adds English to the language map.
		 */
        void AddLanguageEnglish();

		/**
		 * @brief Adds Polish to the language map.
		 */
        void AddLanguagePolish();

		/**
		 * @brief Adds German to the language map.
		 */
        void AddLanguageGerman();

		/**
		 * @brief Retrieves the list of full language names.
		 *
		 * Iterates over the internal language map and returns an array of language names.
		 *
		 * @return A wxArrayString containing the names of all languages in the map.
		 */
        wxArrayString GetLangNames() const;

        void SetLangFolderName(const wxString& name) noexcept { lang_folder = name; }
        const wxString& GetLangFolderName() const noexcept { return lang_folder; }

        void UseShortCatalogNames() noexcept { use_short_catalog_names = true; }
        void DontUseShortCatalogNames() noexcept { use_short_catalog_names = false; }

		/**
		 * @brief Resets the locale to the default language.
		 *
		 * Re-initializes the locale using the default language's short name.
		 */
        void ResetToDefaultLanguage();

    private:
		bwxLanguage default_lang; ///< Default language
		LangMap lang_map; ///< Language map
		wxString lang_folder = bwxDEFAULT_LANG_FOLDER; ///< Language folder name
		bool use_short_catalog_names = true; ///< Flag for short catalog names

		/**
		 * @brief Loads language catalogs based on language information.
		 *
		 * This method constructs the catalog lookup path and attempts to load a series of catalogs
		 * (with or without short names) for the specified language.
		 *
		 * @param lang_info Pointer to wxLanguageInfo containing language details.
		 * @return True if all catalogs were loaded successfully; false if any catalog failed to load.
		 */
        bool LoadCatalogs(const wxLanguageInfo* lang_info);
    };

}

#endif