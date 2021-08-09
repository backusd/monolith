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


	/*
	// Constructor ---

	DWRITE_TEXT_METRICS textMetrics;
	ZeroMemory(&textMetrics, sizeof(DWRITE_TEXT_METRICS));

	DWRITE_TRIMMING trimming;
	ZeroMemory(&trimming, sizeof(DWRITE_TRIMMING));
	trimming.granularity = DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE;

	// Can be controlled via a Theme ---

	ComPtr<ID2D1SolidColorBrush> blackBrush;
	const D2D1_COLOR_F color = D2D1::ColorF(0.0f, 0.0f, 0.0f);
	ThrowIfFailed(
		m_deviceResources->D2DDeviceContext()->CreateSolidColorBrush(color, blackBrush.ReleaseAndGetAddressOf())
	);

	// Update Text Format ---
	std::wstring                  fontFamily = L"Segoe UI";
	ComPtr<IDWriteFontCollection> fontCollection = nullptr;
	DWRITE_FONT_WEIGHT			  fontWeight = DWRITE_FONT_WEIGHT_LIGHT;
	DWRITE_FONT_STYLE			  fontStyle = DWRITE_FONT_STYLE_NORMAL;
	DWRITE_FONT_STRETCH			  fontStretch = DWRITE_FONT_STRETCH_NORMAL;
	float						  fontSize = 20.0f;
	std::wstring				  locale = L"en-US";

	ComPtr<IDWriteTextFormat> textFormat = nullptr;
	ThrowIfFailed(
		m_deviceResources->DWriteFactory()->CreateTextFormat(
			fontFamily.c_str(),
			fontCollection.Get(),
			fontWeight,
			fontStyle,
			fontStretch,
			fontSize,
			locale.c_str(),
			textFormat.ReleaseAndGetAddressOf()
		)
	);

	ComPtr<IDWriteTextFormat3> textFormat3;
	ThrowIfFailed(textFormat.As(&textFormat3));

	DWRITE_TEXT_ALIGNMENT textAlignment = DWRITE_TEXT_ALIGNMENT_LEADING;
	ThrowIfFailed(textFormat3->SetTextAlignment(textAlignment));

	DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment = DWRITE_PARAGRAPH_ALIGNMENT_NEAR;
	ThrowIfFailed(textFormat3->SetParagraphAlignment(paragraphAlignment));

	DWRITE_WORD_WRAPPING wordWrapping = DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP;
	ThrowIfFailed(textFormat3->SetWordWrapping(wordWrapping));

	ThrowIfFailed(textFormat3->SetTrimming(&trimming, nullptr));

	// Update Text Layout ---
	float maxWidth = 800.0f; // std::max(0, width of parent rect)
	float maxHeight = 800.0f; // std::max(0, height of parent rect)

	std::wstring text = L"Suck Me";
	ComPtr<IDWriteTextLayout> textLayout = nullptr;
	ThrowIfFailed(
		m_deviceResources->DWriteFactory()->CreateTextLayout(
			text.c_str(),
			(uint32_t)text.length(),
			textFormat3.Get(),
			maxWidth,
			maxHeight,
			textLayout.ReleaseAndGetAddressOf()
		)
	);

	ComPtr<IDWriteTextLayout4> textLayout4;
	ThrowIfFailed(textLayout.As(&textLayout4));

	ThrowIfFailed(textLayout4->GetMetrics(&textMetrics));


	// Update Screen Translation ---
	D2D1::Matrix3x2F screenTranslation = D2D1::Matrix3x2F::Translation(200.0f, 200.0f);


	// Render ---
	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();

	context->SetTransform(screenTranslation * m_deviceResources->OrientationTransform2D());

	context->DrawTextLayout(
		D2D1::Point2F(0.0f, 0.0f),
		textLayout4.Get(),
		blackBrush.Get()
	);
	*/

}