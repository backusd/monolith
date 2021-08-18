#pragma once
#include "pch.h"
#include "Theme.h"
#include "ColorTheme.h"
#include "FontFamily.h"

#include <memory>
#include <string>

class TextTheme : public Theme
{
public:
	TextTheme(std::string themeName, const std::shared_ptr<DeviceResources>& deviceResources);

	// Color
	ID2D1SolidColorBrush* GetColorBrush() const { return m_color.Get(); }
	constexpr const wchar_t* GetFontFamily() const { return FontFamily::ToWString(m_fontFamily).c_str(); }
	IDWriteFontCollection* GetFontCollection() const { return m_fontCollection.Get(); }
	DWRITE_FONT_WEIGHT GetFontWeight() const { return m_fontWeight; }
	DWRITE_FONT_STYLE GetFontStyle() const { return m_fontStyle; }
	DWRITE_FONT_STRETCH GetFontStretch() const { return m_fontStretch; }
	float GetFontSize() const { return m_fontSize; }
	constexpr const wchar_t* GetLocale() const { return m_locale.c_str(); }
	DWRITE_TEXT_ALIGNMENT GetTextAlignment() const { return m_textAlignment; }
	DWRITE_PARAGRAPH_ALIGNMENT GetParagraphAlignment() const { return m_paragraphAlignment; }
	DWRITE_WORD_WRAPPING GetWordWrapping() const { return m_wordWrapping; }
	DWRITE_TRIMMING GetTrimming() const { return m_trimming; }


	void SetColor(D2D1_COLOR_F color);
	void SetFontFamily(FONT_FAMILY fontFamily);
	void SetFontCollection(); // Not sure how to do this - m_fontCollection is okay to be nullptr
	void SetFontWeight(DWRITE_FONT_WEIGHT fontWeight);
	void SetFontStyle(DWRITE_FONT_STYLE fontStyle);
	void SetFontStretch(DWRITE_FONT_STRETCH fontStretch);
	void SetFontSize(float fontSize);
	void SetLocale(); // Not sure how to do this - m_locale is "en-US" for all known cases
	void SetTextAlignment(DWRITE_TEXT_ALIGNMENT textAlignment);
	void SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT paragraphAlignment);
	void SetWordWrapping(DWRITE_WORD_WRAPPING wordWrapping);
	void SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY granularity);

	Microsoft::WRL::ComPtr<IDWriteTextLayout4> CreateTextLayout(std::wstring text, float maxWidth, float maxHeight);

private:
	void UpdateTextFormat();

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush>	m_color;
	FONT_FAMILY										m_fontFamily;
	Microsoft::WRL::ComPtr<IDWriteFontCollection>	m_fontCollection;
	DWRITE_FONT_WEIGHT								m_fontWeight;
	DWRITE_FONT_STYLE								m_fontStyle;
	DWRITE_FONT_STRETCH								m_fontStretch;
	float											m_fontSize;
	std::wstring									m_locale;
	DWRITE_TEXT_ALIGNMENT							m_textAlignment;
	DWRITE_PARAGRAPH_ALIGNMENT						m_paragraphAlignment;
	DWRITE_WORD_WRAPPING							m_wordWrapping;
	DWRITE_TRIMMING									m_trimming;

	Microsoft::WRL::ComPtr<IDWriteTextFormat3>		m_textFormat;

};