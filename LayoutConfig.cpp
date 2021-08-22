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
		mainLayoutRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f);	// File / Edit / View / etc. bar
		mainLayoutRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f);	// Quick Links bar
		mainLayoutRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);	// Simulation
		RowColDefinitions mainLayoutColumns;
		mainLayoutColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		mainLayoutColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 300.0f);

		std::shared_ptr<Layout> mainLayout = window->GetLayout();
		mainLayout->SetRowDefinitions(mainLayoutRows);
		mainLayout->SetColumnDefinitions(mainLayoutColumns);



		CreateMenuBar(mainLayout);
		CreateQuickBar(mainLayout);
		


		// Simulation Window ================================================================
		std::shared_ptr<SimulationRenderer> simulationRenderer = mainLayout->CreateControl<SimulationRenderer>(2, 0);




		
	}

	void CreateMenuBar(const std::shared_ptr<Layout>& mainLayout)
	{
		RowColDefinitions menuBarColumnDefs;
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 45.0f); // File Drop down
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 75.0f); // Simulation Drop down
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 45.0f); // View Drop down
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);   // 

		std::shared_ptr<Layout> menuBarLayout = mainLayout->CreateSubLayout(0, 0);
		menuBarLayout->SetColumnDefinitions(menuBarColumnDefs);

		CreateFileDropDown(menuBarLayout);
		CreateSimulationDropDown(menuBarLayout);
		CreateViewDropDown(menuBarLayout);
	}
	void CreateFileDropDown(const std::shared_ptr<Layout>& menuBarLayout)
	{
		std::shared_ptr<DropDown> fileDropDown = menuBarLayout->CreateControl<DropDown>(0, 0);
		fileDropDown->SetDropDownSize(100, 200);
		fileDropDown->SetColorTheme(THEME_MENU_DROP_DOWN_COLOR);

		// For the main button, we don't need to set the rows/columns - already 1 of each
		std::shared_ptr<Button> fileDropDownMainButton = fileDropDown->GetMainLayout()->CreateControl<Button>();
		fileDropDownMainButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		std::shared_ptr<Text>   fileDropDownMainButtonText = fileDropDownMainButton->GetLayout()->CreateControl<Text>();
		fileDropDownMainButtonText->SetTextTheme(THEME_MENU_BAR_TEXT);
		fileDropDownMainButtonText->SetText(L"File");

		// Create drop down buttons
		RowColDefinitions fileDropDownRows;
		fileDropDownRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f);
		fileDropDownRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f);
		fileDropDownRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f);
		fileDropDownRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f);
		fileDropDownRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		fileDropDown->GetDropDownLayout()->SetRowDefinitions(fileDropDownRows);

		RowColDefinitions fileDropDownButtonColumns;
		fileDropDownButtonColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f);
		fileDropDownButtonColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);

		// New Simulation
		std::shared_ptr<Button> fileDropDownNewSimulationButton = fileDropDown->GetDropDownLayout()->CreateControl<Button>(0, 0);
		fileDropDownNewSimulationButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		fileDropDownNewSimulationButton->GetLayout()->SetColumnDefinitions(fileDropDownButtonColumns);
		fileDropDownNewSimulationButton->Click(WindowManager::NewSimulationButtonClick);

		std::shared_ptr<Text> fileDropDownNewSimulationGlyph = fileDropDownNewSimulationButton->GetLayout()->CreateControl<Text>(0, 0);
		fileDropDownNewSimulationGlyph->SetTextTheme(THEME_MENU_DROP_DOWN_GLYPH);
		fileDropDownNewSimulationGlyph->SetText(L"\xE710");

		std::shared_ptr<Text> fileDropDownNewSimulationText = fileDropDownNewSimulationButton->GetLayout()->CreateControl<Text>(0, 1);
		fileDropDownNewSimulationText->SetTextTheme(THEME_MENU_DROP_DOWN_TEXT);
		fileDropDownNewSimulationText->SetText(L"New Simulation");

		// Open Simulation
		std::shared_ptr<Button> fileDropDownOpenSimulationButton = fileDropDown->GetDropDownLayout()->CreateControl<Button>(1, 0);
		fileDropDownOpenSimulationButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		fileDropDownOpenSimulationButton->GetLayout()->SetColumnDefinitions(fileDropDownButtonColumns);

		std::shared_ptr<Text> fileDropDownOpenSimulationGlyph = fileDropDownOpenSimulationButton->GetLayout()->CreateControl<Text>(0, 0);
		fileDropDownOpenSimulationGlyph->SetTextTheme(THEME_MENU_DROP_DOWN_GLYPH);
		fileDropDownOpenSimulationGlyph->SetText(L"\xE8E5");

		std::shared_ptr<Text> fileDropDownOpenSimulationText = fileDropDownOpenSimulationButton->GetLayout()->CreateControl<Text>(0, 1);
		fileDropDownOpenSimulationText->SetTextTheme(THEME_MENU_DROP_DOWN_TEXT);
		fileDropDownOpenSimulationText->SetText(L"Open Simulation");

		// Save Simulation
		std::shared_ptr<Button> fileDropDownSaveSimulationButton = fileDropDown->GetDropDownLayout()->CreateControl<Button>(2, 0);
		fileDropDownSaveSimulationButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		fileDropDownSaveSimulationButton->GetLayout()->SetColumnDefinitions(fileDropDownButtonColumns);

		std::shared_ptr<Text> fileDropDownSaveSimulationGlyph = fileDropDownSaveSimulationButton->GetLayout()->CreateControl<Text>(0, 0);
		fileDropDownSaveSimulationGlyph->SetTextTheme(THEME_MENU_DROP_DOWN_GLYPH);
		fileDropDownSaveSimulationGlyph->SetText(L"\xE74E");

		std::shared_ptr<Text> fileDropDownSaveSimulationText = fileDropDownSaveSimulationButton->GetLayout()->CreateControl<Text>(0, 1);
		fileDropDownSaveSimulationText->SetTextTheme(THEME_MENU_DROP_DOWN_TEXT);
		fileDropDownSaveSimulationText->SetText(L"Save Simulation");

		// Save As
		std::shared_ptr<Button> fileDropDownSaveAsButton = fileDropDown->GetDropDownLayout()->CreateControl<Button>(3, 0);
		fileDropDownSaveAsButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		fileDropDownSaveAsButton->GetLayout()->SetColumnDefinitions(fileDropDownButtonColumns);

		std::shared_ptr<Text> fileDropDownSaveAsGlyph = fileDropDownSaveAsButton->GetLayout()->CreateControl<Text>(0, 0);
		fileDropDownSaveAsGlyph->SetTextTheme(THEME_MENU_DROP_DOWN_GLYPH);
		fileDropDownSaveAsGlyph->SetText(L"\xE792");

		std::shared_ptr<Text> fileDropDownSaveAsText = fileDropDownSaveAsButton->GetLayout()->CreateControl<Text>(0, 1);
		fileDropDownSaveAsText->SetTextTheme(THEME_MENU_DROP_DOWN_TEXT);
		fileDropDownSaveAsText->SetText(L"Save As...");
	}
	void CreateSimulationDropDown(const std::shared_ptr<Layout>& menuBarLayout)
	{
		std::shared_ptr<DropDown> dropDown = menuBarLayout->CreateControl<DropDown>(0, 1);
		dropDown->SetDropDownSize(75, 200);
		dropDown->SetColorTheme(THEME_MENU_DROP_DOWN_COLOR);

		// For the main button, we don't need to set the rows/columns - already 1 of each
		std::shared_ptr<Button> dropDownMainButton = dropDown->GetMainLayout()->CreateControl<Button>();
		dropDownMainButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		std::shared_ptr<Text> dropDownMainButtonText = dropDownMainButton->GetLayout()->CreateControl<Text>();
		dropDownMainButtonText->SetTextTheme(THEME_MENU_BAR_TEXT);
		dropDownMainButtonText->SetText(L"Simulation");

		// Create drop down buttons
		RowColDefinitions dropDownRows;
		dropDownRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f); // Reset State
		dropDownRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f); // Add Atom
		dropDownRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f); // Edit Atom
		dropDownRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		dropDown->GetDropDownLayout()->SetRowDefinitions(dropDownRows);

		RowColDefinitions dropDownButtonColumns;
		dropDownButtonColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f);
		dropDownButtonColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);


		// Reset State
		std::shared_ptr<Button> resetStateButton = dropDown->GetDropDownLayout()->CreateControl<Button>(0, 0);
		resetStateButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		resetStateButton->GetLayout()->SetColumnDefinitions(dropDownButtonColumns);

		std::shared_ptr<Text> resetStateGlyph = resetStateButton->GetLayout()->CreateControl<Text>(0, 0);
		resetStateGlyph->SetTextTheme(THEME_MENU_DROP_DOWN_GLYPH);
		resetStateGlyph->SetText(L"\xE72C");

		std::shared_ptr<Text> resetStateText = resetStateButton->GetLayout()->CreateControl<Text>(0, 1);
		resetStateText->SetTextTheme(THEME_MENU_DROP_DOWN_TEXT);
		resetStateText->SetText(L"Reset State");


		// Add Atom
		std::shared_ptr<Button> addAtomButton = dropDown->GetDropDownLayout()->CreateControl<Button>(1, 0);
		addAtomButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		addAtomButton->GetLayout()->SetColumnDefinitions(dropDownButtonColumns);

		std::shared_ptr<Text> addAtomGlyph = addAtomButton->GetLayout()->CreateControl<Text>(0, 0);
		addAtomGlyph->SetTextTheme(THEME_MENU_DROP_DOWN_GLYPH);
		addAtomGlyph->SetText(L"\xECC8");

		std::shared_ptr<Text> addAtomText = addAtomButton->GetLayout()->CreateControl<Text>(0, 1);
		addAtomText->SetTextTheme(THEME_MENU_DROP_DOWN_TEXT);
		addAtomText->SetText(L"Add Atom");


		// Edit Atom
		std::shared_ptr<Button> editAtomButton = dropDown->GetDropDownLayout()->CreateControl<Button>(2, 0);
		editAtomButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		editAtomButton->GetLayout()->SetColumnDefinitions(dropDownButtonColumns);

		std::shared_ptr<Text> editAtomGlyph = editAtomButton->GetLayout()->CreateControl<Text>(0, 0);
		editAtomGlyph->SetTextTheme(THEME_MENU_DROP_DOWN_GLYPH);
		editAtomGlyph->SetText(L"\xEB7E");

		std::shared_ptr<Text> editAtomText = editAtomButton->GetLayout()->CreateControl<Text>(0, 1);
		editAtomText->SetTextTheme(THEME_MENU_DROP_DOWN_TEXT);
		editAtomText->SetText(L"Edit Atom");

	}
	void CreateViewDropDown(const std::shared_ptr<Layout>& menuBarLayout)
	{
		std::shared_ptr<DropDown> dropDown = menuBarLayout->CreateControl<DropDown>(0, 2);
		dropDown->SetDropDownSize(25, 200);
		dropDown->SetColorTheme(THEME_MENU_DROP_DOWN_COLOR);

		// For the main button, we don't need to set the rows/columns - already 1 of each
		std::shared_ptr<Button> dropDownMainButton = dropDown->GetMainLayout()->CreateControl<Button>();
		dropDownMainButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		std::shared_ptr<Text> dropDownMainButtonText = dropDownMainButton->GetLayout()->CreateControl<Text>();
		dropDownMainButtonText->SetTextTheme(THEME_MENU_BAR_TEXT);
		dropDownMainButtonText->SetText(L"View");

		// Create drop down buttons
		RowColDefinitions dropDownRows;
		dropDownRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f); // Atom Summary
		dropDownRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		dropDown->GetDropDownLayout()->SetRowDefinitions(dropDownRows);

		RowColDefinitions dropDownButtonColumns;
		dropDownButtonColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f);	// Glyph
		dropDownButtonColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// Text


		// Atom Summary
		std::shared_ptr<Button> atomSummaryButton = dropDown->GetDropDownLayout()->CreateControl<Button>(0, 0);
		atomSummaryButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		atomSummaryButton->GetLayout()->SetColumnDefinitions(dropDownButtonColumns);

		std::shared_ptr<Text> atomSummaryGlyph = atomSummaryButton->GetLayout()->CreateControl<Text>(0, 0);
		atomSummaryGlyph->SetTextTheme(THEME_MENU_DROP_DOWN_GLYPH);
		atomSummaryGlyph->SetText(L"\xE762");

		std::shared_ptr<Text> atomSummaryText = atomSummaryButton->GetLayout()->CreateControl<Text>(0, 1);
		atomSummaryText->SetTextTheme(THEME_MENU_DROP_DOWN_TEXT);
		atomSummaryText->SetText(L"Atom Summary");
	}

	void CreateQuickBar(const std::shared_ptr<Layout>& mainLayout)
	{
		// Define columns for the quick buttons
		RowColDefinitions columnDefs;
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f); // New Simulation
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f); // Open Simulation
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f); // Save Simulation
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 25.0f); // Play / Pause
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);

		std::shared_ptr<Layout> layout = mainLayout->CreateSubLayout(1, 0);
		layout->SetColumnDefinitions(columnDefs);

		// New Simulation
		std::shared_ptr<Button> newSimulationButton = layout->CreateControl<Button>(0, 0);
		newSimulationButton->SetColorTheme(THEME_QUICK_BAR_BUTTON_COLOR);
		std::shared_ptr<Text> newSimulationText = newSimulationButton->GetLayout()->CreateControl<Text>();
		newSimulationText->SetTextTheme(THEME_QUICK_BAR_GLYPH);
		newSimulationText->SetText(L"\xE710");

		// Open Simulation
		std::shared_ptr<Button> openSimulationButton = layout->CreateControl<Button>(0, 1);
		openSimulationButton->SetColorTheme(THEME_QUICK_BAR_BUTTON_COLOR);
		std::shared_ptr<Text> openSimulationText = openSimulationButton->GetLayout()->CreateControl<Text>();
		openSimulationText->SetTextTheme(THEME_QUICK_BAR_GLYPH);
		openSimulationText->SetText(L"\xE8E5");

		// Save Simulation
		std::shared_ptr<Button> saveSimulationButton = layout->CreateControl<Button>(0, 2);
		saveSimulationButton->SetColorTheme(THEME_QUICK_BAR_BUTTON_COLOR);
		std::shared_ptr<Text> saveSimulationText = saveSimulationButton->GetLayout()->CreateControl<Text>();
		saveSimulationText->SetTextTheme(THEME_QUICK_BAR_GLYPH);
		saveSimulationText->SetText(L"\xE74E");

		// Play / Pause Simulation
		std::shared_ptr<Button> playSimulationButton = layout->CreateControl<Button>(0, 3);
		playSimulationButton->SetColorTheme(THEME_QUICK_BAR_BUTTON_COLOR);
		std::shared_ptr<Text> playSimulationText = playSimulationButton->GetLayout()->CreateControl<Text>();
		playSimulationText->SetTextTheme(THEME_QUICK_BAR_GLYPH);
		playSimulationText->SetText(L"\xE769");
	}
}