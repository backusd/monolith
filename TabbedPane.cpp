#include "TabbedPane.h"

TabbedPane::TabbedPane(const std::shared_ptr<DeviceResources>& deviceResources,
	const std::shared_ptr<Layout>& parentLayout,
	int row, int column, int rowSpan, int columnSpan) :
	Control(deviceResources, parentLayout, row, column, rowSpan, columnSpan),
	m_mouseOverDownState(MouseOverDown::NONE),
	m_mainLayout(nullptr),
	m_tabSubLayout(nullptr),
	m_tabSelectedColorTheme(nullptr),
	m_tabUnselectedColorTheme(nullptr),
	m_tabSelectedBorderTheme(nullptr),
	m_tabUnselectedBorderTheme(nullptr),
	m_panesColorTheme(nullptr),
	m_panesBorderTheme(nullptr),
	m_selectedTabIndex(0),
	m_tabRowType(ROW_COL_TYPE::ROW_COL_TYPE_FIXED),
	m_tabRowValue(30.0f)
{
	SetTabSelectedColorTheme(THEME_DEFAULT_TABBED_PANE_TAB_SELECTED_COLOR);
	SetTabUnselectedColorTheme(THEME_DEFAULT_TABBED_PANE_TAB_UNSELECTED_COLOR);
	SetTabSelectedBorderTheme(THEME_DEFAULT_TABBED_PANE_TAB_SELECTED_BORDER);
	SetTabUnselectedBorderTheme(THEME_DEFAULT_TABBED_PANE_TAB_UNSELECTED_BORDER);
	SetPaneColorTheme(THEME_DEFAULT_TABBED_PANE_PANE_COLOR);
	SetPaneBorderTheme(THEME_DEFAULT_TABBED_PANE_PANE_BORDER);

	// Create a sub layout not as a child of the parent that will house the entire control
	// Because the default will be to have no margins, and row/column index = 0 and row/columnSpan = 1
	// we can automatically assign the layout to be the same as the (0,0) rectangle of the parent layout
	m_mainLayout = std::make_shared<Layout>(m_deviceResources, GetParentRect());

	// Create the main rows for the control (one for tabs, one for the active pane)
	UpdateMainRows();

	// tabSubLayout will stay nullptr, until SetNumberOfTabs is called
	// m_tabs & m_panes will remain empty until SetNumberOfTabs is called
	
}

void TabbedPane::UpdateLayouts()
{
	// Create two rows: one for the tabs and one for the panes
	RowColDefinitions rowDefs;
	rowDefs.AddDefinition(m_tabRowType, m_tabRowValue);				// Tabs Row
	rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 100.0f);	// Pane Row - set to 100 stars so that if we want to star the tab row, we have a reasonable amount of granularity
	m_mainLayout->SetRowDefinitions(rowDefs);

	// Create the tabs sublayout
	m_tabSubLayout = m_mainLayout->CreateSubLayout(0, 0);

	// Create one column for each tab (or just one column if no tabs have been added yet)
	RowColDefinitions columnDefs;
	int rowsToMake = std::max(1, static_cast<int>(m_tabs.size()));
	for (int iii = 0; iii < rowsToMake; ++iii)
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
	m_tabSubLayout->SetColumnDefinitions(columnDefs);

	// Add the tabs to the new tabs sublayout
	for (unsigned int iii = 0; iii < m_tabs.size(); ++iii)
	{
		m_tabSubLayout->AddControl(m_tabs[iii]);

		// Make sure the control is in the correct column
		// This will resize the control if necessary
		m_tabs[iii]->Column(iii);
	}
}

void TabbedPane::UpdateMainRows()
{
	// Create two rows: one for the tabs and one for the panes
	RowColDefinitions rowDefs;
	rowDefs.AddDefinition(m_tabRowType, m_tabRowValue);				// Tabs Row
	rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 100.0f);	// Pane Row - set to 100 stars so that if we want to star the tab row, we have a reasonable amount of granularity
	m_mainLayout->SetRowDefinitions(rowDefs);
}

void TabbedPane::SetNumberOfTabs(int numberOfTabs)
{
	// reset the selected index to 0 so it doesn't overrun the new number of tabs accidentally
	m_selectedTabIndex = 0;

	// clear out all existing tabs
	m_tabs.clear();

	// clear out the existing panes to create new, empty ones
	m_panes.clear();
	m_mainLayout->ClearSubLayouts(); // This MUST be done BEFORE creating the new tab sublayout

	// Create a new tabs sublayout
	m_tabSubLayout = m_mainLayout->CreateSubLayout(0, 0);

	// Create one column for each tab
	RowColDefinitions columnDefs;
	for (int iii = 0; iii < numberOfTabs; ++iii)
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
	m_tabSubLayout->SetColumnDefinitions(columnDefs);

	// Create a new Button for each tab
	std::shared_ptr<Button> newButton;
	for (int iii = 0; iii < numberOfTabs; ++iii)
	{
		newButton = m_tabSubLayout->CreateControl<Button>(0, iii);
		newButton->SetColorTheme(iii == m_selectedTabIndex ? m_tabSelectedColorTheme : m_tabUnselectedColorTheme);
		newButton->SetBorderTheme(iii == m_selectedTabIndex ? m_tabSelectedBorderTheme : m_tabUnselectedBorderTheme);

		// The button click simply needs to populate the new pane
		newButton->Click(
			[this, iii]() 
			{
				this->SelectTab(iii);
			}
		);

		m_tabs.push_back(newButton);
	}

	
	// Create the new panes
	D2D1_RECT_F paneRect = m_mainLayout->GetRect(1, 0);
	for (int iii = 0; iii < numberOfTabs; ++iii)
		m_panes.push_back(std::make_shared<Layout>(m_deviceResources, paneRect));

	// Add the selected pane to the main layout
	m_mainLayout->SetSubLayout(m_panes[m_selectedTabIndex], 1, 0);
}

void TabbedPane::SelectTab(unsigned int index)
{
	m_selectedTabIndex = index;

	// clear the sublayouts
	m_mainLayout->ClearSubLayouts();

	// add back the tabs layout
	m_mainLayout->SetSubLayout(m_tabSubLayout, 0, 0);

	// add the new pane layout
	m_mainLayout->SetSubLayout(m_panes[m_selectedTabIndex], 1, 0);

	// Resize to make sure the new pane layout is the correct size
	Resize();
}



void TabbedPane::ReleaseLayout()
{
	m_mainLayout->ReleaseLayout();
	m_tabSubLayout->ReleaseLayout();
	//for (std::shared_ptr<Button> tab : m_tabs)
	//	tab->ReleaseLayout();
	for (std::shared_ptr<Layout> pane : m_panes)
		pane->ReleaseLayout();

	m_tabs.clear();
	m_panes.clear();

	m_parentLayout = nullptr;
}

bool TabbedPane::Render2D()
{
	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();

	context->SetTransform(m_deviceResources->OrientationTransform2D());

	// Just get the lower pane (exclude the tabs)
	D2D1_RECT_F rect = m_mainLayout->GetRect(
		1,
		0,
		m_mainLayout->RowCount() - 1,
		m_mainLayout->ColumnCount()
	);

	context->FillRectangle(rect, m_panesColorTheme->GetBrush(m_mouseOverDownState));

	D2D1_POINT_2F bottomLeft, topLeft, topRight, bottomRight;
	bottomLeft.x = rect.left;
	bottomLeft.y = rect.bottom;

	topLeft.x = rect.left;
	topLeft.y = rect.top;

	topRight.x = rect.right;
	topRight.y = rect.top;

	bottomRight.x = rect.right;
	bottomRight.y = rect.bottom;

	// if the border theme is not nullptr and stroke width > 0, draw the border
	if (m_panesBorderTheme != nullptr)
	{
		if (m_panesBorderTheme->GetStrokeWidthLeft() > 0.0f)
		{
			context->DrawLine(
				bottomLeft,
				topLeft,
				m_panesBorderTheme->GetBrush(m_mouseOverDownState),
				m_panesBorderTheme->GetStrokeWidthLeft(),
				m_panesBorderTheme->GetStrokeStyle()
			);
		}

		if (m_panesBorderTheme->GetStrokeWidthTop() > 0.0f)
		{
			context->DrawLine(
				topLeft,
				topRight,
				m_panesBorderTheme->GetBrush(m_mouseOverDownState),
				m_panesBorderTheme->GetStrokeWidthTop(),
				m_panesBorderTheme->GetStrokeStyle()
			);
		}

		if (m_panesBorderTheme->GetStrokeWidthRight() > 0.0f)
		{
			context->DrawLine(
				topRight,
				bottomRight,
				m_panesBorderTheme->GetBrush(m_mouseOverDownState),
				m_panesBorderTheme->GetStrokeWidthRight(),
				m_panesBorderTheme->GetStrokeStyle()
			);
		}

		if (m_panesBorderTheme->GetStrokeWidthBottom() > 0.0f)
		{
			context->DrawLine(
				bottomRight,
				bottomLeft,
				m_panesBorderTheme->GetBrush(m_mouseOverDownState),
				m_panesBorderTheme->GetStrokeWidthBottom(),
				m_panesBorderTheme->GetStrokeStyle()
			);
		}
	}




	// Now draw the tabs and the pane
	m_tabSubLayout->Render2DControls();
	m_panes[m_selectedTabIndex]->Render2DControls();

	return true;
}

bool TabbedPane::MouseIsOver(int x, int y)
{
	return	x >= m_mainLayout->Left() &&
			x <= m_mainLayout->Right() &&
			y >= m_mainLayout->Top() &&
			y <= m_mainLayout->Bottom();
}


OnMessageResult TabbedPane::OnLButtonDown(std::shared_ptr<MouseState> mouseState)
{
	// If the mouse is over the tab sublayout, pass the message to the tabs, otherwise pass it the selected pane
	OnMessageResult result = m_tabSubLayout->MouseIsOver(mouseState->X(), mouseState->Y()) ? m_tabSubLayout->OnLButtonDown(mouseState) : m_panes[m_selectedTabIndex]->OnLButtonDown(mouseState);

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
OnMessageResult TabbedPane::OnLButtonUp(std::shared_ptr<MouseState> mouseState)
{
	// If the mouse is over the tab sublayout, pass the message to the tabs, otherwise pass it the selected pane
	OnMessageResult result = m_tabSubLayout->MouseIsOver(mouseState->X(), mouseState->Y()) ? m_tabSubLayout->OnLButtonUp(mouseState) : m_panes[m_selectedTabIndex]->OnLButtonUp(mouseState);

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

	// Capture the mouse and signal that the message was handled
	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}
OnMessageResult TabbedPane::OnMouseMove(std::shared_ptr<MouseState> mouseState)
{
	// We need to pass the message to both so that both can update as the mouse moves between tab and pane
	OnMessageResult result1 = m_tabSubLayout->OnMouseMove(mouseState);
	OnMessageResult result2 = m_panes[m_selectedTabIndex]->OnMouseMove(mouseState);

	// If the message was handled by a sublayout/control, make sure buttonMouseState is NONE and return
	// The same behavior is required when the mouse is not over the control
	bool messagedHandled = (result1 == OnMessageResult::MESSAGE_HANDLED ||
							result1 == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED ||
							result2 == OnMessageResult::MESSAGE_HANDLED ||
							result2 == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED);

	if (messagedHandled || !MouseIsOver(mouseState->X(), mouseState->Y()))
	{
		// Set MouseOverDown state to None because mouse is actually over child control which gets priority
		m_mouseOverDownState = MouseOverDown::NONE;

		if (result1 == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED || result2 == OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED)
			return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
		else if (result1 == OnMessageResult::MESSAGE_HANDLED || result2 == OnMessageResult::MESSAGE_HANDLED)
			return OnMessageResult::MESSAGE_HANDLED;

		return OnMessageResult::NONE;
	}

	// Messaged not handled, so let this control handle it
	m_mouseOverDownState = mouseState->LButtonDown() ? MouseOverDown::MOUSE_DOWN : MouseOverDown::MOUSE_OVER;

	// Capture the mouse and signal that the message was handled
	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}
OnMessageResult TabbedPane::OnMouseLeave()
{
	// Pass message to tabs layout and panes layout first
	OnMessageResult result = m_tabSubLayout->OnMouseLeave();
	result = m_panes[m_selectedTabIndex]->OnMouseLeave();

	// Update the mouse state to NONE 
	m_mouseOverDownState = MouseOverDown::NONE;
	return result;
}

void TabbedPane::Resize()
{
	// Resize the button layout when anything about the parent rect or margins change
	D2D1_RECT_F rect = GetParentRect();
	rect.left += m_marginLeft;
	rect.right -= m_marginRight;
	rect.top += m_marginTop;
	rect.bottom -= m_marginBottom;

	// NOTE: Resizing the button layout will automatically resize all child controls because they belong
	// to the layout, NOT the button itself
	m_mainLayout->OnResize(rect);

	// Don't worry about resizing the non-selected panes - they will be appropriately sized upon being selected
}

void TabbedPane::SetTabRowHeight(ROW_COL_TYPE rowType, float value)
{
	m_tabRowType = rowType;
	m_tabRowValue = value;

	UpdateMainRows();

	
}