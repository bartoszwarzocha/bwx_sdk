///////////////////////////////////////////////////////////////////////////////
// Name:        bwx_dpi_support.h
// Purpose:     CRTP base class for automatic DPI scaling support
// Author:      Bartosz Warzocha & Claude Code
// Created:     2025-11-17
// Copyright:   (c) 2025 Bartosz Warzocha
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <wx/wx.h>
#include <wx/aui/aui.h>
#include <functional>
#include <type_traits>

namespace bwx {
namespace gui {

/// @brief CRTP base class providing automatic DPI scaling support
///
/// Handles system DPI scaling (100%, 125%, 150%, 200%) automatically.
/// Provides both initial DPI setup and dynamic DPI change handling (multi-monitor).
///
/// **Architecture:** CRTP (Curiously Recurring Template Pattern)
/// - Zero overhead (compile-time polymorphism)
/// - Type-safe (template ensures correct base class)
/// - Clean API (type aliases hide template complexity)
///
/// **Usage Example:**
/// ```cpp
/// // Replace: class MainWindow : public wxFrame
/// class MainWindow : public bwx::gui::DPISupportedFrame {
/// public:
///     MainWindow()
///         : DPISupportedFrame(nullptr, wxID_ANY, "My App",
///                             wxDefaultPosition, wxSize(1200, 800))
///     {
///         // Create UI (menu, panels, controls)
///         createMenuBar();
///         createStatusBar();
///         initializeAUI();
///
///         // Initialize DPI AFTER UI is created
///         initializeDPI();
///     }
/// };
/// ```
///
/// **Platform Support:**
/// - Windows: Uses MSWUpdateFontOnDPIChange() for automatic font scaling
/// - Linux: Manual font scaling via SetFont(baseFont.Scaled(scaleFactor))
/// - macOS: Manual font scaling via SetFont(baseFont.Scaled(scaleFactor))
///
/// **Multi-Monitor:**
/// - Automatically handles wxDPIChangedEvent when window moves between monitors
/// - No manual intervention needed
///
/// **Coexists with bwxReactive:**
/// - DPISupport: System DPI scaling (100%, 125%, 150%, 200%)
/// - bwxReactive: User preferences (font slider, theme switcher)
/// - Both systems work independently and can coexist
///
/// @tparam TWindow Base wxWidgets window type (wxFrame, wxDialog, wxPanel)
template<typename TWindow>
class DPISupport : public TWindow {
public:
    // ========================================================================
    // Constructor / Destructor (Perfect Forwarding)
    // ========================================================================

    /// @brief Constructor with perfect forwarding to base class
    ///
    /// **Usage:** Pass all wxFrame/wxDialog/wxPanel constructor arguments
    /// ```cpp
    /// DPISupportedFrame(nullptr, wxID_ANY, "Title", pos, size, style)
    /// ```
    template<typename... Args>
    DPISupport(Args&&... args)
        : TWindow(std::forward<Args>(args)...)
    {
        // Dynamically bind DPI changed event (works with templates)
        // Note: Using Bind() instead of event table because templates
        // don't work with wxBEGIN_EVENT_TABLE macro
        this->Bind(wxEVT_DPI_CHANGED, &DPISupport::onDPIChanged, this);
    }

    /// @brief Destructor (automatic event unbinding)
    ///
    /// wxWidgets automatically unbinds events when window is destroyed.
    /// No manual cleanup needed.
    virtual ~DPISupport() = default;

protected:
    // ========================================================================
    // Public API (called by derived classes)
    // ========================================================================

    /// @brief Initialize DPI scaling (call AFTER UI is fully created)
    ///
    /// **IMPORTANT:** Call this method AFTER creating all UI elements
    /// (menu bar, status bar, panels, controls). DPI scaling must be
    /// applied to a complete UI hierarchy.
    ///
    /// **What it does:**
    /// 1. Gets current DPI scale factor (1.0 = 96 DPI, 1.25 = 120 DPI, etc.)
    /// 2. Platform-specific font initialization:
    ///    - Windows: MSWUpdateFontOnDPIChange() (automatic)
    ///    - Linux/macOS: Manual font scaling
    /// 3. Scales special controls (StatusBar, wxAuiNotebook, wxAUI captions)
    /// 4. Refreshes layout
    ///
    /// **Example:**
    /// ```cpp
    /// MainWindow::MainWindow() : DPISupportedFrame(...) {
    ///     createMenuBar();
    ///     createStatusBar();
    ///     initializeAUI();
    ///     // ... other UI creation ...
    ///
    ///     initializeDPI();  // <-- Call here, after UI is ready
    /// }
    /// ```
    ///
    /// **Thread Safety:** Must be called from GUI thread only.
    void initializeDPI();

    /// @brief Get current DPI scale factor
    /// @return Scale factor (1.0 = 96 DPI, 1.25 = 120 DPI, 1.5 = 144 DPI, 2.0 = 192 DPI)
    ///
    /// **Platform Behavior:**
    /// - Windows: Uses GetDPI() API (Per-Monitor V2 DPI aware)
    /// - Linux: Uses GetDPIScaleFactor() (Wayland/X11 scaling)
    /// - macOS: Uses GetDPIScaleFactor() (Retina display support)
    ///
    /// **Usage:**
    /// ```cpp
    /// double scale = getDPIScaleFactor();
    /// int scaledSize = static_cast<int>(baseSize * scale);
    /// ```
    double getDPIScaleFactor() const {
        return this->GetDPIScaleFactor();
    }

private:
    // ========================================================================
    // Internal State
    // ========================================================================

    bool m_dpiInitialized = false;  ///< Prevents double initialization

    // ========================================================================
    // Internal Implementation (platform-specific)
    // ========================================================================

    /// @brief Platform-specific DPI initialization
    ///
    /// **Windows:**
    /// - Calls MSWUpdateFontOnDPIChange(GetDPI())
    /// - This automatically scales ALL fonts in window hierarchy
    /// - MenuBar, StatusBar, controls, panels - all scaled by wxWidgets
    ///
    /// **Linux/macOS:**
    /// - Manual font scaling via SetFont(baseFont.Scaled(scaleFactor))
    /// - Propagates font to child windows automatically via wxWidgets
    ///
    /// **Implementation Note:**
    /// This method is called by initializeDPI() and is platform-specific.
    void platformInitializeDPI();

    /// @brief Apply DPI scaling to window and children
    /// @param scaleFactor Current DPI scale factor
    ///
    /// **What it does:**
    /// 1. Scales special controls (StatusBar, wxAuiNotebook, wxAUI captions)
    /// 2. Refreshes layout (Layout() + Refresh())
    /// 3. Updates wxAuiManager if present (for wxFrame)
    ///
    /// **Called by:**
    /// - initializeDPI() (once at startup)
    /// - onDPIChanged() (when DPI changes dynamically)
    void applyDPIScaling(double scaleFactor);

    /// @brief Scale special controls that don't inherit fonts automatically
    /// @param scaleFactor Current DPI scale factor
    ///
    /// **Special Controls Handled:**
    ///
    /// 1. **StatusBar:** Always scaled
    ///    - SetFont(GetFont().Scaled(scaleFactor))
    ///
    /// 2. **MenuBar:** DON'T scale on Windows!
    ///    - Windows Issue #22957: SetFont() causes sizing bugs
    ///    - System handles MenuBar scaling via DPI manifest
    ///    - Linux/macOS: Can be scaled if needed (future enhancement)
    ///
    /// 3. **wxAuiNotebook:** Scaled via wxAuiTabArt
    ///    - Recursive search for all wxAuiNotebook instances
    ///    - Scale via SetNormalFont(), SetSelectedFont(), SetMeasuringFont()
    ///    - Also scale wxAuiTabArt (the actual tab renderer)
    ///
    /// 4. **wxAUI Captions:** Scaled via wxAuiDockArt (only for wxFrame)
    ///    - Compile-time check: std::is_base_of<wxFrame, TWindow>
    ///    - Scale pane caption fonts (Navigator, Properties, etc.)
    ///
    /// **Implementation Note:**
    /// Uses recursive lambda for wxAuiNotebook search (nested notebooks supported).
    void scaleSpecialControls(double scaleFactor);

    /// @brief Handle DPI change event (multi-monitor scenarios)
    /// @param event wxDPIChangedEvent with old/new DPI
    ///
    /// **When Fired:**
    /// User drags window between monitors with different DPI settings.
    /// Example: 100% DPI monitor → 150% DPI monitor
    ///
    /// **What it does:**
    /// 1. Calls event.Skip() to let wxWidgets update fonts automatically
    /// 2. Gets new DPI scale factor
    /// 3. Re-applies scaling to special controls
    /// 4. Refreshes layout
    ///
    /// **Platform Support:**
    /// - Windows: Full support (Per-Monitor V2 DPI aware)
    /// - Linux: Partial support (depends on Wayland/X11)
    /// - macOS: N/A (Retina display is fixed DPI)
    ///
    /// **Bound Dynamically:**
    /// This method is bound in constructor via Bind(wxEVT_DPI_CHANGED, ...)
    /// instead of using event table (templates don't support event tables).
    void onDPIChanged(wxDPIChangedEvent& event);
};

// ============================================================================
// Type Aliases (Clean API)
// ============================================================================

/// @brief DPI-aware wxFrame (use instead of wxFrame)
///
/// **Usage:**
/// ```cpp
/// class MainWindow : public bwx::gui::DPISupportedFrame {
///     // Automatic DPI scaling!
/// };
/// ```
using DPISupportedFrame = DPISupport<wxFrame>;

/// @brief DPI-aware wxDialog (use instead of wxDialog)
///
/// **Usage:**
/// ```cpp
/// class SettingsDialog : public bwx::gui::DPISupportedDialog {
///     // Automatic DPI scaling!
/// };
/// ```
using DPISupportedDialog = DPISupport<wxDialog>;

/// @brief DPI-aware wxPanel (use instead of wxPanel)
///
/// **Usage:**
/// ```cpp
/// class MyPanel : public bwx::gui::DPISupportedPanel {
///     // Automatic DPI scaling!
/// };
/// ```
///
/// **Note:** Panels typically inherit font from parent window,
/// so DPISupportedPanel may not be needed unless panel has
/// special controls (wxAuiNotebook, custom rendering, etc.)
using DPISupportedPanel = DPISupport<wxPanel>;

} // namespace gui
} // namespace bwx
