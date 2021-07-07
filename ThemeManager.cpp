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
		// Default Color - White
		std::shared_ptr<ColorTheme> defaultColorTheme = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_BUTTON_COLOR, deviceResources);
		defaultColorTheme->SetAllColors(D2D1::ColorF(D2D1::ColorF::White));

		// Menu Button Color
		std::shared_ptr<ColorTheme> menuButtonColorTheme = ThemeManager::CreateTheme<ColorTheme>(THEME_MENU_BAR_BUTTON_COLOR, deviceResources);
		menuButtonColorTheme->SetColor(D2D1::ColorF(D2D1::ColorF::Green));
		menuButtonColorTheme->SetColorPointerOver(D2D1::ColorF(D2D1::ColorF::Red));
		menuButtonColorTheme->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Blue));

		// Default Text
		std::shared_ptr<TextTheme> defaultTextTheme = ThemeManager::CreateTheme<TextTheme>(THEME_DEFAULT_TEXT, deviceResources);

		// Menu Button Text
		std::shared_ptr<TextTheme> menuButtonTextTheme = ThemeManager::CreateTheme<TextTheme>(THEME_MENU_BAR_TEXT, deviceResources);
		menuButtonTextTheme->SetColor(D2D1::ColorF(D2D1::ColorF::White));
		menuButtonTextTheme->SetFontFamily(FONT_FAMILY::SEGOE_UI);
		menuButtonTextTheme->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
		menuButtonTextTheme->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
		menuButtonTextTheme->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
		menuButtonTextTheme->SetFontSize(12.0f);
		menuButtonTextTheme->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
		menuButtonTextTheme->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_NEAR);
		menuButtonTextTheme->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
		menuButtonTextTheme->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

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

void ThemeManager::Destroy()
{
	for (const auto& [key, value] : m_themesMap)
	{
		value->Destroy();
	}
}