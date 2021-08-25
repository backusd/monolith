#include "Text.h"

using Microsoft::WRL::ComPtr;

Text::Text(const std::shared_ptr<DeviceResources>& deviceResources, 
		   const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan) :
	Control(deviceResources, parentLayout, row, column, rowSpan, columnSpan),
	m_textMetrics(DWRITE_TEXT_METRICS()),
	m_text(L""),
	m_screenTranslation(D2D1::Matrix3x2F::Translation(0.0f, 0.0f)),
	m_textLayout(nullptr)
{
	SetTextTheme(THEME_DEFAULT_TEXT);

	ZeroMemory(&m_textMetrics, sizeof(DWRITE_TEXT_METRICS));

	// Call text changed to initialize the text layout
	TextChanged();
}

bool Text::Render2D()
{
	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();

	context->SetTransform(m_screenTranslation * m_deviceResources->OrientationTransform2D());

	context->DrawTextLayout(
		D2D1::Point2F(0.0f, 0.0f),
		m_textLayout.Get(),
		m_textTheme->GetColorBrush(),
		D2D1_DRAW_TEXT_OPTIONS_CLIP			// <-- investigate these options, clipping is interesting
	);

	return true;
}

void Text::TextChanged()
{
	D2D1_RECT_F rect = GetParentRect();

	// Adjust the rect with the margins
	rect = D2D1::RectF(rect.left + m_marginLeft, rect.top + m_marginTop, rect.right - m_marginRight, rect.bottom - m_marginBottom);

	float maxWidth = rect.right - rect.left; // std::max(0, width of parent rect)
	float maxHeight = rect.bottom - rect.top; // std::max(0, height of parent rect)

	m_textLayout = m_textTheme->CreateTextLayout(m_text, maxWidth, maxHeight);

	ThrowIfFailed(m_textLayout->GetMetrics(&m_textMetrics));


	// Update Screen Translation ---
	m_screenTranslation = D2D1::Matrix3x2F::Translation(rect.left, rect.top);
}

void Text::Pop()
{ 
	if (m_text.size() > 0) 
		m_text.pop_back(); 
	
	TextChanged(); 
}
