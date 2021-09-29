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


	// Default ListView Item format method
	template <typename T>
	static std::shared_ptr<Layout> DefaultListViewFormatAddedItem(std::shared_ptr<T> item, bool highlighted)
	{
		// just return an empty layout
		return std::make_shared<Layout>(nullptr, 0.0f, 0.0f, 0.0f, 0.0f);
	}

	// Default ListView Item format method
	template <typename T>
	static void DefaultListViewValueChangedUpdateLayoutMethod(std::shared_ptr<T> item, std::shared_ptr<Layout> layout) {}

	// Default ListView Item click method
	template <typename T>
	static void DefaultListViewItemClickMethod(std::shared_ptr<T> item) {}

	// Default ListView Item highlight methods
	template <typename T>
	static void DefaultListViewHightlightItemLayoutMethod(std::shared_ptr<Layout> item) {}
	template <typename T>
	static void DefaultListViewUnhightlightItemLayoutMethod(std::shared_ptr<Layout> item) {}

	// Slider methods
	static void DefaultSliderValueChangedMethod(float value) {}

private:
	// Disallow creation of a WindowManager object
	WindowManager() {}

	//std::vector<WindowBase*> m_windows;
	static std::vector<std::shared_ptr<WindowBase>> m_windows;

};