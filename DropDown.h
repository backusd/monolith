#pragma once
#include "pch.h"
#include "Control.h"
#include "Button.h"
#include "ThemeManager.h"

#include <string>
#include <vector>

/*
Drop down will contain the following:
    - A main button that will reside within the parent layout
	- A layout to house a list of controls that will be rendered vertically
		* This layout will define the height/width of each child control, so when we add a new control
			we need to create a new row and specify the height we want

	What we really need is for the DropDown control to have two layouts: one for the main button and the other
	for the drop down menu. We can then use the main layout to generate the main button
*/


class DropDown : public Control
{
public:
	DropDown(const std::shared_ptr<DeviceResources>& deviceResources,
		     const std::shared_ptr<Layout>& parentLayout, 
		     int row, int column, int rowSpan, int columnSpan);

	void ReleaseLayout() override;

	bool Render2D() override;
	//void OnPaint() override;

	void OnLayoutResize() override { Resize(); }
	void OnMarginChanged() override { Resize(); }
	bool MouseIsOver(int x, int y) override;
	

	OnMessageResult OnLButtonDown(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnLButtonUp(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseMove(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseLeave() override;
	
	void SetColorTheme(std::string name) { m_colorTheme = std::static_pointer_cast<ColorTheme>(ThemeManager::GetTheme(name)); }

	std::shared_ptr<Layout> GetMainLayout() { return m_mainLayout; }
	std::shared_ptr<Layout> GetDropDownLayout() { return m_dropDownLayout; }
	void SetDropDownSize(float height, float width);

private:
	void Resize();
	bool MouseIsOverMainLayout(int x, int y);
	bool MouseIsOverDropDownLayout(int x, int y);

	std::shared_ptr<Layout> m_mainLayout;
	std::shared_ptr<Layout> m_dropDownLayout;

	std::shared_ptr<ColorTheme> m_colorTheme;

	bool m_dropDownIsOpen;

	float m_dropDownWidth;
	float m_dropDownHeight;
};