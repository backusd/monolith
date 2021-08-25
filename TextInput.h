#pragma once
#include "pch.h"
#include "Control.h"
#include "ThemeManager.h"
#include "ColorTheme.h"
#include "MouseOverDown.h"
#include "WindowManager.h"
#include "Text.h"


#include <string>

class TextInput : public Control
{
public:
	TextInput(const std::shared_ptr<DeviceResources>& deviceResources,
		const std::shared_ptr<Layout>& parentLayout);

	TextInput(const std::shared_ptr<DeviceResources>& deviceResources,
		const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan);

	// To release the button's layout, just pass along to the button's layout
	void ReleaseLayout() override;

	bool Render2D() override;

	void OnLayoutResize() override { Resize(); }
	void OnMarginChanged() override { Resize(); }
	bool MouseIsOver(int x, int y) override;

	OnMessageResult OnLButtonDown(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnLButtonUp(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseMove(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseLeave() override;

	OnMessageResult OnKeyDown(unsigned char keycode) override;
	OnMessageResult OnKeyUp(unsigned char keycode) override;
	OnMessageResult OnChar(char key) override;

	void SetBackgroundTheme(std::string name) { m_backgroundTheme = std::static_pointer_cast<ColorTheme>(ThemeManager::GetTheme(name)); }
	void SetBorderTheme(std::string name) { m_borderTheme = std::static_pointer_cast<ColorTheme>(ThemeManager::GetTheme(name)); }

	std::shared_ptr<Layout> GetLayout() { return m_layout; }

	void SetPlaceHolderText(std::string text) { m_placeholderText = text; }

private:
	void Resize();

	// bool for when actively entering text
	bool m_activelyEnteringText;

	// Layout will hold the Text control
	std::shared_ptr<Layout> m_layout;

	// Text control for the input text
	std::shared_ptr<Text> m_text;

	std::string m_placeholderText;

	MouseOverDown m_mouseOverDownState;

	// Themes
	std::shared_ptr<ColorTheme> m_backgroundTheme;
	std::shared_ptr<ColorTheme> m_borderTheme;
	std::string m_textTheme;
	std::string m_placeholderTextTheme;
	

};