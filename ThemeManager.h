#pragma once
#include "pch.h"
#include "Theme.h"
#include "ColorTheme.h"
#include "TextTheme.h"
#include "ThemeDefines.h"
#include "DeviceResources.h"

#include <map>
#include <string>
#include <memory>


class ThemeManager
{
public:
	static void Initialize(const std::shared_ptr<DeviceResources>& deviceResources);

	static std::shared_ptr<Theme> GetTheme(std::string name);
	static void AddTheme(std::shared_ptr<Theme> theme);

	template<typename T>
	static std::shared_ptr<T> CreateTheme(std::string name, const std::shared_ptr<DeviceResources>& deviceResources);

private:
	ThemeManager() {}

	static std::map<std::string, std::shared_ptr<Theme>> m_themesMap;
	static bool m_initialized;

};



template<typename T>
std::shared_ptr<T> ThemeManager::CreateTheme(std::string name, const std::shared_ptr<DeviceResources>& deviceResources)
{
	std::shared_ptr<T> theme = std::make_shared<T>(name, deviceResources);

	AddTheme(std::static_pointer_cast<Theme>(theme));

	return theme;
}