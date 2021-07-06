#pragma once
#include "pch.h"
#include "Theme.h"


class ColorTheme : public Theme
{
public:
	ColorTheme(std::string themeName);

	ID2D1SolidColorBrush* Color() const { return m_color.Get(); }
	ID2D1SolidColorBrush* ColorPointerOver() const { return m_colorPointerOver.Get(); }
	ID2D1SolidColorBrush* ColorPointerDown() const { return m_colorPointerDown.Get(); }

	// Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> Color() { return m_color; }
	// Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> ColorPointerOver() { return m_colorPointerOver; }
	// Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> ColorPointerDown() { return m_colorPointerDown; }

	/*
	void SetAllColors(D2D1_COLOR_F color, ID2D1HwndRenderTarget* renderTarget);
	void SetColor(D2D1_COLOR_F color, ID2D1HwndRenderTarget* renderTarget);
	void SetColorPointerOver(D2D1_COLOR_F color, ID2D1HwndRenderTarget* renderTarget);
	void SetColorPointerDown(D2D1_COLOR_F color, ID2D1HwndRenderTarget* renderTarget);
	*/

	void SetAllColors(D2D1_COLOR_F color, ID2D1DeviceContext* context);
	void SetColor(D2D1_COLOR_F color, ID2D1DeviceContext* context);
	void SetColorPointerOver(D2D1_COLOR_F color, ID2D1DeviceContext* context);
	void SetColorPointerDown(D2D1_COLOR_F color, ID2D1DeviceContext* context);

	void Destroy() override;

private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_color;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_colorPointerOver;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_colorPointerDown;
};