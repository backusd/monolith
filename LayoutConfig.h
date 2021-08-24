#pragma once
#include "pch.h"
#include "Layout.h"
#include "ContentWindow.h"
#include "Control.h"
#include "Button.h"
#include "ColorTheme.h"
#include "DropDown.h"
#include "ThemeDefines.h"
#include "Text.h"
#include "SimulationRenderer.h"
#include "WindowManager.h"

namespace LayoutConfiguration
{
	// Configure the layout for the main application window
	void ConfigureMainWindow(const std::shared_ptr<ContentWindow>& window);

	void CreateMenuBar(const std::shared_ptr<Layout>& mainLayout);
	void CreateFileDropDown(const std::shared_ptr<Layout>& menuBarLayout);
	void CreateSimulationDropDown(const std::shared_ptr<Layout>& menuBarLayout);
	void CreateViewDropDown(const std::shared_ptr<Layout>& menuBarLayout);

	void CreateQuickBar(const std::shared_ptr<Layout>& mainLayout);

	void CreateRightPane(const std::shared_ptr<Layout>& mainLayout);
}