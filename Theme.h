#pragma once
#include "pch.h"


#include <string>




class Theme
{
public:
	Theme(std::string themeName);

	std::string Name() { return m_themeName; }

	virtual void Destroy() = 0;

private:
	std::string m_themeName;

};