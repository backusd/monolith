#include "LineTheme.h"

LineTheme::LineTheme(std::string themeName, const std::shared_ptr<DeviceResources>& deviceResources) :
	Theme(themeName, deviceResources),
	m_color(nullptr),
	m_strokeStyle(nullptr),
	m_strokeWidth(1.0f)
{
}

ID2D1StrokeStyle* LineTheme::GetStrokeStyle() const
{
	if (m_strokeStyle == nullptr)
		return (ID2D1StrokeStyle*)0; // this will be a solid stroke

	return m_strokeStyle.Get();
}

void LineTheme::SetColor(D2D1_COLOR_F color)
{
	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();
	context->CreateSolidColorBrush(color, m_color.ReleaseAndGetAddressOf());
}

/* Example Usage ==================================================

float dashes[] = { 1.0f, 2.0f, 2.0f, 3.0f, 2.0f, 2.0f };
tabbedPanePaneBorder->SetStrokeStyle(
		dashes,
		ARRAYSIZE(dashes),
		D2D1_CAP_STYLE_FLAT,
		D2D1_CAP_STYLE_FLAT,
		D2D1_CAP_STYLE_ROUND,
		D2D1_LINE_JOIN_MITER,
		10.0f,
		D2D1_DASH_STYLE_CUSTOM,
		0.0f
	);
*/
void LineTheme::SetStrokeStyle(
	const float* dashes,
	UINT32 dashesCount,
	D2D1_CAP_STYLE startCap,
	D2D1_CAP_STYLE endCap,
	D2D1_CAP_STYLE dashCap,
	D2D1_LINE_JOIN lineJoin,
	float miterLimit,
	D2D1_DASH_STYLE dashStyle,
	float dashOffset)
{
	// Dash array for dashStyle D2D1_DASH_STYLE_CUSTOM
	// float dashes[] = { 1.0f, 2.0f, 2.0f, 3.0f, 2.0f, 2.0f };

	ID2D1Factory7* factory = m_deviceResources->D2DFactory();
	factory->CreateStrokeStyle(
		D2D1::StrokeStyleProperties(
			startCap, endCap, dashCap,
			lineJoin, miterLimit,
			dashStyle, dashOffset
		),
		dashes,
		dashesCount,
		&m_strokeStyle
	);
}