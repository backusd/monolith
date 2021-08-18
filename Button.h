#pragma once
#include "pch.h"
#include "Control.h"
#include "ThemeManager.h"
#include "ColorTheme.h"
#include "MouseOverDown.h"
#include "ButtonClickMethods.h"


#include <string>

class Button : public Control
{
public:
	Button(const std::shared_ptr<DeviceResources>& deviceResources, 
		   const std::shared_ptr<Layout>& parentLayout);

	Button(const std::shared_ptr<DeviceResources>& deviceResources, 
		   const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan);

	// To clear the button's contents, just pass along to the button's layout
	void ClearContents() override { m_buttonLayout->ClearContents(); }


	bool Render2D() override;
	// void OnPaint() override;


	void OnLayoutResize() override { Resize(); }
	void OnMarginChanged() override { Resize(); }
	bool MouseIsOver(int x, int y) override;

	OnMessageResult OnLButtonDown(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnLButtonUp(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseMove(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseLeave() override;


	void SetColorTheme(std::string name) { m_colorTheme = std::static_pointer_cast<ColorTheme>(ThemeManager::GetTheme(name)); }

	std::shared_ptr<Layout> GetLayout() { return m_buttonLayout; }

private:
	void Resize();

	std::shared_ptr<Layout> m_buttonLayout;

	MouseOverDown m_mouseOverDownState;

	// Themes
	std::shared_ptr<ColorTheme> m_colorTheme;
	//std::string m_borderTheme;



	std::function<void()> ClickMethod;
};