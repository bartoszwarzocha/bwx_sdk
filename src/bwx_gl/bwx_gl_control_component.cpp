/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_control_component.cpp
// Purpose:     BWX_SDK Library; Input control component (ECS)
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <bwx_sdk/bwx_gl/bwx_gl_control_component.h>
#include <bwx_sdk/bwx_gl/bwx_gl_movement_component.h>
#include <bwx_sdk/bwx_gl/bwx_gl_node.h>

#include <wx/event.h>
#include <wx/window.h>

namespace bwx_sdk {

    bwxGLControlComponent::bwxGLControlComponent()
        : m_mouseSensitivityX(0.1f), m_mouseSensitivityY(0.1f), m_mouseWheelSensitivity(1.0f) {
    }

    void bwxGLControlComponent::BindMovement(bwxGL_MOVEMENT_TYPE moveType,
        const std::vector<unsigned int>& keys,
        const std::vector<unsigned int>& modifiers) {
        m_keyBindings.push_back({ moveType, keys, modifiers });
    }

    void bwxGLControlComponent::BindDefaultMovementRules(bool useJump, bool useUpAndDown) {
        BindMovement(bwxGL_MOVEMENT_TYPE::FORWARD, { static_cast<unsigned int>('W'), static_cast<unsigned int>('w'), WXK_UP });
        BindMovement(bwxGL_MOVEMENT_TYPE::BACKWARD, { static_cast <unsigned int>('S'), static_cast<unsigned int>('s'), WXK_DOWN });
        BindMovement(bwxGL_MOVEMENT_TYPE::LEFT, { static_cast<unsigned int>('A'), static_cast<unsigned int>('a'), WXK_LEFT });
        BindMovement(bwxGL_MOVEMENT_TYPE::RIGHT, { static_cast<unsigned int>('D'), static_cast<unsigned int>('d'), WXK_RIGHT });
        if (useJump) {
            BindMovement(bwxGL_MOVEMENT_TYPE::JUMP, { WXK_SPACE });
        }
        if (useUpAndDown) {
            BindMovement(bwxGL_MOVEMENT_TYPE::UP, { static_cast<unsigned int>('Q'), static_cast<unsigned int>('q') });
            BindMovement(bwxGL_MOVEMENT_TYPE::DOWN, { static_cast<unsigned int>('E'), static_cast<unsigned int>('e') });
        }
    }

    void bwxGLControlComponent::BindCustomAction(const wxString& actionName,
        const std::vector<unsigned int>& keys,
        const std::function<void(float)>& callback,
        const std::vector<unsigned int>& modifiers) {
        m_customActions.push_back({ actionName, keys, modifiers, callback });
    }

    void bwxGLControlComponent::BindMouseWheelToMovement(bwxGL_MOVEMENT_TYPE moveType, float sensitivity) {
        m_mouseBindings[wxEVT_MOUSEWHEEL] = moveType;
        m_mouseWheelSensitivity = sensitivity;
    }

    bool bwxGLControlComponent::AreKeysPressed(const std::vector<unsigned int>& keys,
        const std::vector<unsigned int>& modifiers) {
        bool modifiersPressed = true;
        for (auto mod : modifiers)
            modifiersPressed &= wxGetKeyState(static_cast<wxKeyCode>(mod));

        if (!modifiersPressed) return false;

        for (auto key : keys)
            if (wxGetKeyState(static_cast<wxKeyCode>(key)))
                return true;

        return false;
    }

    void bwxGLControlComponent::OnMouseMotion(wxMouseEvent& event) {
        auto owner = GetNode();
        if (!owner) return;
        auto movement = owner->GetComponent<bwxGLMovementComponent>();
        if (!movement) return;

        wxPoint pos = event.GetPosition();
        wxPoint delta = pos - m_lastMousePos;
        m_lastMousePos = pos;

        float dx = static_cast<float>(delta.x) * m_mouseSensitivityX;
        float dy = static_cast<float>(delta.y) * m_mouseSensitivityY;

        movement->RotateEuler(glm::vec3(-dy, -dx, 0)); // domyœlnie: pitch, yaw
    }

    void bwxGLControlComponent::OnMouseWheel(wxMouseEvent& event) {
        auto owner = GetNode();
        if (!owner) return;
        auto movement = owner->GetComponent<bwxGLMovementComponent>();
        if (!movement) return;

        int rot = event.GetWheelRotation();
        if (rot > 0) {
            movement->ProcessMovement(bwxGL_MOVEMENT_TYPE::ZOOM_IN, m_mouseWheelSensitivity);
        }
        else if (rot < 0) {
            movement->ProcessMovement(bwxGL_MOVEMENT_TYPE::ZOOM_OUT, m_mouseWheelSensitivity);
        }
    }

    void bwxGLControlComponent::Update(float delta) {
        auto owner = GetNode();
        if (!owner) return;
        auto movement = owner->GetComponent<bwxGLMovementComponent>();
        if (!movement) return;

        for (const auto& binding : m_keyBindings) {
            if (AreKeysPressed(binding.keys, binding.modifiers)) {
                movement->ProcessMovement(binding.movementType, delta);
            }
        }

        for (const auto& action : m_customActions) {
            if (AreKeysPressed(action.keys, action.modifiers)) {
                action.callback(delta);
            }
        }
    }

} // namespace bwx_sdk
