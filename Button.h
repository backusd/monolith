#pragma once
#include "pch.h"
#include "Control.h"
#include "ThemeManager.h"
#include "ColorTheme.h"

#include <string>

enum class ButtonMouseState
{
	NONE,
	MOUSE_OVER,
	MOUSE_DOWN
};

class Button : public Control
{
public:
	Button(const std::shared_ptr<DeviceResources>& deviceResources, 
		   const std::shared_ptr<Layout>& parentLayout);

	Button(const std::shared_ptr<DeviceResources>& deviceResources, 
		   const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan);

	// To clear the button's contents, just pass along to the button's layout
	void ClearContents() override { m_buttonLayout->ClearContents(); }

	void OnPaint(ID2D1HwndRenderTarget* renderTarget) override;
	void OnLayoutResize() override { Resize(); }
	void OnMarginChanged() override { Resize(); }
	bool MouseIsOver(int x, int y) override;
	std::shared_ptr<OnMessageResult> OnLButtonDown(std::shared_ptr<MouseState> mouseState) override;
	std::shared_ptr<OnMessageResult> OnLButtonUp(std::shared_ptr<MouseState> mouseState) override;
	std::shared_ptr<OnMessageResult> OnMouseMove(std::shared_ptr<MouseState> mouseState) override;
	std::shared_ptr<OnMessageResult> OnMouseLeave() override;

	void SetColorTheme(std::string name) { m_colorTheme = std::static_pointer_cast<ColorTheme>(ThemeManager::GetTheme(name)); }

	std::shared_ptr<Layout> GetLayout() { return m_buttonLayout; }

private:
	D2D1_RECT_F GetParentRect() { return m_parentLayout->GetRect(m_row, m_column, m_rowSpan, m_columnSpan); }
	void Resize();
	void UpdateButtonMouseState(ButtonMouseState newState, std::shared_ptr<OnMessageResult> result);

	std::shared_ptr<Layout> m_buttonLayout;

	ButtonMouseState m_buttonMouseState;

	// Themes
	std::shared_ptr<ColorTheme> m_colorTheme;
	//std::string m_borderTheme;
};