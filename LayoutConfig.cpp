#include "LayoutConfig.h"


namespace LayoutConfiguration
{
	void ConfigureMainWindow(const std::shared_ptr<ContentWindow>& window)
	{		
		// Themes ======================================================================
		// 
		// All themes are defined/created in ThemeManager::Initialize()
		//

		// Main layout ==================================
		RowColDefinitions mainLayoutRows;
		mainLayoutRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f);
		mainLayoutRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		RowColDefinitions mainLayoutColumns;
		mainLayoutColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		mainLayoutColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 300.0f);

		std::shared_ptr<Layout> mainLayout = window->GetLayout();
		mainLayout->SetRowDefinitions(mainLayoutRows);
		mainLayout->SetColumnDefinitions(mainLayoutColumns);

		// Menu Bar ===============================================================
		RowColDefinitions menuBarColumnDefs;
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 45.0f); // File Drop down
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 45.0f); // Edit Drop down
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 45.0f); // View Drop down
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);   // 

		std::shared_ptr<Layout> menuBarLayout = mainLayout->CreateSubLayout(0, 0);
		menuBarLayout->SetColumnDefinitions(menuBarColumnDefs);

		// Menu controls ===============================================================
		std::shared_ptr<DropDown> fileDropDown = menuBarLayout->CreateControl<DropDown>(0, 0);
		fileDropDown->SetDropDownSize(50, 200);
		//fileDropDown->Margin(0.0f, 4.0f);

		// For the main button, we don't need to set the rows/columns - already 1 of each
		std::shared_ptr<Button> fileDropDownMainButton = fileDropDown->GetMainLayout()->CreateControl<Button>();
		fileDropDownMainButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		std::shared_ptr<Text>   fileDropDownMainButtonText = fileDropDownMainButton->GetLayout()->CreateControl<Text>();
		fileDropDownMainButtonText->SetTextTheme(THEME_MENU_BAR_TEXT);
		fileDropDownMainButtonText->SetText(L"File");

		// Create two drop down buttons
		RowColDefinitions fileDropDownRows;
		fileDropDownRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f);
		fileDropDownRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f);
		fileDropDownRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);

		fileDropDown->GetDropDownLayout()->SetRowDefinitions(fileDropDownRows);

		std::shared_ptr<Button> fileDropDownButton1 = fileDropDown->GetDropDownLayout()->CreateControl<Button>(0, 0);
		fileDropDownButton1->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		std::shared_ptr<Text>   fileDropDownButtonText1 = fileDropDownButton1->GetLayout()->CreateControl<Text>();
		fileDropDownButtonText1->SetTextTheme(THEME_MENU_BAR_TEXT);
		fileDropDownButtonText1->SetText(L"Drop Down Item 1");

		std::shared_ptr<Button> fileDropDownButton2 = fileDropDown->GetDropDownLayout()->CreateControl<Button>(1, 0);
		fileDropDownButton2->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		std::shared_ptr<Text>   fileDropDownButtonText2 = fileDropDownButton2->GetLayout()->CreateControl<Text>();
		fileDropDownButtonText2->SetTextTheme(THEME_MENU_BAR_TEXT);
		fileDropDownButtonText2->SetText(L"Drop Down Item 2");


		
	//	std::shared_ptr<Button> fileDropDown = menuBarLayout->CreateControl<Button>(0, 0);
	//	fileDropDown->Margin(0.0f, 4.0f);
	//	fileDropDown->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);

	//	std::shared_ptr<Text> fileText = fileDropDown->GetLayout()->CreateControl<Text>();
	//	fileText->SetTextTheme(THEME_MENU_BAR_TEXT);
	//	fileText->SetText(L"File");
		

		std::shared_ptr<Button> editDropDown = menuBarLayout->CreateControl<Button>(0, 1);
		editDropDown->Margin(0.0f, 4.0f);
		editDropDown->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);

		std::shared_ptr<Text> editText = editDropDown->GetLayout()->CreateControl<Text>();
		editText->SetTextTheme(THEME_MENU_BAR_TEXT);
		editText->SetText(L"Edit");

		
		std::shared_ptr<Button> viewDropDown = menuBarLayout->CreateControl<Button>(0, 2);
		viewDropDown->Margin(0.0f, 4.0f);
		viewDropDown->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);

		std::shared_ptr<Text> viewText = viewDropDown->GetLayout()->CreateControl<Text>();
		viewText->SetTextTheme(THEME_MENU_BAR_TEXT);
		viewText->SetText(L"View");
		


		// Simulation Window ================================================================
		std::shared_ptr<SimulationRenderer> simulationRenderer = mainLayout->CreateControl<SimulationRenderer>(1, 0);




		
	}
}