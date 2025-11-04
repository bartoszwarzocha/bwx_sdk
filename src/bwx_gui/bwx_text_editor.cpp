/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_text_editor.cpp
// Purpose:     Main text editor control implementation
// Author:      Kalahari Team
// Created:     2025-11-04
// Copyright:   (c) 2025 Kalahari Project
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/clipbrd.h>
#include <wx/dcbuffer.h>
#include <bwx_sdk/bwx_gui/bwx_text_editor.h>

namespace bwx_sdk {
namespace gui {

// ============================================================================
// Event Table
// ============================================================================

wxBEGIN_EVENT_TABLE(bwxTextEditor, wxControl)
	EVT_PAINT(bwxTextEditor::OnPaint)
	EVT_SIZE(bwxTextEditor::OnSize)
	EVT_CHAR(bwxTextEditor::OnChar)
	EVT_KEY_DOWN(bwxTextEditor::OnKeyDown)
	EVT_LEFT_DOWN(bwxTextEditor::OnLeftDown)
	EVT_MOTION(bwxTextEditor::OnMotion)
	EVT_SET_FOCUS(bwxTextEditor::OnSetFocus)
	EVT_KILL_FOCUS(bwxTextEditor::OnKillFocus)
	EVT_TIMER(wxID_ANY, bwxTextEditor::OnCaretTimer)
	EVT_MOUSEWHEEL(bwxTextEditor::OnMouseWheel)
wxEND_EVENT_TABLE()

// ============================================================================
// Construction / Destruction
// ============================================================================

bwxTextEditor::bwxTextEditor()
	: wxControl()
	, m_document()
	, m_viewMode(VIEW_FULL)
	, m_renderer(nullptr)
	, m_caretTimer(this, wxID_ANY)
	, m_caretVisible(true)
	, m_hasFocus(false)
	, m_scrollY(0)
	, m_isSelecting(false)
{
	Init();
}

bwxTextEditor::bwxTextEditor(wxWindow* parent,
                             wxWindowID id,
                             const wxPoint& pos,
                             const wxSize& size,
                             long style,
                             const wxValidator& validator,
                             const wxString& name)
	: wxControl()
	, m_document()
	, m_viewMode(VIEW_FULL)
	, m_renderer(nullptr)
	, m_caretTimer(this, wxID_ANY)
	, m_caretVisible(true)
	, m_hasFocus(false)
	, m_scrollY(0)
	, m_isSelecting(false)
{
	Init();
	Create(parent, id, pos, size, style, validator, name);
}

bwxTextEditor::~bwxTextEditor()
{
	// Stop timer
	StopCaretTimer();

	// Unregister as document observer
	m_document.RemoveObserver(this);
}

void bwxTextEditor::Init()
{
	// Register as document observer
	m_document.AddObserver(this);

	// Create renderer
	CreateRenderer();
}

bool bwxTextEditor::Create(wxWindow* parent,
                           wxWindowID id,
                           const wxPoint& pos,
                           const wxSize& size,
                           long style,
                           const wxValidator& validator,
                           const wxString& name)
{
	// Create control
	if (!wxControl::Create(parent, id, pos, size, style | wxWANTS_CHARS, validator, name))
		return false;

	// Set background style for double buffering
	SetBackgroundStyle(wxBG_STYLE_PAINT);

	// Set initial size
	SetInitialSize(size);

	// Update renderer size
	if (m_renderer)
	{
		wxSize clientSize = GetClientSize();
		m_renderer->OnResize(clientSize.GetWidth(), clientSize.GetHeight());
	}

	return true;
}

void bwxTextEditor::CreateRenderer()
{
	// MVP: Only Full View renderer
	switch (m_viewMode)
	{
		case VIEW_FULL:
			m_renderer = std::make_unique<FullViewRenderer>();
			break;

		case VIEW_PAGE:
		case VIEW_TYPEWRITER:
		case VIEW_PUBLISHER:
			// Future: Other renderers (Tasks #00020-#00022)
			wxLogWarning("View mode %d not yet implemented, using Full View", m_viewMode);
			m_renderer = std::make_unique<FullViewRenderer>();
			m_viewMode = VIEW_FULL;
			break;

		default:
			wxLogError("Unknown view mode %d", m_viewMode);
			m_renderer = std::make_unique<FullViewRenderer>();
			m_viewMode = VIEW_FULL;
			break;
	}

	// Connect document to renderer
	if (m_renderer)
		m_renderer->SetDocument(&m_document);
}

// ============================================================================
// wxControl Overrides
// ============================================================================

wxSize bwxTextEditor::DoGetBestSize() const
{
	// Minimum size: 400x300
	return wxSize(400, 300);
}

// ============================================================================
// View Mode
// ============================================================================

void bwxTextEditor::SetViewMode(ViewMode mode)
{
	if (m_viewMode == mode)
		return;

	m_viewMode = mode;
	CreateRenderer();

	// Invalidate layout
	if (m_renderer)
	{
		wxSize clientSize = GetClientSize();
		m_renderer->OnResize(clientSize.GetWidth(), clientSize.GetHeight());
	}

	Refresh();
}

// ============================================================================
// Editing Operations
// ============================================================================

void bwxTextEditor::Copy()
{
	Selection sel = m_document.GetSelection();
	if (!sel.active || sel.IsEmpty())
		return;

	wxString text = m_document.GetText(sel.GetMin(), sel.GetMax());

	if (wxTheClipboard->Open())
	{
		wxTheClipboard->SetData(new wxTextDataObject(text));
		wxTheClipboard->Close();
	}
}

void bwxTextEditor::Cut()
{
	Selection sel = m_document.GetSelection();
	if (!sel.active || sel.IsEmpty())
		return;

	// Copy to clipboard first
	Copy();

	// Delete selection
	m_document.DeleteText(sel.GetMin(), sel.GetMax());
}

void bwxTextEditor::Paste()
{
	if (!wxTheClipboard->Open())
		return;

	if (wxTheClipboard->IsSupported(wxDF_TEXT))
	{
		wxTextDataObject data;
		wxTheClipboard->GetData(data);
		wxString text = data.GetText();

		// Delete selection if active
		Selection sel = m_document.GetSelection();
		if (sel.active && !sel.IsEmpty())
		{
			m_document.DeleteText(sel.GetMin(), sel.GetMax());
		}

		// Insert at cursor
		Cursor cursor = m_document.GetCursor();
		m_document.InsertText(cursor.position, text);
	}

	wxTheClipboard->Close();
}

void bwxTextEditor::SelectAll()
{
	int length = m_document.GetLength();
	if (length > 0)
	{
		m_document.SetSelection(0, length);
		Refresh();
	}
}

void bwxTextEditor::Undo()
{
	if (m_document.CanUndo())
	{
		m_document.Undo();
	}
}

void bwxTextEditor::Redo()
{
	if (m_document.CanRedo())
	{
		m_document.Redo();
	}
}

bool bwxTextEditor::CanUndo() const
{
	return m_document.CanUndo();
}

bool bwxTextEditor::CanRedo() const
{
	return m_document.CanRedo();
}

// ============================================================================
// File I/O (stub for Days 11-12)
// ============================================================================

bool bwxTextEditor::LoadFromFile(const wxString& WXUNUSED(path))
{
	// TODO: Implement in Days 11-12
	wxLogWarning("LoadFromFile not yet implemented");
	return false;
}

bool bwxTextEditor::SaveToFile(const wxString& WXUNUSED(path))
{
	// TODO: Implement in Days 11-12
	wxLogWarning("SaveToFile not yet implemented");
	return false;
}

// ============================================================================
// Event Handlers - Painting & Sizing
// ============================================================================

void bwxTextEditor::OnPaint(wxPaintEvent& WXUNUSED(event))
{
	// Use buffered paint DC for flicker-free rendering
	wxPaintDC pdc(this);
	wxBufferedDC dc(&pdc, GetClientSize());

	if (!m_renderer)
		return;

	// Get client rect
	wxRect clientRect = GetClientRect();

	// Render document
	m_renderer->Render(dc, clientRect, m_scrollY);
}

void bwxTextEditor::OnSize(wxSizeEvent& event)
{
	// Update renderer
	if (m_renderer)
	{
		wxSize size = event.GetSize();
		m_renderer->OnResize(size.GetWidth(), size.GetHeight());
	}

	// Update scrollbar
	UpdateScrollbar();

	// Refresh
	Refresh();

	event.Skip();
}

// ============================================================================
// Event Handlers - Keyboard
// ============================================================================

void bwxTextEditor::OnChar(wxKeyEvent& event)
{
	// Handle printable character input
	wxChar ch = event.GetUnicodeKey();

	if (ch != WXK_NONE && ch >= 32)
	{
		// Printable character
		HandleCharInput(ch);
	}
	else
	{
		// Non-printable - skip
		event.Skip();
	}
}

void bwxTextEditor::OnKeyDown(wxKeyEvent& event)
{
	int keyCode = event.GetKeyCode();
	bool ctrl = event.ControlDown();
	bool shift = event.ShiftDown();

	// Handle key commands
	HandleKeyCommand(keyCode, ctrl, shift);

	// Skip for default handling
	event.Skip();
}

void bwxTextEditor::HandleCharInput(wxChar ch)
{
	// Delete selection if active
	Selection sel = m_document.GetSelection();
	if (sel.active && !sel.IsEmpty())
	{
		m_document.DeleteText(sel.GetMin(), sel.GetMax());
	}

	// Insert character at cursor
	Cursor cursor = m_document.GetCursor();
	m_document.InsertText(cursor.position, wxString(ch));

	// Refresh
	Refresh();
}

void bwxTextEditor::HandleKeyCommand(int keyCode, bool ctrl, bool shift)
{
	Cursor cursor = m_document.GetCursor();
	int textLength = m_document.GetLength();

	switch (keyCode)
	{
		// ====================================================================
		// Navigation - Arrow Keys
		// ====================================================================

		case WXK_LEFT:
			if (cursor.position > 0)
			{
				if (shift)
				{
					// Extend selection
					Selection sel = m_document.GetSelection();
					if (!sel.active)
					{
						sel.startPos = cursor.position;
						sel.endPos = cursor.position - 1;
						sel.active = true;
					}
					else
					{
						if (cursor.position == sel.endPos)
							sel.endPos--;
						else
							sel.startPos--;
					}
					m_document.SetSelection(sel.startPos, sel.endPos);
				}
				else
				{
					// Clear selection
					m_document.ClearSelection();
				}

				m_document.SetCursorPosition(cursor.position - 1);
			}
			break;

		case WXK_RIGHT:
			if (cursor.position < textLength)
			{
				if (shift)
				{
					// Extend selection
					Selection sel = m_document.GetSelection();
					if (!sel.active)
					{
						sel.startPos = cursor.position;
						sel.endPos = cursor.position + 1;
						sel.active = true;
					}
					else
					{
						if (cursor.position == sel.startPos)
							sel.startPos++;
						else
							sel.endPos++;
					}
					m_document.SetSelection(sel.startPos, sel.endPos);
				}
				else
				{
					// Clear selection
					m_document.ClearSelection();
				}

				m_document.SetCursorPosition(cursor.position + 1);
			}
			break;

		case WXK_UP:
			// TODO: Implement line-based navigation (needs renderer support)
			break;

		case WXK_DOWN:
			// TODO: Implement line-based navigation (needs renderer support)
			break;

		// ====================================================================
		// Navigation - Home/End
		// ====================================================================

		case WXK_HOME:
			if (ctrl)
			{
				// Ctrl+Home - go to document start
				m_document.SetCursorPosition(0);
			}
			else
			{
				// Home - go to line start
				// TODO: Implement (needs line tracking)
			}
			if (!shift)
				m_document.ClearSelection();
			break;

		case WXK_END:
			if (ctrl)
			{
				// Ctrl+End - go to document end
				m_document.SetCursorPosition(textLength);
			}
			else
			{
				// End - go to line end
				// TODO: Implement (needs line tracking)
			}
			if (!shift)
				m_document.ClearSelection();
			break;

		// ====================================================================
		// Editing - Backspace/Delete
		// ====================================================================

		case WXK_BACK:
			{
				Selection sel = m_document.GetSelection();
				if (sel.active && !sel.IsEmpty())
				{
					// Delete selection
					m_document.DeleteText(sel.GetMin(), sel.GetMax());
				}
				else if (cursor.position > 0)
				{
					// Delete character before cursor
					m_document.DeleteText(cursor.position - 1, cursor.position);
				}
				Refresh();
			}
			break;

		case WXK_DELETE:
			{
				Selection sel = m_document.GetSelection();
				if (sel.active && !sel.IsEmpty())
				{
					// Delete selection
					m_document.DeleteText(sel.GetMin(), sel.GetMax());
				}
				else if (cursor.position < textLength)
				{
					// Delete character after cursor
					m_document.DeleteText(cursor.position, cursor.position + 1);
				}
				Refresh();
			}
			break;

		// ====================================================================
		// Editing - Enter
		// ====================================================================

		case WXK_RETURN:
		case WXK_NUMPAD_ENTER:
			{
				// Delete selection if active
				Selection sel = m_document.GetSelection();
				if (sel.active && !sel.IsEmpty())
				{
					m_document.DeleteText(sel.GetMin(), sel.GetMax());
				}

				// Insert newline
				Cursor cur = m_document.GetCursor();
				m_document.InsertText(cur.position, "\n");
				Refresh();
			}
			break;

		// ====================================================================
		// Ctrl+Key Commands
		// ====================================================================

		case 'A':
			if (ctrl)
				SelectAll();
			break;

		case 'C':
			if (ctrl)
				Copy();
			break;

		case 'X':
			if (ctrl)
				Cut();
			break;

		case 'V':
			if (ctrl)
				Paste();
			break;

		case 'Z':
			if (ctrl)
			{
				if (shift)
					Redo();  // Ctrl+Shift+Z
				else
					Undo();  // Ctrl+Z
			}
			break;

		case 'Y':
			if (ctrl)
				Redo();  // Ctrl+Y
			break;

		// ====================================================================
		// Formatting Commands (Ctrl+B/I/U)
		// ====================================================================

		case 'B':
			if (ctrl)
			{
				// Toggle bold
				Selection sel = m_document.GetSelection();
				if (sel.active && !sel.IsEmpty())
				{
					TextFormat format = m_document.GetFormatAt(sel.GetMin());
					format.bold = !format.bold;
					m_document.ApplyFormat(sel.GetMin(), sel.GetMax(), format);
					Refresh();
				}
			}
			break;

		case 'I':
			if (ctrl)
			{
				// Toggle italic
				Selection sel = m_document.GetSelection();
				if (sel.active && !sel.IsEmpty())
				{
					TextFormat format = m_document.GetFormatAt(sel.GetMin());
					format.italic = !format.italic;
					m_document.ApplyFormat(sel.GetMin(), sel.GetMax(), format);
					Refresh();
				}
			}
			break;

		case 'U':
			if (ctrl)
			{
				// Toggle underline
				Selection sel = m_document.GetSelection();
				if (sel.active && !sel.IsEmpty())
				{
					TextFormat format = m_document.GetFormatAt(sel.GetMin());
					format.underline = !format.underline;
					m_document.ApplyFormat(sel.GetMin(), sel.GetMax(), format);
					Refresh();
				}
			}
			break;

		default:
			// Unhandled key
			break;
	}

	// Update caret and scrollbar
	UpdateCaret();
	UpdateScrollbar();
}

// ============================================================================
// Event Handlers - Mouse
// ============================================================================

void bwxTextEditor::OnLeftDown(wxMouseEvent& event)
{
	// Set focus
	if (!HasFocus())
		SetFocus();

	// Get click position
	wxPoint pos = event.GetPosition();

	// Hit test
	if (m_renderer)
	{
		int docPos = m_renderer->HitTest(pos.x, pos.y, m_scrollY);
		m_document.SetCursorPosition(docPos);

		// Start selection
		if (event.ShiftDown())
		{
			// Extend selection
			Selection sel = m_document.GetSelection();
			if (!sel.active)
			{
				sel.startPos = m_document.GetCursor().position;
				sel.active = true;
			}
			sel.endPos = docPos;
			m_document.SetSelection(sel.startPos, sel.endPos);
		}
		else
		{
			// Clear selection, start new
			m_document.ClearSelection();
			m_isSelecting = true;
		}

		Refresh();
	}
}

void bwxTextEditor::OnMotion(wxMouseEvent& event)
{
	// Handle selection with mouse drag
	if (event.LeftIsDown() && m_isSelecting)
	{
		wxPoint pos = event.GetPosition();

		if (m_renderer)
		{
			int docPos = m_renderer->HitTest(pos.x, pos.y, m_scrollY);
			Cursor cursor = m_document.GetCursor();

			// Update selection
			if (docPos != cursor.position)
			{
				m_document.SetSelection(cursor.position, docPos);
				Refresh();
			}
		}
	}
	else
	{
		m_isSelecting = false;
	}
}

void bwxTextEditor::OnMouseWheel(wxMouseEvent& event)
{
	// Scroll on mouse wheel
	int delta = event.GetWheelRotation();
	int lines = event.GetLinesPerAction();

	// Calculate scroll amount (negative delta = scroll down)
	int scrollAmount = -delta / event.GetWheelDelta() * lines * 20;  // 20 pixels per line

	int newScrollY = m_scrollY + scrollAmount;

	// Clamp to valid range
	int maxScroll = 0;
	if (m_renderer)
	{
		int totalHeight = m_renderer->GetTotalHeight();
		int clientHeight = GetClientSize().GetHeight();
		maxScroll = std::max(0, totalHeight - clientHeight);
	}

	newScrollY = std::max(0, std::min(newScrollY, maxScroll));

	if (newScrollY != m_scrollY)
	{
		m_scrollY = newScrollY;
		UpdateScrollbar();
		Refresh();
	}
}

// ============================================================================
// Event Handlers - Focus
// ============================================================================

void bwxTextEditor::OnSetFocus(wxFocusEvent& event)
{
	m_hasFocus = true;
	StartCaretTimer();
	Refresh();
	event.Skip();
}

void bwxTextEditor::OnKillFocus(wxFocusEvent& event)
{
	m_hasFocus = false;
	StopCaretTimer();
	Refresh();
	event.Skip();
}

void bwxTextEditor::OnCaretTimer(wxTimerEvent& WXUNUSED(event))
{
	// Toggle caret visibility
	m_caretVisible = !m_caretVisible;
	Refresh();
}

// ============================================================================
// IDocumentObserver Implementation
// ============================================================================

void bwxTextEditor::OnTextChanged()
{
	// Text changed - refresh
	if (m_renderer)
		m_renderer->InvalidateLayout();

	UpdateScrollbar();
	Refresh();
}

void bwxTextEditor::OnCursorMoved()
{
	// Cursor moved - update caret
	UpdateCaret();
	Refresh();
}

void bwxTextEditor::OnSelectionChanged()
{
	// Selection changed - refresh
	Refresh();
}

void bwxTextEditor::OnFormatChanged()
{
	// Format changed - refresh
	if (m_renderer)
		m_renderer->InvalidateLayout();

	Refresh();
}

// ============================================================================
// Caret Management
// ============================================================================

void bwxTextEditor::UpdateCaret()
{
	// Scroll to make cursor visible
	ScrollToCursor();

	// Reset caret blink
	m_caretVisible = true;
	if (m_hasFocus)
		StartCaretTimer();
}

void bwxTextEditor::ShowCaret()
{
	m_caretVisible = true;
	Refresh();
}

void bwxTextEditor::HideCaret()
{
	m_caretVisible = false;
	Refresh();
}

void bwxTextEditor::StartCaretTimer()
{
	// Blink rate: 500ms (standard)
	m_caretTimer.Start(500);
}

void bwxTextEditor::StopCaretTimer()
{
	m_caretTimer.Stop();
}

// ============================================================================
// Scrolling
// ============================================================================

void bwxTextEditor::UpdateScrollbar()
{
	// TODO: Implement proper scrollbar integration (SetScrollbar, etc.)
	// For MVP, scrolling works but without visual scrollbar
}

void bwxTextEditor::ScrollToCursor()
{
	if (!m_renderer)
		return;

	Cursor cursor = m_document.GetCursor();
	wxRect cursorRect = m_renderer->GetCursorRect(cursor.position);

	int clientHeight = GetClientSize().GetHeight();

	// Check if cursor is above visible area
	if (cursorRect.y - m_scrollY < 0)
	{
		m_scrollY = cursorRect.y;
	}
	// Check if cursor is below visible area
	else if (cursorRect.y + cursorRect.height - m_scrollY > clientHeight)
	{
		m_scrollY = cursorRect.y + cursorRect.height - clientHeight;
	}

	// Clamp scroll position
	int totalHeight = m_renderer->GetTotalHeight();
	int maxScroll = std::max(0, totalHeight - clientHeight);
	m_scrollY = std::max(0, std::min(m_scrollY, maxScroll));
}

void bwxTextEditor::ScrollTo(int y)
{
	if (!m_renderer)
		return;

	int totalHeight = m_renderer->GetTotalHeight();
	int clientHeight = GetClientSize().GetHeight();
	int maxScroll = std::max(0, totalHeight - clientHeight);

	m_scrollY = std::max(0, std::min(y, maxScroll));
	UpdateScrollbar();
	Refresh();
}

} // namespace gui
} // namespace bwx_sdk
