/////////////////////////////////////////////////////////////////////////////
// Name:        bwx_text_renderer.h
// Purpose:     Text renderer interface and Full View renderer
// Author:      Kalahari Team
// Created:     2025-11-04
// Copyright:   (c) 2025 Kalahari Project
// Licence:     wxWidgets licence
/////////////////////////////////////////////////////////////////////////////

#ifndef _BWX_TEXT_RENDERER_H_
#define _BWX_TEXT_RENDERER_H_

#include <wx/wxprec.h>
#ifndef WX_PRECOMP
	#include <wx/wx.h>
#endif

#include <wx/graphics.h>
#include <vector>
#include <memory>
#include <map>

#include <bwx_sdk/bwx_gui/bwx_text_document.h>

namespace bwx_sdk {
namespace gui {

// Forward declaration
class bwxTextDocument;

// ============================================================================
// Text Renderer Interface (Strategy Pattern)
// ============================================================================

/// Abstract renderer interface - allows swapping rendering strategies
/// (MVP: FullViewRenderer; Future: PageViewRenderer, TypewriterRenderer, PublisherRenderer)
class ITextRenderer
{
public:
	virtual ~ITextRenderer() = default;

	/// Render document content to device context
	/// @param dc Device context to render to
	/// @param clientRect Client area rectangle
	/// @param scrollY Vertical scroll position
	virtual void Render(wxDC& dc, const wxRect& clientRect, int scrollY) = 0;

	/// Hit test - convert screen coordinates to document position
	/// @param x X coordinate (client coords)
	/// @param y Y coordinate (client coords)
	/// @param scrollY Vertical scroll position
	/// @return Document position (linear offset), or -1 if outside text
	virtual int HitTest(int x, int y, int scrollY) const = 0;

	/// Get cursor rectangle for given document position
	/// @param position Document position
	/// @return Rectangle for cursor (line from top to bottom of character)
	virtual wxRect GetCursorRect(int position) const = 0;

	/// Get selection rectangles for given range
	/// @param startPos Start of selection
	/// @param endPos End of selection
	/// @return Vector of rectangles covering selection (multi-line support)
	virtual std::vector<wxRect> GetSelectionRects(int startPos, int endPos) const = 0;

	/// Notify renderer of size change (triggers layout recalculation)
	/// @param width New client width
	/// @param height New client height
	virtual void OnResize(int width, int height) = 0;

	/// Invalidate layout (forces recalculation on next render)
	virtual void InvalidateLayout() = 0;

	/// Get total content height (for scrollbar)
	/// @return Total height in pixels
	virtual int GetTotalHeight() const = 0;

	/// Set document reference
	/// @param doc Document to render
	virtual void SetDocument(bwxTextDocument* doc) = 0;
};

// ============================================================================
// Full View Renderer - Continuous text, no page boundaries
// ============================================================================

/// Full View renderer - continuous text flow (HTML-like)
///
/// Features:
/// - Word wrap at client width minus margins
/// - No page boundaries
/// - Vertical scrolling only
/// - Efficient viewport culling (render visible lines only)
/// - Layout caching (recalculate only on text/size change)
class FullViewRenderer : public ITextRenderer
{
public:
	/// Constructor
	FullViewRenderer();

	/// Destructor
	~FullViewRenderer();

	// ========================================================================
	// ITextRenderer Implementation
	// ========================================================================

	void Render(wxDC& dc, const wxRect& clientRect, int scrollY) override;
	int HitTest(int x, int y, int scrollY) const override;
	wxRect GetCursorRect(int position) const override;
	std::vector<wxRect> GetSelectionRects(int startPos, int endPos) const override;
	void OnResize(int width, int height) override;
	void InvalidateLayout() override;
	int GetTotalHeight() const override;
	void SetDocument(bwxTextDocument* doc) override;

	// ========================================================================
	// Configuration
	// ========================================================================

	/// Set left margin
	void SetMarginLeft(int margin) { m_marginLeft = margin; InvalidateLayout(); }

	/// Set right margin
	void SetMarginRight(int margin) { m_marginRight = margin; InvalidateLayout(); }

	/// Set line spacing multiplier (1.0 = single spacing, 1.5 = 1.5x, 2.0 = double)
	void SetLineSpacing(double spacing) { m_lineSpacing = spacing; InvalidateLayout(); }

	/// Set selection color
	/// @param color RGB color for selection background
	void SetSelectionColor(const wxColour& color) { m_selectionColor = color; }

	/// Set selection opacity
	/// @param opacity Alpha value (0-255, where 0 = transparent, 255 = opaque)
	void SetSelectionOpacity(int opacity) { m_selectionOpacity = opacity; }

	/// Get left margin
	int GetMarginLeft() const { return m_marginLeft; }

	/// Get right margin
	int GetMarginRight() const { return m_marginRight; }

	/// Get line spacing
	double GetLineSpacing() const { return m_lineSpacing; }

	/// Get selection color
	wxColour GetSelectionColor() const { return m_selectionColor; }

	/// Get selection opacity
	int GetSelectionOpacity() const { return m_selectionOpacity; }

private:
	// ========================================================================
	// Layout Structures
	// ========================================================================

	/// Single character position info (for hit testing)
	struct CharInfo
	{
		int x;          ///< X position in line
		int width;      ///< Character width
	};

	/// Layout line - single line of wrapped text
	struct LayoutLine
	{
		int startPos;                   ///< Start position in document
		int endPos;                     ///< End position in document (exclusive)
		int y;                          ///< Y position (top of line)
		int height;                     ///< Line height (max font height in line)
		std::vector<CharInfo> charInfo; ///< Character positions (for hit testing)

		/// Check if document position is in this line
		bool Contains(int pos) const { return pos >= startPos && pos < endPos; }
	};

	// ========================================================================
	// Layout Calculation
	// ========================================================================

	/// Calculate layout (word wrap, line breaks)
	void CalculateLayout();

	/// Calculate layout for single line (word wrap)
	void CalculateLine(const wxString& text, int startPos, wxDC& dc, int& yPos);

	/// Measure text extent with formatting
	wxSize MeasureText(const wxString& text, const TextFormat& format, wxDC& dc);

	/// Get or create font from format
	wxFont GetFont(const TextFormat& format);

	/// Check if layout is valid
	bool IsLayoutValid() const { return m_layoutValid; }

	// ========================================================================
	// Rendering Helpers
	// ========================================================================

	/// Render single line
	void RenderLine(wxDC& dc, const LayoutLine& line, int scrollY);

	/// Render cursor
	void RenderCursor(wxDC& dc, int scrollY);

	/// Render selection
	void RenderSelection(wxDC& dc, int scrollY);

	/// Apply text format to DC (font, color)
	void ApplyFormat(wxDC& dc, const TextFormat& format);

	/// Get visible line range (for viewport culling)
	void GetVisibleLineRange(int scrollY, int clientHeight, int& firstLine, int& lastLine) const;

	// ========================================================================
	// Member Variables
	// ========================================================================

	bwxTextDocument* m_document;        ///< Document reference
	std::vector<LayoutLine> m_lines;    ///< Layout lines
	int m_clientWidth;                  ///< Client width
	int m_clientHeight;                 ///< Client height
	int m_marginLeft;                   ///< Left margin
	int m_marginRight;                  ///< Right margin
	double m_lineSpacing;               ///< Line spacing multiplier
	bool m_layoutValid;                 ///< Is layout up-to-date?

	// Selection appearance (Task #00019 Settings)
	wxColour m_selectionColor;          ///< Selection background color
	int m_selectionOpacity;             ///< Selection opacity (0-255)

	// Font cache (expensive to create, so reuse)
	std::map<wxString, wxFont> m_fontCache; ///< Font cache (key = fontName-size-bold-italic-underline)
};

} // namespace gui
} // namespace bwx_sdk

#endif // _BWX_TEXT_RENDERER_H_
