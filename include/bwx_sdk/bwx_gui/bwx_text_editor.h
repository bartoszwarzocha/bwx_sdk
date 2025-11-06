/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_text_editor.h
// Purpose:     Main text editor control - integrates document + renderer
// Author:      Kalahari Team
// Created:     2025-11-04
// Copyright:   (c) 2025 Kalahari Project
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_TEXT_EDITOR_H_
#define _BWX_TEXT_EDITOR_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/timer.h>
#include <memory>

#include <bwx_sdk/bwx_gui/bwx_text_document.h>
#include <bwx_sdk/bwx_gui/bwx_text_renderer.h>

namespace bwx_sdk {
namespace gui {

// ============================================================================
// bwxTextEditor - Main Text Editor Control
// ============================================================================

/// Main text editor control - combines document model + renderer + event handling
///
/// Features:
/// - Full text editing (insert, delete, cursor, selection)
/// - Formatting (bold, italic, underline, font, size, color)
/// - Undo/Redo (Command pattern)
/// - Clipboard (Copy, Cut, Paste)
/// - Multiple view modes (MVP: Full View only)
/// - wxControl integration (standard wxWidgets widget)
///
/// Architecture:
/// - Model: bwxTextDocument (text storage, formatting, undo/redo)
/// - View: ITextRenderer (Strategy Pattern - swappable renderers)
/// - Controller: bwxTextEditor (event handling, coordination)
class bwxTextEditor : public wxControl, public IDocumentObserver
{
public:
	// ========================================================================
	// View Modes
	// ========================================================================

	/// View mode enumeration
	enum ViewMode
	{
		VIEW_FULL = 0,       ///< Full View - continuous text, no pages (MVP)
		VIEW_PAGE,           ///< Page View - MS Word-like (Task #00020)
		VIEW_TYPEWRITER,     ///< Typewriter Mode - immersive (Task #00021)
		VIEW_PUBLISHER       ///< Publisher View - manuscript (Task #00022)
	};

	// ========================================================================
	// Construction
	// ========================================================================

	/// Default constructor (for two-phase construction)
	bwxTextEditor();

	/// Full constructor
	/// @param parent Parent window
	/// @param id Window ID
	/// @param pos Initial position
	/// @param size Initial size
	/// @param style Window style
	/// @param validator Validator
	/// @param name Window name
	bwxTextEditor(wxWindow* parent,
	              wxWindowID id,
	              const wxPoint& pos = wxDefaultPosition,
	              const wxSize& size = wxDefaultSize,
	              long style = wxBORDER_SUNKEN,
	              const wxValidator& validator = wxDefaultValidator,
	              const wxString& name = wxASCII_STR("bwxTextEditor"));

	/// Destructor
	virtual ~bwxTextEditor();

	/// Create (for two-phase construction)
	bool Create(wxWindow* parent,
	            wxWindowID id,
	            const wxPoint& pos = wxDefaultPosition,
	            const wxSize& size = wxDefaultSize,
	            long style = wxBORDER_SUNKEN,
	            const wxValidator& validator = wxDefaultValidator,
	            const wxString& name = wxASCII_STR("bwxTextEditor"));

	// ========================================================================
	// Document Access
	// ========================================================================

	/// Get document reference (non-const)
	bwxTextDocument& GetDocument() { return m_document; }

	/// Get document reference (const)
	const bwxTextDocument& GetDocument() const { return m_document; }

	// ========================================================================
	// View Mode
	// ========================================================================

	/// Set view mode (MVP: only VIEW_FULL supported)
	/// @param mode New view mode
	void SetViewMode(ViewMode mode);

	/// Get current view mode
	/// @return Current view mode
	ViewMode GetViewMode() const { return m_viewMode; }

	/// Get renderer (for settings configuration)
	/// @return Pointer to current renderer (nullptr if not initialized)
	/// @note For MVP, cast to FullViewRenderer* to access configuration methods
	ITextRenderer* GetRenderer() { return m_renderer.get(); }

	/// Get renderer (const version)
	const ITextRenderer* GetRenderer() const { return m_renderer.get(); }

	// ========================================================================
	// Editing Operations
	// ========================================================================

	/// Copy selected text to clipboard
	void Copy();

	/// Cut selected text to clipboard (copy + delete)
	void Cut();

	/// Paste text from clipboard at cursor
	void Paste();

	/// Select all text
	void SelectAll();

	/// Undo last operation
	void Undo();

	/// Redo last undone operation
	void Redo();

	/// Check if undo is available
	/// @return true if undo stack is not empty
	bool CanUndo() const;

	/// Check if redo is available
	/// @return true if redo stack is not empty
	bool CanRedo() const;

	// ========================================================================
	// File I/O (future - Days 11-12)
	// ========================================================================

	/// Load document from .ktxt file
	/// @param path File path
	/// @return true on success, false on error
	bool LoadFromFile(const wxString& path);

	/// Save document to .ktxt file
	/// @param path File path
	/// @return true on success, false on error
	bool SaveToFile(const wxString& path);

protected:
	// ========================================================================
	// wxControl Overrides
	// ========================================================================

	/// Get best size for control
	wxSize DoGetBestSize() const override;

	// ========================================================================
	// Event Handlers
	// ========================================================================

	/// Paint event - render document
	void OnPaint(wxPaintEvent& event);

	/// Size event - update renderer
	void OnSize(wxSizeEvent& event);

	/// Character input event (printable characters)
	void OnChar(wxKeyEvent& event);

	/// Key down event (navigation, special keys)
	void OnKeyDown(wxKeyEvent& event);

	/// Left mouse button down - move cursor
	void OnLeftDown(wxMouseEvent& event);

	/// Mouse motion - extend selection
	void OnMotion(wxMouseEvent& event);

	/// Set focus event - start caret blink
	void OnSetFocus(wxFocusEvent& event);

	/// Kill focus event - stop caret blink
	void OnKillFocus(wxFocusEvent& event);

	/// Caret timer event - toggle caret visibility
	void OnCaretTimer(wxTimerEvent& event);

	/// Mouse wheel event - scroll
	void OnMouseWheel(wxMouseEvent& event);

	// ========================================================================
	// IDocumentObserver Implementation
	// ========================================================================

	/// Called when document text changes
	void OnTextChanged() override;

	/// Called when cursor position changes
	void OnCursorMoved() override;

	/// Called when selection changes
	void OnSelectionChanged() override;

	/// Called when text formatting changes
	void OnFormatChanged() override;

private:
	// ========================================================================
	// Initialization
	// ========================================================================

	/// Initialize control (common for both constructors)
	void Init();

	/// Create renderer for current view mode
	void CreateRenderer();

	// ========================================================================
	// Input Handling
	// ========================================================================

	/// Handle character input (insert at cursor)
	/// @param ch Character to insert
	void HandleCharInput(wxChar ch);

	/// Handle key command (Ctrl+key, navigation)
	/// @param keyCode Key code
	/// @param ctrl Ctrl key pressed?
	/// @param shift Shift key pressed?
	void HandleKeyCommand(int keyCode, bool ctrl, bool shift);

	// ========================================================================
	// Caret Management
	// ========================================================================

	/// Update caret position (scroll if needed)
	void UpdateCaret();

	/// Show caret
	void ShowCaret();

	/// Hide caret
	void HideCaret();

	/// Start caret blink timer
	void StartCaretTimer();

	/// Stop caret blink timer
	void StopCaretTimer();

	// ========================================================================
	// Scrolling
	// ========================================================================

	/// Update scrollbar
	void UpdateScrollbar();

	/// Scroll to make cursor visible
	void ScrollToCursor();

	/// Scroll to position
	/// @param y Scroll position (pixels from top)
	void ScrollTo(int y);

	// ========================================================================
	// Member Variables
	// ========================================================================

	// Document & Rendering
	bwxTextDocument m_document;                  ///< Document model
	ViewMode m_viewMode;                         ///< Current view mode
	std::unique_ptr<ITextRenderer> m_renderer;   ///< Current renderer (Strategy Pattern)

	// Caret
	wxTimer m_caretTimer;                        ///< Caret blink timer
	bool m_caretVisible;                         ///< Is caret currently visible?
	bool m_hasFocus;                             ///< Does control have focus?

	// Scrolling
	int m_scrollY;                               ///< Current scroll position (pixels)

	// Mouse Selection
	bool m_isSelecting;                          ///< Is user currently selecting with mouse?

	// ========================================================================
	// Event Table
	// ========================================================================

	wxDECLARE_EVENT_TABLE();
};

} // namespace gui
} // namespace bwx_sdk

#endif // _BWX_TEXT_EDITOR_H_
