/// @file bwx_reactive_dialog.h
/// @brief Reactive dialog base class for automatic font/theme updates
///
/// Provides wxDialog subclass that automatically responds to broadcast
/// font/theme changes via bwxReactive Observer pattern.
///
/// **Architecture:**
/// - Inherits from wxDialog (GUI functionality)
/// - Inherits from bwxReactive (Observer pattern)
/// - Auto-registers in static registry on construction
/// - Auto-unregisters on destruction
/// - Handles sizer invalidation and layout refresh automatically
///
/// **Usage:**
/// ```cpp
/// class MyDialog : public bwx::gui::ReactiveDialog {
/// public:
///     MyDialog(wxWindow* parent)
///         : ReactiveDialog(parent, wxID_ANY, "Settings")
///     {
///         // Build UI normally
///         wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
///         sizer->Add(new wxButton(this, wxID_OK, "OK"));
///         SetSizer(sizer);
///     }
/// };
///
/// // Dialog automatically reacts to font broadcasts!
/// bwx::gui::bwxReactive::broadcastFontScaleChange(1.5);
/// ```

#pragma once

#include "bwx_reactive.h"
#include <wx/dialog.h>
#include <algorithm>

namespace bwx {
namespace gui {

/// @brief Reactive wxDialog that responds to font/theme broadcasts
///
/// Automatically handles sizer invalidation and layout refresh when
/// font scaling changes. Only grows (never shrinks) to prevent UI
/// from becoming unusable.
class bwxReactiveDialog : public wxDialog, public bwxReactive {
public:
    // ========================================================================
    // Constructors (forward to wxDialog)
    // ========================================================================

    /// @brief Constructor with all wxDialog parameters
    /// @param parent Parent window
    /// @param id Window ID
    /// @param title Dialog title
    /// @param pos Dialog position (default: centered on parent)
    /// @param size Dialog size (default: sizer-based)
    /// @param style Dialog style flags (default: standard dialog)
    /// @param name Window name (default: "dialogBox")
    bwxReactiveDialog(
        wxWindow* parent,
        wxWindowID id,
        const wxString& title,
        const wxPoint& pos = wxDefaultPosition,
        const wxSize& size = wxDefaultSize,
        long style = wxDEFAULT_DIALOG_STYLE,
        const wxString& name = wxDialogNameStr
    ) : wxDialog(parent, id, title, pos, size, style, name),
        bwxReactive()
    {
        // Dialog is now registered in bwxReactive static registry
    }

    /// @brief Virtual destructor
    virtual ~bwxReactiveDialog() = default;

protected:
    // ========================================================================
    // bwxReactive Implementation
    // ========================================================================

    /// @brief Handle font scale change (implements bwxReactive)
    /// @param scale New font scaling factor
    ///
    /// **Automatic behavior:**
    /// 1. Invalidate cached best sizes (forces sizer recalculation)
    /// 2. Recalculate layout (respects new font sizes)
    /// 3. Send size event (propagates to nested sizers)
    /// 4. Resize dialog if needed (only grow, never shrink)
    ///
    /// **Why this works:**
    /// - InvalidateBestSize() clears wxWindow's cached size
    /// - Layout() triggers sizer recalculation
    /// - SendSizeEvent() ensures nested wxStaticBoxSizer updates
    /// - Only grows to prevent UI from becoming unusable
    void onFontScaleChanged(double /*scale*/) override {
        // Step 1: Clear cached sizes (critical for wxStaticBoxSizer!)
        InvalidateBestSize();

        // Step 2: Recalculate layout with new font sizes
        Layout();

        // Step 3: Propagate size event to all nested sizers
        // wxSEND_EVENT_POST ensures it happens after current event processing
        SendSizeEvent(wxSEND_EVENT_POST);

        // Step 4: Resize dialog if content needs more space
        // Only grow, never shrink (prevents dialog from becoming too small)
        wxSize currentSize = GetSize();
        wxSize bestSize = GetBestSize();

        wxSize finalSize(
            std::max(currentSize.GetWidth(), bestSize.GetWidth()),
            std::max(currentSize.GetHeight(), bestSize.GetHeight())
        );

        if (finalSize != currentSize) {
            SetSize(finalSize);
        }

        // Step 5: Visual refresh
        Refresh();
    }

    /// @brief Handle theme change (implements bwxReactive)
    /// @param themeName New theme name
    ///
    /// **Default:** Does nothing (dialogs typically don't change with theme)
    ///
    /// **Override if needed:**
    /// ```cpp
    /// void onThemeChanged(const std::string& theme) override {
    ///     if (theme == "dark") {
    ///         SetBackgroundColour(*wxBLACK);
    ///     }
    /// }
    /// ```
    void onThemeChanged(const std::string& /*themeName*/) override {
        // Default: no theme handling for dialogs
        // Override in subclass if custom theme logic needed
    }
};

// ============================================================================
// Type Aliases (convenience name)
// ============================================================================

/// @brief Type alias for bwxReactiveDialog
///
/// **Usage:**
/// ```cpp
/// class SettingsDialog : public bwx::gui::ReactiveDialog {
///     // ... dialog implementation
/// };
/// ```
using ReactiveDialog = bwxReactiveDialog;

} // namespace gui
} // namespace bwx
