/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_sizer.h
// Purpose:     BWX_SDK Library; wxBoxSizer extension
// Author:      Name Surname <name.surname@email.org>
// Created:     2018-02-12
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_SIZER_H_
#define _BWX_SIZER_H_

#include "bwx_sdk/bwx_globals.h"
#include "bwx_sdk/bwx_core/bwx_core.h"

#include <wx/sizer.h>
#include <wx/stattext.h>
#include <wx/button.h>

#include <vector>

namespace bwx_sdk {

#define bwxSIZER_EXPAND true
#define bwxSIZER_NO_EXPAND false
#define bwxSIZER_LTOR true
#define bwxSIZER_NO_LTOR false

#define bwxSIZER_DEFAULT_MARGIN 5

	/**
	 * @brief Concept ensuring that only valid wxWidgets types (wxWindow or wxSizer) can be added to bwxBoxSizer.
	 */
	template<typename T>
	concept bwxIsValidTypeForSizer = std::is_base_of_v<wxWindow, std::remove_pointer_t<T>> ||
	                              std::is_base_of_v<wxSizer, std::remove_pointer_t<T>>;

	/**
	 * @brief Extension of wxBoxSizer for the BWX_SDK library.
	 *
	 * The bwxBoxSizer class provides additional functionality to the standard wxBoxSizer.
	 * It includes methods for adding controls and sizers with specific properties, such as
	 * alignment and expansion. It also supports adding labels to controls and managing margins
	 * between elements.
	 */
	class bwxBoxSizer : public wxBoxSizer
	{
		_inline(int, margin, Margin); ///< Margin between controls
		_inline(int, labelWidth, LabelWidth); ///< Width of the labels
		_inline(int, labelHmargin, LabelHMargin); ///< Horizontal margin of the labels
		_inline(int, labelVmargin, LabelVMargin); ///< Vertical margin of the labels

	public:
		/**
		 * @brief Constructor.
		 * @param orientation Orientation of the sizer.
		 */
		bwxBoxSizer(int orientation = wxVERTICAL);

		/**
		 * @brief Adds a control to the sizer.
		 * @param w Control to be added.
		 * @param prop Proportion.
		 * @param flags Flags.
		 * @param marg Margin.
		 * @return Pointer to the added wxSizerItem.
		 */
		wxSizerItem* Add(wxWindow* w, int prop, int flags, int marg);
		
		/**
		 * @brief Adds a sizer to the sizer.
		 * @param s Sizer to be added.
		 * @param prop Proportion.
		 * @param flags Flags.
		 * @param marg Margin.
		 * @return Pointer to the added wxSizerItem.
		 */
		wxSizerItem* Add(wxSizer* s, int prop, int flags, int marg);

		wxSizerItem* Add0(wxWindow* w) { return Add0Left(w); }
		wxSizerItem* Add1(wxWindow* w) { return Add1Left(w); }
		wxSizerItem* Add0Expand(wxWindow* w) { return Add0ExpandLeft(w); }
		wxSizerItem* Add1Expand(wxWindow* w) { return Add1ExpandLeft(w); }
		wxSizerItem* Add0Left(wxWindow* w);
		wxSizerItem* Add1Left(wxWindow* w);
		wxSizerItem* Add0ExpandLeft(wxWindow* w);
		wxSizerItem* Add1ExpandLeft(wxWindow* w);
		wxSizerItem* Add0Center(wxWindow* w);
		wxSizerItem* Add1Center(wxWindow* w);
		wxSizerItem* Add0ExpandCenter(wxWindow* w);
		wxSizerItem* Add1ExpandCenter(wxWindow* w);
		wxSizerItem* Add0Right(wxWindow* w);
		wxSizerItem* Add1Right(wxWindow* w);
		wxSizerItem* Add0ExpandRight(wxWindow* w);
		wxSizerItem* Add1ExpandRight(wxWindow* w);
		wxSizerItem* Add0CenterVertical(wxWindow* w);
		wxSizerItem* Add1CenterVertical(wxWindow* w);
		wxSizerItem* Add0ExpandCenterVertical(wxWindow* w);
		wxSizerItem* Add1ExpandCenterVertical(wxWindow* w);

		/**
		 * @brief Adds a control to the sizer.
		 * @param w Control to be added.
		 * @param prop Proportion.
		 * @return Pointer to the added wxSizerItem.
		 */
		wxSizerItem* AddX(wxWindow* w, int prop) { return AddXLeft(w, prop); }
		
		/**
		 * @brief Adds a control to the sizer.
		 * @param w Control to be added.
		 * @param prop Proportion.
		 * @return Pointer to the added wxSizerItem.
		 */
		wxSizerItem* AddXExpand(wxWindow* w, int prop) { return AddXExpandLeft(w, prop); }
		
		/**
		 * @brief Adds a control to the sizer.
		 * @param w Control to be added.
		 * @param prop Proportion.
		 * @return Pointer to the added wxSizerItem.
		 */
		wxSizerItem* AddXLeft(wxWindow* w, int prop);

		/**
		 * @brief Adds a control to the sizer.
		 * @param w Control to be added.
		 * @param prop Proportion.
		 * @return Pointer to the added wxSizerItem.
		 */
		wxSizerItem* AddXExpandLeft(wxWindow* w, int prop);

		/**
		* @brief Adds a panel to the sizer.
		*/
		wxSizerItem* AddPanel(wxWindow* p);

		wxSizerItem* Add0(wxSizer* s) { return Add0Left(s); }
		wxSizerItem* Add1(wxSizer* s) { return Add1Left(s); }
		wxSizerItem* Add0Expand(wxSizer* s) { return Add0ExpandLeft(s); }
		wxSizerItem* Add1Expand(wxSizer* s) { return Add1ExpandLeft(s); }
		wxSizerItem* Add0Left(wxSizer* s);
		wxSizerItem* Add1Left(wxSizer* s);
		wxSizerItem* Add0ExpandLeft(wxSizer* s);
		wxSizerItem* Add1ExpandLeft(wxSizer* s);
		wxSizerItem* Add0Center(wxSizer* s);
		wxSizerItem* Add1Center(wxSizer* s);
		wxSizerItem* Add0ExpandCenter(wxSizer* s);
		wxSizerItem* Add1ExpandCenter(wxSizer* s);
		wxSizerItem* Add0Right(wxSizer* s);
		wxSizerItem* Add1Right(wxSizer* s);
		wxSizerItem* Add0ExpandRight(wxSizer* s);
		wxSizerItem* Add1ExpandRight(wxSizer* s);
		wxSizerItem* Add0CenterVertical(wxSizer* s);
		wxSizerItem* Add1CenterVertical(wxSizer* s);
		wxSizerItem* Add0ExpandCenterVertical(wxSizer* s);
		wxSizerItem* Add1ExpandCenterVertical(wxSizer* s);

		/**
		 * @brief Adds a sizer to the sizer.
		 * @param s Sizer to be added.
		 * @param prop Proportion.
		 * @return Pointer to the added wxSizerItem.
		 */
		wxSizerItem* AddX(wxSizer* s, int prop) { return AddXLeft(s, prop); }

		/**
		 * @brief Adds a sizer to the sizer.
		 * @param s Sizer to be added.
		 * @param prop Proportion.
		 * @return Pointer to the added wxSizerItem.
		 */
		wxSizerItem* AddXExpand(wxSizer* s, int prop) { return AddXExpandLeft(s, prop); }

		/**
		 * @brief Adds a sizer to the sizer.
		 * @param s Sizer to be added.
		 * @param prop Proportion.
		 * @return Pointer to the added wxSizerItem.
		 */
		wxSizerItem* AddXLeft(wxSizer* s, int prop);

		/**
		 * @brief Adds a sizer to the sizer.
		 * @param s Sizer to be added.
		 * @param prop Proportion.
		 * @return Pointer to the added wxSizerItem.
		 */
		wxSizerItem* AddXExpandLeft(wxSizer* s, int prop);

		/**
		 * @brief Adds a label and a control to the sizer.
		 * @param w Control to be added.
		 * @param label Label text.
		 * @param expand Whether the control should expand.
		 * @param l_to_r Whether the label should be placed to the left of the control.
		 * @param orient Alignment of the label.
		 * @param extra_flags Additional flags to be applied.
		 * @return Pointer to the added wxSizerItem.
		 */
		wxSizerItem* AddWithLabel(wxWindow* w, const wxString& label, bool expand = true, bool l_to_r = false, wxAlignment orient = wxALIGN_LEFT, int extra_flags = 0);

		/**
		 * @brief Adds a label and a control to the sizer.
		 * @param label Label text.
		 * @param w Control to be added.
		 * @param expand Whether the control should expand.
		 * @param l_to_r Whether the label should be placed to the left of the control.
		 * @param orient Alignment of the label.
		 * @param extra_flags Additional flags to be applied.
		 * @return Pointer to the added wxSizerItem.
		 */
		wxSizerItem* AddButtons(wxWindow* btn1, wxWindow* btn2 = NULL, wxWindow* btn3 = NULL, wxWindow* btn4 = NULL, wxOrientation orient = wxHORIZONTAL, bwxAlign align = wxALIGN_RIGHT, int marg = bwxSIZER_DEFAULT_MARGIN);

		/**
		 * @brief Adds multiple controls to the sizer.
		 * @tparam Args Variadic template arguments ensuring only valid wxWidgets types.
		 * @param orient Orientation of the sizer (wxHORIZONTAL or wxVERTICAL).
		 * @param align Alignment of the controls.
		 * @param margin Margin applied between elements.
		 * @param args Controls to be added.
		 * @return Pointer to the added wxSizerItem.
		 */
		template<typename... Args>
			requires (bwxIsValidTypeForSizer<Args> && ...)
		wxSizerItem* AddCtrls(wxOrientation orient, wxAlignment align, int margin, Args... args)
		{
			std::vector<wxWindow*> controls;
			((args ? controls.push_back(args) : void()), ...);

			if (controls.empty()) return nullptr;

			wxBoxSizer* s = new wxBoxSizer(orient);

			if (orient == wxHORIZONTAL)
			{
				s->AddSpacer(GetMargin());
				for (wxWindow* ctrl : controls)
				{
					s->Add(ctrl, 0, wxTOP | wxRIGHT | wxBOTTOM, GetMargin());
				}
			}
			else
			{
				for (wxWindow* ctrl : controls)
				{
					s->Add(ctrl, 0, wxLEFT | wxTOP | wxRIGHT, 4);
				}
				s->AddSpacer(GetMargin());
			}

			return Add_(s, 0, align | wxALL, margin);
		}

	private:
		/**
		 * @brief Recalculates margins for the sizer items.
		 */
		void RecalculateMargins();

		/**
		 * @brief Internal method to add a window to the sizer.
		 */
		wxSizerItem* Add_(wxWindow* w, int prop = 1, int flags = 0, int marg = bwxSIZER_DEFAULT_MARGIN);

		/**
		 * @brief Internal method to add a sizer to the sizer.
		 */
		wxSizerItem* Add_(wxSizer* s, int prop = 1, int flags = 0, int marg = bwxSIZER_DEFAULT_MARGIN);
	};

/**
* @brief Horizontal box sizer.
*/
#define bwxBoxSizerH(name) bwxBoxSizer* name = new bwxBoxSizer(bwxHRZ)

/**
* @brief Vertical box sizer.
*/
#define bwxBoxSizerV(name) bwxBoxSizer* name = new bwxBoxSizer(bwxVRT)

/**
* @brief Sets a horizontal box sizer to a parent window.
*/
#define bwxSetBoxSizerH(name, parent) bwxBoxSizerH(name); parent->SetSizer(name)

/**
* @brief Sets a vertical box sizer to a parent window.
*/
#define bwxSetBoxSizerV(name, parent) bwxBoxSizerV(name); parent->SetSizer(name)

}

#endif // _BWX_SIZER_H_