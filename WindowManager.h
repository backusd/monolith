#pragma once
#include "pch.h"

#include "Layout.h"
#include "WindowBase.h"

#include <functional>
#include <memory>
#include <optional>
#include <vector>

class WindowManager
{
public:
	static void AddWindow(std::shared_ptr<WindowBase> window) { m_windows.push_back(window); }
	static void DestroyWindows();

	static void UpdateRenderPresent();

	static std::optional<int> ProcessMessages();


	// Button click methods
	static void DefaultClickMethod() {}
	static void NewSimulationButtonClick();

	// Default ListView Item format method
	template <typename T>
	static std::shared_ptr<Layout> DefaultListViewFormatAddedItem(std::shared_ptr<T> item)
	{
		// just return an empty layout
		return std::make_shared<Layout>(nullptr, 0.0f, 0.0f, 0.0f, 0.0f);
	}

	// Slider methods
	static void DefaultSliderValueChangedMethod(float value) {}

private:
	// Disallow creation of a WindowManager object
	WindowManager() {}

	//std::vector<WindowBase*> m_windows;
	static std::vector<std::shared_ptr<WindowBase>> m_windows;

};