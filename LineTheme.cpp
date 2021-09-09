#include "LineTheme.h"

LineTheme::LineTheme(std::string themeName, const std::shared_ptr<DeviceResources>& deviceResources) :
	Theme(themeName, deviceResources),
	m_color(nullptr),
	m_strokeWidth(1.0f)
{
}

void LineTheme::SetColor(D2D1_COLOR_F color)
{
	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();
	context->CreateSolidColorBrush(color, m_color.ReleaseAndGetAddressOf());
}
