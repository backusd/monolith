#include "Button.h"

Button::Button(const std::shared_ptr<DeviceResources>& deviceResources, 
			   const std::shared_ptr<Layout>& parentLayout) : 
	Button(deviceResources, parentLayout, 0, 0, 0, 0)
{
}

Button::Button(const std::shared_ptr<DeviceResources>& deviceResources, 
			   const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan) :
	Control(deviceResources, parentLayout, row, column, rowSpan, columnSpan),
	m_buttonLayout(nullptr),
	m_colorTheme(nullptr),
	m_mouseOverDownState(MouseOverDown::NONE)
{
	SetColorTheme(THEME_DEFAULT_BUTTON_COLOR);

	// Create its own sub layout not as a child of the parent
	// Because the default will be to have no margins, and row/column index = 0 and row/columnSpan = 1
	// we can automatically assign the layout to be the same as the (0,0) rectangle of the parent layout
	m_buttonLayout = std::make_shared<Layout>( m_deviceResources, GetParentRect() );
}


void Button::OnPaint()
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
	m_buttonLayout->OnPaint();
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

std::shared_ptr<OnMessageResult> Button::OnLButtonDown(std::shared_ptr<MouseState> mouseState)
{
	// First check if a sublayout wants to handle the message
	std::shared_ptr<OnMessageResult> result = m_buttonLayout->OnLButtonDown(mouseState);

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

	// Set the state to MOUSE_DOWN
	UpdateButtonMouseState(MouseOverDown::MOUSE_DOWN, result);

	return result;
}

std::shared_ptr<OnMessageResult> Button::OnLButtonUp(std::shared_ptr<MouseState> mouseState)
{
	// First check if a sublayout wants to handle the message
	std::shared_ptr<OnMessageResult> result = m_buttonLayout->OnLButtonUp(mouseState);

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

	// Set the state to MOUSE_DOWN
	UpdateButtonMouseState(MouseOverDown::MOUSE_OVER, result);

	return result;
}

std::shared_ptr<OnMessageResult> Button::OnMouseMove(std::shared_ptr<MouseState> mouseState)
{
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
}

void Button::UpdateButtonMouseState(MouseOverDown newState, std::shared_ptr<OnMessageResult> result)
{
	// If the buttonMouseState needs updating, update and set the redraw flag
	if (m_mouseOverDownState != newState)
	{
		m_mouseOverDownState = newState;
		result->Redraw(true);
	}
}

std::shared_ptr<OnMessageResult> Button::OnMouseLeave()
{
	// Pass message to layout/subcontrols first
	std::shared_ptr<OnMessageResult> result = m_buttonLayout->OnMouseLeave();

	// Update the mouse state to NONE 
	UpdateButtonMouseState(MouseOverDown::NONE, result);

	return result;
}
