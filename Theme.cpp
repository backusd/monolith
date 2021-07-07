#include "Theme.h"

Theme::Theme(std::string themeName, const std::shared_ptr<DeviceResources>& deviceResources) :
	m_themeName(themeName),
	m_deviceResources(deviceResources)
{
}