///////////////////////////////////////////////////////////////////////////////
// Name:        bwx_reactive.cpp
// Purpose:     Implementation of reactive GUI control base class
// Author:      Bartosz Warzocha & Claude Code
// Created:     2025-11-15
// Copyright:   (c) 2025 Bartosz Warzocha
// Licence:     wxWidgets licence
///////////////////////////////////////////////////////////////////////////////

#include "bwx_sdk/bwx_gui/bwx_reactive.h"
#include <algorithm>

namespace bwx {
namespace gui {

// ============================================================================
// Static Registry Initialization
// ============================================================================

std::vector<bwxReactive*> bwxReactive::s_controls;

// ============================================================================
// Lifecycle
// ============================================================================

bwxReactive::bwxReactive() {
    // Register this control in static registry
    s_controls.push_back(this);
}

bwxReactive::~bwxReactive() {
    // Unregister this control from static registry
    auto it = std::find(s_controls.begin(), s_controls.end(), this);
    if (it != s_controls.end()) {
        s_controls.erase(it);
    }
}

// ============================================================================
// Broadcast API
// ============================================================================

void bwxReactive::broadcastFontScaleChange(double scale) {
    // Iterate through all registered controls and notify
    // TODO (Task #00044): Copy vector before iteration to prevent
    // iterator invalidation if controls are destroyed during broadcast
    for (auto* control : s_controls) {
        control->onFontScaleChanged(scale);
    }
}

void bwxReactive::broadcastThemeChange(const std::string& themeName) {
    // Iterate through all registered controls and notify
    // TODO (Task #00044): Copy vector before iteration to prevent
    // iterator invalidation if controls are destroyed during broadcast
    for (auto* control : s_controls) {
        control->onThemeChanged(themeName);
    }
}

size_t bwxReactive::getRegisteredControlsCount() {
    return s_controls.size();
}

} // namespace gui
} // namespace bwx
