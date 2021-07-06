#pragma once
#include "pch.h"
#include "Layout.h"
#include "ContentWindow.h"
#include "Control.h"
#include "Button.h"
#include "ColorTheme.h"
#include "ThemeDefines.h"
#include "Text.h"

namespace LayoutConfiguration
{
	// Configure the layout for the main application window
	void ConfigureMainWindow(const std::shared_ptr<ContentWindow>& window)
	{
		// Define the Row / Column structure of the main layout ==================================
		std::shared_ptr<Layout> mainLayout = window->GetLayout();

		RowColDefinitions rowdefs;
		rowdefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 20.0f);
		rowdefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		mainLayout->SetRowDefinitions(rowdefs);

		RowColDefinitions coldefs;
		coldefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		coldefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 200.0f);
		mainLayout->SetColumnDefinitions(coldefs);



		std::shared_ptr<Text> text = mainLayout->CreateControl<Text>();



		
		// Menu Bar Layout
		std::shared_ptr<Layout> menuBarLayout = mainLayout->CreateSubLayout(0, 0);

		RowColDefinitions menuBarColumnDefs;
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);

		menuBarLayout->SetColumnDefinitions(menuBarColumnDefs);

		// Themes ======================================================================
		// All themes are defined/created in ThemeManager::Initialize()

		
		// Menu controls ===============================================================
		std::shared_ptr<Button> button = menuBarLayout->CreateControl<Button>(0, 0);
		button->Margin(1.0f);
		button->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);

		RowColDefinitions evenSplit;
		evenSplit.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		evenSplit.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);

		std::shared_ptr<Layout> buttonLayout = button->GetLayout();
		buttonLayout->SetRowDefinitions(evenSplit);
		buttonLayout->SetColumnDefinitions(evenSplit);

		// TEST controls ===============================================================
		RowColDefinitions even3way;
		even3way.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		even3way.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		even3way.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);

		std::shared_ptr<Layout> testLayout = mainLayout->CreateSubLayout(1, 0);
		testLayout->SetColumnDefinitions(even3way);
		testLayout->SetRowDefinitions(even3way);

		std::shared_ptr<Button> testButton = testLayout->CreateControl<Button>(1, 1);
		testButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);

		// internal
		std::shared_ptr<Layout> testButtonInternalLayout = testButton->GetLayout();
		testButtonInternalLayout->SetRowDefinitions(even3way);
		testButtonInternalLayout->SetColumnDefinitions(even3way);

		std::shared_ptr<Button> testButtonInternal = testButtonInternalLayout->CreateControl<Button>(1, 1);
		testButtonInternal->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);

		// internal 2
		std::shared_ptr<Layout> testButtonInternalLayout2 = testButtonInternal->GetLayout();
		testButtonInternalLayout2->SetRowDefinitions(even3way);
		testButtonInternalLayout2->SetColumnDefinitions(even3way);

		std::shared_ptr<Button> testButtonInternal2 = testButtonInternalLayout2->CreateControl<Button>(1, 1, 2, 1);
		testButtonInternal2->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);

		
	}
}