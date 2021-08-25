#include "ComboBox.h"

ComboBox::ComboBox(const std::shared_ptr<DeviceResources>& deviceResources,
	const std::shared_ptr<Layout>& parentLayout,
	int row, int column, int rowSpan, int columnSpan) :
	Control(deviceResources, parentLayout, row, column, rowSpan, columnSpan),
	m_mainLayout(nullptr),
	m_dropDownLayout(nullptr),
	m_dropDownIsOpen(false),
	m_colorTheme(nullptr),
	m_dropDownHeight(100.0f),
	m_dropDownWidth(100.0f),
	m_mainText(nullptr)
{
	SetColorTheme(THEME_DEFAULT_DROP_DOWN_COLOR);

	// Create a sub layout not as a child of the parent that will house the main button
	// Because the default will be to have no margins, and row/column index = 0 and row/columnSpan = 1
	// we can automatically assign the layout to be the same as the (0,0) rectangle of the parent layout
	m_mainLayout = std::make_shared<Layout>(m_deviceResources, GetParentRect());

	// Main layout will have two controls, the down arrow glyph and the main text
	RowColDefinitions columnDefs;
	columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f);
	columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
	m_mainLayout->SetColumnDefinitions(columnDefs);

	// Down arrow glyph
	std::shared_ptr<Text> downArrowGlyph = m_mainLayout->CreateControl<Text>(0, 0);
	downArrowGlyph->SetTextTheme(THEME_DEFAULT_COMBO_BOX_DOWN_ARROW_GLYPH);
	downArrowGlyph->SetText(L"\xE70D");

	// Main text
	m_mainText = m_mainLayout->CreateControl<Text>(0, 1);
	m_mainText->SetTextTheme(THEME_DEFAULT_COMBO_BOX_TEXT);


	// Create the drop down layout with the top aligned with bottom of the main layout and
	// left aligned with the main layout
	D2D1_RECT_F dropDownRect;
	dropDownRect.top = m_mainLayout->Bottom();
	dropDownRect.left = m_mainLayout->Left();
	dropDownRect.right = dropDownRect.left + m_dropDownWidth;
	dropDownRect.bottom = dropDownRect.top + m_dropDownHeight;

	m_dropDownLayout = std::make_shared<Layout>(m_deviceResources, dropDownRect);


}

void ComboBox::SetDropDownSize(float height, float width)
{
	m_dropDownHeight = height;
	m_dropDownWidth = width;

	Resize();
}

void ComboBox::ReleaseLayout()
{
	m_mainLayout->ReleaseLayout();
	m_dropDownLayout->ReleaseLayout();

	m_parentLayout = nullptr;
}

bool ComboBox::Render2D()
{
	m_mainLayout->Render2DControls();

	// Draw the background of the drop down region and then draw the drop down controls over it
	if (m_dropDownIsOpen)
	{
		ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();
		context->SetTransform(m_deviceResources->OrientationTransform2D());
		D2D1_RECT_F rect = m_dropDownLayout->GetRect(
			0,
			0,
			m_dropDownLayout->RowCount(),
			m_dropDownLayout->ColumnCount()
		);
		context->FillRectangle(rect, m_colorTheme->GetBrush(MouseOverDown::NONE));


		m_dropDownLayout->Render2DControls();
	}

	return true;
}

void ComboBox::Resize()
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

bool ComboBox::MouseIsOver(int x, int y)
{
	return MouseIsOverMainLayout(x, y) || (m_dropDownIsOpen && MouseIsOverDropDownLayout(x, y));
}
bool ComboBox::MouseIsOverMainLayout(int x, int y)
{
	return	x >= m_mainLayout->Left() &&
		x <= m_mainLayout->Right() &&
		y >= m_mainLayout->Top() &&
		y <= m_mainLayout->Bottom();
}
bool ComboBox::MouseIsOverDropDownLayout(int x, int y)
{
	return	x >= m_dropDownLayout->Left() &&
		x <= m_dropDownLayout->Right() &&
		y >= m_dropDownLayout->Top() &&
		y <= m_dropDownLayout->Bottom();
}


OnMessageResult ComboBox::OnLButtonDown(std::shared_ptr<MouseState> mouseState)
{
	// If the drop down is open and mouse is over, send message to drop down layout.
	// Otherwise, just send message to main layout
	if (m_dropDownIsOpen && m_dropDownLayout->MouseIsOver(mouseState->X(), mouseState->Y()))
	{
		return m_dropDownLayout->OnLButtonDown(mouseState);
	}

	return m_mainLayout->OnLButtonDown(mouseState);
}

OnMessageResult ComboBox::OnLButtonUp(std::shared_ptr<MouseState> mouseState)
{
	OnMessageResult result;

	// If the drop down is open and mouse is over, send message to drop down layout.
	// Otherwise, just send message to main layout
	if (m_dropDownIsOpen && m_dropDownLayout->MouseIsOver(mouseState->X(), mouseState->Y()))
		result = m_dropDownLayout->OnLButtonUp(mouseState);
	else
		result = m_mainLayout->OnLButtonUp(mouseState);

	// If the click was handled, then expand/collapse the drop down
	if (result == OnMessageResult::MESSAGE_HANDLED || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED)
		m_dropDownIsOpen = !m_dropDownIsOpen;

	return result;
}

OnMessageResult ComboBox::OnMouseMove(std::shared_ptr<MouseState> mouseState)
{
	// If the drop down is not open, just send the message to the main layout. Otherwise,
	// send to both layouts
	OnMessageResult result1 = m_mainLayout->OnMouseMove(mouseState);
	OnMessageResult result2 = OnMessageResult::NONE;

	if (m_dropDownIsOpen)
	{
		result2 = m_dropDownLayout->OnMouseMove(mouseState);

		// If the message was not handled and is not over either layouts, collapse the dropdown
		bool messagedHandled = result2 == OnMessageResult::MESSAGE_HANDLED || result2 == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
		if (!messagedHandled &&
			!m_mainLayout->MouseIsOver(mouseState->X(), mouseState->Y()) &&
			!m_dropDownLayout->MouseIsOver(mouseState->X(), mouseState->Y()))
		{
			m_dropDownIsOpen = false;
		}
	}

	// Combine results
	bool capture = result1 == OnMessageResult::CAPTURE_MOUSE || result1 == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED || result2 == OnMessageResult::CAPTURE_MOUSE || result2 == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
	bool handled = result1 == OnMessageResult::MESSAGE_HANDLED || result1 == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED || result2 == OnMessageResult::MESSAGE_HANDLED || result2 == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;

	if (capture && handled)
		return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;

	if (capture)
		return OnMessageResult::CAPTURE_MOUSE;

	if (handled)
		return OnMessageResult::MESSAGE_HANDLED;

	return OnMessageResult::NONE;
}

OnMessageResult ComboBox::OnMouseLeave()
{
	// Pass to each layout
	OnMessageResult result = m_mainLayout->OnMouseLeave();
	result = m_dropDownLayout->OnMouseLeave();

	m_dropDownIsOpen = false;

	return result;
}
