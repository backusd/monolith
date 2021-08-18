#include "Button.h"

Button::Button(const std::shared_ptr<DeviceResources>& deviceResources, 
			   const std::shared_ptr<Layout>& parentLayout) : 
	Button(deviceResources, parentLayout, 0, 0, 1, 1)
{
}

Button::Button(const std::shared_ptr<DeviceResources>& deviceResources, 
			   const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan) :
	Control(deviceResources, parentLayout, row, column, rowSpan, columnSpan),
	m_buttonLayout(nullptr),
	m_colorTheme(nullptr),
	m_mouseOverDownState(MouseOverDown::NONE),
	ClickMethod(DefaultClickMethod)
{
	SetColorTheme(THEME_DEFAULT_BUTTON_COLOR);

	// Create its own sub layout not as a child of the parent
	// Because the default will be to have no margins, and row/column index = 0 and row/columnSpan = 1
	// we can automatically assign the layout to be the same as the (0,0) rectangle of the parent layout
	m_buttonLayout = std::make_shared<Layout>( m_deviceResources, GetParentRect() );
}

void Button::ClearContents()
{
	m_buttonLayout->ClearContents();
	m_deviceResources = nullptr;
	m_colorTheme = nullptr;
}


bool Button::Render2D()
{
	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();

	context->SetTransform(m_deviceResources->OrientationTransform2D());

	D2D1_RECT_F rect = m_buttonLayout->GetRect(
		0, 
		0, 
		m_buttonLayout->RowCount(), 
		m_buttonLayout->ColumnCount()
	);

	context->FillRectangle(rect, m_colorTheme->GetBrush(m_mouseOverDownState));

	// Now paint the layout and child controls
	m_buttonLayout->Render2DControls();

	return true;
}

void Button::Resize()
{
	// Resize the button layout when anything about the parent rect or margins change
	D2D1_RECT_F rect = GetParentRect();
	rect.left += m_marginLeft;
	rect.right -= m_marginRight;
	rect.top += m_marginTop;
	rect.bottom -= m_marginBottom;

	// NOTE: Resizing the button layout will automatically resize all child controls because they belong
	// to the layout, NOT the button itself
	m_buttonLayout->OnResize(rect);
}

bool Button::MouseIsOver(int x, int y)
{
	return	x >= m_buttonLayout->Left() &&
			x <= m_buttonLayout->Right() &&
			y >= m_buttonLayout->Top() &&
			y <= m_buttonLayout->Bottom();
}

OnMessageResult Button::OnLButtonDown(std::shared_ptr<MouseState> mouseState)
{
	// Regardless of whether or not there is a captured control, the behavior remains the same: Pass the message
	// to the button layout and if the message was handled, don't do anything. Otherwise, react to the message.
	//
	// Message has not been handled so pass it to the button layout
	OnMessageResult result = m_buttonLayout->OnLButtonDown(mouseState);

	// If the message was handled by a sublayout/control, make sure buttonMouseState is NONE and return
	// The same behavior is required when the mouse is not over the button
	bool messagedHandled = result == OnMessageResult::MESSAGE_HANDLED || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
	if (messagedHandled || !MouseIsOver(mouseState->X(), mouseState->Y()))
	{
		// Set MouseOverDown state to None because mouse is actually over child control which gets priority
		m_mouseOverDownState = MouseOverDown::NONE;
		return result;
	}

	// Messaged not handled, so let this button handle it
	m_mouseOverDownState = MouseOverDown::MOUSE_DOWN;

	// Capture the mouse and signal that the message was handled
	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}

OnMessageResult Button::OnLButtonUp(std::shared_ptr<MouseState> mouseState)
{
	// Regardless of whether or not there is a captured control, the behavior remains the same: Pass the message
	// to the button layout and if the message was handled, don't do anything. Otherwise, react to the message.
	//
	// Message has not been handled so pass it to the button layout
	OnMessageResult result = m_buttonLayout->OnLButtonUp(mouseState);

	// If the message was handled by a sublayout/control, make sure buttonMouseState is NONE and return
	// The same behavior is required when the mouse is not over the button
	bool messagedHandled = result == OnMessageResult::MESSAGE_HANDLED || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
	if (messagedHandled || !MouseIsOver(mouseState->X(), mouseState->Y()))
	{
		// Set MouseOverDown state to None because mouse is actually over child control which gets priority
		m_mouseOverDownState = MouseOverDown::NONE;
		return result;
	}

	// Messaged not handled, so let this button handle it
	m_mouseOverDownState = MouseOverDown::MOUSE_OVER;

	// Call the click method
	ClickMethod();

	// Capture the mouse and signal that the message was handled
	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}

OnMessageResult Button::OnMouseMove(std::shared_ptr<MouseState> mouseState)
{
	// Regardless of whether or not there is a captured control, the behavior remains the same: Pass the message
	// to the button layout and if the message was handled, don't do anything. Otherwise, react to the message.
	//
	// Message has not been handled so pass it to the button layout
	OnMessageResult result = m_buttonLayout->OnMouseMove(mouseState);

	// If the message was handled by a sublayout/control, make sure buttonMouseState is NONE and return
	// The same behavior is required when the mouse is not over the button
	bool messagedHandled = result == OnMessageResult::MESSAGE_HANDLED || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
	if (messagedHandled || !MouseIsOver(mouseState->X(), mouseState->Y()))
	{
		// Set MouseOverDown state to None because mouse is actually over child control which gets priority
		m_mouseOverDownState = MouseOverDown::NONE;
		return result;
	}

	// Messaged not handled, so let this button handle it
	m_mouseOverDownState = mouseState->LButtonDown() ? MouseOverDown::MOUSE_DOWN : MouseOverDown::MOUSE_OVER;

	// Capture the mouse and signal that the message was handled
	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}

OnMessageResult Button::OnMouseLeave()
{
	// Pass message to layout/subcontrols first
	OnMessageResult result = m_buttonLayout->OnMouseLeave();

	// Update the mouse state to NONE 
	m_mouseOverDownState = MouseOverDown::NONE;
	return result;
}
