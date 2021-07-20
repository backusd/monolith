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
	// If the drop down is open and mouse is over, send message to drop down layout.
	// Otherwise, just send message to main layout
	if (m_dropDownIsOpen && m_dropDownLayout->MouseIsOver(mouseState->X(), mouseState->Y()))
	{
		return m_dropDownLayout->OnLButtonDown(mouseState);
	}

	return m_mainLayout->OnLButtonDown(mouseState);
}

std::shared_ptr<OnMessageResult> DropDown::OnLButtonUp(std::shared_ptr<MouseState> mouseState)
{
	// If the drop down is open and mouse is over, send message to drop down layout.
	// Otherwise, just send message to main layout
	if (m_dropDownIsOpen && m_dropDownLayout->MouseIsOver(mouseState->X(), mouseState->Y()))
	{
		return m_dropDownLayout->OnLButtonUp(mouseState);
	}

	std::shared_ptr<OnMessageResult> result = m_mainLayout->OnLButtonUp(mouseState);

	// If the click was handled, then expand/collapse the drop down
	if (result->MessageHandled())
	{
		m_dropDownIsOpen = !m_dropDownIsOpen;
		result->Redraw(true);
	}

	return result;
}

std::shared_ptr<OnMessageResult> DropDown::OnMouseMove(std::shared_ptr<MouseState> mouseState)
{
	// If the drop down is not open, just send the message to the main layout. Otherwise,
	// send to both layouts
	std::shared_ptr<OnMessageResult> result1 = m_mainLayout->OnMouseMove(mouseState);
	std::shared_ptr<OnMessageResult> result2 = nullptr;
	if (m_dropDownIsOpen)
	{
		result2 = m_dropDownLayout->OnMouseMove(mouseState);

		// If the message was not handled and is not over either layouts, collapse the dropdown
		if (!result2->MessageHandled() && 
			!m_mainLayout->MouseIsOver(mouseState->X(), mouseState->Y()) && 
			!m_dropDownLayout->MouseIsOver(mouseState->X(), mouseState->Y()))
		{
			m_dropDownIsOpen = false;
			result1->Redraw(true);
		}
	}
	else
		result2 = std::make_shared<OnMessageResult>();

	// Combine results
	result1->CaptureMouse(result1->CaptureMouse() || result2->CaptureMouse());
	result1->Redraw(result1->Redraw() || result2->Redraw());
	result1->MessageHandled(result1->MessageHandled() || result2->MessageHandled());

	return result1;

}

std::shared_ptr<OnMessageResult> DropDown::OnMouseLeave()
{
	// Pass to each layout
	std::shared_ptr<OnMessageResult> result = m_mainLayout->OnMouseLeave();
	result = m_dropDownLayout->OnMouseLeave();

	m_dropDownIsOpen = false;
	result->Redraw(true);

	return result;
}