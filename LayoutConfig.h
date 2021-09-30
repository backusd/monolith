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

#include <functional>

namespace LayoutConfiguration
{
	// Configure the layout for the main application window
	void ConfigureMainWindow(const std::shared_ptr<ContentWindow>& window);

	void CreateMenuBar(const std::shared_ptr<ContentWindow>& window);
	void CreateFileDropDown(const std::shared_ptr<ContentWindow>& window);
	void CreateSimulationDropDown(const std::shared_ptr<ContentWindow>& window);
	void CreateViewDropDown(const std::shared_ptr<ContentWindow>& window);

	// Display controls to create a new simulation
	//		Only pass the layout the controls need to be created in
	void CreateNewSimulationControls(const std::shared_ptr<Layout> layout);

	// Display prompt on right side for whether or not to save the simulation
	void CreateSaveSimulationPromptControls(const std::shared_ptr<Layout> layout);




	void CreateQuickBar(const std::shared_ptr<ContentWindow>& window);

	void CreateRightPane(const std::shared_ptr<ContentWindow>& window);
}