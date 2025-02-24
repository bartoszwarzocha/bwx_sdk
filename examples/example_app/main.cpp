/////////////////////////////////////////////////////////////////////////////
// Name:        main.cpp
// Purpose:     BWX_SDK Libarary Example App
// Author:      Bartosz Warzocha
// Created:     2015-01-01
// RCS-ID:
// Copyright:   (c) Bartosz Warzocha (bartosz.warzocha@gmail.com)
// Licence:		wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wx.h>
#include <bwx_sdk/bwx_core/bwx_core.h>

class MyApp : public wxApp {
public:
    virtual bool OnInit() override;
};

class MyFrame : public wxFrame {
public:
    MyFrame()
        : wxFrame(nullptr, wxID_ANY, "Example BWX_SDK Application", wxDefaultPosition, wxSize(400, 300)) {
        wxPanel* panel = new wxPanel(this);
        wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);

        wxButton* btn = new wxButton(panel, wxID_ANY, "Test BWX_SDK function");

        btn->Bind(wxEVT_BUTTON, [](wxCommandEvent&) { bwx_sdk::bwxStdPathsInfo(); });

        sizer->Add(btn, 0, wxALL | wxEXPAND, 10);

        panel->SetSizer(sizer);
    }
};

bool MyApp::OnInit() {
    MyFrame* frame = new MyFrame();
    frame->Show(true);
    return true;
}

wxIMPLEMENT_APP(MyApp);