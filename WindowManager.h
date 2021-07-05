#pragma once
#include "pch.h"

#include <vector>
#include <memory>

class WindowBase;

class WindowManager
{
public:
	static void AddWindow(std::shared_ptr<WindowBase> window) { m_windows.push_back(window); }
	static void DestroyWindow(WindowBase*);

private:
	// Disallow creation of a WindowManager object
	WindowManager() {}

	//std::vector<WindowBase*> m_windows;
	static std::vector<std::shared_ptr<WindowBase>> m_windows;

};