#pragma once
#include "pch.h"
#include "Control.h"
#include "ThemeManager.h"
#include "ColorTheme.h"
#include "MouseOverDown.h"
#include "WindowManager.h"


#include <string>

class Button : public Control
{
public:
	Button(const std::shared_ptr<DeviceResources>& deviceResources, 
		   const std::shared_ptr<Layout>& parentLayout);

	Button(const std::shared_ptr<DeviceResources>& deviceResources, 
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

	void Click(std::function<void()> method) { ClickMethod = method; }

	void SetColorTheme(std::string name) {  m_colorTheme = std::static_pointer_cast<ColorTheme>(ThemeManager::GetTheme(name)); }
	void SetBorderTheme(std::string name) { m_borderTheme = std::static_pointer_cast<BorderTheme>(ThemeManager::GetTheme(name)); }

	std::shared_ptr<Layout> GetLayout() { return m_buttonLayout; }

private:
	void Resize();

	std::shared_ptr<Layout> m_buttonLayout;

	MouseOverDown m_mouseOverDownState;

	// Themes
	std::shared_ptr<ColorTheme>  m_colorTheme;
	std::shared_ptr<BorderTheme> m_borderTheme;

	std::function<void()> ClickMethod;
};