#include "Layout.h"
#include "Control.h"

Layout::Layout(const std::shared_ptr<DeviceResources>& deviceResources, D2D1_RECT_F rect) :
	Layout(deviceResources, rect.top, rect.left, rect.bottom - rect.top, rect.right - rect.left)
{
}

Layout::Layout(const std::shared_ptr<DeviceResources>& deviceResources, 
	           float top, float left, float height, float width) :
	m_deviceResources(deviceResources),
	m_top(top),
	m_left(left),
	m_height(height),
	m_width(width),
	m_mouseCapturedControl(nullptr),
	m_mouseCapturedLayout(nullptr)
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

	std::shared_ptr<Layout> layout = std::make_shared<Layout>(m_deviceResources, top, left, height, width);

	// Add Layout to the tuple
	m_subLayouts.push_back(
		std::tuple<std::shared_ptr<Layout>, int, int>(layout, rowIndex, columnIndex)
	);

	return layout;
}
std::shared_ptr<Layout> Layout::GetSubLayout(int rowIndex, int columnIndex)
{
	// Get the size and location of the layout to retrieve
	float top = m_rows[rowIndex].Top();
	float left = m_columns[columnIndex].Left();
	float height = m_rows[rowIndex].Height();
	float width = m_columns[columnIndex].Width();

	// search each layout for the one with the correct size/location
	std::shared_ptr<Layout> layout = nullptr;
	for (std::tuple<std::shared_ptr<Layout>, int, int> subLayoutTuple : m_subLayouts)
	{
		layout = subLayoutTuple._Myfirst._Val;

		// Return the layout that matches the size/location
		if (layout->m_top == top && layout->m_left == left && layout->m_height == height && layout->m_width == width)
			return layout;
	}

	return nullptr;
}
void Layout::SetSubLayout(std::shared_ptr<Layout> layout, int rowIndex, int columnIndex)
{
	// Set the size / location for the layout
	float top = m_rows[rowIndex].Top();
	float left = m_columns[columnIndex].Left();
	float height = m_rows[rowIndex].Height();
	float width = m_columns[columnIndex].Width();

	layout->OnResize(top, left, height, width);

	// Add Layout to the tuple
	m_subLayouts.push_back(
		std::tuple<std::shared_ptr<Layout>, int, int>(layout, rowIndex, columnIndex)
	);
}


void Layout::Clear()
{
	// ReleaseLayout will release references from child controls to their parents layout so that 
	// there all resources are properly released. It will also clear all controls and layouts
	ReleaseLayout();

	RowColDefinitions defaultDefinition;
	defaultDefinition.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);

	SetRowDefinitions(defaultDefinition);
	SetColumnDefinitions(defaultDefinition);
}

/*
void Layout::PaintBorders()
{
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> blackBrush;
	const D2D1_COLOR_F color = D2D1::ColorF(0.0f, 0.0f, 0.0f);

	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();
	context->CreateSolidColorBrush(color, blackBrush.ReleaseAndGetAddressOf());

	context->SetTransform(m_deviceResources->OrientationTransform2D());

	D2D1_POINT_2F p1, p2;
	// Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> blackBrush;
	// const D2D1_COLOR_F color = D2D1::ColorF(0.0f, 0.0f, 0.0f);
	// renderTarget->CreateSolidColorBrush(color, blackBrush.ReleaseAndGetAddressOf());

	for (RowCol row : m_rows)
	{
		p1.x = row.Left();
		p1.y = row.Top();

		p2.x = p1.x + row.Width();
		p2.y = p1.y;

		context->DrawLine(p1, p2, blackBrush.Get());

		p1.y += row.Height();
		p2.y = p1.y;

		context->DrawLine(p1, p2, blackBrush.Get());
	}

	for (RowCol col : m_columns)
	{
		p1.x = col.Left();
		p1.y = col.Top();

		p2.x = p1.x;
		p2.y = p1.y + col.Height();

		context->DrawLine(p1, p2, blackBrush.Get());

		p1.x += col.Width();
		p2.x = p1.x;

		context->DrawLine(p1, p2, blackBrush.Get());
	}
}
*/

D2D1_RECT_F Layout::GetRect(int rowIndex, int columnIndex, int rowSpan, int columnSpan)
{
	D2D1_RECT_F rect;
	rect.left = m_columns[columnIndex].Left();
	rect.top = m_rows[rowIndex].Top();

	rect.right = m_columns[columnIndex + columnSpan - 1].Left() + m_columns[columnIndex + columnSpan - 1].Width();
	rect.bottom = m_rows[rowIndex + rowSpan - 1].Top() + m_rows[rowIndex + rowSpan - 1].Height();

	return rect;
}

OnMessageResult Layout::OnLButtonDown(const std::shared_ptr<MouseState>& mouseState)
{
	// We want the user to be able to "click out of" a control that has captured the mouse. For example,
	// the TextInput control should be able to capture all events until the user clicks on something else
	// That means the OnLButtonDown message needs to get passed to the appropriate control if the captured
	// control does not want to handle the message

	OnMessageResult result = OnMessageResult::NONE;

	// Pass OnLButtonDown message to the control that has captured the mouse if it exists
	if (m_mouseCapturedControl != nullptr)
		result = m_mouseCapturedControl->OnLButtonDown(mouseState);
	else if (m_mouseCapturedLayout != nullptr)
		result = m_mouseCapturedLayout->OnLButtonDown(mouseState);
	
	if (result != OnMessageResult::NONE)
		return result;

	// If the result is still NONE, then the captured control did not want to handle it and we need
	// to pass the message to all other controls

	// Clear both values as they should now only be set from the two loops below
	m_mouseCapturedControl = nullptr;
	m_mouseCapturedLayout = nullptr;

	// First, attempt to pass the message along to one of the controls bound to the layout
	for (std::shared_ptr<Control> control : m_controls)
	{
		// Pass message only if mouse is over the control
		if (control->MouseIsOver(mouseState->X(), mouseState->Y()))
		{
			result = control->OnLButtonDown(mouseState);

			// Capture the mouse if necessary
			if (result == OnMessageResult::CAPTURE_MOUSE || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED)
				m_mouseCapturedControl = control;

			// return if handled
			if (result == OnMessageResult::MESSAGE_HANDLED || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED)
				return result;
		}
	}

	// Next, attempt to pass the event to one of the sub-layouts
	std::shared_ptr<Layout> layout;
	for (std::tuple<std::shared_ptr<Layout>, int, int> subLayoutTuple : m_subLayouts)
	{
		layout = subLayoutTuple._Myfirst._Val;

		// Two child layouts cannot overlap, so if mouse is over one of them, can pass the message
		// and immediately return the result
		if (layout->MouseIsOver(mouseState->X(), mouseState->Y()))
		{
			result = layout->OnLButtonDown(mouseState);

			// Capture the mouse if necessary
			if (result == OnMessageResult::CAPTURE_MOUSE || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED)
				m_mouseCapturedLayout = layout;

			return result;
		}
	}

	return result;
}
OnMessageResult Layout::OnLButtonUp(const std::shared_ptr<MouseState>& mouseState)
{
	// Pass LButtonUp message to the control that has captured the mouse if it exists
	if (m_mouseCapturedControl != nullptr)
	{
		return m_mouseCapturedControl->OnLButtonUp(mouseState);
	}
	else if (m_mouseCapturedLayout != nullptr)
	{
		return m_mouseCapturedLayout->OnLButtonUp(mouseState);
	}

	return OnMessageResult::NONE;
}
OnMessageResult Layout::OnLButtonDoubleClick(const std::shared_ptr<MouseState>& mouseState)
{
	// Pass OnLButtonDoubleClick message to the control that has captured the mouse if it exists
	if (m_mouseCapturedControl != nullptr)
	{
		return m_mouseCapturedControl->OnLButtonDoubleClick(mouseState);
	}
	else if (m_mouseCapturedLayout != nullptr)
	{
		return m_mouseCapturedLayout->OnLButtonDoubleClick(mouseState);
	}

	return OnMessageResult::NONE;
}

OnMessageResult Layout::OnMouseMove(const std::shared_ptr<MouseState>& mouseState)
{
	OnMessageResult result = OnMessageResult::NONE;

	// If there is already a control within this layout that has been captured, immediately pass the message
	if (m_mouseCapturedControl != nullptr)
		result = m_mouseCapturedControl->OnMouseMove(mouseState);
	else if (m_mouseCapturedLayout != nullptr)
		result = m_mouseCapturedLayout->OnMouseMove(mouseState);

	// If the control/layout no longer wants to be captured, release it
	if (!(result == OnMessageResult::CAPTURE_MOUSE || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED))
	{
		m_mouseCapturedControl = nullptr;
		m_mouseCapturedLayout = nullptr;
	}

	// If the message has been handled, return the result
	if (result == OnMessageResult::MESSAGE_HANDLED || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED)
		return result;


	// Message has not been fully handled, so pass message along to controls then sub-layouts
	//
	// First, attempt to pass the message along to one of the controls bound to the layout
	for (std::shared_ptr<Control> control : m_controls)
	{
		// Pass message only if mouse is over the control
		if (control->MouseIsOver(mouseState->X(), mouseState->Y()))
		{
			result = control->OnMouseMove(mouseState);

			// Capture the mouse if necessary
			if (result == OnMessageResult::CAPTURE_MOUSE || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED)
			{
				m_mouseCapturedControl = control;
				m_mouseCapturedLayout = nullptr;
			}

			// return if handled
			if (result == OnMessageResult::MESSAGE_HANDLED || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED)
				return result;
		}
	}

	// Next, attempt to pass the event to one of the sub-layouts
	std::shared_ptr<Layout> layout;
	for (std::tuple<std::shared_ptr<Layout>, int, int> subLayoutTuple : m_subLayouts)
	{
		layout = subLayoutTuple._Myfirst._Val;

		// Two child layouts cannot overlap, so if mouse is over one of them, can pass the message
		// and immediately return the result
		if (layout->MouseIsOver(mouseState->X(), mouseState->Y()))
		{
			result = layout->OnMouseMove(mouseState);

			// Capture the mouse if necessary
			if (result == OnMessageResult::CAPTURE_MOUSE || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED)
			{
				m_mouseCapturedControl = nullptr;
				m_mouseCapturedLayout = layout;
			}

			return result;
		}
	}

	return result;
}
OnMessageResult Layout::OnMouseLeave()
{
	OnMessageResult result = OnMessageResult::NONE;

	// If there is already a control within this layout that has been captured, immediately pass the message
	if (m_mouseCapturedControl != nullptr)
		result = m_mouseCapturedControl->OnMouseLeave();
	else if (m_mouseCapturedLayout != nullptr)
		result = m_mouseCapturedLayout->OnMouseLeave();

	// If the control/layout no longer wants to be captured, release it
	if (!(result == OnMessageResult::CAPTURE_MOUSE || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED))
	{
		m_mouseCapturedControl = nullptr;
		m_mouseCapturedLayout = nullptr;
	}

	// If the message has been handled, return the result
	if (result == OnMessageResult::MESSAGE_HANDLED || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED)
		return result;

	// Message has not been fully handled, so pass message along to controls then sub-layouts
	//
	// Pass message along to all controls
	for (std::shared_ptr<Control> control : m_controls)
	{
		result = control->OnMouseLeave();

		// Capture the mouse if necessary
		// NOTE: This should not ever be the case. A control should not necessarily be able to capture the mouse
		//       after it has left the client area. 
		if (result == OnMessageResult::CAPTURE_MOUSE || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED)
		{
			m_mouseCapturedControl = control;
			m_mouseCapturedLayout = nullptr;
		}

		// return if handled
		if (result == OnMessageResult::MESSAGE_HANDLED || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED)
			return result;
	}

	// Next, pass the event to all of the sub-layouts
	std::shared_ptr<Layout> layout;
	for (std::tuple<std::shared_ptr<Layout>, int, int> subLayoutTuple : m_subLayouts)
	{
		layout = subLayoutTuple._Myfirst._Val;

		result = layout->OnMouseLeave();

		// Capture the mouse if necessary
		if (result == OnMessageResult::CAPTURE_MOUSE || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED)
		{
			m_mouseCapturedControl = nullptr;
			m_mouseCapturedLayout = layout;
		}
	}

	return result;
}
OnMessageResult Layout::OnMouseWheel(int wheelDelta)
{
	// Pass OnMouseWheel message to the control that has captured the mouse if it exists
	if (m_mouseCapturedControl != nullptr)
	{
		return m_mouseCapturedControl->OnMouseWheel(wheelDelta);
	}
	else if (m_mouseCapturedLayout != nullptr)
	{
		return m_mouseCapturedLayout->OnMouseWheel(wheelDelta);
	}

	return OnMessageResult::NONE;
}

OnMessageResult Layout::OnKeyDown(unsigned char keycode)
{
	OnMessageResult result = OnMessageResult::NONE;

	// If there is already a control within this layout that has been captured, immediately pass the message
	if (m_mouseCapturedControl != nullptr)
		result = m_mouseCapturedControl->OnKeyDown(keycode);
	else if (m_mouseCapturedLayout != nullptr)
		result = m_mouseCapturedLayout->OnKeyDown(keycode);

	// If the control/layout no longer wants to be captured, release it
	if (!(result == OnMessageResult::CAPTURE_MOUSE || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED))
	{
		m_mouseCapturedControl = nullptr;
		m_mouseCapturedLayout = nullptr;
	}

	// Don't iterate over each control - only pass message to captured control / layout
	return result;
}
OnMessageResult Layout::OnKeyUp(unsigned char keycode)
{
	OnMessageResult result = OnMessageResult::NONE;

	// If there is already a control within this layout that has been captured, immediately pass the message
	if (m_mouseCapturedControl != nullptr)
		result = m_mouseCapturedControl->OnKeyUp(keycode);
	else if (m_mouseCapturedLayout != nullptr)
		result = m_mouseCapturedLayout->OnKeyUp(keycode);

	// If the control/layout no longer wants to be captured, release it
	if (!(result == OnMessageResult::CAPTURE_MOUSE || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED))
	{
		m_mouseCapturedControl = nullptr;
		m_mouseCapturedLayout = nullptr;
	}

	// Don't iterate over each control - only pass message to captured control / layout
	return result;
}
OnMessageResult Layout::OnChar(char key)
{
	OnMessageResult result = OnMessageResult::NONE;

	// If there is already a control within this layout that has been captured, immediately pass the message
	if (m_mouseCapturedControl != nullptr)
		result = m_mouseCapturedControl->OnChar(key);
	else if (m_mouseCapturedLayout != nullptr)
		result = m_mouseCapturedLayout->OnChar(key);

	// If the control/layout no longer wants to be captured, release it
	if (!(result == OnMessageResult::CAPTURE_MOUSE || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED))
	{
		m_mouseCapturedControl = nullptr;
		m_mouseCapturedLayout = nullptr;
	}

	// Don't iterate over each control - only pass message to captured control / layout
	return result;
}


void Layout::ReleaseLayout()
{
	// Call ReleaseLayout for each control
	for (auto control : m_controls)
		control->ReleaseLayout();

	// Call ReleaseLayout for each sublayout
	for (auto sub : m_subLayouts)
		sub._Myfirst._Val->ReleaseLayout();

	m_mouseCapturedControl = nullptr;
	m_mouseCapturedLayout = nullptr;

	// Clear all controls
	m_controls.clear();

	// Clear all sublayouts
	m_subLayouts.clear();
}


void Layout::Update(StepTimer const& stepTimer)
{
	// Pass the Update call along to each child control
	// ONLY 3D rendering controls should react to this - other controls
	// should NOT override Control::Update
	for (std::shared_ptr<Control> control : m_controls)
	{
		control->Update(stepTimer);
	}

	// Pass the Update call along to each sub layout as a 3D control can live in a sub-layout
	for (std::tuple<std::shared_ptr<Layout>, int, int> subLayoutTuple : m_subLayouts)
	{
		subLayoutTuple._Myfirst._Val->Update(stepTimer);
	}
}

bool Layout::Render3DControls()
{
	// Pass the Render call along to each child control
	// ONLY 3D rendering controls should react to this - other controls
	// should NOT override Control::Render3D
	bool needsPresent = false;
	for (std::shared_ptr<Control> control : m_controls)
	{
		if (control->Render3D())
			needsPresent = true;
	}

	// Pass the Render call along to each sub layout as a 3D control can live in a sub-layout
	for (std::tuple<std::shared_ptr<Layout>, int, int> subLayoutTuple : m_subLayouts)
	{
		if (subLayoutTuple._Myfirst._Val->Render3DControls())
			needsPresent = true;
	}

	return needsPresent;
}

bool Layout::Render2DControls()
{
	// Pass the Render call along to each child control
	// ONLY 2D rendering controls should react to this - other controls
	// should NOT override Control::Render2D
	bool needsPresent = false;
	for (std::shared_ptr<Control> control : m_controls)
	{
		if (control->Render2D())
			needsPresent = true;
	}


	//PaintBorders();


	// Pass the Render call along to each sub layout as a 2D control can live in a sub-layout
	for (std::tuple<std::shared_ptr<Layout>, int, int> subLayoutTuple : m_subLayouts)
	{
		if (subLayoutTuple._Myfirst._Val->Render2DControls())
			needsPresent = true;
	}

	return needsPresent;
}

bool Layout::Render2DCapturedControl()
{
	if (m_mouseCapturedControl != nullptr)
		return m_mouseCapturedControl->Render2D();

	if (m_mouseCapturedLayout != nullptr)
		return m_mouseCapturedLayout->Render2DCapturedControl();

	return false;
}

std::shared_ptr<Control> Layout::GetChildControl(unsigned int index)
{
	return m_controls.size() > index ? m_controls[index] : nullptr;
}

std::shared_ptr<Control> Layout::GetChildControl(std::wstring controlName)
{
	// Iterate over all controls, if a name matches, return that control
	for (std::shared_ptr<Control> control : m_controls)
	{
		if (control->Name() == controlName)
			return control;
	}

	// Iterate over all sublayouts, if a sublayout returns non-nullptr, return that value
	for (std::tuple<std::shared_ptr<Layout>, int, int> subLayoutTuple : m_subLayouts)
	{
		std::shared_ptr<Control> control = subLayoutTuple._Myfirst._Val->GetChildControl(controlName);
		if (control != nullptr)
			return control;
	}

	return nullptr;
}