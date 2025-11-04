/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_text_renderer.cpp
// Purpose:     Full View text renderer implementation
// Author:      Kalahari Team
// Created:     2025-11-04
// Copyright:   (c) 2025 Kalahari Project
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <bwx_sdk/bwx_gui/bwx_text_renderer.h>
#include <algorithm>

namespace bwx_sdk {
namespace gui {

// ============================================================================
// FullViewRenderer Implementation
// ============================================================================

FullViewRenderer::FullViewRenderer()
	: m_document(nullptr)
	, m_lines()
	, m_clientWidth(800)
	, m_clientHeight(600)
	, m_marginLeft(20)
	, m_marginRight(20)
	, m_lineSpacing(1.2)
	, m_layoutValid(false)
	, m_fontCache()
{
}

FullViewRenderer::~FullViewRenderer()
{
	// Font cache cleared automatically (std::map destructor)
}

void FullViewRenderer::SetDocument(bwxTextDocument* doc)
{
	m_document = doc;
	InvalidateLayout();
}

void FullViewRenderer::OnResize(int width, int height)
{
	if (width != m_clientWidth || height != m_clientHeight)
	{
		m_clientWidth = width;
		m_clientHeight = height;
		InvalidateLayout();
	}
}

void FullViewRenderer::InvalidateLayout()
{
	m_layoutValid = false;
}

int FullViewRenderer::GetTotalHeight() const
{
	if (m_lines.empty())
		return 0;

	const LayoutLine& lastLine = m_lines.back();
	return lastLine.y + lastLine.height;
}

// ============================================================================
// Layout Calculation
// ============================================================================

void FullViewRenderer::CalculateLayout()
{
	if (!m_document || m_layoutValid)
		return;

	m_lines.clear();

	wxString text = m_document->GetText();
	if (text.IsEmpty())
	{
		m_layoutValid = true;
		return;
	}

	// Create temporary DC for measurements
	// Use client width/height for proper text extent calculations
	wxMemoryDC memDC;
	wxBitmap tempBitmap(std::max(m_clientWidth, 1), std::max(m_clientHeight, 1));
	memDC.SelectObject(tempBitmap);

	int yPos = m_marginLeft;  // Start with top margin

	// Process text line by line (split by \n)
	int lineStart = 0;
	for (size_t i = 0; i <= text.Length(); ++i)
	{
		if (i == text.Length() || text[i] == '\n')
		{
			// Found line end
			wxString lineText = text.Mid(lineStart, i - lineStart);

			if (lineText.IsEmpty())
			{
				// Empty line (just newline)
				TextFormat defaultFormat;
				wxFont font = GetFont(defaultFormat);
				memDC.SetFont(font);

				wxSize textSize = memDC.GetTextExtent("M");  // Measure M for line height
				int lineHeight = static_cast<int>(textSize.GetHeight() * m_lineSpacing);

				LayoutLine emptyLine;
				emptyLine.startPos = lineStart;
				emptyLine.endPos = lineStart;
				emptyLine.y = yPos;
				emptyLine.height = lineHeight;
				m_lines.push_back(emptyLine);

				yPos += lineHeight;
			}
			else
			{
				// Non-empty line - word wrap
				CalculateLine(lineText, lineStart, memDC, yPos);
			}

			lineStart = i + 1;
		}
	}

	m_layoutValid = true;
}

void FullViewRenderer::CalculateLine(const wxString& text, int startPos, wxDC& dc, int& yPos)
{

	// Get default format for this line (simplified - take format at start position)
	TextFormat format = m_document->GetFormatAt(startPos);
	wxFont font = GetFont(format);
	dc.SetFont(font);

	// Word wrap algorithm
	std::vector<wxString> words;
	std::vector<int> wordStartPositions;

	// Split into words
	wxString currentWord;
	int wordStart = startPos;
	for (size_t i = 0; i < text.Length(); ++i)
	{
		wxChar ch = text[i];
		if (ch == ' ' || ch == '\t')
		{
			if (!currentWord.IsEmpty())
			{
				words.push_back(currentWord);
				wordStartPositions.push_back(wordStart);
				currentWord.Clear();
			}
			// Add space as separate "word"
			words.push_back(wxString(ch));
			wordStartPositions.push_back(startPos + i);
			wordStart = startPos + i + 1;
		}
		else
		{
			if (currentWord.IsEmpty())
				wordStart = startPos + i;
			currentWord += ch;
		}
	}
	if (!currentWord.IsEmpty())
	{
		words.push_back(currentWord);
		wordStartPositions.push_back(wordStart);
	}

	// Pack words into lines
	int currentX = m_marginLeft;
	int lineStartPos = startPos;
	wxString currentLine;
	std::vector<CharInfo> currentCharInfo;

	int lineHeight = static_cast<int>(dc.GetTextExtent("M").GetHeight() * m_lineSpacing);

	for (size_t i = 0; i < words.size(); ++i)
	{
		wxString word = words[i];
		wxSize wordSize = dc.GetTextExtent(word);

		if (currentX + wordSize.GetWidth() > m_clientWidth - m_marginRight && !currentLine.IsEmpty())
		{
			// Word doesn't fit - create new line
			LayoutLine line;
			line.startPos = lineStartPos;
			line.endPos = wordStartPositions[i];
			line.y = yPos;
			line.height = lineHeight;
			line.charInfo = currentCharInfo;
			m_lines.push_back(line);

			yPos += lineHeight;

			// Start new line with current word
			currentLine = word;
			currentX = m_marginLeft + wordSize.GetWidth();
			lineStartPos = wordStartPositions[i];

			// Calculate char info for new line
			currentCharInfo.clear();
			int charX = m_marginLeft;
			for (size_t j = 0; j < word.Length(); ++j)
			{
				wxString ch = word.Mid(j, 1);
				int charWidth = dc.GetTextExtent(ch).GetWidth();
				currentCharInfo.push_back({charX, charWidth});
				charX += charWidth;
			}
		}
		else
		{
			// Word fits - add to current line
			if (!currentLine.IsEmpty() || i > 0)
			{
				// Add char info for word
				int charX = currentX;
				for (size_t j = 0; j < word.Length(); ++j)
				{
					wxString ch = word.Mid(j, 1);
					int charWidth = dc.GetTextExtent(ch).GetWidth();
					currentCharInfo.push_back({charX, charWidth});
					charX += charWidth;
				}
			}
			else
			{
				// First word in line
				int charX = m_marginLeft;
				for (size_t j = 0; j < word.Length(); ++j)
				{
					wxString ch = word.Mid(j, 1);
					int charWidth = dc.GetTextExtent(ch).GetWidth();
					currentCharInfo.push_back({charX, charWidth});
					charX += charWidth;
				}
			}

			currentLine += word;
			currentX += wordSize.GetWidth();
		}
	}

	// Add final line
	if (!currentLine.IsEmpty())
	{
		LayoutLine line;
		line.startPos = lineStartPos;
		line.endPos = startPos + text.Length();
		line.y = yPos;
		line.height = lineHeight;
		line.charInfo = currentCharInfo;
		m_lines.push_back(line);

		yPos += lineHeight;
	}
}

// ============================================================================
// Rendering
// ============================================================================

void FullViewRenderer::Render(wxDC& dc, const wxRect& clientRect, int scrollY)
{
	if (!m_document)
		return;

	// Recalculate layout if needed
	if (!m_layoutValid)
		CalculateLayout();

	// Clear background
	dc.SetBackground(*wxWHITE_BRUSH);
	dc.Clear();

	// Get visible line range
	int firstLine, lastLine;
	GetVisibleLineRange(scrollY, clientRect.GetHeight(), firstLine, lastLine);

	// Render visible lines
	for (int i = firstLine; i <= lastLine && i < (int)m_lines.size(); ++i)
	{
		RenderLine(dc, m_lines[i], scrollY);
	}

	// Render selection
	if (m_document->GetSelection().active)
		RenderSelection(dc, scrollY);

	// Render cursor
	RenderCursor(dc, scrollY);
}

void FullViewRenderer::RenderLine(wxDC& dc, const LayoutLine& line, int scrollY)
{
	if (!m_document)
		return;

	int yPos = line.y - scrollY;

	// Get text for this line
	wxString lineText = m_document->GetText(line.startPos, line.endPos);

	// Get format runs for this line
	std::vector<FormatRun> runs = m_document->GetFormatRuns(line.startPos, line.endPos);

	if (runs.empty())
	{
		// No formatting - use default
		TextFormat defaultFormat;
		ApplyFormat(dc, defaultFormat);
		dc.DrawText(lineText, m_marginLeft, yPos);
	}
	else
	{
		// Render each format run
		int xPos = m_marginLeft;
		for (const FormatRun& run : runs)
		{
			int runStart = std::max(run.startPos, line.startPos) - line.startPos;
			int runEnd = std::min(run.endPos, line.endPos) - line.startPos;

			if (runStart < runEnd)
			{
				wxString runText = lineText.Mid(runStart, runEnd - runStart);
				ApplyFormat(dc, run.format);
				dc.DrawText(runText, xPos, yPos);

				// Advance x position
				wxSize textSize = dc.GetTextExtent(runText);
				xPos += textSize.GetWidth();
			}
		}
	}
}

void FullViewRenderer::RenderCursor(wxDC& dc, int scrollY)
{
	if (!m_document)
		return;

	Cursor cursor = m_document->GetCursor();
	wxRect cursorRect = GetCursorRect(cursor.position);

	if (cursorRect.IsEmpty())
		return;

	// Adjust for scroll
	cursorRect.y -= scrollY;

	// Draw cursor line
	dc.SetPen(*wxBLACK_PEN);
	dc.DrawLine(cursorRect.x, cursorRect.y, cursorRect.x, cursorRect.y + cursorRect.height);
}

void FullViewRenderer::RenderSelection(wxDC& dc, int scrollY)
{
	if (!m_document)
		return;

	Selection sel = m_document->GetSelection();
	if (!sel.active || sel.IsEmpty())
		return;

	std::vector<wxRect> rects = GetSelectionRects(sel.GetMin(), sel.GetMax());

	// Draw selection rectangles
	dc.SetBrush(wxBrush(wxColour(100, 150, 255, 128)));  // Semi-transparent blue
	dc.SetPen(*wxTRANSPARENT_PEN);

	for (wxRect rect : rects)
	{
		rect.y -= scrollY;
		dc.DrawRectangle(rect);
	}
}

void FullViewRenderer::ApplyFormat(wxDC& dc, const TextFormat& format)
{
	wxFont font = GetFont(format);
	dc.SetFont(font);
	dc.SetTextForeground(format.textColor);
	dc.SetTextBackground(format.backgroundColor);
}

void FullViewRenderer::GetVisibleLineRange(int scrollY, int clientHeight, int& firstLine, int& lastLine) const
{
	firstLine = 0;
	lastLine = m_lines.size() - 1;

	// Binary search for first visible line
	for (size_t i = 0; i < m_lines.size(); ++i)
	{
		if (m_lines[i].y + m_lines[i].height > scrollY)
		{
			firstLine = i;
			break;
		}
	}

	// Find last visible line
	for (size_t i = firstLine; i < m_lines.size(); ++i)
	{
		if (m_lines[i].y > scrollY + clientHeight)
		{
			lastLine = i - 1;
			break;
		}
	}

	// Clamp
	firstLine = std::max(0, firstLine);
	lastLine = std::min((int)m_lines.size() - 1, lastLine);
}

// ============================================================================
// Hit Testing
// ============================================================================

int FullViewRenderer::HitTest(int x, int y, int scrollY) const
{
	if (m_lines.empty())
		return 0;

	int adjustedY = y + scrollY;

	// Find line
	for (size_t i = 0; i < m_lines.size(); ++i)
	{
		const LayoutLine& line = m_lines[i];
		if (adjustedY >= line.y && adjustedY < line.y + line.height)
		{
			// Found line - now find character
			if (x < m_marginLeft)
				return line.startPos;

			int localX = x - m_marginLeft;
			for (size_t j = 0; j < line.charInfo.size(); ++j)
			{
				const CharInfo& charInfo = line.charInfo[j];
				int charLeft = charInfo.x - m_marginLeft;
				int charRight = charLeft + charInfo.width;

				if (localX >= charLeft && localX < charRight)
				{
					// Found character - return position
					// If click is in left half of char, return before char; otherwise after
					if (localX < charLeft + charInfo.width / 2)
						return line.startPos + j;
					else
						return line.startPos + j + 1;
				}
			}

			// Click after last character in line
			return line.endPos;
		}
	}

	// Click below all lines
	return m_document ? m_document->GetLength() : 0;
}

wxRect FullViewRenderer::GetCursorRect(int position) const
{
	if (m_lines.empty())
		return wxRect(m_marginLeft, 0, 1, 20);

	// Find line containing position
	for (const LayoutLine& line : m_lines)
	{
		if (line.Contains(position))
		{
			int localPos = position - line.startPos;
			if (localPos < (int)line.charInfo.size())
			{
				const CharInfo& charInfo = line.charInfo[localPos];
				return wxRect(charInfo.x, line.y, 1, line.height);
			}
			else
			{
				// Cursor at end of line
				int xPos = m_marginLeft;
				if (!line.charInfo.empty())
				{
					const CharInfo& lastChar = line.charInfo.back();
					xPos = lastChar.x + lastChar.width;
				}
				return wxRect(xPos, line.y, 1, line.height);
			}
		}
	}

	// Position not found - return at end
	if (!m_lines.empty())
	{
		const LayoutLine& lastLine = m_lines.back();
		int xPos = m_marginLeft;
		if (!lastLine.charInfo.empty())
		{
			const CharInfo& lastChar = lastLine.charInfo.back();
			xPos = lastChar.x + lastChar.width;
		}
		return wxRect(xPos, lastLine.y, 1, lastLine.height);
	}

	return wxRect(m_marginLeft, 0, 1, 20);
}

std::vector<wxRect> FullViewRenderer::GetSelectionRects(int startPos, int endPos) const
{
	std::vector<wxRect> rects;

	if (m_lines.empty() || startPos >= endPos)
		return rects;

	// Find lines intersecting selection
	for (const LayoutLine& line : m_lines)
	{
		if (line.endPos <= startPos || line.startPos >= endPos)
			continue;

		int selStart = std::max(startPos, line.startPos);
		int selEnd = std::min(endPos, line.endPos);

		int localStart = selStart - line.startPos;
		int localEnd = selEnd - line.startPos;

		int xStart = m_marginLeft;
		if (localStart > 0 && localStart <= (int)line.charInfo.size())
			xStart = line.charInfo[localStart - 1].x + line.charInfo[localStart - 1].width;

		int xEnd = m_marginLeft;
		if (localEnd > 0 && localEnd <= (int)line.charInfo.size())
		{
			if (localEnd < (int)line.charInfo.size())
				xEnd = line.charInfo[localEnd].x;
			else
			{
				const CharInfo& lastChar = line.charInfo.back();
				xEnd = lastChar.x + lastChar.width;
			}
		}

		wxRect selRect(xStart, line.y, xEnd - xStart, line.height);
		rects.push_back(selRect);
	}

	return rects;
}

// ============================================================================
// Font Management
// ============================================================================

wxFont FullViewRenderer::GetFont(const TextFormat& format)
{
	// Create cache key
	wxString key = wxString::Format("%s-%d-%d-%d-%d",
		format.fontName,
		format.fontSize,
		format.bold ? 1 : 0,
		format.italic ? 1 : 0,
		format.underline ? 1 : 0);

	// Check cache
	auto it = m_fontCache.find(key);
	if (it != m_fontCache.end())
		return it->second;

	// Create new font
	wxFont font(format.fontSize,
		wxFONTFAMILY_DEFAULT,
		format.italic ? wxFONTSTYLE_ITALIC : wxFONTSTYLE_NORMAL,
		format.bold ? wxFONTWEIGHT_BOLD : wxFONTWEIGHT_NORMAL,
		format.underline,
		format.fontName);

	// Cache it
	m_fontCache[key] = font;

	return font;
}

wxSize FullViewRenderer::MeasureText(const wxString& text, const TextFormat& format, wxDC& dc)
{
	wxFont font = GetFont(format);
	dc.SetFont(font);
	return dc.GetTextExtent(text);
}

} // namespace gui
} // namespace bwx_sdk
