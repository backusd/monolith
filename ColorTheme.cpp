#include "ColorTheme.h"

ColorTheme::ColorTheme(std::string themeName) : 
	Theme(themeName),
	m_color(nullptr),
	m_colorPointerOver(nullptr),
	m_colorPointerDown(nullptr)
{
}


void ColorTheme::SetAllColors(D2D1_COLOR_F color, ID2D1HwndRenderTarget* renderTarget)
{
	renderTarget->CreateSolidColorBrush(color, m_color.ReleaseAndGetAddressOf());
	renderTarget->CreateSolidColorBrush(color, m_colorPointerOver.ReleaseAndGetAddressOf());
	renderTarget->CreateSolidColorBrush(color, m_colorPointerDown.ReleaseAndGetAddressOf());
}

void ColorTheme::SetColor(D2D1_COLOR_F color, ID2D1HwndRenderTarget* renderTarget)
{
	renderTarget->CreateSolidColorBrush(color, m_color.ReleaseAndGetAddressOf());
}

void ColorTheme::SetColorPointerOver(D2D1_COLOR_F color, ID2D1HwndRenderTarget* renderTarget)
{
	renderTarget->CreateSolidColorBrush(color, m_colorPointerOver.ReleaseAndGetAddressOf());
}

void ColorTheme::SetColorPointerDown(D2D1_COLOR_F color, ID2D1HwndRenderTarget* renderTarget)
{
	renderTarget->CreateSolidColorBrush(color, m_colorPointerDown.ReleaseAndGetAddressOf());
}

void ColorTheme::Destroy()
{
	m_color = nullptr;
	m_colorPointerOver = nullptr;
	m_colorPointerDown = nullptr;
}