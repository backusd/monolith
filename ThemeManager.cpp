#include "ThemeManager.h"

std::map<std::string, std::shared_ptr<Theme>> ThemeManager::m_themesMap;
bool ThemeManager::m_initialized = false;

void ThemeManager::Initialize(ID2D1DeviceContext* context)
{
	if (!m_initialized)
	{
		// Create all themes for the application
		std::shared_ptr<ColorTheme> defaultColorTheme = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_BUTTON_COLOR);
		defaultColorTheme->SetAllColors(D2D1::ColorF(D2D1::ColorF::White), context);

		std::shared_ptr<ColorTheme> menuButtonColorTheme = ThemeManager::CreateTheme<ColorTheme>(THEME_MENU_BAR_BUTTON_COLOR);
		menuButtonColorTheme->SetColor(D2D1::ColorF(D2D1::ColorF::Green), context);
		menuButtonColorTheme->SetColorPointerOver(D2D1::ColorF(D2D1::ColorF::Red), context);
		menuButtonColorTheme->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Blue), context);

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