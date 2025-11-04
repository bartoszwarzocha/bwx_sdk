/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_text_document.cpp
// Purpose:     Document model implementation - Gap Buffer storage, formatting, undo/redo
// Author:      Kalahari Team
// Created:     2025-11-04
// Copyright:   (c) 2025 Kalahari Project
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <bwx_sdk/bwx_gui/bwx_text_document.h>
#include <algorithm>
#include <fstream>

namespace bwx_sdk {
namespace gui {

// ============================================================================
// Gap Buffer Storage Implementation
// ============================================================================

/// Gap Buffer text storage - efficient for sequential editing
///
/// Buffer layout: [text before gap][GAP][text after gap]
/// Example: "Hello World" with cursor at position 6
/// Buffer: [H][e][l][l][o][ ][_][_][_][W][o][r][l][d]
///                          ↑ gap_start = 6
///                          ↑ gap_end = 9 (gap size = 3)
///
/// Operations:
/// - Insert at gap: O(1) - fill gap, expand if needed
/// - Insert elsewhere: O(n) - move gap to position
/// - Delete at gap: O(1) - expand gap
/// - Get char: O(1) - check if before/after gap
class GapBufferStorage : public ITextStorage
{
public:
	/// Constructor with initial capacity
	explicit GapBufferStorage(int initialCapacity = 1024)
		: m_buffer(nullptr)
		, m_bufferSize(0)
		, m_gapStart(0)
		, m_gapEnd(0)
	{
		Allocate(initialCapacity);
	}

	/// Destructor
	~GapBufferStorage()
	{
		delete[] m_buffer;
	}

	// ========================================================================
	// ITextStorage Interface Implementation
	// ========================================================================

	wxString GetText() const override
	{
		if (m_gapStart == 0 && m_gapEnd == m_bufferSize)
			return wxEmptyString;

		// Concatenate text before gap and after gap
		wxString result;
		result.Alloc(GetLength());

		// Text before gap
		for (int i = 0; i < m_gapStart; ++i)
			result.Append(m_buffer[i]);

		// Text after gap
		for (int i = m_gapEnd; i < m_bufferSize; ++i)
			result.Append(m_buffer[i]);

		return result;
	}

	wxString GetText(int startPos, int endPos) const override
	{
		if (startPos < 0 || endPos > GetLength() || startPos >= endPos)
			return wxEmptyString;

		wxString result;
		result.Alloc(endPos - startPos);

		for (int i = startPos; i < endPos; ++i)
			result.Append(GetChar(i));

		return result;
	}

	void SetText(const wxString& text) override
	{
		int newLength = text.Length();
		int newCapacity = std::max(1024, newLength + 512);

		// Reallocate buffer
		delete[] m_buffer;
		m_buffer = new wxChar[newCapacity];
		m_bufferSize = newCapacity;

		// Copy text to buffer (gap at end)
		for (int i = 0; i < newLength; ++i)
			m_buffer[i] = text[i];

		m_gapStart = newLength;
		m_gapEnd = m_bufferSize;
	}

	void InsertText(int pos, const wxString& text) override
	{
		if (pos < 0 || pos > GetLength())
			return;

		int textLength = text.Length();
		if (textLength == 0)
			return;

		// Move gap to insertion position
		MoveGapTo(pos);

		// Expand gap if needed
		int gapSize = m_gapEnd - m_gapStart;
		if (textLength > gapSize)
			ExpandGap(textLength - gapSize + 512);

		// Insert text into gap
		for (int i = 0; i < textLength; ++i)
			m_buffer[m_gapStart++] = text[i];
	}

	void DeleteText(int startPos, int endPos) override
	{
		if (startPos < 0 || endPos > GetLength() || startPos >= endPos)
			return;

		// Move gap to deletion start
		MoveGapTo(startPos);

		// Expand gap to cover deleted text
		m_gapEnd += (endPos - startPos);
	}

	wxChar GetChar(int pos) const override
	{
		if (pos < 0 || pos >= GetLength())
			return 0;

		// Character is either before gap or after gap
		if (pos < m_gapStart)
			return m_buffer[pos];
		else
			return m_buffer[pos + (m_gapEnd - m_gapStart)];
	}

	int GetLength() const override
	{
		return m_bufferSize - (m_gapEnd - m_gapStart);
	}

	void Clear() override
	{
		m_gapStart = 0;
		m_gapEnd = m_bufferSize;
	}

private:
	// ========================================================================
	// Private Helper Methods
	// ========================================================================

	/// Allocate buffer with given capacity
	void Allocate(int capacity)
	{
		m_buffer = new wxChar[capacity];
		m_bufferSize = capacity;
		m_gapStart = 0;
		m_gapEnd = capacity;
	}

	/// Move gap to position
	void MoveGapTo(int pos)
	{
		if (pos < m_gapStart)
		{
			// Move gap backward
			int distance = m_gapStart - pos;
			for (int i = 0; i < distance; ++i)
				m_buffer[m_gapEnd - 1 - i] = m_buffer[m_gapStart - 1 - i];

			m_gapStart -= distance;
			m_gapEnd -= distance;
		}
		else if (pos > m_gapStart)
		{
			// Move gap forward
			int distance = pos - m_gapStart;
			for (int i = 0; i < distance; ++i)
				m_buffer[m_gapStart + i] = m_buffer[m_gapEnd + i];

			m_gapStart += distance;
			m_gapEnd += distance;
		}
	}

	/// Expand gap by additionalSize
	void ExpandGap(int additionalSize)
	{
		int newCapacity = m_bufferSize + additionalSize;
		wxChar* newBuffer = new wxChar[newCapacity];

		// Copy text before gap
		for (int i = 0; i < m_gapStart; ++i)
			newBuffer[i] = m_buffer[i];

		// Copy text after gap (moved forward)
		int afterGapSize = m_bufferSize - m_gapEnd;
		for (int i = 0; i < afterGapSize; ++i)
			newBuffer[newCapacity - afterGapSize + i] = m_buffer[m_gapEnd + i];

		delete[] m_buffer;
		m_buffer = newBuffer;
		m_gapEnd = newCapacity - afterGapSize;
		m_bufferSize = newCapacity;
	}

	// ========================================================================
	// Member Variables
	// ========================================================================

	wxChar* m_buffer;      ///< Character buffer
	int m_bufferSize;      ///< Total buffer size
	int m_gapStart;        ///< Start of gap
	int m_gapEnd;          ///< End of gap (exclusive)
};

// ============================================================================
// Text Command Implementations
// ============================================================================

/// Insert text command
class InsertTextCommand : public ITextCommand
{
public:
	InsertTextCommand(int pos, const wxString& text)
		: m_position(pos), m_text(text)
	{
	}

	void Execute(bwxTextDocument* doc) override
	{
		doc->InsertTextInternal(m_position, m_text);
	}

	void Undo(bwxTextDocument* doc) override
	{
		doc->DeleteTextInternal(m_position, m_position + m_text.Length());
	}

	bool CanMerge(const ITextCommand* other) const override
	{
		const InsertTextCommand* insertCmd = dynamic_cast<const InsertTextCommand*>(other);
		if (!insertCmd)
			return false;

		// Only merge single-character insertions (typical typing)
		// Don't merge multi-character inserts (programmatic operations)
		if (m_text.Length() != 1 || insertCmd->m_text.Length() != 1)
			return false;

		// Can merge if inserting at adjacent position
		return insertCmd->m_position == m_position + (int)m_text.Length();
	}

	void Merge(ITextCommand* other) override
	{
		InsertTextCommand* insertCmd = dynamic_cast<InsertTextCommand*>(other);
		if (insertCmd)
			m_text += insertCmd->m_text;
	}

	wxString GetTypeName() const override { return "InsertText"; }

private:
	int m_position;
	wxString m_text;
};

/// Delete text command
class DeleteTextCommand : public ITextCommand
{
public:
	DeleteTextCommand(int startPos, int endPos)
		: m_startPos(startPos), m_endPos(endPos), m_deletedText()
	{
	}

	void Execute(bwxTextDocument* doc) override
	{
		// Store deleted text for undo
		m_deletedText = doc->GetText(m_startPos, m_endPos);
		doc->DeleteTextInternal(m_startPos, m_endPos);
	}

	void Undo(bwxTextDocument* doc) override
	{
		doc->InsertTextInternal(m_startPos, m_deletedText);
	}

	wxString GetTypeName() const override { return "DeleteText"; }

private:
	int m_startPos;
	int m_endPos;
	wxString m_deletedText;
};

/// Apply format command
class ApplyFormatCommand : public ITextCommand
{
public:
	ApplyFormatCommand(int startPos, int endPos, const TextFormat& format)
		: m_startPos(startPos), m_endPos(endPos), m_newFormat(format)
	{
	}

	void Execute(bwxTextDocument* doc) override
	{
		// Store old format runs for undo
		m_oldRuns = doc->GetFormatRuns(m_startPos, m_endPos);
		doc->ApplyFormatInternal(m_startPos, m_endPos, m_newFormat);
	}

	void Undo(bwxTextDocument* doc) override
	{
		doc->RestoreFormatRuns(m_startPos, m_endPos, m_oldRuns);
	}

	wxString GetTypeName() const override { return "ApplyFormat"; }

private:
	int m_startPos;
	int m_endPos;
	TextFormat m_newFormat;
	std::vector<FormatRun> m_oldRuns;
};

// ============================================================================
// bwxTextDocument Implementation
// ============================================================================

bwxTextDocument::bwxTextDocument()
	: m_storage(std::make_unique<GapBufferStorage>())
	, m_formatRuns()
	, m_cursor()
	, m_selection()
	, m_metadata()
	, m_undoStack()
	, m_redoStack()
	, m_maxUndoStack(100)
	, m_observers()
{
	// Initialize with default format run covering empty document
	m_formatRuns.push_back(FormatRun(0, 0, TextFormat()));
}

bwxTextDocument::~bwxTextDocument()
{
	// Clear undo/redo stacks (unique_ptr handles deletion)
	m_undoStack.clear();
	m_redoStack.clear();
}

// ============================================================================
// Text Operations
// ============================================================================

wxString bwxTextDocument::GetText() const
{
	return m_storage->GetText();
}

wxString bwxTextDocument::GetText(int startPos, int endPos) const
{
	return m_storage->GetText(startPos, endPos);
}

void bwxTextDocument::SetText(const wxString& text)
{
	m_storage->SetText(text);

	// Reset format runs
	m_formatRuns.clear();
	m_formatRuns.push_back(FormatRun(0, text.Length(), TextFormat()));

	// Reset cursor and selection
	m_cursor = Cursor();
	m_selection = Selection();

	// Clear undo history (new document)
	ClearUndoHistory();

	// Update metadata
	UpdateWordCount();
	m_metadata.modified = wxDateTime::Now();

	NotifyTextChanged();
}

void bwxTextDocument::InsertText(int pos, const wxString& text)
{
	if (text.IsEmpty())
		return;

	// Create and execute command
	auto cmd = std::make_unique<InsertTextCommand>(pos, text);
	cmd->Execute(this);

	AddCommand(std::move(cmd));
}

void bwxTextDocument::DeleteText(int startPos, int endPos)
{
	if (startPos >= endPos)
		return;

	// Create and execute command
	auto cmd = std::make_unique<DeleteTextCommand>(startPos, endPos);
	cmd->Execute(this);

	AddCommand(std::move(cmd));
}

wxChar bwxTextDocument::GetChar(int pos) const
{
	return m_storage->GetChar(pos);
}

int bwxTextDocument::GetLength() const
{
	return m_storage->GetLength();
}

void bwxTextDocument::Clear()
{
	SetText(wxEmptyString);
}

// ============================================================================
// Internal Text Operations (called by commands, no undo)
// ============================================================================

void bwxTextDocument::InsertTextInternal(int pos, const wxString& text)
{
	if (text.IsEmpty())
		return;

	int textLength = text.Length();

	// Insert into storage
	m_storage->InsertText(pos, text);

	// Adjust format runs
	for (auto& run : m_formatRuns)
	{
		if (pos < run.startPos)
		{
			// Insertion is before this run - shift entire run
			run.startPos += textLength;
			run.endPos += textLength;
		}
		else if (pos >= run.startPos && pos < run.endPos)
		{
			// Insertion is inside this run - expand run
			run.endPos += textLength;
		}
		else if (pos == run.endPos && run.startPos == run.endPos)
		{
			// Special case: empty run at insertion point - expand it
			run.endPos += textLength;
		}
		// If pos >= run.endPos (and run is not empty), don't modify the run
	}

	// Adjust cursor
	if (m_cursor.position >= pos)
		m_cursor.position += textLength;

	// Adjust selection
	if (m_selection.active)
	{
		if (m_selection.startPos >= pos)
			m_selection.startPos += textLength;
		if (m_selection.endPos >= pos)
			m_selection.endPos += textLength;
	}

	// Update metadata
	UpdateWordCount();
	m_metadata.modified = wxDateTime::Now();

	NotifyTextChanged();
}

void bwxTextDocument::DeleteTextInternal(int startPos, int endPos)
{
	if (startPos >= endPos)
		return;

	int deleteLength = endPos - startPos;

	// Delete from storage
	m_storage->DeleteText(startPos, endPos);

	// Adjust format runs
	auto it = m_formatRuns.begin();
	while (it != m_formatRuns.end())
	{
		if (it->endPos <= startPos)
		{
			// Run entirely before deletion - no change
			++it;
		}
		else if (it->startPos >= endPos)
		{
			// Run entirely after deletion - shift left
			it->startPos -= deleteLength;
			it->endPos -= deleteLength;
			++it;
		}
		else if (it->startPos >= startPos && it->endPos <= endPos)
		{
			// Run entirely within deletion - remove
			it = m_formatRuns.erase(it);
		}
		else if (it->startPos < startPos && it->endPos > endPos)
		{
			// Run spans deletion - shrink
			it->endPos -= deleteLength;
			++it;
		}
		else if (it->startPos < startPos)
		{
			// Run starts before, ends within deletion - truncate
			it->endPos = startPos;
			++it;
		}
		else
		{
			// Run starts within deletion, ends after - shift and truncate
			it->startPos = startPos;
			it->endPos -= deleteLength;
			++it;
		}
	}

	// Ensure at least one format run exists
	if (m_formatRuns.empty())
		m_formatRuns.push_back(FormatRun(0, 0, TextFormat()));

	// Adjust cursor
	if (m_cursor.position >= endPos)
		m_cursor.position -= deleteLength;
	else if (m_cursor.position > startPos)
		m_cursor.position = startPos;

	// Adjust selection
	if (m_selection.active)
	{
		if (m_selection.startPos >= endPos)
			m_selection.startPos -= deleteLength;
		else if (m_selection.startPos > startPos)
			m_selection.startPos = startPos;

		if (m_selection.endPos >= endPos)
			m_selection.endPos -= deleteLength;
		else if (m_selection.endPos > startPos)
			m_selection.endPos = startPos;

		if (m_selection.startPos == m_selection.endPos)
			m_selection.active = false;
	}

	// Update metadata
	UpdateWordCount();
	m_metadata.modified = wxDateTime::Now();

	NotifyTextChanged();
}

// ============================================================================
// Formatting Operations
// ============================================================================

void bwxTextDocument::ApplyFormat(int startPos, int endPos, const TextFormat& format)
{
	if (startPos >= endPos)
		return;

	// Create and execute command
	auto cmd = std::make_unique<ApplyFormatCommand>(startPos, endPos, format);
	cmd->Execute(this);

	AddCommand(std::move(cmd));
}

void bwxTextDocument::ApplyFormatInternal(int startPos, int endPos, const TextFormat& format)
{
	if (startPos >= endPos)
		return;

	// Split runs at boundaries
	SplitRunAt(startPos);
	SplitRunAt(endPos);

	// Apply format to runs in range
	for (auto& run : m_formatRuns)
	{
		if (run.startPos >= startPos && run.endPos <= endPos)
			run.format = format;
	}

	// Merge adjacent runs with same format
	MergeAdjacentRuns();

	NotifyFormatChanged();
}

TextFormat bwxTextDocument::GetFormatAt(int pos) const
{
	// Binary search for run containing position
	for (const auto& run : m_formatRuns)
	{
		if (run.Contains(pos))
			return run.format;
	}

	// Default format
	return TextFormat();
}

std::vector<FormatRun> bwxTextDocument::GetFormatRuns(int startPos, int endPos) const
{
	std::vector<FormatRun> result;

	for (const auto& run : m_formatRuns)
	{
		if (run.endPos <= startPos)
			continue;
		if (run.startPos >= endPos)
			break;

		// Run intersects range
		FormatRun intersect = run;
		intersect.startPos = std::max(intersect.startPos, startPos);
		intersect.endPos = std::min(intersect.endPos, endPos);
		result.push_back(intersect);
	}

	return result;
}

void bwxTextDocument::RestoreFormatRuns(int startPos, int endPos, const std::vector<FormatRun>& runs)
{
	// Remove runs in range
	auto it = m_formatRuns.begin();
	while (it != m_formatRuns.end())
	{
		if (it->startPos >= startPos && it->endPos <= endPos)
			it = m_formatRuns.erase(it);
		else
			++it;
	}

	// Insert restored runs
	m_formatRuns.insert(m_formatRuns.end(), runs.begin(), runs.end());

	// Sort and merge
	std::sort(m_formatRuns.begin(), m_formatRuns.end(),
		[](const FormatRun& a, const FormatRun& b) { return a.startPos < b.startPos; });
	MergeAdjacentRuns();

	NotifyFormatChanged();
}

void bwxTextDocument::ClearFormatting()
{
	m_formatRuns.clear();
	m_formatRuns.push_back(FormatRun(0, GetLength(), TextFormat()));
	NotifyFormatChanged();
}

void bwxTextDocument::ClearFormatting(int startPos, int endPos)
{
	ApplyFormat(startPos, endPos, TextFormat());
}

void bwxTextDocument::SplitRunAt(int pos)
{
	for (size_t i = 0; i < m_formatRuns.size(); ++i)
	{
		FormatRun& run = m_formatRuns[i];
		if (pos > run.startPos && pos < run.endPos)
		{
			// Split run at position
			FormatRun newRun(pos, run.endPos, run.format);
			run.endPos = pos;
			m_formatRuns.insert(m_formatRuns.begin() + i + 1, newRun);
			return;
		}
	}
}

void bwxTextDocument::MergeAdjacentRuns()
{
	if (m_formatRuns.size() <= 1)
		return;

	std::vector<FormatRun> merged;
	merged.push_back(m_formatRuns[0]);

	for (size_t i = 1; i < m_formatRuns.size(); ++i)
	{
		FormatRun& last = merged.back();
		const FormatRun& current = m_formatRuns[i];

		if (last.endPos == current.startPos && last.format == current.format)
		{
			// Merge with previous run
			last.endPos = current.endPos;
		}
		else
		{
			merged.push_back(current);
		}
	}

	m_formatRuns = merged;
}

// ============================================================================
// Cursor & Selection
// ============================================================================

void bwxTextDocument::SetCursor(const Cursor& cursor)
{
	m_cursor = cursor;
	m_cursor.position = std::max(0, std::min(m_cursor.position, GetLength()));
	UpdateCursorLineColumn();
	NotifyCursorMoved();
}

void bwxTextDocument::SetCursorPosition(int pos)
{
	m_cursor.position = std::max(0, std::min(pos, GetLength()));
	UpdateCursorLineColumn();
	NotifyCursorMoved();
}

void bwxTextDocument::MoveCursor(int offset)
{
	SetCursorPosition(m_cursor.position + offset);
}

void bwxTextDocument::SetSelection(int startPos, int endPos)
{
	m_selection.startPos = std::max(0, std::min(startPos, GetLength()));
	m_selection.endPos = std::max(0, std::min(endPos, GetLength()));
	m_selection.active = (m_selection.startPos != m_selection.endPos);
	NotifySelectionChanged();
}

void bwxTextDocument::ClearSelection()
{
	m_selection.active = false;
	m_selection.startPos = 0;
	m_selection.endPos = 0;
	NotifySelectionChanged();
}

void bwxTextDocument::SelectAll()
{
	SetSelection(0, GetLength());
}

wxString bwxTextDocument::GetSelectedText() const
{
	if (!m_selection.active)
		return wxEmptyString;

	return GetText(m_selection.GetMin(), m_selection.GetMax());
}

bool bwxTextDocument::DeleteSelection()
{
	if (!m_selection.active || m_selection.IsEmpty())
		return false;

	DeleteText(m_selection.GetMin(), m_selection.GetMax());
	ClearSelection();
	return true;
}

void bwxTextDocument::UpdateCursorLineColumn()
{
	// Calculate line and column from position
	m_cursor.line = 0;
	m_cursor.column = 0;

	wxString text = GetText();
	for (int i = 0; i < m_cursor.position && i < (int)text.Length(); ++i)
	{
		if (text[i] == '\n')
		{
			m_cursor.line++;
			m_cursor.column = 0;
		}
		else
		{
			m_cursor.column++;
		}
	}
}

// ============================================================================
// Undo / Redo
// ============================================================================

void bwxTextDocument::Undo()
{
	if (!CanUndo())
		return;

	// Pop command from undo stack
	std::unique_ptr<ITextCommand> cmd = std::move(m_undoStack.back());
	m_undoStack.pop_back();

	// Execute undo
	cmd->Undo(this);

	// Push to redo stack
	m_redoStack.push_back(std::move(cmd));
}

void bwxTextDocument::Redo()
{
	if (!CanRedo())
		return;

	// Pop command from redo stack
	std::unique_ptr<ITextCommand> cmd = std::move(m_redoStack.back());
	m_redoStack.pop_back();

	// Execute redo
	cmd->Execute(this);

	// Push to undo stack
	m_undoStack.push_back(std::move(cmd));
}

bool bwxTextDocument::CanUndo() const
{
	return !m_undoStack.empty();
}

bool bwxTextDocument::CanRedo() const
{
	return !m_redoStack.empty();
}

void bwxTextDocument::ClearUndoHistory()
{
	m_undoStack.clear();
	m_redoStack.clear();
}

void bwxTextDocument::AddCommand(std::unique_ptr<ITextCommand> cmd)
{
	// Clear redo stack (new action invalidates redo)
	m_redoStack.clear();

	// Try to merge with last command (for consecutive typing)
	if (!m_undoStack.empty())
	{
		ITextCommand* lastCmd = m_undoStack.back().get();
		if (lastCmd->CanMerge(cmd.get()))
		{
			lastCmd->Merge(cmd.get());
			return;
		}
	}

	// Add to undo stack
	m_undoStack.push_back(std::move(cmd));

	// Enforce undo limit
	if ((int)m_undoStack.size() > m_maxUndoStack)
		m_undoStack.pop_front();
}

// ============================================================================
// Metadata
// ============================================================================

void bwxTextDocument::UpdateWordCount()
{
	wxString text = GetText();
	m_metadata.characterCount = text.Length();

	// Count words (simple algorithm: split by whitespace)
	m_metadata.wordCount = 0;
	bool inWord = false;

	for (size_t i = 0; i < text.Length(); ++i)
	{
		wxChar ch = text[i];
		bool isWhitespace = (ch == ' ' || ch == '\t' || ch == '\n' || ch == '\r');

		if (!isWhitespace && !inWord)
		{
			m_metadata.wordCount++;
			inWord = true;
		}
		else if (isWhitespace)
		{
			inWord = false;
		}
	}
}

// ============================================================================
// File I/O (Stub - will implement with JSON in Day 11-12)
// ============================================================================

bool bwxTextDocument::LoadFromFile(const wxString& path)
{
	// TODO: Implement JSON loading (Day 11-12)
	wxLogWarning("bwxTextDocument::LoadFromFile() not yet implemented");
	return false;
}

bool bwxTextDocument::SaveToFile(const wxString& path)
{
	// TODO: Implement JSON saving (Day 11-12)
	wxLogWarning("bwxTextDocument::SaveToFile() not yet implemented");
	return false;
}

// ============================================================================
// Observer Pattern
// ============================================================================

void bwxTextDocument::AddObserver(IDocumentObserver* observer)
{
	if (observer && std::find(m_observers.begin(), m_observers.end(), observer) == m_observers.end())
		m_observers.push_back(observer);
}

void bwxTextDocument::RemoveObserver(IDocumentObserver* observer)
{
	m_observers.erase(std::remove(m_observers.begin(), m_observers.end(), observer), m_observers.end());
}

void bwxTextDocument::NotifyTextChanged()
{
	for (auto* observer : m_observers)
		observer->OnTextChanged();
}

void bwxTextDocument::NotifyCursorMoved()
{
	for (auto* observer : m_observers)
		observer->OnCursorMoved();
}

void bwxTextDocument::NotifySelectionChanged()
{
	for (auto* observer : m_observers)
		observer->OnSelectionChanged();
}

void bwxTextDocument::NotifyFormatChanged()
{
	for (auto* observer : m_observers)
		observer->OnFormatChanged();
}

} // namespace gui
} // namespace bwx_sdk
