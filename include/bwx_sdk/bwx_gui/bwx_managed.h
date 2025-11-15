///////////////////////////////////////////////////////////////////////////////
// Name:        bwx_managed.h
// Purpose:     Template wrapper for wxWidgets controls with reactive behavior
// Author:      Bartosz Warzocha & Claude Code
// Created:     2025-11-15
// Copyright:   (c) 2025 Bartosz Warzocha
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include "bwx_reactive.h"
#include <wx/wx.h>

namespace bwx {
namespace gui {

/// @brief Managed control template - wraps wxWidgets controls with reactive behavior
///
/// **Purpose:** Adds automatic dynamic UI updates (font scaling, theme switching, etc.)
/// to any wxWidgets control through broadcast events.
///
/// **Pattern:** Multiple inheritance (concrete class + pure interface)
/// - `WxWidget` (e.g., wxButton, wxStaticText) - Concrete wxWidgets control
/// - `bwxReactive` - Pure interface for reactive behavior
///
/// **Safety:** Multiple inheritance is safe here because:
/// - wxButton is a concrete class with implementation
/// - bwxReactive is a pure interface (only virtual methods)
/// - No diamond problem (no common base class)
/// - wxWidgets uses this pattern extensively (wxTextCtrl, wxComboBox)
///
/// **Usage:**
/// ```cpp
/// // Instead of:
/// wxStaticText* label = new wxStaticText(parent, id, "Hello");
///
/// // Use:
/// bwx::StaticText* label = new bwx::StaticText(parent, id, "Hello");
///
/// // Optional: Disable reactions for specific control
/// label->setFontChangeEnabled(false);  // Don't update font on broadcast
/// ```
///
/// **Type Aliases (convenience):**
/// ```cpp
/// using StaticText = bwxManaged<wxStaticText>;
/// using Button = bwxManaged<wxButton>;
/// using TextCtrl = bwxManaged<wxTextCtrl>;
/// // ... etc.
/// ```
///
/// @tparam WxWidget Any wxWindow-derived control (wxButton, wxStaticText, etc.)
template<typename WxWidget>
class bwxManaged : public WxWidget, public bwxReactive {
public:
    // ========================================================================
    // Constructor (perfect forwarding to wxWidget)
    // ========================================================================

    /// @brief Constructor - forwards all arguments to wxWidget constructor
    /// @tparam Args Variadic template arguments (any wxWidget constructor signature)
    /// @param args Arguments forwarded to wxWidget constructor
    ///
    /// **Perfect forwarding:**
    /// - Accepts any number of arguments
    /// - Preserves lvalue/rvalue references
    /// - Calls appropriate wxWidget constructor
    ///
    /// **Automatic initialization:**
    /// - Calls wxWidget constructor first (base class)
    /// - Calls bwxReactive constructor (auto-registers in registry)
    /// - Stores original font size for accurate scaling
    ///
    /// **Example:**
    /// ```cpp
    /// // All wxStaticText constructors supported:
    /// auto* label1 = new bwx::StaticText(parent, id, "Text");
    /// auto* label2 = new bwx::StaticText(parent, id, "Text", pos, size);
    /// auto* label3 = new bwx::StaticText(parent, id, "Text", pos, size, style);
    /// ```
    template<typename... Args>
    bwxManaged(Args&&... args)
        : WxWidget(std::forward<Args>(args)...)
    {
        // Store original font size for accurate scaling
        // GetFont() returns current font (from wxWidget base class)
        m_originalFontSize = this->GetFont().GetPointSize();
    }

    // ========================================================================
    // Configuration Flags (granular control per instance)
    // ========================================================================

    /// @brief Enable/disable font change reactions for this control
    /// @param enabled true = react to font broadcasts, false = ignore
    ///
    /// **Use case:** Monospace text editors should not scale fonts
    /// ```cpp
    /// bwx::TextCtrl* editor = new bwx::TextCtrl(parent, id, "");
    /// editor->setFontChangeEnabled(false);  // Keep monospace font fixed
    /// ```
    void setFontChangeEnabled(bool enabled) { m_enableFontChange = enabled; }

    /// @brief Enable/disable theme change reactions for this control
    /// @param enabled true = react to theme broadcasts, false = ignore
    ///
    /// **Use case:** Custom-colored controls should not change with theme
    /// ```cpp
    /// bwx::Button* logo = new bwx::Button(parent, id, "Logo");
    /// logo->SetBackgroundColour(wxColour(255, 0, 0));  // Red brand color
    /// logo->setThemeChangeEnabled(false);  // Keep red always
    /// ```
    void setThemeChangeEnabled(bool enabled) { m_enableThemeChange = enabled; }

protected:
    // ========================================================================
    // bwxReactive Implementation (automatic reactions)
    // ========================================================================

    /// @brief Handle font scale change event (implements bwxReactive)
    /// @param scale New scaling factor (0.8 - 2.0 typical range)
    ///
    /// **Automatic behavior:**
    /// 1. Check if font changes are enabled
    /// 2. Calculate new font size: originalSize * scale
    /// 3. Update control font (uses wxWindow::SetFont)
    /// 4. Refresh control (visual update)
    ///
    /// **wxWindow API usage:**
    /// - this->GetFont() - inherited from wxWindow
    /// - this->SetFont() - inherited from wxWindow
    /// - this->Refresh() - inherited from wxWindow
    ///
    /// All wxWidgets controls inherit wxWindow, so this works universally!
    void onFontScaleChanged(double scale) override {
        if (!m_enableFontChange) return;

        // Calculate new font size based on original (not current!)
        // This prevents cumulative scaling errors
        int newSize = static_cast<int>(m_originalFontSize * scale);

        // Get current font from wxWidget
        wxFont font = this->GetFont();
        font.SetPointSize(newSize);

        // Update wxWidget font
        this->SetFont(font);

        // Refresh visual appearance
        this->Refresh();
    }

    /// @brief Handle theme change event (implements bwxReactive)
    /// @param themeName New theme name ("Light", "Dark", "Custom", etc.)
    ///
    /// **Current implementation:** Placeholder (Task #00044)
    ///
    /// **Future implementation (Task #00044):**
    /// ```cpp
    /// if (themeName == "Dark") {
    ///     this->SetBackgroundColour(wxColour(30, 30, 30));
    ///     this->SetForegroundColour(wxColour(220, 220, 220));
    /// } else {
    ///     this->SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    ///     this->SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
    /// }
    /// this->Refresh();
    /// ```
    void onThemeChanged(const std::string& themeName) override {
        if (!m_enableThemeChange) return;

        // TODO (Task #00044): Implement theme change handling
        // For now, this is a placeholder
        (void)themeName;  // Suppress unused parameter warning
    }

private:
    /// @brief Configuration flags (per-instance granular control)
    bool m_enableFontChange = true;   ///< React to font scale broadcasts?
    bool m_enableThemeChange = true;  ///< React to theme change broadcasts?

    /// @brief Original font size (stored at construction time)
    ///
    /// **Why store original?**
    /// - Prevents cumulative scaling errors
    /// - Example: 1.0x → 1.5x → 1.0x should return to original size
    /// - If we scaled from current size, we'd get drift over time
    ///
    /// **Initialization:** Constructor calls this->GetFont().GetPointSize()
    int m_originalFontSize = 10;
};

// ============================================================================
// Type Aliases (convenience names for common controls)
// ============================================================================

/// @brief Managed wxStaticText (reactive text label)
using StaticText = bwxManaged<wxStaticText>;

/// @brief Managed wxButton (reactive button)
using Button = bwxManaged<wxButton>;

/// @brief Managed wxTextCtrl (reactive text control)
///
/// **Note:** For code editors, consider disabling font changes:
/// ```cpp
/// bwx::TextCtrl* editor = new bwx::TextCtrl(...);
/// editor->setFontChangeEnabled(false);
/// ```
using TextCtrl = bwxManaged<wxTextCtrl>;

/// @brief Managed wxChoice (reactive choice control)
using Choice = bwxManaged<wxChoice>;

/// @brief Managed wxCheckBox (reactive checkbox)
using CheckBox = bwxManaged<wxCheckBox>;

/// @brief Managed wxRadioButton (reactive radio button)
using RadioButton = bwxManaged<wxRadioButton>;

/// @brief Managed wxSpinCtrl (reactive spin control)
using SpinCtrl = bwxManaged<wxSpinCtrl>;

/// @brief Managed wxSpinCtrlDouble (reactive double spin control)
using SpinCtrlDouble = bwxManaged<wxSpinCtrlDouble>;

/// @brief Managed wxSlider (reactive slider)
using Slider = bwxManaged<wxSlider>;

/// @brief Managed wxPanel (reactive panel)
using Panel = bwxManaged<wxPanel>;

// TODO (Task #00044): Add more type aliases as needed:
// - using ListBox = bwxManaged<wxListBox>;
// - using ComboBox = bwxManaged<wxComboBox>;
// - using TreeCtrl = bwxManaged<wxTreeCtrl>;
// - using ListCtrl = bwxManaged<wxListCtrl>;
// - using Notebook = bwxManaged<wxNotebook>;
// - using Splitter = bwxManaged<wxSplitterWindow>;

} // namespace gui
} // namespace bwx
