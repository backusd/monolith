#include "Layout.h"
#include "Control.h"

std::shared_ptr<Control> Layout::m_mouseCapturedControl = nullptr;
bool Layout::m_mouseAlreadyCapturedForThisMessage = false;

Layout::Layout(D2D1_RECT_F rect) : 
	Layout(rect.top, rect.left, rect.bottom - rect.top, rect.right - rect.left)
{
}

Layout::Layout(float top, float left, float height, float width) :
	m_top(top),
	m_left(left),
	m_height(height),
	m_width(width)
{
	// Default grid layout to one row and one column that fills the window
	RowColDefinitions defaultDefinition;
	defaultDefinition.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);

	SetRowDefinitions(defaultDefinition);
	SetColumnDefinitions(defaultDefinition);
}

void Layout::OnResize(D2D1_RECT_F rect)
{
	OnResize(rect.top, rect.left, rect.bottom - rect.top, rect.right - rect.left);
}

void Layout::OnResize(float top, float left, float height, float width) 
{ 
	m_top = top; 
	m_left = left; 
	m_height = height;
	m_width = width; 

	UpdateLayout(); 

	// Update the sub-layouts AFTER the layout has been updated
	float t, l, h, w;
	int rowIndex, colIndex;
	std::shared_ptr<Layout> layout;
	for (std::tuple<std::shared_ptr<Layout>, int, int> tup : m_subLayouts)
	{
		layout = tup._Myfirst._Val;
		rowIndex = tup._Get_rest()._Myfirst._Val;
		colIndex = tup._Get_rest()._Get_rest()._Myfirst._Val;

		t = m_rows[rowIndex].Top();
		l = m_columns[colIndex].Left();
		h = m_rows[rowIndex].Height();
		w = m_columns[colIndex].Width();

		layout->OnResize(t, l, h, w);
	}

	// Now re-size the controls
	for (std::shared_ptr<Control> control : m_controls)
	{
		control->OnLayoutResize();
	}
}

void Layout::UpdateLayout()
{
	// Clear existing definitions
	m_rows.clear();
	m_columns.clear();

	// Rows ---------------------------------------------------------

	// Compute star size for rows
	float starSize = 0.0f;
	float starCounter = 0.0f;
	float fixedCounter = 0.0f;
	for (unsigned int iii = 0; iii < m_rowDefinitions.Count(); ++iii)
	{
		switch (m_rowDefinitions.GetDefinition(iii).Type())
		{
		case ROW_COL_TYPE::ROW_COL_TYPE_STAR:  starCounter += m_rowDefinitions.GetDefinition(iii).Value(); break;
		case ROW_COL_TYPE::ROW_COL_TYPE_FIXED: fixedCounter += m_rowDefinitions.GetDefinition(iii).Value(); break;
		}
	}

	// Star size = (window height - fixed pixel heights) / number of stars
	starSize = (m_height - fixedCounter) / starCounter;

	// Now create each row
	float rowTop = m_top;		// This will be incremented as we add each row
	float rowLeft = m_left;		// All rows will be aligned to the left
	float rowHeight = 0.0f;		// Will be computed for each row
	float rowWidth = m_width;	// All rows will span the width of the layout

	for (unsigned int iii = 0; iii < m_rowDefinitions.Count(); ++iii)
	{
		switch (m_rowDefinitions.GetDefinition(iii).Type())
		{
		case ROW_COL_TYPE::ROW_COL_TYPE_STAR:
			rowHeight = starSize * m_rowDefinitions.GetDefinition(iii).Value();
			m_rows.push_back(RowCol(rowTop, rowLeft, rowHeight, rowWidth));
			rowTop += rowHeight;
			break;

		case ROW_COL_TYPE::ROW_COL_TYPE_FIXED:
			rowHeight = m_rowDefinitions.GetDefinition(iii).Value();
			m_rows.push_back(RowCol(rowTop, rowLeft, rowHeight, rowWidth));
			rowTop += rowHeight;
			break;
		}
	}

	// Columns ---------------------------------------------------------
	// Compute star size for columns
	starSize = 0.0f;
	starCounter = 0.0f;
	fixedCounter = 0.0f;
	for (unsigned int iii = 0; iii < m_columnDefinitions.Count(); ++iii)
	{
		switch (m_columnDefinitions.GetDefinition(iii).Type())
		{
		case ROW_COL_TYPE::ROW_COL_TYPE_STAR:  starCounter += m_columnDefinitions.GetDefinition(iii).Value(); break;
		case ROW_COL_TYPE::ROW_COL_TYPE_FIXED: fixedCounter += m_columnDefinitions.GetDefinition(iii).Value(); break;
		}
	}

	// Star size = (window width - fixed pixels) / number of stars
	starSize = (m_width - fixedCounter) / starCounter;

	// Now create each column
	float colTop = m_top;		// Top of each column is always top of layout
	float colLeft = m_left;		// This value will be incremented as we add columns
	float colHeight = m_height;	// All columns will span the height of the layout
	float colWidth = 0.0f;		// Will be computed for each column

	for (unsigned int iii = 0; iii < m_columnDefinitions.Count(); ++iii)
	{
		switch (m_columnDefinitions.GetDefinition(iii).Type())
		{
		case ROW_COL_TYPE::ROW_COL_TYPE_STAR:
			colWidth = starSize * m_columnDefinitions.GetDefinition(iii).Value();
			m_columns.push_back(RowCol(colTop, colLeft, colHeight, colWidth));
			colLeft += colWidth;
			break;

		case ROW_COL_TYPE::ROW_COL_TYPE_FIXED:
			colWidth = m_columnDefinitions.GetDefinition(iii).Value();
			m_columns.push_back(RowCol(colTop, colLeft, colHeight, colWidth));
			colLeft += colWidth;
			break;
		}
	}
}

std::shared_ptr<Layout> Layout::CreateSubLayout(int rowIndex, int columnIndex)
{
	// Create a new layout confined to the row/column defined by the indices
	float top = m_rows[rowIndex].Top();
	float left = m_columns[columnIndex].Left();
	float height = m_rows[rowIndex].Height();
	float width = m_columns[columnIndex].Width();

	std::shared_ptr<Layout> layout = std::make_shared<Layout>(top, left, height, width);

	// Add Layout to the tuple
	m_subLayouts.push_back(
		std::tuple<std::shared_ptr<Layout>, int, int>(layout, rowIndex, columnIndex)
	);

	return layout;
}

void Layout::OnPaint(ID2D1HwndRenderTarget* renderTarget)
{
	// Paint controls first
	for (std::shared_ptr<Control> control : m_controls)
	{
		control->OnPaint(renderTarget);
	}


	PaintBorders(renderTarget);


	// Paint the sub layouts
	for (auto sub : m_subLayouts) 
	{
		sub._Myfirst._Val->OnPaint(renderTarget);
	}
}

void Layout::PaintBorders(ID2D1HwndRenderTarget* renderTarget)
{
	D2D1_POINT_2F p1, p2;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> blackBrush;
	const D2D1_COLOR_F color = D2D1::ColorF(0.0f, 0.0f, 0.0f);
	renderTarget->CreateSolidColorBrush(color, blackBrush.ReleaseAndGetAddressOf());

	for (RowCol row : m_rows)
	{
		p1.x = row.Left();
		p1.y = row.Top();

		p2.x = p1.x + row.Width();
		p2.y = p1.y;

		renderTarget->DrawLine(p1, p2, blackBrush.Get());

		p1.y += row.Height();
		p2.y = p1.y;

		renderTarget->DrawLine(p1, p2, blackBrush.Get());
	}

	for (RowCol col : m_columns)
	{
		p1.x = col.Left();
		p1.y = col.Top();

		p2.x = p1.x;
		p2.y = p1.y + col.Height();

		renderTarget->DrawLine(p1, p2, blackBrush.Get());

		p1.x += col.Width();
		p2.x = p1.x;

		renderTarget->DrawLine(p1, p2, blackBrush.Get());
	}
}

D2D1_RECT_F Layout::GetRect(int rowIndex, int columnIndex, int rowSpan = 1, int columnSpan = 1)
{
	D2D1_RECT_F rect;
	rect.left = m_columns[columnIndex].Left();
	rect.top = m_rows[rowIndex].Top();

	rect.right = m_columns[columnIndex + columnSpan - 1].Left() + m_columns[columnIndex + columnSpan - 1].Width();
	rect.bottom = m_rows[rowIndex + rowSpan - 1].Top() + m_rows[rowIndex + rowSpan - 1].Height();

	return rect;
}

std::shared_ptr<OnMessageResult> Layout::OnLButtonDown(const std::shared_ptr<MouseState>& mouseState, bool triggeredFromWindow)
{
	// We are going to say that in order for a control to react to the LButtonDown, it must capture the control
	// So there is no need to check all sublayouts and controls, just the captured control
	// 
	// Pass OnLButtonDown message to the control that has captured the mouse if it exists
	// Only run this section on the main window layout i.e. it must be triggered by the main window
	if (triggeredFromWindow && m_mouseCapturedControl != nullptr)
	{
		return m_mouseCapturedControl->OnLButtonDown(mouseState);
	}

	return std::make_shared<OnMessageResult>();
}
std::shared_ptr<OnMessageResult> Layout::OnLButtonUp(const std::shared_ptr<MouseState>& mouseState, bool triggeredFromWindow)
{
	// We are going to say that in order for a control to react to the LButtonUp, it must capture the control
	// So there is no need to check all sublayouts and controls, just the captured control
	// 
	// Pass LButtonUp message to the control that has captured the mouse if it exists
	// Only run this section on the main window layout i.e. it must be triggered by the main window
	if (triggeredFromWindow && m_mouseCapturedControl != nullptr)
	{
		return m_mouseCapturedControl->OnLButtonUp(mouseState);
	}

	return std::make_shared<OnMessageResult>();
}

std::shared_ptr<OnMessageResult> Layout::OnMouseMove(const std::shared_ptr<MouseState>& mouseState, bool triggeredFromWindow)
{
	// If this is the main window layout, initialize the already captured variable to false
	if(triggeredFromWindow)
		m_mouseAlreadyCapturedForThisMessage = false;

	std::shared_ptr<OnMessageResult> result = nullptr;

	bool redraw = false;

	// Pass mouse moved message to the control that has captured the mouse if it exists
	// Only run this section on the main window layout i.e. it must be triggered by the main window
	if (triggeredFromWindow && m_mouseCapturedControl != nullptr)
	{
		result = m_mouseCapturedControl->OnMouseMove(mouseState);

		// If the control wants to continue to capture the mouse, then set already captured flag
		// Else, set captured control to nullptr
		if (result->CaptureMouse())
			m_mouseAlreadyCapturedForThisMessage = true;
		else
			m_mouseCapturedControl = nullptr;

		if (result->Redraw())
			redraw = true;

		// If the message has been handled, return the result
		if (result->MessageHandled())
			return result;
	}

	// Message has not been fully handled, so pass message along to controls then sub-layouts
	//
	// First, attempt to pass the message along to one of the controls bound to the layout
	for (std::shared_ptr<Control> control : m_controls)
	{
		if (control->MouseIsOver(mouseState->X(), mouseState->Y()))
		{
			result = control->OnMouseMove(mouseState);

			redraw = redraw || result->Redraw();
			result->Redraw(redraw || result->Redraw());

			// Capture the mouse if necessary
			// Only update the m_mouseCapturedControl if it has not already been assigned for this message
			if (result->CaptureMouse() && !m_mouseAlreadyCapturedForThisMessage)
			{
				m_mouseCapturedControl = control;
				m_mouseAlreadyCapturedForThisMessage = true;
			}

			// return if handled
			if (result->MessageHandled())
				return result;
		}
	}

	// Next, attempt to pass the event to one of the sub-layouts
	std::shared_ptr<Layout> layout;
	for (std::tuple<std::shared_ptr<Layout>, int, int> subLayoutTuple : m_subLayouts)
	{
		layout = subLayoutTuple._Myfirst._Val;

		if (layout->MouseIsOver(mouseState->X(), mouseState->Y()))
		{
			result = layout->OnMouseMove(mouseState);
			if (result != nullptr)
				result->Redraw(redraw || result->Redraw());
			return result;
		}
	}

	if (result != nullptr)
		return result;
	
	return std::make_shared<OnMessageResult>();
}
std::shared_ptr<OnMessageResult> Layout::OnMouseLeave(bool triggeredFromWindow)
{
	// If this is the main window layout, initialize the already captured variable to false
	if (triggeredFromWindow)
		m_mouseAlreadyCapturedForThisMessage = false;

	std::shared_ptr<OnMessageResult> result = nullptr;

	bool redraw = false;

	// Pass mouse moved message to the control that has captured the mouse if it exists
	// Only run this section on the main window layout i.e. it must be triggered by the main window
	if (triggeredFromWindow && m_mouseCapturedControl != nullptr)
	{
		result = m_mouseCapturedControl->OnMouseLeave();

		// If the control wants to continue to capture the mouse, then set already captured flag
		// Else, set captured control to nullptr
		if (result->CaptureMouse())
			m_mouseAlreadyCapturedForThisMessage = true;
		else
			m_mouseCapturedControl = nullptr;

		if (result->Redraw())
			redraw = true;

		// If the message has been handled, return the result
		if (result->MessageHandled())
			return result;
	}

	// Message has not been fully handled, so pass message along to controls then sub-layouts
	//
	// Should only reach here if there is no captured control, so make sure it is nullptr
	m_mouseCapturedControl = nullptr;

	// Pass message along to all controls
	for (std::shared_ptr<Control> control : m_controls)
	{
		result = control->OnMouseLeave();

		// Controls are not allowed to newly capture the mouse on this message
		// (however they are allowed to continue to capture the mouse)
		if (result->CaptureMouse())
			result->CaptureMouse(false);

		// Keep track of whether or not we need to redraw
		redraw = redraw || result->Redraw();
		result->Redraw(redraw || result->Redraw());
	}

	// Next, attempt to pass the event to all of the sub-layouts
	std::shared_ptr<Layout> layout;
	for (std::tuple<std::shared_ptr<Layout>, int, int> subLayoutTuple : m_subLayouts)
	{
		layout = subLayoutTuple._Myfirst._Val;

		result = layout->OnMouseLeave();

		// Keep track of whether or not we need to redraw
		redraw = redraw || result->Redraw();
		result->Redraw(redraw || result->Redraw());
	}

	if (result != nullptr)
		return result;

	return std::make_shared<OnMessageResult>();
}
