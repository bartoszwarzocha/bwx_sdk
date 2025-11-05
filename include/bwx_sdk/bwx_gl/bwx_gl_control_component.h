/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_gl_control_component.h
// Purpose:     BWX_SDK Library; Control component (ECS)
// Author:      Bartosz Warzocha <bartosz.warzocha@gmail.com>
// Created:     2025-03-22
// Copyright:   (c) 2025 by Bartosz Warzocha
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_GL_CONTROL_COMPONENT_H_
#define _BWX_GL_CONTROL_COMPONENT_H_

#if defined(__APPLE__)
#error OpenGL functionality is not available for macOS.
#endif

#define GLM_ENABLE_EXPERIMENTAL

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
#include <wx/wx.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/transform.hpp>

#include "bwx_gl_component.h"

namespace bwx_sdk {

	enum class bwxGL_MOVEMENT_TYPE;

    struct bwxGLKeyBinding {
        bwxGL_MOVEMENT_TYPE movementType;
        std::vector<unsigned int> keys;
        std::vector<unsigned int> modifiers;
    };

    struct bwxGLMouseBinding {
        bwxGL_MOVEMENT_TYPE movementType;
        float sensitivity;
    };

    struct bwxGLCustomAction {
        wxString actionName;
        std::vector<unsigned int> keys;
        std::vector<unsigned int> modifiers;
        std::function<void(float delta)> callback;
    };

	class bwxGLControlComponent : public bwxGLComponent {
	public:

        explicit bwxGLControlComponent();

        void BindMovement(bwxGL_MOVEMENT_TYPE moveType,
            const std::vector<unsigned int>& keys,
            const std::vector<unsigned int>& modifiers = {});

        void BindDefaultMovementRules(bool useJump = false, bool useUpAndDown = false);

        void BindCustomAction(const wxString& actionName,
            const std::vector<unsigned int>& keys,
            const std::function<void(float delta)>& callback,
            const std::vector<unsigned int>& modifiers = {});

        void BindMouseWheelToMovement(bwxGL_MOVEMENT_TYPE moveType, float sensitivity = 1.0f);

        void Update(float delta);

    private:
        void OnMouseMotion(wxMouseEvent& event);
        void OnMouseWheel(wxMouseEvent& event);

        std::vector<bwxGLKeyBinding> m_keyBindings;
        std::unordered_map<int, bwxGL_MOVEMENT_TYPE> m_mouseBindings;
        std::vector<bwxGLCustomAction> m_customActions;

        wxPoint m_lastMousePos;
        float m_mouseSensitivityX;
        float m_mouseSensitivityY;
        float m_mouseWheelSensitivity;

        bool AreKeysPressed(const std::vector<unsigned int>& keys, const std::vector<unsigned int>& modifiers);

	};

} // namespace bwx_sdk

#endif // _BWX_GL_CONTROL_COMPONENT_H_