#include "ComboBox.h"

ComboBox::ComboBox(const std::shared_ptr<DeviceResources>& deviceResources,
	const std::shared_ptr<Layout>& parentLayout,
	int row, int column, int rowSpan, int columnSpan) :
	Control(deviceResources, parentLayout, row, column, rowSpan, columnSpan),
	m_mainLayout(nullptr),
	m_dropDownListView(nullptr),
	m_dropDownListViewLayout(nullptr),
	m_dropDownIsOpen(false),
	m_mouseX(0),
	m_mouseY(0),
	m_backgroundTheme(nullptr),
	m_borderTheme(nullptr),
	m_dropDownItemHeight(30.0f),
	m_dropDownWidth(100.0f),
	m_dropDownItemCount(0),
	m_mainText(nullptr),
	SelectionChangedMethod([](std::wstring) {})
{
	SetBackgroundTheme(THEME_DEFAULT_COMBO_BOX_BACKGROUND_COLOR);
	SetBorderTheme(THEME_DEFAULT_COMBO_BOX_BORDER_COLOR);

	// Create a sub layout not as a child of the parent that will house the main button
	// Because the default will be to have no margins, and row/column index = 0 and row/columnSpan = 1
	// we can automatically assign the layout to be the same as the (0,0) rectangle of the parent layout
	m_mainLayout = std::make_shared<Layout>(m_deviceResources, GetParentRect());

	std::shared_ptr<Button> mainButton = m_mainLayout->CreateControl<Button>(0, 0);
	mainButton->SetColorTheme(THEME_DEFAULT_COMBO_BOX_BUTTON_COLOR);
	mainButton->Click([this]() {
		this->SwitchDropDownIsOpen();
		});

	std::shared_ptr<Layout> mainButtonLayout = mainButton->GetLayout();

	// Main button layout will have two controls, the down arrow glyph and the main text
	RowColDefinitions columnDefs;
	columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f);
	columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
	mainButtonLayout->SetColumnDefinitions(columnDefs);

	// Down arrow glyph
	std::shared_ptr<Text> downArrowGlyph = mainButtonLayout->CreateControl<Text>(0, 0);
	downArrowGlyph->SetTextTheme(THEME_DEFAULT_COMBO_BOX_DOWN_ARROW_GLYPH);
	downArrowGlyph->SetText(L"\xE70D");

	// Create the main button text
	m_mainText = mainButtonLayout->CreateControl<Text>(0, 1);
	m_mainText->SetTextTheme(THEME_DEFAULT_COMBO_BOX_TEXT);

	// ListView uses the GetParentRect() function to size itself, so we need to create a layout that it will reside in.
	D2D1_RECT_F dropDownRect;
	dropDownRect.top = m_mainLayout->Bottom();
	dropDownRect.left = m_mainLayout->Left();
	dropDownRect.right = m_mainLayout->Right();		// Set the width of the drop down to the width of the main layout
	dropDownRect.bottom = m_deviceResources->PixelsToDIPS(static_cast<float>(m_deviceResources->WindowRect().bottom)) - 10;	// Make the bottom of the list view 10 pixels above the bottom of the window

	m_dropDownListViewLayout = std::make_shared<Layout>(m_deviceResources, dropDownRect);

	// Allow the layout to color itself
	m_dropDownListViewLayout->SetColorTheme(THEME_DEFAULT_COMBO_BOX_BACKGROUND_COLOR);
	m_dropDownListViewLayout->SetBackgroundColorMargins(0.0f, 0.0f, 0.0f, 5.0f);

	// Create the listview control within the drop down layout
	m_dropDownListView = m_dropDownListViewLayout->CreateControl<ListView<std::wstring>>(0, 0);
	m_dropDownListView->SetItemHeight(m_dropDownItemHeight);
	m_dropDownListView->SetHighlightItemLayoutMethod(
		[](std::shared_ptr<Layout> layout)
		{
			// I don't think we need to implement this method because no listview item will be highlighted


			//std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(layout->GetChildControl(0));
			//if (button != nullptr)
			//	button->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_HIGHLIGHTED_COLOR);
		}
	);
	m_dropDownListView->SetUnhighlightItemLayoutMethod(
		[](std::shared_ptr<Layout> layout)
		{
			// I don't think we need to implement this method because no listview item will be highlighted


			//std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(layout->GetChildControl(0));
			//if (button != nullptr)
			//	button->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_COLOR);
		}
	);
	m_dropDownListView->SetItemClickMethod(
		[this,
		weakMainText = std::weak_ptr<Text>(m_mainText)](std::shared_ptr<std::wstring> itemText)
		{
			// auto comboBox = weakComboBox.lock();
			auto mainText = weakMainText.lock();

			// When an item is clicked, the listview should collapse
			// If the item clicked differs from the previously selected item, then
			// the new item should populate the main layout and the SelectionChanged
			// event should be triggered

			this->CollapseDropDown();

			std::wstring _itemText = *itemText;
			if (mainText->GetText() != _itemText)
			{
				mainText->SetText(_itemText);
				this->SelectionChangedMethod(_itemText);
			}
		}
	);
	m_dropDownListView->SetValueChangedUpdateLayoutMethod(
		[](std::shared_ptr<std::wstring> itemText, std::shared_ptr<Layout> layout)
		{
			// I don't think we need to implement this method because all listview items will be static


			/*
			std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(layout->GetChildControl(0));
			std::shared_ptr<Layout> buttonLayout = button->GetLayout();

			// Text of the atom type
			std::shared_ptr<Text> atomNameText = std::dynamic_pointer_cast<Text>(buttonLayout->GetChildControl(0));
			atomNameText->SetText(atom->Name());

			// Atom position values
			std::shared_ptr<Text> positionValueText = std::dynamic_pointer_cast<Text>(buttonLayout->GetChildControl(2));
			std::ostringstream positionOSS;
			positionOSS.precision(3);
			positionOSS << std::fixed << "(" << atom->Position().x << ", " << atom->Position().y << ", " << atom->Position().z << ")";
			positionValueText->SetText(positionOSS.str());

			// Atom velocity values
			std::shared_ptr<Text> velocityValueText = std::dynamic_pointer_cast<Text>(buttonLayout->GetChildControl(4));
			std::ostringstream velocityOSS;
			velocityOSS.precision(3);
			velocityOSS << std::fixed << "(" << atom->Velocity().x << ", " << atom->Velocity().y << ", " << atom->Velocity().z << ")";
			velocityValueText->SetText(velocityOSS.str());
			*/
		}
	);

	m_dropDownListView->SetFormatFunction(
		[this,
		weakDeviceResources = std::weak_ptr<DeviceResources>(m_deviceResources),
		weakListView = std::weak_ptr<ListView<std::wstring>>(m_dropDownListView)](std::shared_ptr<std::wstring> itemText, bool highlighted)
		{
			//auto comboBox = weakComboBox.lock();
			auto deviceResources = weakDeviceResources.lock();
			auto listView = weakListView.lock();

			// Create a new layout object with the same height and a reasonable width (it will get resized)
			std::shared_ptr<Layout> newLayout = std::make_shared<Layout>(deviceResources, 0.0f, 0.0f, this->GetDropDownItemHeight(), this->GetDropDownItemWidth());

			std::shared_ptr<Button> newButton = newLayout->CreateControl<Button>();
			newButton->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_COLOR);
			newButton->SetBorderTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_BORDER);
			newButton->Margin(0.0f, 0.0f);
			std::shared_ptr<Layout> newButtonLayout = newButton->GetLayout();

			// Create a single text control in the button
			std::shared_ptr<Text> text = newButtonLayout->CreateControl<Text>();
			text->SetTextTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_TEXT);
			text->SetText(*itemText);
			text->Margin(10.0f, 0.0f, 0.0f, 0.0f);

			return newLayout;
		}
	);
}

void ComboBox::SetDropDownItemHeight(float height)
{
	m_dropDownItemHeight = height;
	Resize();
}

void ComboBox::ReleaseLayout()
{
	m_mainLayout->ReleaseLayout();
	m_dropDownListViewLayout->ReleaseLayout();

	m_parentLayout = nullptr;
}

bool ComboBox::Render2D()
{	
	// Draw the text of the main layout
	m_mainLayout->Render2DControls();	

	// Draw the listview
	if (m_dropDownIsOpen)
		m_dropDownListViewLayout->Render2DControls();

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
	rect.bottom = m_deviceResources->PixelsToDIPS(static_cast<float>(m_deviceResources->WindowRect().bottom)) - 10;	// Make the bottom of the list view 10 pixels above the bottom of the window

	m_dropDownListViewLayout->OnResize(rect);
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
	return	x >= m_dropDownListViewLayout->Left() &&
		x <= m_dropDownListViewLayout->Right() &&
		y >= m_dropDownListViewLayout->Top() &&
		y <= m_dropDownListViewLayout->Bottom();
}


OnMessageResult ComboBox::OnLButtonDown(std::shared_ptr<MouseState> mouseState)
{
	if (m_dropDownIsOpen && m_dropDownListViewLayout->MouseIsOver(mouseState->X(), mouseState->Y()))
	{
		return m_dropDownListViewLayout->OnLButtonDown(mouseState);
	}

	return m_mainLayout->OnLButtonDown(mouseState);
}

OnMessageResult ComboBox::OnLButtonUp(std::shared_ptr<MouseState> mouseState)
{
	OnMessageResult result;

	// If the drop down is open and mouse is over, send message to drop down layout.
	// Otherwise, just send message to main layout
	if (m_dropDownIsOpen && m_dropDownListViewLayout->MouseIsOver(mouseState->X(), mouseState->Y()))
	{
		result = m_dropDownListViewLayout->OnLButtonUp(mouseState);

		// If the click was handled, then expand/collapse the drop down
		//if (result == OnMessageResult::MESSAGE_HANDLED || result == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED)

	}
	else
		result = m_mainLayout->OnLButtonUp(mouseState);

	return result;
}

OnMessageResult ComboBox::OnMouseMove(std::shared_ptr<MouseState> mouseState)
{
	// Keep mouse location info so that the OnMouseWheel event can test if mouse is over the drop down
	m_mouseX = mouseState->X();
	m_mouseY = mouseState->Y();

	// If the drop down is not open, just send the message to the main layout. Otherwise,
	// send to both layouts
	OnMessageResult result1 = m_mainLayout->OnMouseMove(mouseState);
	OnMessageResult result2 = OnMessageResult::NONE;

	if (m_dropDownIsOpen)
	{
		result2 = m_dropDownListViewLayout->OnMouseMove(mouseState);

		// If the message was not handled and is not over either layouts, collapse the dropdown
		bool messagedHandled = result2 == OnMessageResult::MESSAGE_HANDLED || result2 == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
		if (!messagedHandled &&
			!m_mainLayout->MouseIsOver(mouseState->X(), mouseState->Y()) &&
			!m_dropDownListViewLayout->MouseIsOver(mouseState->X(), mouseState->Y()))
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
	
	//result = m_dropDownLayout->OnMouseLeave();
	result = m_dropDownListViewLayout->OnMouseLeave();

	m_dropDownIsOpen = false;

	return result;
}

OnMessageResult ComboBox::OnMouseWheel(int wheelDelta)
{
	if (m_dropDownIsOpen && MouseIsOverDropDownLayout(m_mouseX, m_mouseY))
		return m_dropDownListViewLayout->OnMouseWheel(wheelDelta);

	return OnMessageResult::NONE;
}

void ComboBox::AddComboBoxItem(std::wstring text)
{
	// Just add the text as a new item to the list view
	std::shared_ptr<std::wstring> _text = std::make_shared<std::wstring>(text);
	m_dropDownListView->AddItem(_text);

	// if the main text is empty (because no items have been added yet), add this to the main text
	if (m_mainText->GetText() == L"")
		m_mainText->SetText(text);
}

void ComboBox::SelectItem(std::wstring text)
{
	if (m_mainText->GetText() != text)
	{
		m_mainText->SetText(text);
		SelectionChangedMethod(text);
	}
}