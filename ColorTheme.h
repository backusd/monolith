#pragma once
#include "pch.h"
#include "Theme.h"


class ColorTheme : public Theme
{
public:
	ColorTheme(std::string themeName);

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> Color() { return m_color; }
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> ColorPointerOver() { return m_colorPointerOver; }
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> ColorPointerDown() { return m_colorPointerDown; }

	void SetAllColors(D2D1_COLOR_F color, ID2D1HwndRenderTarget* renderTarget);
	void SetColor(D2D1_COLOR_F color, ID2D1HwndRenderTarget* renderTarget);
	void SetColorPointerOver(D2D1_COLOR_F color, ID2D1HwndRenderTarget* renderTarget);
	void SetColorPointerDown(D2D1_COLOR_F color, ID2D1HwndRenderTarget* renderTarget);

	void Destroy() override;

private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_color;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_colorPointerOver;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_colorPointerDown;
};