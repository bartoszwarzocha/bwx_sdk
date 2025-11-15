///////////////////////////////////////////////////////////////////////////////
// Name:        bwx_reactive.h
// Purpose:     Base class for reactive GUI controls with broadcast capability
// Author:      Bartosz Warzocha & Claude Code
// Created:     2025-11-15
// Copyright:   (c) 2025 Bartosz Warzocha
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <string>

namespace bwx {
namespace gui {

/// @brief Base class for all reactive GUI controls
///
/// Provides static registry and broadcast mechanism for dynamic UI updates.
/// All managed controls inherit from this class and implement event handlers.
///
/// **Architecture:**
/// - Static registry: All live controls auto-register on construction
/// - Broadcast API: Centralized event distribution (font scale, theme, etc.)
/// - Pure virtual handlers: Subclasses must implement reactions
///
/// **Usage:**
/// ```cpp
/// // In MainWindow or coordinator:
/// bwxReactive::broadcastFontScaleChange(1.5);  // All controls update!
///
/// // In custom control:
/// class MyControl : public wxButton, public bwxReactive {
///     void onFontScaleChanged(double scale) override {
///         // Update this control's font
///     }
/// };
/// ```
///
/// **Thread Safety:** Single-threaded (GUI thread only) for MVP.
/// Future: Add mutex for multi-threaded safety if needed.
class bwxReactive {
public:
    // ========================================================================
    // Broadcast API (called by MainWindow or other coordinators)
    // ========================================================================

    /// @brief Broadcast font scale change to all registered controls
    /// @param scale New font scaling factor (0.8 - 2.0 recommended range)
    ///
    /// Iterates through all registered controls and calls onFontScaleChanged().
    /// Controls can opt-out by checking enable flags in their implementation.
    static void broadcastFontScaleChange(double scale);

    /// @brief Broadcast theme change to all registered controls
    /// @param themeName New theme name ("Light", "Dark", "Custom", etc.)
    ///
    /// Iterates through all registered controls and calls onThemeChanged().
    /// Allows dynamic theme switching without application restart.
    static void broadcastThemeChange(const std::string& themeName);

    /// @brief Get number of registered reactive controls (diagnostics)
    /// @return Number of controls in static registry
    ///
    /// **Usage:** For debugging and verification
    /// ```cpp
    /// size_t count = bwxReactive::getRegisteredControlsCount();
    /// Logger::getInstance().debug("Reactive controls registered: {}", count);
    /// ```
    static size_t getRegisteredControlsCount();

    // Future extensibility:
    // static void broadcastAccessibilityChange(bool enabled);
    // static void broadcastLanguageChange(const std::string& locale);
    // static void broadcastHighContrastChange(bool enabled);
    // static void broadcastAnimationStateChange(bool enabled);

protected:
    // ========================================================================
    // Lifecycle (automatic registration/unregistration)
    // ========================================================================

    /// @brief Constructor - registers control in static registry
    ///
    /// Automatically called when control is created.
    /// Adds `this` pointer to static registry vector.
    bwxReactive();

    /// @brief Destructor - unregisters control from static registry
    ///
    /// Automatically called when control is destroyed.
    /// Removes `this` pointer from static registry vector.
    ///
    /// **Important:** Derived classes must call this destructor properly.
    /// Multiple inheritance is safe: wxButton dtor → bwxReactive dtor.
    virtual ~bwxReactive();

    // ========================================================================
    // Event Handlers (pure virtual - subclasses must implement)
    // ========================================================================

    /// @brief Handle font scale change event
    /// @param scale New scaling factor (0.8 - 2.0 typical range)
    ///
    /// **Implementation guidelines:**
    /// - Check enable flag (if control has opt-out)
    /// - Calculate new font size: baseSize * scale
    /// - Update control font: SetFont(newFont)
    /// - Refresh control: Refresh()
    ///
    /// **Example:**
    /// ```cpp
    /// void onFontScaleChanged(double scale) override {
    ///     if (!m_enableFontChange) return;
    ///     int newSize = static_cast<int>(m_originalFontSize * scale);
    ///     wxFont font = GetFont();
    ///     font.SetPointSize(newSize);
    ///     SetFont(font);
    ///     Refresh();
    /// }
    /// ```
    virtual void onFontScaleChanged(double scale) = 0;

    /// @brief Handle theme change event
    /// @param themeName New theme name ("Light", "Dark", "Custom", etc.)
    ///
    /// **Implementation guidelines:**
    /// - Check enable flag (if control has opt-out)
    /// - Update colors based on theme
    /// - Update icons/images if needed
    /// - Refresh control: Refresh()
    ///
    /// **Example:**
    /// ```cpp
    /// void onThemeChanged(const std::string& themeName) override {
    ///     if (!m_enableThemeChange) return;
    ///     if (themeName == "Dark") {
    ///         SetBackgroundColour(wxColour(30, 30, 30));
    ///         SetForegroundColour(wxColour(220, 220, 220));
    ///     } else {
    ///         SetBackgroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOW));
    ///         SetForegroundColour(wxSystemSettings::GetColour(wxSYS_COLOUR_WINDOWTEXT));
    ///     }
    ///     Refresh();
    /// }
    /// ```
    virtual void onThemeChanged(const std::string& themeName) = 0;

private:
    /// @brief Static registry of all live reactive controls
    ///
    /// **Implementation notes:**
    /// - Vector stores raw pointers (lifetime managed by wxWidgets)
    /// - Controls auto-register on construction, auto-unregister on destruction
    /// - Broadcast iterates this vector to notify all controls
    ///
    /// **Future improvement (if needed):**
    /// - Use std::weak_ptr for safer lifetime management
    /// - Add mutex for thread-safety
    /// - Copy vector before iteration (prevents iterator invalidation)
    static std::vector<bwxReactive*> s_controls;
};

} // namespace gui
} // namespace bwx
