/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_text_document.h
// Purpose:     Document model for bwxTextEditor - text storage, formatting, undo/redo
// Author:      Kalahari Team
// Created:     2025-11-04
// Copyright:   (c) 2025 Kalahari Project
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_TEXT_DOCUMENT_H_
#define _BWX_TEXT_DOCUMENT_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/datetime.h>
#include <memory>
#include <vector>
#include <deque>

namespace bwx_sdk {
namespace gui {

// Forward declarations
class bwxTextDocument;
class ITextCommand;

// ============================================================================
// Text Format Structure
// ============================================================================

/// Text formatting attributes (character-level)
struct TextFormat
{
	// Font attributes
	wxString fontName;
	int fontSize;
	wxColour textColor;
	wxColour backgroundColor;

	// Style attributes
	bool bold;
	bool italic;
	bool underline;

	/// Default constructor - creates default format (Arial 12pt black on white)
	TextFormat()
		: fontName("Arial")
		, fontSize(12)
		, textColor(*wxBLACK)
		, backgroundColor(*wxWHITE)
		, bold(false)
		, italic(false)
		, underline(false)
	{
	}

	/// Equality comparison (for format merging)
	bool operator==(const TextFormat& other) const
	{
		return fontName == other.fontName
			&& fontSize == other.fontSize
			&& textColor == other.textColor
			&& backgroundColor == other.backgroundColor
			&& bold == other.bold
			&& italic == other.italic
			&& underline == other.underline;
	}

	bool operator!=(const TextFormat& other) const { return !(*this == other); }
};

// ============================================================================
// Format Run - contiguous region with uniform formatting
// ============================================================================

/// Format run - contiguous range of text with same formatting
struct FormatRun
{
	int startPos;       ///< Start position (inclusive)
	int endPos;         ///< End position (exclusive)
	TextFormat format;  ///< Formatting for this range

	/// Default constructor
	FormatRun() : startPos(0), endPos(0) { }

	/// Full constructor
	FormatRun(int start, int end, const TextFormat& fmt)
		: startPos(start), endPos(end), format(fmt) { }

	/// Check if position is within this run
	bool Contains(int pos) const { return pos >= startPos && pos < endPos; }

	/// Get length of this run
	int GetLength() const { return endPos - startPos; }
};

// ============================================================================
// Cursor Structure
// ============================================================================

/// Cursor position in document
struct Cursor
{
	int position;   ///< Linear offset in text (primary)
	int line;       ///< Line number (calculated, cached)
	int column;     ///< Column in line (calculated, cached)
	bool preferNext;///< For line wrap ambiguity resolution

	/// Default constructor - cursor at start of document
	Cursor() : position(0), line(0), column(0), preferNext(false) { }

	/// Constructor with position
	explicit Cursor(int pos) : position(pos), line(0), column(0), preferNext(false) { }

	/// Equality comparison
	bool operator==(const Cursor& other) const { return position == other.position; }
	bool operator!=(const Cursor& other) const { return !(*this == other); }
};

// ============================================================================
// Selection Structure
// ============================================================================

/// Text selection (range of characters)
struct Selection
{
	int startPos;   ///< Anchor point (where selection started)
	int endPos;     ///< Current point (where cursor is)
	bool active;    ///< Is selection currently visible?

	/// Default constructor - no selection
	Selection() : startPos(0), endPos(0), active(false) { }

	/// Constructor with range
	Selection(int start, int end) : startPos(start), endPos(end), active(true) { }

	/// Get minimum position (normalized start)
	int GetMin() const { return std::min(startPos, endPos); }

	/// Get maximum position (normalized end)
	int GetMax() const { return std::max(startPos, endPos); }

	/// Get selection length
	int GetLength() const { return GetMax() - GetMin(); }

	/// Check if selection contains position
	bool Contains(int pos) const { return active && pos >= GetMin() && pos < GetMax(); }

	/// Check if selection is empty
	bool IsEmpty() const { return !active || startPos == endPos; }
};

// ============================================================================
// Document Metadata
// ============================================================================

/// Document metadata (word count, timestamps, etc.)
struct DocumentMetadata
{
	wxString title;
	wxString author;
	wxDateTime created;
	wxDateTime modified;
	int wordCount;
	int characterCount;

	/// Default constructor
	DocumentMetadata()
		: title("")
		, author("")
		, created(wxDateTime::Now())
		, modified(wxDateTime::Now())
		, wordCount(0)
		, characterCount(0)
	{
	}
};

// ============================================================================
// Document Observer Interface
// ============================================================================

/// Observer interface for document changes (UI updates)
class IDocumentObserver
{
public:
	virtual ~IDocumentObserver() = default;

	/// Called when text content changes
	virtual void OnTextChanged() = 0;

	/// Called when cursor moves
	virtual void OnCursorMoved() = 0;

	/// Called when selection changes
	virtual void OnSelectionChanged() = 0;

	/// Called when formatting changes
	virtual void OnFormatChanged() = 0;
};

// ============================================================================
// Text Command Interface (Command Pattern for Undo/Redo)
// ============================================================================

/// Command interface for undo/redo operations
class ITextCommand
{
public:
	virtual ~ITextCommand() = default;

	/// Execute the command (apply change to document)
	virtual void Execute(bwxTextDocument* doc) = 0;

	/// Undo the command (revert change)
	virtual void Undo(bwxTextDocument* doc) = 0;

	/// Check if this command can be merged with another
	/// (Used for merging consecutive single-character inserts during typing)
	virtual bool CanMerge(const ITextCommand* /*other*/) const { return false; }

	/// Merge another command into this one
	virtual void Merge(ITextCommand* /*other*/) { }

	/// Get command type name (for debugging)
	virtual wxString GetTypeName() const = 0;
};

// ============================================================================
// Text Storage Interface (Strategy Pattern)
// ============================================================================

/// Text storage interface - allows swapping storage implementation
/// (MVP: Gap Buffer; Future: Rope or Piece Table if profiling shows issues)
class ITextStorage
{
public:
	virtual ~ITextStorage() = default;

	/// Get full text
	virtual wxString GetText() const = 0;

	/// Get text in range [startPos, endPos)
	virtual wxString GetText(int startPos, int endPos) const = 0;

	/// Set full text (replaces all content)
	virtual void SetText(const wxString& text) = 0;

	/// Insert text at position
	virtual void InsertText(int pos, const wxString& text) = 0;

	/// Delete text in range [startPos, endPos)
	virtual void DeleteText(int startPos, int endPos) = 0;

	/// Get character at position
	virtual wxChar GetChar(int pos) const = 0;

	/// Get length of text
	virtual int GetLength() const = 0;

	/// Clear all text
	virtual void Clear() = 0;
};

// ============================================================================
// Main Document Class
// ============================================================================

/// Text document model - manages text content, formatting, cursor, undo/redo
///
/// This class is independent of how the text is displayed (View).
/// It uses Strategy Pattern for text storage (Gap Buffer by default).
/// It uses Command Pattern for undo/redo operations.
/// It uses Observer Pattern to notify UI of changes.
class bwxTextDocument
{
public:
	// ========================================================================
	// Construction / Destruction
	// ========================================================================

	/// Default constructor - creates empty document
	bwxTextDocument();

	/// Destructor
	~bwxTextDocument();

	// ========================================================================
	// Text Operations
	// ========================================================================

	/// Get full text content
	wxString GetText() const;

	/// Get text in range [startPos, endPos)
	wxString GetText(int startPos, int endPos) const;

	/// Set full text (replaces all content, clears undo history)
	void SetText(const wxString& text);

	/// Insert text at position (creates undo command)
	void InsertText(int pos, const wxString& text);

	/// Delete text in range [startPos, endPos) (creates undo command)
	void DeleteText(int startPos, int endPos);

	/// Get character at position
	wxChar GetChar(int pos) const;

	/// Get length of text
	int GetLength() const;

	/// Clear all text (clears undo history)
	void Clear();

	// ========================================================================
	// Formatting Operations
	// ========================================================================

	/// Apply formatting to range [startPos, endPos) (creates undo command)
	void ApplyFormat(int startPos, int endPos, const TextFormat& format);

	/// Get formatting at position
	TextFormat GetFormatAt(int pos) const;

	/// Get all format runs (for rendering)
	const std::vector<FormatRun>& GetFormatRuns() const { return m_formatRuns; }

	/// Get format runs that intersect range [startPos, endPos)
	std::vector<FormatRun> GetFormatRuns(int startPos, int endPos) const;

	/// Clear all formatting (reset to default)
	void ClearFormatting();

	/// Clear formatting in range [startPos, endPos) (creates undo command)
	void ClearFormatting(int startPos, int endPos);

	// ========================================================================
	// Cursor & Selection
	// ========================================================================

	/// Get current cursor
	Cursor GetCursor() const { return m_cursor; }

	/// Set cursor position
	void SetCursor(const Cursor& cursor);

	/// Set cursor position (simple version)
	void SetCursorPosition(int pos);

	/// Move cursor by offset (positive = forward, negative = backward)
	void MoveCursor(int offset);

	/// Get current selection
	Selection GetSelection() const { return m_selection; }

	/// Set selection range
	void SetSelection(int startPos, int endPos);

	/// Clear selection
	void ClearSelection();

	/// Select all text
	void SelectAll();

	/// Get selected text
	wxString GetSelectedText() const;

	/// Delete selected text (if any) (creates undo command)
	bool DeleteSelection();

	// ========================================================================
	// Undo / Redo
	// ========================================================================

	/// Undo last operation
	void Undo();

	/// Redo last undone operation
	void Redo();

	/// Check if undo is available
	bool CanUndo() const;

	/// Check if redo is available
	bool CanRedo() const;

	/// Get undo stack size
	int GetUndoStackSize() const { return static_cast<int>(m_undoStack.size()); }

	/// Get redo stack size
	int GetRedoStackSize() const { return static_cast<int>(m_redoStack.size()); }

	/// Clear undo/redo history
	void ClearUndoHistory();

	/// Set maximum undo stack size (default: 100)
	void SetMaxUndoStack(int maxSize) { m_maxUndoStack = maxSize; }

	/// Get maximum undo stack size
	int GetMaxUndoStack() const { return m_maxUndoStack; }

	// ========================================================================
	// File I/O
	// ========================================================================

	/// Load document from .ktxt file (JSON format)
	bool LoadFromFile(const wxString& path);

	/// Save document to .ktxt file (JSON format)
	bool SaveToFile(const wxString& path);

	// ========================================================================
	// Metadata
	// ========================================================================

	/// Get document metadata
	DocumentMetadata GetMetadata() const { return m_metadata; }

	/// Set document metadata
	void SetMetadata(const DocumentMetadata& metadata) { m_metadata = metadata; }

	/// Update word count and character count
	void UpdateWordCount();

	/// Get word count (cached, call UpdateWordCount() to refresh)
	int GetWordCount() const { return m_metadata.wordCount; }

	/// Get character count (cached, call UpdateWordCount() to refresh)
	int GetCharacterCount() const { return m_metadata.characterCount; }

	// ========================================================================
	// Observer Pattern
	// ========================================================================

	/// Add observer
	void AddObserver(IDocumentObserver* observer);

	/// Remove observer
	void RemoveObserver(IDocumentObserver* observer);

	// ========================================================================
	// Internal API (used by ITextCommand implementations)
	// ========================================================================

	/// Insert text without creating undo command (called by InsertTextCommand)
	void InsertTextInternal(int pos, const wxString& text);

	/// Delete text without creating undo command (called by DeleteTextCommand)
	void DeleteTextInternal(int startPos, int endPos);

	/// Apply format without creating undo command (called by ApplyFormatCommand)
	void ApplyFormatInternal(int startPos, int endPos, const TextFormat& format);

	/// Restore format runs (used by ApplyFormatCommand::Undo)
	void RestoreFormatRuns(int startPos, int endPos, const std::vector<FormatRun>& runs);

private:
	// ========================================================================
	// Private Methods
	// ========================================================================

	/// Add command to undo stack
	void AddCommand(std::unique_ptr<ITextCommand> cmd);

	/// Notify observers that text changed
	void NotifyTextChanged();

	/// Notify observers that cursor moved
	void NotifyCursorMoved();

	/// Notify observers that selection changed
	void NotifySelectionChanged();

	/// Notify observers that formatting changed
	void NotifyFormatChanged();

	/// Update cursor line/column from position
	void UpdateCursorLineColumn();

	/// Merge format runs if possible (called after ApplyFormat)
	void MergeAdjacentRuns();

	/// Split format run at position (called before ApplyFormat)
	void SplitRunAt(int pos);

	// ========================================================================
	// Member Variables
	// ========================================================================

	std::unique_ptr<ITextStorage> m_storage;              ///< Text storage (Gap Buffer)
	std::vector<FormatRun> m_formatRuns;                  ///< Formatting runs
	Cursor m_cursor;                                       ///< Current cursor
	Selection m_selection;                                 ///< Current selection
	DocumentMetadata m_metadata;                           ///< Metadata (word count, etc.)

	std::deque<std::unique_ptr<ITextCommand>> m_undoStack; ///< Undo stack
	std::deque<std::unique_ptr<ITextCommand>> m_redoStack; ///< Redo stack
	int m_maxUndoStack;                                    ///< Maximum undo stack size

	std::vector<IDocumentObserver*> m_observers;           ///< Registered observers
};

} // namespace gui
} // namespace bwx_sdk

#endif // _BWX_TEXT_DOCUMENT_H_
