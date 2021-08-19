#include "ThemeManager.h"

std::map<std::string, std::shared_ptr<Theme>> ThemeManager::m_themesMap;
bool ThemeManager::m_initialized = false;

void ThemeManager::Initialize(const std::shared_ptr<DeviceResources>& deviceResources)
{
	if (!m_initialized)
	{
		ID2D1DeviceContext6* context = deviceResources->D2DDeviceContext();

		// Create all themes for the application
		//
		// Default Button Color - White
		std::shared_ptr<ColorTheme> defaultColorTheme = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_BUTTON_COLOR, deviceResources);
		defaultColorTheme->SetAllColors(D2D1::ColorF(D2D1::ColorF::White));

		
		// Menu Button Color
		std::shared_ptr<ColorTheme> menuButtonColorTheme = ThemeManager::CreateTheme<ColorTheme>(THEME_MENU_BAR_BUTTON_COLOR, deviceResources);
		menuButtonColorTheme->SetColor(D2D1::ColorF(45.0f / 255.0f, 45.0f / 255.0f, 48.0f / 255.0f));
		menuButtonColorTheme->SetColorPointerOver(D2D1::ColorF(90.0f / 255.0f, 90.0f / 255.0f, 96.0f / 255.0f));
		menuButtonColorTheme->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Black));


		// Default drop down color
		std::shared_ptr<ColorTheme> defaultDropDownColorTheme = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_DROP_DOWN_COLOR, deviceResources);
		defaultDropDownColorTheme->SetAllColors(D2D1::ColorF(D2D1::ColorF::White));

		// Menu drop down Color
		std::shared_ptr<ColorTheme> menuDropDownColorTheme = ThemeManager::CreateTheme<ColorTheme>(THEME_MENU_DROP_DOWN_COLOR, deviceResources);
		menuDropDownColorTheme->SetColor(D2D1::ColorF(45.0f / 255.0f, 45.0f / 255.0f, 48.0f / 255.0f));



		// Default Text
		std::shared_ptr<TextTheme> defaultTextTheme = ThemeManager::CreateTheme<TextTheme>(THEME_DEFAULT_TEXT, deviceResources);

		// Menu Button Text
		std::shared_ptr<TextTheme> menuButtonTextTheme = ThemeManager::CreateTheme<TextTheme>(THEME_MENU_BAR_TEXT, deviceResources);
		menuButtonTextTheme->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		menuButtonTextTheme->SetFontFamily(FONT_FAMILY::SEGOE_UI);
		menuButtonTextTheme->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
		menuButtonTextTheme->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		menuButtonTextTheme->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		menuButtonTextTheme->SetFontSize(13.0f);
		menuButtonTextTheme->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
		menuButtonTextTheme->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		menuButtonTextTheme->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
		menuButtonTextTheme->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

		// Menu Drop Down Text
		std::shared_ptr<TextTheme> menuDropDownTextTheme = ThemeManager::CreateTheme<TextTheme>(THEME_MENU_DROP_DOWN_TEXT, deviceResources);
		menuDropDownTextTheme->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		menuDropDownTextTheme->SetFontFamily(FONT_FAMILY::SEGOE_UI);
		menuDropDownTextTheme->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
		menuDropDownTextTheme->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		menuDropDownTextTheme->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		menuDropDownTextTheme->SetFontSize(13.0f);
		menuDropDownTextTheme->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
		menuDropDownTextTheme->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		menuDropDownTextTheme->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
		menuDropDownTextTheme->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

		// Menu Drop Down Glyph
		std::shared_ptr<TextTheme> menuDropDownGlyphTheme = ThemeManager::CreateTheme<TextTheme>(THEME_MENU_DROP_DOWN_GLYPH, deviceResources);
		menuDropDownGlyphTheme->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		menuDropDownGlyphTheme->SetFontFamily(FONT_FAMILY::SEGOE_MDL2_ASSETS);
		menuDropDownGlyphTheme->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
		menuDropDownGlyphTheme->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		menuDropDownGlyphTheme->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		menuDropDownGlyphTheme->SetFontSize(13.0f);
		menuDropDownGlyphTheme->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
		menuDropDownGlyphTheme->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
		menuDropDownGlyphTheme->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
		menuDropDownGlyphTheme->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);
		

		m_initialized = true;
	}
}

std::shared_ptr<Theme> ThemeManager::GetTheme(std::string name)
{
	if (m_themesMap.find(name) != m_themesMap.end())
		return m_themesMap[name];

	return nullptr;
}

void ThemeManager::AddTheme(std::shared_ptr<Theme> theme)
{
	m_themesMap.insert(std::pair(theme->Name(), theme));
}
