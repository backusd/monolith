#pragma once
#include "pch.h"
#include "Control.h"
#include "Button.h"
#include "ThemeManager.h"

#include <string>
#include <vector>

/*
TabbedPane will contain the following:
	- A list of buttons across the top of the control that when clicked, will present a new pane
	- A list of layouts, one for each tab that is created
	- Default will be to start with only one, but will expose methods to add more
*/


class TabbedPane : public Control
{
public:
	TabbedPane(const std::shared_ptr<DeviceResources>& deviceResources,
		const std::shared_ptr<Layout>& parentLayout,
		int row, int column, int rowSpan, int columnSpan);

	void ReleaseLayout() override;

	bool Render2D() override;

	void OnLayoutResize() override { Resize(); }
	void OnMarginChanged() override { Resize(); }
	bool MouseIsOver(int x, int y) override;


	OnMessageResult OnLButtonDown(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnLButtonUp(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseMove(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseLeave() override;

	// ======

	void SetTabSelectedColorTheme(std::string name) { m_tabSelectedColorTheme = std::static_pointer_cast<ColorTheme>(ThemeManager::GetTheme(name)); }
	void SetTabUnselectedColorTheme(std::string name) { m_tabUnselectedColorTheme = std::static_pointer_cast<ColorTheme>(ThemeManager::GetTheme(name)); }
	void SetTabSelectedBorderTheme(std::string name) { m_tabSelectedBorderTheme = std::static_pointer_cast<BorderTheme>(ThemeManager::GetTheme(name)); }
	void SetTabUnselectedBorderTheme(std::string name) { m_tabUnselectedBorderTheme = std::static_pointer_cast<BorderTheme>(ThemeManager::GetTheme(name)); }
	void SetPaneColorTheme(std::string name) { m_panesColorTheme = std::static_pointer_cast<ColorTheme>(ThemeManager::GetTheme(name)); }
	void SetPaneBorderTheme(std::string name) { m_panesBorderTheme = std::static_pointer_cast<BorderTheme>(ThemeManager::GetTheme(name)); }

	void SetNumberOfTabs(int numberOfTabs);
	void SelectTab(unsigned int index);

	void SetTabRowHeight(ROW_COL_TYPE rowType, float value);

	std::shared_ptr<Layout> GetTabLayout(int index) { return m_tabs[index]->GetLayout(); }
	std::shared_ptr<Layout> GetPaneLayout(int index) { return m_panes[index]; }

private:
	void Resize();

	void UpdateLayouts();


	void UpdateMainRows();

	// ==========
	MouseOverDown m_mouseOverDownState;	// Only used for deciding the color of the pane background

	std::shared_ptr<Layout> m_mainLayout;	// Will be the layout for the entire control
	std::shared_ptr<Layout> m_tabSubLayout;	// Sublayout for the tabs

	std::vector<std::shared_ptr<Button>> m_tabs;
	std::vector<std::shared_ptr<Layout>> m_panes;

	std::shared_ptr<ColorTheme> m_tabSelectedColorTheme;
	std::shared_ptr<ColorTheme> m_tabUnselectedColorTheme;

	std::shared_ptr<BorderTheme> m_tabSelectedBorderTheme;
	std::shared_ptr<BorderTheme> m_tabUnselectedBorderTheme;

	std::shared_ptr<ColorTheme>	 m_panesColorTheme;		// We will probably mostly want this to be the same as the selected tab color
	std::shared_ptr<BorderTheme> m_panesBorderTheme;

	unsigned int m_selectedTabIndex;

	// Allow for setting of the tab row height
	// (Panes will always be given 100.0 STAR height)
	ROW_COL_TYPE	m_tabRowType;
	float			m_tabRowValue;


};