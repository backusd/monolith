#include "Slider.h"

Slider::Slider(const std::shared_ptr<DeviceResources>& deviceResources,
	const std::shared_ptr<Layout>& parentLayout) :
	Slider(deviceResources, parentLayout, 0, 0, 1, 1)
{
}

Slider::Slider(const std::shared_ptr<DeviceResources>& deviceResources,
	const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan) :
	Control(deviceResources, parentLayout, row, column, rowSpan, columnSpan),
	m_layout(nullptr),
	m_textInput(nullptr),
	m_lineTheme(nullptr),
	m_rectColorTheme(nullptr),
	m_rectMouseOverDownState(MouseOverDown::NONE),
	m_sliderMin(0.0f),
	m_sliderMax(1.0f),
	m_sliderValue(0.0f),
	m_rectWidth(20.0f),
	ValueChangedMethod([](float value) {})
{
	SetLineTheme(THEME_DEFAULT_LINE_COLOR);
	SetRectColorTheme(THEME_DEFAULT_SLIDER_RECT_COLOR);

	// Create its own sub layout not as a child of the parent
	// Because the default will be to have no margins, and row/column index = 0 and row/columnSpan = 1
	// we can automatically assign the layout to be the same as the (0,0) rectangle of the parent layout
	m_layout = std::make_shared<Layout>(m_deviceResources, GetParentRect());

	// Create two columns - the left column will house the text input control
	RowColDefinitions columnDefs;
	columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 60.0f);
	columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
	m_layout->SetColumnDefinitions(columnDefs);

	// Add Text Input control
	m_textInput = m_layout->CreateControl<TextInput>(0, 0);
	m_textInput->SetText(m_sliderValue);
}

void Slider::ReleaseLayout()
{
	m_layout->ReleaseLayout();
	m_parentLayout = nullptr;
}


bool Slider::Render2D()
{
	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();
	context->SetTransform(m_deviceResources->OrientationTransform2D());

	// Get the right column where the slider resides
	D2D1_RECT_F rightRect = m_layout->GetRect(0, 1);

	// Draw the slider line
	float y = rightRect.top + (rightRect.bottom - rightRect.top) / 2.0f;
	context->DrawLine(
		D2D1::Point2F(rightRect.left + 5.0f, y),  // 5.0f padding
		D2D1::Point2F(rightRect.right - 5.0f, y), // 5.0f padding
		m_lineTheme->GetBrush()
	);

	// Draw the slider box
	float rectHalfWidth = m_rectWidth / 2.0f;
	float minX = rightRect.left + 4.5f + rectHalfWidth; // far left on the line plus half of the rect's width (use 4.5 so line doesn't peek out)
	float maxX = rightRect.right - 4.5f - rectHalfWidth;
	float rectCenterX = minX + ((m_sliderValue - m_sliderMin) / (m_sliderMax - m_sliderMin)) * (maxX - minX);

	m_sliderRect = D2D1::RectF(
		rectCenterX - rectHalfWidth,
		rightRect.top,
		rectCenterX + rectHalfWidth,
		rightRect.bottom
	);

	context->FillRectangle(m_sliderRect, m_rectColorTheme->GetBrush(m_rectMouseOverDownState));

	// Draw the textInput control
	m_layout->Render2DControls();

	return true;
}

void Slider::Resize()
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

bool Slider::MouseIsOver(int x, int y)
{
	return	x >= m_layout->Left() &&
		x <= m_layout->Right() &&
		y >= m_layout->Top() &&
		y <= m_layout->Bottom();
}

bool Slider::MouseIsOverSlider(int x, int y)
{
	return	x >= m_sliderRect.left &&
			x <= m_sliderRect.right &&
			y >= m_sliderRect.top &&
			y <= m_sliderRect.bottom;
}

OnMessageResult Slider::OnLButtonDown(std::shared_ptr<MouseState> mouseState)
{
	// If the mouse is pressed down on the slider, handle that first
	if (MouseIsOverSlider(mouseState->X(), mouseState->Y()))
	{
		m_rectMouseOverDownState = MouseOverDown::MOUSE_DOWN;
		return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
	}

	m_rectMouseOverDownState = MouseOverDown::NONE;

	// Just return whatever the TextInput returns
	return m_layout->OnLButtonDown(mouseState);
}

OnMessageResult Slider::OnLButtonUp(std::shared_ptr<MouseState> mouseState)
{
	// If the mouse is pressed down on the slider, handle that first
	if (MouseIsOverSlider(mouseState->X(), mouseState->Y()))
	{
		m_rectMouseOverDownState = MouseOverDown::MOUSE_OVER;
		return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
	}

	m_rectMouseOverDownState = MouseOverDown::NONE;

	// Just return whatever the TextInput returns
	return m_layout->OnLButtonDown(mouseState);
}

OnMessageResult Slider::OnMouseMove(std::shared_ptr<MouseState> mouseState)
{
	// First, if the mouse was pressed on the slider and is now moving, react to that first
	if (m_rectMouseOverDownState == MouseOverDown::MOUSE_DOWN)
	{
		// Update the slider position based on the mouse location
		// 
		// Get the right column where the slider resides
		D2D1_RECT_F rightRect = m_layout->GetRect(0, 1);

		// Draw the slider box
		float rectHalfWidth = m_rectWidth / 2.0f;
		float minX = rightRect.left + 5.0f + rectHalfWidth; // far left on the line plus half of the rect's width
		float maxX = rightRect.right - 5.0f - rectHalfWidth;

		// Compute the rect center location, making sure it is within the min/max
		float mouseX = static_cast<float>(mouseState->X());
		mouseX = std::max(mouseX, minX);
		mouseX = std::min(mouseX, maxX);

		m_sliderValue = ((mouseX - minX) / (maxX - minX)) * (m_sliderMax - m_sliderMin) + m_sliderMin;

		// Update the value and apply it to TextInput
		m_textInput->SetText(m_sliderValue);

		// Call the value changed method
		ValueChangedMethod(m_sliderValue);

		return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
	}

	// Slider is not actively moving, so first pass it to the TextInput control
	// NOTE: Do not pass it to the layout which houses the TextInput because when the mouse
	//       leaves the TextInput control, the message will not get passed and the TextInput will
	//       remain in the MouseOver color state
	OnMessageResult result = m_textInput->OnMouseMove(mouseState);

	bool messagedHandled = result == OnMessageResult::MESSAGE_HANDLED || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
	if (messagedHandled || !MouseIsOver(mouseState->X(), mouseState->Y()))
	{
		// Set MouseOverDown state to None because mouse is actually over child control which gets priority
		m_rectMouseOverDownState = MouseOverDown::NONE;
		return result;
	}

	// Message not handled by layout (TextInput), so just text to see if it is over the slider rect
	m_rectMouseOverDownState = MouseIsOverSlider(mouseState->X(), mouseState->Y()) ? MouseOverDown::MOUSE_OVER : MouseOverDown::NONE;

	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}

OnMessageResult Slider::OnMouseLeave()
{
	return OnMessageResult::NONE;
}

OnMessageResult Slider::OnKeyDown(unsigned char keycode)
{
	return m_layout->OnKeyDown(keycode);
}
OnMessageResult Slider::OnKeyUp(unsigned char keycode)
{
	return m_layout->OnKeyUp(keycode);
}
OnMessageResult Slider::OnChar(char key)
{
	OnMessageResult result;

	// Only pass numeric and related keys
	switch (key)
	{
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	case '.':
	case '-':
	case '\r':
	case '\b':
		result = m_layout->OnChar(key);
		break;

	default:
		return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
	}

	// If the enter key was pressed, try to update the location of the slider
	if (key == '\r')
	{
		std::wstring text = m_textInput->GetText();

		// If no text is given, just set it to the minimum value
		if (text.empty())
		{
			m_sliderValue = m_sliderMin;
		}
		else
		{
			try
			{
				m_sliderValue = std::stof(text);
				m_sliderValue = std::max(m_sliderMin, m_sliderValue);
				m_sliderValue = std::min(m_sliderMax, m_sliderValue);
			}
			catch (const std::invalid_argument& /*ia*/)
			{
				m_sliderValue = m_sliderMin;
			}
		}

		m_textInput->SetText(m_sliderValue);

		// Call the value changed method
		ValueChangedMethod(m_sliderValue);
	}

	return result;
}

void Slider::SetMin(float min) 
{ 
	m_sliderMin = min;
	if (m_sliderValue < m_sliderMin)
	{
		m_sliderValue = m_sliderMin;
		m_textInput->SetText(m_sliderValue);

		// Slider Value changed, so call the changed method
		ValueChangedMethod(m_sliderValue);
	}
}

void Slider::SetMax(float max) 
{ 
	m_sliderMax = max;
	if (m_sliderValue > m_sliderMax)
	{
		m_sliderValue = m_sliderMax;
		m_textInput->SetText(m_sliderValue);

		// Slider Value changed, so call the changed method
		ValueChangedMethod(m_sliderValue);
	}
}

void Slider::SetValue(float value)
{
	// Make sure the slider value is within the correct range
	m_sliderValue = std::max(m_sliderMin, value);
	m_sliderValue = std::min(m_sliderMax, m_sliderValue);

	// Set the text
	m_textInput->SetText(m_sliderValue);

	// Call the value changed method
	ValueChangedMethod(m_sliderValue);
}