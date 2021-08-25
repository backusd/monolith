#include "TextInput.h"

TextInput::TextInput(const std::shared_ptr<DeviceResources>& deviceResources,
	const std::shared_ptr<Layout>& parentLayout) :
	TextInput(deviceResources, parentLayout, 0, 0, 1, 1)
{
}

TextInput::TextInput(const std::shared_ptr<DeviceResources>& deviceResources,
	const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan) :
	Control(deviceResources, parentLayout, row, column, rowSpan, columnSpan),
	m_layout(nullptr),
	m_backgroundTheme(nullptr),
	m_borderTheme(nullptr),
	m_mouseOverDownState(MouseOverDown::NONE),
	m_placeholderText(""),
	m_activelyEnteringText(false)
{
	SetBackgroundTheme(THEME_DEFAULT_TEXT_INPUT_BACKGROUND_COLOR);
	SetBorderTheme(THEME_DEFAULT_TEXT_INPUT_BORDER_COLOR);

	// Create its own sub layout not as a child of the parent
	// Because the default will be to have no margins, and row/column index = 0 and row/columnSpan = 1
	// we can automatically assign the layout to be the same as the (0,0) rectangle of the parent layout
	m_layout = std::make_shared<Layout>(m_deviceResources, GetParentRect());

	m_textTheme = THEME_DEFAULT_TEXT_INPUT_TEXT;
	m_placeholderTextTheme = THEME_DEFAULT_TEXT_INPUT_PLACEHOLDER_TEXT;

	// Create the text control that will live inside the layout
	m_text = m_layout->CreateControl<Text>(0, 0);
	m_text->SetTextTheme(m_placeholderTextTheme);
	m_text->Margin(5.0f, 0.0f);
}

void TextInput::ReleaseLayout()
{
	m_layout->ReleaseLayout();
	m_parentLayout = nullptr;
}

bool TextInput::Render2D()
{
	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();

	context->SetTransform(m_deviceResources->OrientationTransform2D());

	D2D1_RECT_F rect = m_layout->GetRect(
		0,
		0,
		m_layout->RowCount(),
		m_layout->ColumnCount()
	);

	// Fill the rectangle then draw the border
	context->FillRectangle(rect, m_backgroundTheme->GetBrush(m_mouseOverDownState));
	context->DrawRectangle(rect, m_borderTheme->GetBrush(m_mouseOverDownState));	

	if (m_activelyEnteringText)
	{
		// Get the width of the text
		float textWidth = m_text->Width();

		// Draw the vertical line
		float x = rect.left + m_text->MarginLeft() + textWidth + 1.0f;
		D2D1_POINT_2F pt1 = D2D1::Point2F(x, rect.top + 3.0f);
		D2D1_POINT_2F pt2 = D2D1::Point2F(x, rect.bottom - 3.0f);

		context->DrawLine(pt1, pt2, m_borderTheme->GetBrush(m_mouseOverDownState), 1.0f);
	}

	// Now draw the layout/text control
	m_layout->Render2DControls();

	return true;
}

void TextInput::Resize()
{
	// Resize the button layout when anything about the parent rect or margins change
	D2D1_RECT_F rect = GetParentRect();
	rect.left += m_marginLeft;
	rect.right -= m_marginRight;
	rect.top += m_marginTop;
	rect.bottom -= m_marginBottom;

	// NOTE: Resizing the button layout will automatically resize all child controls because they belong
	// to the layout, NOT the button itself
	m_layout->OnResize(rect);
}

bool TextInput::MouseIsOver(int x, int y)
{
	return	x >= m_layout->Left() &&
		x <= m_layout->Right() &&
		y >= m_layout->Top() &&
		y <= m_layout->Bottom();
}

OnMessageResult TextInput::OnLButtonDown(std::shared_ptr<MouseState> mouseState)
{
	// If mouse is over the control, we want to capture it and set activelyEnteringText to true
	if (MouseIsOver(mouseState->X(), mouseState->Y()))
	{
		m_activelyEnteringText = true;
		m_mouseOverDownState = MouseOverDown::MOUSE_DOWN;
		return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
	}

	// Otherwise, set activelyEnteringText to false and return as unhandled
	m_activelyEnteringText = false;
	m_mouseOverDownState = MouseOverDown::NONE;
	return OnMessageResult::NONE;
}

OnMessageResult TextInput::OnLButtonUp(std::shared_ptr<MouseState> mouseState)
{
	// Don't worry about adjusting activelyEnteringText value - that should only change on L button down
	m_mouseOverDownState = MouseIsOver(mouseState->X(), mouseState->Y()) ? MouseOverDown::MOUSE_OVER : MouseOverDown::NONE;

	// Always return captured & handled because only on L button down should that change
	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}

OnMessageResult TextInput::OnMouseMove(std::shared_ptr<MouseState> mouseState)
{
	// Don't worry about adjusting activelyEnteringText value - that should only change on L button down
	m_mouseOverDownState = MouseIsOver(mouseState->X(), mouseState->Y()) ? MouseOverDown::MOUSE_OVER : MouseOverDown::NONE;

	// Always return captured & handled because only on L button down should that change
	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}

OnMessageResult TextInput::OnMouseLeave()
{
	// Update the mouse state to NONE 
	m_mouseOverDownState = MouseOverDown::NONE;
	m_activelyEnteringText = false;
	return OnMessageResult::NONE;
}

OnMessageResult TextInput::OnKeyDown(unsigned char keycode)
{
	// Will need to handle left/right arrow keys

	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}
OnMessageResult TextInput::OnKeyUp(unsigned char keycode)
{
	// Will need to handle left/right arrow keys

	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}
OnMessageResult TextInput::OnChar(char key)
{
	// Handle backspace character
	switch (key)
	{
	case '\r':
		// Enter key was pressed - stop actively inputing text
		m_activelyEnteringText = false;
		m_mouseOverDownState = MouseOverDown::NONE;
		return OnMessageResult::MESSAGE_HANDLED;
		
	case '\b':	m_text->Pop(); break;
	default:	m_text->AddChar(key); break;
	}	

	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}
