#pragma once
#include "pch.h"
#include "DeviceResources.h"


#include <string>
#include <memory>




class Theme
{
public:
	Theme(std::string themeName, const std::shared_ptr<DeviceResources>& deviceResources);
	virtual ~Theme() {};

	std::string Name() { return m_themeName; }

	virtual void Destroy() = 0;

protected:
	std::string m_themeName;
	std::shared_ptr<DeviceResources> m_deviceResources;

};