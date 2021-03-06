#pragma once
#include "pch.h"
#include "Layout.h"
#include "ContentWindow.h"
#include "Control.h"
#include "Button.h"
#include "ColorTheme.h"
#include "DropDown.h"
#include "TabbedPane.h"
#include "ThemeDefines.h"
#include "Text.h"
#include "SimulationRenderer.h"
#include "WindowManager.h"

#include <functional>

namespace LayoutConfiguration
{
	// Configure the layout for the main application window
	void ConfigureMainWindow(const std::shared_ptr<ContentWindow>& window);

	// Nomenclature:
	//		Create__  : Used to create controls/layouts that live for the life of the application
	//		Display__ : Used tp create/display controls/layouts that are dynamically created and destroyed

	void CreateMenuBar(const std::shared_ptr<ContentWindow>& window);
	void CreateFileDropDown(const std::shared_ptr<ContentWindow>& window);
	void CreateSimulationDropDown(const std::shared_ptr<ContentWindow>& window);
	void CreateViewDropDown(const std::shared_ptr<ContentWindow>& window);

	// Display controls to create a new simulation
	void DisplaySimulationDetailsControls(const std::shared_ptr<ContentWindow>& window);

	void DisplayAddAtomsControls(const std::shared_ptr<ContentWindow>& window);
	void DisplayAddAtomsControlsAtomInfoButtonClick(const std::shared_ptr<ContentWindow>& window);
	void DisplayAddAtomsControlsBondInfoButtonClick(const std::shared_ptr<ContentWindow>& window);

	void DisplayAddMoleculeControls(const std::shared_ptr<ContentWindow>& window);
	void DisplayCreateBondControls(const std::shared_ptr<ContentWindow>& window);
	void DisplayEditVelocityArrowsControls(const std::shared_ptr<ContentWindow>& window);
	void DisplayResetStateControls(const std::shared_ptr<ContentWindow>& window);
	void DisplayNewSimulationQuickBarControls(const std::shared_ptr<ContentWindow>& window);
	void DisplaySimulationPlayingControls(const std::shared_ptr<ContentWindow>& window);

	// Display prompt on right side for whether or not to save the simulation
	void DisplaySaveSimulationPromptControls(const std::shared_ptr<ContentWindow>& window);

	// Display prompt on the right side for whether or not to delete the currently selected bond
	void DisplayDeleteBondPromptControls(const std::shared_ptr<ContentWindow>& window, const std::shared_ptr<Bond>& selectedBond);



	void CreateQuickBar(const std::shared_ptr<ContentWindow>& window);

	void CreateRightPane(const std::shared_ptr<ContentWindow>& window);
}