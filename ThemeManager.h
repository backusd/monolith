#pragma once
#include "pch.h"
#include "Theme.h"
#include "ColorTheme.h"
#include "ThemeDefines.h"

#include <map>
#include <string>
#include <memory>


class ThemeManager
{
public:
	static void Initialize(ID2D1DeviceContext* context);

	static std::shared_ptr<Theme> GetTheme(std::string name);
	static void AddTheme(std::shared_ptr<Theme> theme);

	static void Destroy();

	template<typename T>
	static std::shared_ptr<T> CreateTheme(std::string name);

private:
	ThemeManager() {}

	static std::map<std::string, std::shared_ptr<Theme>> m_themesMap;
	static bool m_initialized;
};



template<typename T>
std::shared_ptr<T> ThemeManager::CreateTheme(std::string name)
{
	std::shared_ptr<T> theme = std::make_shared<T>(name);

	AddTheme(theme);

	return theme;
}