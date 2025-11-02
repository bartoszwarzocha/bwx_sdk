/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_sizer.cpp
// Purpose:     BWX_SDK Library; wxBoxSizer extension
// Author:      Name Surname <name.surname@email.org>
// Created:     2018-02-12
// Copyright:   (c) 2025 by Name Surname
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h> 
#endif

#include <bwx_sdk/bwx_gui/bwx_sizer.h>

namespace bwx_sdk {

	bwxBoxSizer::bwxBoxSizer(int orientation) 
		: wxBoxSizer(orientation), m_margin(5), m_labelWidth(90), m_labelHmargin(10), m_labelVmargin(5)
	{

	}

	wxSizerItem* bwxBoxSizer::Add_(wxWindow* w, int prop, int flags, int marg)
	{
		wxSizerItem* ret = wxBoxSizer::Add(w, prop, flags, marg);
		RecalculateMargins();
		RecalcSizes();
		return ret;
	}

	wxSizerItem* bwxBoxSizer::Add_(wxSizer* s, int prop, int flags, int marg)
	{
		wxSizerItem* ret = wxBoxSizer::Add(s, prop, flags, marg);
		RecalculateMargins();
		RecalcSizes();
		return ret;
	}

	void bwxBoxSizer::RecalculateMargins()
	{
		wxSizerItem* si = nullptr;

		for (unsigned int i = 0; i < (unsigned int)this->GetItemCount(); i++)
		{
			si = this->GetItem(i);

			if (si)
			{
				int f = si->GetFlag();

				if (i == 0)
				{
					bwxAddByteFlag(f, wxALL);
					si->SetFlag(f);
				}
				else
				{
					if (bwxIsByteFlagSet(f, wxALL))
					{
						bwxRemoveByteFlag(f, wxALL);
					}

					if (this->GetOrientation() == wxVERTICAL)
					{
						if (bwxIsByteFlagSet(f, wxTOP))
						{
							bwxRemoveByteFlag(f, wxTOP);
						}
						si->SetFlag(f | wxLEFT | wxBOTTOM | wxRIGHT);
					}
					else
					{
						if (bwxIsByteFlagSet(f, wxLEFT))
						{
							bwxRemoveByteFlag(f, wxLEFT);
						}
						si->SetFlag(f | wxTOP | wxRIGHT | wxBOTTOM);
					}
				}
			}
		}

		wxBoxSizer::RecalcSizes();
	}

	wxSizerItem* bwxBoxSizer::Add(wxWindow* w, int prop, int flags, int marg)
	{
		return Add_(w, prop, flags, marg);
	}

	wxSizerItem* bwxBoxSizer::Add(wxSizer* s, int prop, int flags, int marg)
	{
		return Add_(s, prop, flags, marg);
	}

	wxSizerItem* bwxBoxSizer::Add0Left(wxWindow* w)
	{
		return Add_(w, 0, wxALIGN_LEFT | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add1Left(wxWindow* w)
	{
		return Add_(w, 1, wxALIGN_LEFT | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add0ExpandLeft(wxWindow* w)
	{
		return Add_(w, 0, wxALIGN_LEFT | wxEXPAND | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add1ExpandLeft(wxWindow* w)
	{
		return Add_(w, 1, wxALIGN_LEFT | wxEXPAND | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add0Center(wxWindow* w)
	{
		return Add_(w, 0, wxALIGN_CENTER | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add1Center(wxWindow* w)
	{
		return Add_(w, 1, wxALIGN_CENTER | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add0ExpandCenter(wxWindow* w)
	{
		return Add_(w, 0, wxALIGN_CENTER | wxEXPAND | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add1ExpandCenter(wxWindow* w)
	{
		return Add_(w, 1, wxALIGN_CENTER | wxEXPAND | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add0Right(wxWindow* w)
	{
		return Add_(w, 0, wxALIGN_RIGHT | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add1Right(wxWindow* w)
	{
		return Add_(w, 1, wxALIGN_RIGHT | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add0ExpandRight(wxWindow* w)
	{
		return Add_(w, 0, wxALIGN_RIGHT | wxEXPAND | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add1ExpandRight(wxWindow* w)
	{
		return Add_(w, 1, wxALIGN_RIGHT | wxEXPAND | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add0CenterVertical(wxWindow* w)
	{
		return Add_(w, 0, wxALIGN_CENTER_VERTICAL | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add1CenterVertical(wxWindow* w)
	{
		return Add_(w, 1, wxALIGN_CENTER_VERTICAL | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add0ExpandCenterVertical(wxWindow* w)
	{
		return Add_(w, 0, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::Add1ExpandCenterVertical(wxWindow* w)
	{
		return Add_(w, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::AddXLeft(wxWindow* w, int prop)
	{
		return Add_(w, prop, wxALIGN_LEFT | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::AddXExpandLeft(wxWindow* w, int prop)
	{
		return Add_(w, prop, wxALIGN_LEFT | wxEXPAND | wxALL, GetMargin());
	}

	wxSizerItem* bwxBoxSizer::AddPanel(wxWindow* p)
	{
		return Add_(p, 1, wxEXPAND | wxALL, 0);
	}

	// Sizer
	wxSizerItem* bwxBoxSizer::Add0Left(wxSizer* s)
	{
		return Add_(s, 0, wxALIGN_LEFT | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add1Left(wxSizer* s)
	{
		return Add_(s, 1, wxALIGN_LEFT | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add0ExpandLeft(wxSizer* s)
	{
		return Add_(s, 0, wxALIGN_LEFT | wxEXPAND | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add1ExpandLeft(wxSizer* s)
	{
		return Add_(s, 1, wxALIGN_LEFT | wxEXPAND | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add0Center(wxSizer* s)
	{
		return Add_(s, 0, wxALIGN_CENTER | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add1Center(wxSizer* s)
	{
		return Add_(s, 1, wxALIGN_CENTER | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add0ExpandCenter(wxSizer* s)
	{
		return Add_(s, 0, wxALIGN_CENTER | wxEXPAND | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add1ExpandCenter(wxSizer* s)
	{
		return Add_(s, 1, wxALIGN_CENTER | wxEXPAND | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add0Right(wxSizer* s)
	{
		return Add_(s, 0, wxALIGN_RIGHT | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add1Right(wxSizer* s)
	{
		return Add_(s, 1, wxALIGN_RIGHT | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add0ExpandRight(wxSizer* s)
	{
		return Add_(s, 0, wxALIGN_RIGHT | wxEXPAND | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add1ExpandRight(wxSizer* s)
	{
		return Add_(s, 1, wxALIGN_RIGHT | wxEXPAND | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add0CenterVertical(wxSizer* s)
	{
		return Add_(s, 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add1CenterVertical(wxSizer* s)
	{
		return Add_(s, 1, wxALIGN_CENTER_VERTICAL | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add0ExpandCenterVertical(wxSizer* s)
	{
		return Add_(s, 0, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::Add1ExpandCenterVertical(wxSizer* s)
	{
		return Add_(s, 1, wxALIGN_CENTER_VERTICAL | wxEXPAND | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::AddXLeft(wxSizer* s, int prop)
	{
		return Add_(s, prop, wxALIGN_LEFT | wxALL, 0);
	}

	wxSizerItem* bwxBoxSizer::AddXExpandLeft(wxSizer* s, int prop)
	{
		return Add_(s, prop, wxALIGN_LEFT | wxEXPAND | wxALL, 0);
	}


	wxSizerItem* bwxBoxSizer::AddWithLabel(wxWindow* w, const wxString& label, bool expand, bool l_to_r, wxAlignment orient, int extra_flags)
	{
		wxBoxSizer* contener = new wxBoxSizer((orient == wxALIGN_LEFT) ? wxHORIZONTAL : wxVERTICAL);
		if (!label.IsEmpty())
		{
			contener->Add(new wxStaticText(w->GetParent(), wxID_ANY, label, wxDefaultPosition, wxSize(GetLabelWidth(), -1), ((l_to_r) ? (wxALIGN_RIGHT) : (0))), 0, wxALIGN_CENTER_VERTICAL | wxALL, 0);
			contener->AddSpacer((orient == wxALIGN_LEFT) ? GetLabelHMargin() : GetLabelVMargin());
		}
		contener->Add(w, 1, wxEXPAND | wxALL, 0);
		return Add_(contener, 0, (expand) ? (wxEXPAND | wxALL) : (wxALL) | extra_flags, GetMargin());
	}


	wxSizerItem* bwxBoxSizer::AddButtons(wxWindow* btn1, wxWindow* btn2, wxWindow* btn3, wxWindow* btn4, wxOrientation orient, bwxAlign align, int marg)
	{
		if (btn1 == NULL && btn2 == NULL && btn3 == NULL && btn4 == NULL) return NULL;

		wxBoxSizer* row = new wxBoxSizer(orient);

		if (orient == wxHORIZONTAL)
		{
			row->AddSpacer(GetMargin());
			if (btn1 != NULL) row->Add(btn1, 0, wxTOP | wxRIGHT | wxBOTTOM, GetMargin());
			if (btn2 != NULL) row->Add(btn2, 0, wxTOP | wxRIGHT | wxBOTTOM, GetMargin());
			if (btn3 != NULL) row->Add(btn3, 0, wxTOP | wxRIGHT | wxBOTTOM, GetMargin());
			if (btn4 != NULL) row->Add(btn4, 0, wxTOP | wxRIGHT | wxBOTTOM, GetMargin());
		}
		else
		{
			if (btn1 != NULL) row->Add(btn1, 0, wxLEFT | wxTOP | wxRIGHT, 4);
			if (btn2 != NULL) row->Add(btn2, 0, wxLEFT | wxTOP | wxRIGHT, 4);
			if (btn3 != NULL) row->Add(btn3, 0, wxLEFT | wxTOP | wxRIGHT, 4);
			if (btn4 != NULL) row->Add(btn4, 0, wxLEFT | wxTOP | wxRIGHT, 4);
			row->AddSpacer(GetMargin());
		}

		return Add_(row, 0, align | wxALL, marg);
	}

}