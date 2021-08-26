#include "BorderTheme.h"

BorderTheme::BorderTheme(std::string themeName, const std::shared_ptr<DeviceResources>& deviceResources) :
	Theme(themeName, deviceResources),
	m_color(nullptr),
	m_colorPointerOver(nullptr),
	m_colorPointerDown(nullptr),
	m_strokeWidth(0.0f)
{
}

ID2D1SolidColorBrush* BorderTheme::GetBrush(MouseOverDown state) const
{
	switch (state)
	{
	case MouseOverDown::NONE:       return m_color.Get();
	case MouseOverDown::MOUSE_OVER: return m_colorPointerOver.Get();
	case MouseOverDown::MOUSE_DOWN: return m_colorPointerDown.Get();
	}

	return nullptr;
}

void BorderTheme::SetAllColors(D2D1_COLOR_F color)
{
	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();
	context->CreateSolidColorBrush(color, m_color.ReleaseAndGetAddressOf());
	context->CreateSolidColorBrush(color, m_colorPointerOver.ReleaseAndGetAddressOf());
	context->CreateSolidColorBrush(color, m_colorPointerDown.ReleaseAndGetAddressOf());
}

void BorderTheme::SetColor(D2D1_COLOR_F color)
{
	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();
	context->CreateSolidColorBrush(color, m_color.ReleaseAndGetAddressOf());
}

void BorderTheme::SetColorPointerOver(D2D1_COLOR_F color)
{
	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();
	context->CreateSolidColorBrush(color, m_colorPointerOver.ReleaseAndGetAddressOf());
}

void BorderTheme::SetColorPointerDown(D2D1_COLOR_F color)
{
	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();
	context->CreateSolidColorBrush(color, m_colorPointerDown.ReleaseAndGetAddressOf());
}