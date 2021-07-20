#include "DropDown.h"

DropDown::DropDown(const std::shared_ptr<DeviceResources>& deviceResources,
				   const std::shared_ptr<Layout>& parentLayout, 
				   int row, int column, int rowSpan, int columnSpan) :
	Control(deviceResources, parentLayout, row, column, rowSpan, columnSpan),
	m_mainLayout(nullptr),
	m_dropDownLayout(nullptr),
	m_dropDownIsOpen(false),
	m_dropDownHeight(100.0f),
	m_dropDownWidth(100.0f)
{
	// Create a sub layout not as a child of the parent that will house the main button
	// Because the default will be to have no margins, and row/column index = 0 and row/columnSpan = 1
	// we can automatically assign the layout to be the same as the (0,0) rectangle of the parent layout
	m_mainLayout = std::make_shared<Layout>(m_deviceResources, GetParentRect());

	// Create the drop down layout with the top aligned with bottom of the main layout and
	// left aligned with the main layout
	D2D1_RECT_F dropDownRect;
	dropDownRect.top = m_mainLayout->Bottom();
	dropDownRect.left = m_mainLayout->Left();
	dropDownRect.right = dropDownRect.left + m_dropDownWidth;
	dropDownRect.bottom = dropDownRect.top + m_dropDownHeight;

	m_dropDownLayout = std::make_shared<Layout>(m_deviceResources, dropDownRect);
}

void DropDown::SetDropDownSize(float height, float width)
{
	m_dropDownHeight = height;
	m_dropDownWidth = width;

	Resize();
}

void DropDown::ClearContents()
{
	m_mainLayout->ClearContents();
	m_dropDownLayout->ClearContents();
}

void DropDown::OnPaint()
{
	m_mainLayout->OnPaint();

	if (m_dropDownIsOpen)
		m_dropDownLayout->OnPaint();
}

void DropDown::Resize()
{
	// Resize the main layout when anything about the parent rect or margins change
	D2D1_RECT_F rect = GetParentRect();
	rect.left += m_marginLeft;
	rect.right -= m_marginRight;
	rect.top += m_marginTop;
	rect.bottom -= m_marginBottom;

	m_mainLayout->OnResize(rect);

	rect.top = rect.bottom;
	rect.right = rect.left + m_dropDownWidth;
	rect.bottom = rect.top + m_dropDownHeight;

	m_dropDownLayout->OnResize(rect);
}

bool DropDown::MouseIsOver(int x, int y)
{
	return MouseIsOverMainLayout(x, y) || (m_dropDownIsOpen && MouseIsOverDropDownLayout(x, y));
}
bool DropDown::MouseIsOverMainLayout(int x, int y)
{
	return	x >= m_mainLayout->Left() &&
		x <= m_mainLayout->Right() &&
		y >= m_mainLayout->Top() &&
		y <= m_mainLayout->Bottom();
}
bool DropDown::MouseIsOverDropDownLayout(int x, int y)
{
	return	x >= m_dropDownLayout->Left() &&
		x <= m_dropDownLayout->Right() &&
		y >= m_dropDownLayout->Top() &&
		y <= m_dropDownLayout->Bottom();
}


std::shared_ptr<OnMessageResult> DropDown::OnLButtonDown(std::shared_ptr<MouseState> mouseState)
{
	return nullptr;

	/*
	// Because the mouse can move between the two layouts, we need to pass the message along to both layouts
	std::shared_ptr<OnMessageResult> result1 = m_mainLayout->OnLButtonDown(mouseState, true);

	if (result1->MessageHandled())
	{
		// Capture mouse and redraw
		result1->CaptureMouse(true);
		result1->CaptureOverride(true);
		result1->Redraw(true);

		return result1;
	}

	// If the drop down is not visible, just return the earlier result
	if (!m_dropDownIsOpen)
		return result1;

	std::shared_ptr<OnMessageResult> result2 = m_dropDownLayout->OnLButtonDown(mouseState);

	// If any of the values are true, we want to keep that value
	result1->CaptureMouse(result1->CaptureMouse() || result2->CaptureMouse());
	result1->Redraw(result1->Redraw() || result2->Redraw());
	result1->MessageHandled(result1->MessageHandled() || result2->MessageHandled());

	// If either layout wanted to capture the mouse, then we need to capture it for the drop down and override
	if (result1->CaptureMouse())
		result1->CaptureOverride(true);

	return result1;
	*/
}

std::shared_ptr<OnMessageResult> DropDown::OnLButtonUp(std::shared_ptr<MouseState> mouseState)
{
	return nullptr;

	/*
	// If the main layout handles the message, just change the visibility of the drop down
	std::shared_ptr<OnMessageResult> result1 = m_mainLayout->OnLButtonUp(mouseState, true);

	if (result1->MessageHandled())
	{
		m_dropDownIsOpen = !m_dropDownIsOpen;

		// Capture mouse and redraw
		result1->CaptureMouse(true);
		result1->CaptureOverride(true);
		result1->Redraw(true);

		return result1;
	}

	// If the drop down is not visible, just return the earlier result
	if (!m_dropDownIsOpen)
		return result1;

	// Main layout did not handle the message, so pass it to the drop down menu
	std::shared_ptr<OnMessageResult> result2 = m_dropDownLayout->OnLButtonUp(mouseState);

	// If any of the values are true, we want to keep that value
	result1->CaptureMouse(result1->CaptureMouse() || result2->CaptureMouse());
	result1->Redraw(result1->Redraw() || result2->Redraw());
	result1->MessageHandled(result1->MessageHandled() || result2->MessageHandled());

	// If either layout wanted to capture the mouse, then we need to capture it for the drop down and override
	if (result1->CaptureMouse())
		result1->CaptureOverride(true);

	return result1;
	*/
}

std::shared_ptr<OnMessageResult> DropDown::OnMouseMove(std::shared_ptr<MouseState> mouseState)
{
	return nullptr;

	/*
	// Because the mouse can move between the two layouts, we need to pass the message along to both layouts
	// Set forceMessagePass to true because we need the sub controls to react to it
	std::shared_ptr<OnMessageResult> result1 = m_mainLayout->OnMouseMove(mouseState, false, true);

	if (result1->MessageHandled())
	{
		// Capture mouse and redraw
		result1->CaptureMouse(true);
		result1->CaptureOverride(true);
		result1->Redraw(true);

		return result1;
	}


	// If the drop down is not visible, just return the earlier result
	if (!m_dropDownIsOpen)
		return result1;

	// Set forceMessagePass to true because we need the sub controls to react to it
	std::shared_ptr<OnMessageResult> result2 = m_dropDownLayout->OnMouseMove(mouseState, false, true);

	// If any of the values are true, we want to keep that value
	result1->CaptureMouse(result1->CaptureMouse() || result2->CaptureMouse());
	result1->Redraw(result1->Redraw() || result2->Redraw());
	result1->MessageHandled(result1->MessageHandled() || result2->MessageHandled());

	// If either layout wanted to capture the mouse, then we need to capture it for the drop down and override
	if (result1->CaptureMouse())
		result1->CaptureOverride(true);

	return result1;
	*/

	/*
	std::shared_ptr<OnMessageResult> result = nullptr;

	// If the mouse is over the main control, get the result from there
	if (MouseIsOverMainLayout(mouseState->X(), mouseState->Y()))
	{
		result = m_mainLayout->OnMouseMove(mouseState);
	}
	else if (MouseIsOverDropDownLayout(mouseState->X(), mouseState->Y()))
	{
		result = m_dropDownLayout->OnMouseMove(mouseState);
	}
	else
	{
		result = std::make_shared<OnMessageResult>();
		UpdateButtonMouseState(MouseOverDown::NONE, result);
		return result;
	}

	// This button will handle the message, so just set that value now
	result->MessageHandled(true);

	// Capture the mouse for as long as the mouse is over the control
	result->CaptureMouse(true);

	// The drop down control needs to override the captured control because a sub-button would have already been captured
	result->CaptureOverride(true);

	// Set the state to MOUSE_OVER
	UpdateButtonMouseState(MouseOverDown::MOUSE_OVER, result);

	return result;
	*/

	/*
	// First check if a sublayout wants to handle the message
	std::shared_ptr<OnMessageResult> result = m_buttonLayout->OnMouseMove(mouseState);

	// If the message was handled by a sublayout/control, make sure buttonMouseState is NONE and return
	// The same behavior is required when the mouse is not over the button
	if (result->MessageHandled() || !MouseIsOver(mouseState->X(), mouseState->Y()))
	{
		UpdateButtonMouseState(MouseOverDown::NONE, result);

		return result;
	}

	// Mouse is over the button AND the message has not yet been handled
	// 
	// This button will handle the message, so just set that value now
	result->MessageHandled(true);

	// Capture the mouse for as long as the mouse is over the control
	result->CaptureMouse(true);

	// First check if L Button is down
	if (mouseState->LButtonDown())
	{
		// Set the state to MOUSE_DOWN
		UpdateButtonMouseState(MouseOverDown::MOUSE_DOWN, result);
	}
	else
	{
		// Set the state to MOUSE_OVER
		UpdateButtonMouseState(MouseOverDown::MOUSE_OVER, result);
	}

	return result;
	*/
}

/*
void DropDown::UpdateButtonMouseState(MouseOverDown newState, std::shared_ptr<OnMessageResult> result)
{
	// If the buttonMouseState needs updating, update and set the redraw flag
	if (m_mouseOverDownState != newState)
	{
		m_mouseOverDownState = newState;
		result->Redraw(true);
	}
}
*/