#pragma once
#include "pch.h"
#include "Control.h"
#include "MouseOverDown.h"
#include "TextInput.h"
#include "ThemeManager.h"

#include <string>
#include <stdexcept>


class Slider : public Control
{
public:
	Slider(const std::shared_ptr<DeviceResources>& deviceResources,
		const std::shared_ptr<Layout>& parentLayout);

	Slider(const std::shared_ptr<DeviceResources>& deviceResources,
		const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan);

	// To release the button's layout, just pass along to the button's layout
	void ReleaseLayout() override;


	bool Render2D() override;


	void OnLayoutResize() override { Resize(); }
	void OnMarginChanged() override { Resize(); }

	bool MouseIsOver(int x, int y) override;
	bool MouseIsOverSlider(int x, int y);

	OnMessageResult OnLButtonDown(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnLButtonUp(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseMove(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseLeave() override;

	OnMessageResult OnKeyDown(unsigned char keycode) override;
	OnMessageResult OnKeyUp(unsigned char keycode) override;
	OnMessageResult OnChar(char key) override;


	void SetLineTheme(std::string name) { m_lineTheme = std::static_pointer_cast<LineTheme>(ThemeManager::GetTheme(name)); }
	void SetRectColorTheme(std::string name) { m_rectColorTheme = std::static_pointer_cast<ColorTheme>(ThemeManager::GetTheme(name)); }

	std::shared_ptr<Layout> GetLayout() { return m_layout; }

	void SetMin(float min);
	void SetMax(float max);

private:
	void Resize();

	std::shared_ptr<Layout> m_layout;

	// state for mouse relative to the rect
	MouseOverDown m_rectMouseOverDownState;

	// Theme for the line
	std::shared_ptr<LineTheme>  m_lineTheme;
	std::shared_ptr<ColorTheme> m_rectColorTheme;

	// Text Input to show/change value
	std::shared_ptr<TextInput> m_textInput;

	// Rectangle for the slider
	D2D1_RECT_F m_sliderRect;

	// min / max / value
	float m_sliderMin;
	float m_sliderMax;
	float m_sliderValue;

	float m_rectWidth;
};