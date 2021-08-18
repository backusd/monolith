#include "Text.h"

using Microsoft::WRL::ComPtr;

Text::Text(const std::shared_ptr<DeviceResources>& deviceResources, 
		   const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan) :
	Control(deviceResources, parentLayout, row, column, rowSpan, columnSpan),
	m_textMetrics(DWRITE_TEXT_METRICS()),
	m_text(L"")
{
	SetTextTheme(THEME_DEFAULT_TEXT);

	ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));
}

bool Text::Render2D()
{
	D2D1_RECT_F rect = GetParentRect();

	float maxWidth = rect.right - rect.left; // std::max(0, width of parent rect)
	float maxHeight = rect.bottom - rect.top; // std::max(0, height of parent rect)

	ComPtr<IDWriteTextLayout4> textLayout = m_textTheme->CreateTextLayout(m_text, maxWidth, maxHeight);

	ThrowIfFailed(textLayout->GetMetrics(&m_textMetrics));


	// Update Screen Translation ---
	D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(rect.left, rect.top);


	// Render ---
	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();

	context->SetTransform(screenTranslation * m_deviceResources->OrientationTransform2D());

	context->DrawTextLayout(
		D2D1::Point2F(0.0f, 0.0f),
		textLayout.Get(),
		m_textTheme->GetColorBrush(),
		D2D1_DRAW_TEXT_OPTIONS_CLIP			// <-- investigate these options, clipping is interesting
	);

	return true;
}