///////////////////////////////////////////////////////////////////////////////
// Name:        bwx_dpi_support.cpp
// Purpose:     Implementation of DPISupport template class
// Author:      Bartosz Warzocha & Claude Code
// Created:     2025-11-17
// Copyright:   (c) 2025 Bartosz Warzocha
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#include <bwx_sdk/bwx_gui/bwx_dpi_support.h>
#include <wx/aui/auibook.h>
#include <wx/aui/tabart.h>

namespace bwx {
namespace gui {

// ============================================================================
// initializeDPI() - Main initialization method
// ============================================================================

template<typename TWindow>
void DPISupport<TWindow>::initializeDPI() {
    // Prevent double initialization
    if (m_dpiInitialized) {
        return;
    }
    m_dpiInitialized = true;

    // Get current DPI scale factor
    double scaleFactor = getDPIScaleFactor();

    // Platform-specific font initialization
    platformInitializeDPI();

    // Apply DPI scaling to special controls
    applyDPIScaling(scaleFactor);
}

// ============================================================================
// platformInitializeDPI() - Platform-specific initialization
// ============================================================================

template<typename TWindow>
void DPISupport<TWindow>::platformInitializeDPI() {
    // Get system default font and scale factor
    wxFont baseFont = wxSystemSettings::GetFont(wxSYS_DEFAULT_GUI_FONT);
    double scaleFactor = getDPIScaleFactor();
    wxFont scaledFont = baseFont.Scaled(scaleFactor);

    // Set scaled font on this window
    this->SetFont(scaledFont);

    // CRITICAL: wxWidgets does NOT automatically propagate fonts!
    // We must manually propagate to all child windows.
    // EXCEPTION: Skip wxAuiNotebook (handled separately in scaleSpecialControls)
    std::function<void(wxWindow*)> propagateFont;
    propagateFont = [&](wxWindow* window) {
        if (!window) return;

        // Skip wxAuiNotebook - it will be scaled in scaleSpecialControls()
        // to avoid double scaling (propagation + special handling)
        if (dynamic_cast<wxAuiNotebook*>(window)) {
            return;  // Don't propagate to wxAuiNotebook
        }

        // Set font on this window
        window->SetFont(scaledFont);

        // Recursively propagate to all children
        for (wxWindow* child : window->GetChildren()) {
            propagateFont(child);
        }
    };

    // Start propagation from this window's children
    propagateFont(this);
}

// ============================================================================
// applyDPIScaling() - Apply scaling to window and children
// ============================================================================

template<typename TWindow>
void DPISupport<TWindow>::applyDPIScaling(double scaleFactor) {
    // Scale special controls that don't inherit fonts automatically
    scaleSpecialControls(scaleFactor);

    // Refresh layout to apply changes
    this->Layout();
    this->Refresh();

    // If this is a wxFrame with wxAuiManager, update it
    // Compile-time check using std::is_base_of
    if constexpr (std::is_base_of_v<wxFrame, TWindow>) {
        wxAuiManager* auiMgr = wxAuiManager::GetManager(this);
        if (auiMgr) {
            auiMgr->Update();
        }
    }
}

// ============================================================================
// scaleSpecialControls() - Scale StatusBar, wxAuiNotebook, wxAUI captions
// ============================================================================

template<typename TWindow>
void DPISupport<TWindow>::scaleSpecialControls(double scaleFactor) {
    // ========================================================================
    // 1. StatusBar - Always scale (only for wxFrame)
    // ========================================================================
    // Compile-time check: Only wxFrame has GetStatusBar()
    if constexpr (std::is_base_of_v<wxFrame, TWindow>) {
        if (wxStatusBar* statusBar = this->GetStatusBar()) {
            wxFont font = statusBar->GetFont();
            statusBar->SetFont(font.Scaled(scaleFactor));
        }
    }

    // ========================================================================
    // 2. MenuBar - Scale on all platforms (Issue #22957 FIXED in wxWidgets 3.3.1)
    // ========================================================================
    // wxWidgets Issue #22957 (MenuBar SetFont causing sizing bugs on high DPI)
    // was fixed in PR #22958 (November 2022) and included in wxWidgets 3.3.1.
    // The fix ensures correct DPI scaling for owner-drawn menu items.
    //
    // Compile-time check: Only wxFrame has GetMenuBar()
    if constexpr (std::is_base_of_v<wxFrame, TWindow>) {
        wxMenuBar* menuBar = this->GetMenuBar();
        if (menuBar) {
            wxFont font = menuBar->GetFont();
            menuBar->SetFont(font.Scaled(scaleFactor));
        }
    }

    // ========================================================================
    // 3. wxAuiNotebook - Scale via wxAuiTabArt (recursive search)
    // ========================================================================
    // wxAuiNotebook tabs don't inherit font from window hierarchy.
    // We must explicitly set fonts on:
    // - The notebook itself (SetFont, SetNormalFont, SetSelectedFont, SetMeasuringFont)
    // - The wxAuiTabArt (tab renderer)
    //
    // Use recursive lambda to find all wxAuiNotebook instances in hierarchy.
    std::function<void(wxWindow*)> scaleNotebooks;
    scaleNotebooks = [&](wxWindow* window) {
        if (!window) return;

        // Check if this is a wxAuiNotebook
        if (wxAuiNotebook* notebook = dynamic_cast<wxAuiNotebook*>(window)) {
            wxFont font = notebook->GetFont();
            wxFont scaledFont = font.Scaled(scaleFactor);

            // Set fonts on notebook
            notebook->SetFont(scaledFont);
            notebook->SetNormalFont(scaledFont);
            notebook->SetSelectedFont(scaledFont);
            notebook->SetMeasuringFont(scaledFont);

            // Set fonts on wxAuiTabArt (the actual tab renderer)
            if (wxAuiTabArt* tabArt = notebook->GetArtProvider()) {
                tabArt->SetNormalFont(scaledFont);
                tabArt->SetSelectedFont(scaledFont);
                tabArt->SetMeasuringFont(scaledFont);
            }
        }

        // Recursively check all children (supports nested notebooks)
        for (wxWindow* child : window->GetChildren()) {
            scaleNotebooks(child);
        }
    };

    // Start recursive search from this window
    scaleNotebooks(this);

    // ========================================================================
    // 4. wxAUI Captions - Scale via wxAuiDockArt (only for wxFrame)
    // ========================================================================
    // wxAUI pane captions (Navigator, Properties, Statistics, etc.) are drawn
    // by wxAuiDockArt and don't inherit font from window hierarchy.
    //
    // Compile-time check: Only apply to wxFrame (not wxDialog or wxPanel)
    if constexpr (std::is_base_of_v<wxFrame, TWindow>) {
        wxAuiManager* auiMgr = wxAuiManager::GetManager(this);
        if (auiMgr) {
            wxAuiDockArt* art = auiMgr->GetArtProvider();
            if (art) {
                // Get current font and scale it
                wxFont font = this->GetFont();
                wxFont scaledFont = font.Scaled(scaleFactor);

                // Set scaled font for pane captions
                art->SetFont(wxAUI_DOCKART_CAPTION_FONT, scaledFont);
            }
        }
    }
}

// ============================================================================
// onDPIChanged() - Event handler for dynamic DPI changes (multi-monitor)
// ============================================================================

template<typename TWindow>
void DPISupport<TWindow>::onDPIChanged(wxDPIChangedEvent& event) {
    // Let wxWidgets handle font updates automatically
    // This is important! MSWUpdateFontOnDPIChange() is called internally
    event.Skip();

    // Get new DPI scale factor
    wxSize newDPI = event.GetNewDPI();
    double newScale = static_cast<double>(newDPI.x) / 96.0;

    // Re-apply scaling to special controls
    // (StatusBar, wxAuiNotebook, wxAUI captions)
    applyDPIScaling(newScale);
}

// ============================================================================
// Explicit Template Instantiations
// ============================================================================
// These are required to generate code for specific template types.
// Without these, linker will fail with "undefined reference" errors.

template class DPISupport<wxFrame>;
template class DPISupport<wxDialog>;
template class DPISupport<wxPanel>;

} // namespace gui
} // namespace bwx
