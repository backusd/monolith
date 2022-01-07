#pragma once
#include "pch.h"
#include "Control.h"
#include "Button.h"
#include "ListView.h"
#include "Text.h"
#include "ThemeManager.h"


#include <string>
#include <vector>
#include <memory>


class ComboBox : public Control, public std::enable_shared_from_this<ComboBox>
{
public:
	ComboBox(const std::shared_ptr<DeviceResources>& deviceResources,
				const std::shared_ptr<Layout>& parentLayout,
				int row, int column, int rowSpan, int columnSpan);

	void ReleaseLayout() override;

	bool Render2D() override;

	void OnLayoutResize() override { Resize(); }
	void OnMarginChanged() override { Resize(); }
	bool MouseIsOver(int x, int y) override;

	// Right now, combobox items are just Text controls, could expand this in the future
	void AddComboBoxItem(std::wstring text);
	void SelectItem(std::wstring text);


	OnMessageResult OnLButtonDown(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnLButtonUp(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseMove(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseLeave() override;
	OnMessageResult OnMouseWheel(int wheelDelta) override;

	void SetBackgroundTheme(std::string name) { m_backgroundTheme = std::static_pointer_cast<ColorTheme>(ThemeManager::GetTheme(name)); }
	void SetBorderTheme(std::string name) { m_borderTheme = std::static_pointer_cast<ColorTheme>(ThemeManager::GetTheme(name)); }


	std::shared_ptr<Layout> GetMainLayout() { return m_mainLayout; }
	
	float GetDropDownItemWidth() { return m_dropDownWidth; }
	float GetDropDownItemHeight() { return m_dropDownItemHeight; }
	void SetDropDownItemHeight(float height);

	void SwitchDropDownIsOpen() { m_dropDownIsOpen = !m_dropDownIsOpen; }
	void CollapseDropDown() { m_dropDownIsOpen = false; }
	void ExpandDropDown() { m_dropDownIsOpen = true; }

	// Event Method assignments
	void SelectionChanged(std::function<void(std::wstring)> method) { SelectionChangedMethod = method; }


private:
	void Resize();
	bool MouseIsOverMainLayout(int x, int y);
	bool MouseIsOverDropDownLayout(int x, int y);

	std::shared_ptr<Layout> m_mainLayout;

	std::shared_ptr<Layout> m_dropDownListViewLayout;
	std::shared_ptr<ListView<std::wstring>> m_dropDownListView;

	// Keep mouse location info so that the OnMouseWheel event can test if mouse is over the drop down
	int m_mouseX;
	int m_mouseY;

	// Themes
	std::shared_ptr<ColorTheme> m_backgroundTheme;
	std::shared_ptr<ColorTheme> m_borderTheme;

	std::shared_ptr<Text> m_mainText;

	bool m_dropDownIsOpen;

	float m_dropDownWidth;
	float m_dropDownItemHeight;
	int m_dropDownItemCount;

	// Event methods
	std::function<void(std::wstring)> SelectionChangedMethod;

};