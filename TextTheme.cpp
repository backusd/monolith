#include "TextTheme.h"

using Microsoft::WRL::ComPtr;

TextTheme::TextTheme(std::string themeName, const std::shared_ptr<DeviceResources>& deviceResources) :
	Theme(themeName, deviceResources),
	m_color(nullptr),
	m_fontFamily(FONT_FAMILY::SEGOE_UI),
	m_fontCollection(nullptr),
	m_fontWeight(DWRITE_FONT_WEIGHT_LIGHT),
	m_fontStyle(DWRITE_FONT_STYLE_NORMAL),
	m_fontStretch(DWRITE_FONT_STRETCH_NORMAL),
	m_fontSize(12.0f),
	m_locale(L"en-US"),
	m_textAlignment(DWRITE_TEXT_ALIGNMENT_LEADING),
	m_paragraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT_NEAR),
	m_wordWrapping(DWRITE_WORD_WRAPPING_NO_WRAP),
	m_trimming(DWRITE_TRIMMING())
{
	// Default text color is white
	ID2D1DeviceContext6* context = m_deviceResources->D2DDeviceContext();
	context->CreateSolidColorBrush(D2D1::ColorF(1.0f, 1.0f, 1.0f), m_color.ReleaseAndGetAddressOf());

	// Create the trimming structure
	ZeroMemory(&m_trimming, sizeof(DWRITE_TRIMMING));
	m_trimming.granularity = DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE;

	// Create the text format
	UpdateTextFormat();
}

void TextTheme::SetColor(D2D1_COLOR_F color)
{
	m_deviceResources->D2DDeviceContext()->CreateSolidColorBrush(color, m_color.ReleaseAndGetAddressOf());
	UpdateTextFormat();
}
void TextTheme::SetFontFamily(FONT_FAMILY fontFamily)
{
	m_fontFamily = fontFamily;
	UpdateTextFormat();
}
void TextTheme::SetFontCollection() // Not sure how to do this - m_fontCollection is okay to be nullptr
{
	m_fontCollection = nullptr;
}
void TextTheme::SetFontWeight(DWRITE_FONT_WEIGHT fontWeight)
{
	m_fontWeight = fontWeight;
	UpdateTextFormat();
}
void TextTheme::SetFontStyle(DWRITE_FONT_STYLE fontStyle)
{
	m_fontStyle = fontStyle;
	UpdateTextFormat();
}
void TextTheme::SetFontStretch(DWRITE_FONT_STRETCH fontStretch)
{
	m_fontStretch = fontStretch;
	UpdateTextFormat();
}
void TextTheme::SetFontSize(float fontSize)
{
	m_fontSize = fontSize;
	UpdateTextFormat();
}
void TextTheme::SetLocale() // Not sure how to do this - m_locale is "en-US" for all known cases
{
	m_locale = L"en-US";
	UpdateTextFormat();
}
void TextTheme::SetTextAlignment(DWRITE_TEXT_ALIGNMENT textAlignment)
{
	m_textAlignment = textAlignment;
	UpdateTextFormat();
}
void TextTheme::SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment)
{
	m_paragraphAlignment = paragraphAlignment;
	UpdateTextFormat();
}
void TextTheme::SetWordWrapping(DWRITE_WORD_WRAPPING wordWrapping) 
{
	m_wordWrapping = wordWrapping;
	UpdateTextFormat();
}
void TextTheme::SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY granularity)
{
	m_trimming.granularity = granularity;
	UpdateTextFormat();
}


void TextTheme::UpdateTextFormat()
{
	ComPtr<IDWriteTextFormat> textFormat = nullptr;
	ThrowIfFailed(
		m_deviceResources->DWriteFactory()->CreateTextFormat(
			FontFamily::ToWString(m_fontFamily).c_str(),
			m_fontCollection.Get(),
			m_fontWeight,
			m_fontStyle,
			m_fontStretch,
			m_fontSize,
			m_locale.c_str(),
			textFormat.ReleaseAndGetAddressOf()
		)
	);

	ThrowIfFailed(textFormat.As(&m_textFormat));

	ThrowIfFailed(m_textFormat->SetTextAlignment(m_textAlignment));
	ThrowIfFailed(m_textFormat->SetParagraphAlignment(m_paragraphAlignment));
	ThrowIfFailed(m_textFormat->SetWordWrapping(m_wordWrapping));
	ThrowIfFailed(m_textFormat->SetTrimming(&m_trimming, nullptr));
}

ComPtr<IDWriteTextLayout4> TextTheme::CreateTextLayout(std::wstring text, float maxWidth, float maxHeight)
{
	ComPtr<IDWriteTextLayout> textLayout;
	ThrowIfFailed(
		m_deviceResources->DWriteFactory()->CreateTextLayout(
			text.c_str(),
			(uint32_t)text.length(),
			m_textFormat.Get(),
			maxWidth,
			maxHeight,
			textLayout.ReleaseAndGetAddressOf()
		)
	);

	ComPtr<IDWriteTextLayout4> textLayout4;
	ThrowIfFailed(textLayout.As(&textLayout4));

	return textLayout4;
}
