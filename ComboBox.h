#pragma once
#include "pch.h"
#include "Control.h"
#include "Button.h"
#include "Text.h"
#include "ThemeManager.h"


#include <string>
#include <vector> 


class ComboBox : public Control
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


	OnMessageResult OnLButtonDown(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnLButtonUp(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseMove(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseLeave() override;

	void SetBackgroundTheme(std::string name) { m_backgroundTheme = std::static_pointer_cast<ColorTheme>(ThemeManager::GetTheme(name)); }
	void SetBorderTheme(std::string name) { m_borderTheme = std::static_pointer_cast<ColorTheme>(ThemeManager::GetTheme(name)); }


	std::shared_ptr<Layout> GetMainLayout() { return m_mainLayout; }
	std::shared_ptr<Layout> GetDropDownLayout() { return m_dropDownLayout; }
	
	void SetDropDownWidth(float width);
	void SetDropDownItemHeight(float height);

private:
	void Resize();
	bool MouseIsOverMainLayout(int x, int y);
	bool MouseIsOverDropDownLayout(int x, int y);

	std::shared_ptr<Layout> m_mainLayout;
	std::shared_ptr<Layout> m_dropDownLayout;

	// Themes
	std::shared_ptr<ColorTheme> m_backgroundTheme;
	std::shared_ptr<ColorTheme> m_borderTheme;

	std::shared_ptr<Text> m_mainText;

	bool m_dropDownIsOpen;

	float m_dropDownWidth;
	float m_dropDownItemHeight;
	int m_dropDownItemCount;
};