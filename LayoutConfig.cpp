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



		CreateMenuBar(window);
		CreateQuickBar(window);
		CreateRightPane(window);
		


		// Simulation Window ================================================================
		std::shared_ptr<SimulationRenderer> simulationRenderer = mainLayout->CreateControl<SimulationRenderer>(2, 0);
		
	}

	void CreateMenuBar(const std::shared_ptr<ContentWindow>& window)
	{
		std::shared_ptr<Layout> mainLayout = window->GetLayout();

		RowColDefinitions menuBarColumnDefs;
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 45.0f); // File Drop down
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 75.0f); // Simulation Drop down
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 45.0f); // View Drop down
		menuBarColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);   // 

		std::shared_ptr<Layout> menuBarLayout = mainLayout->CreateSubLayout(0, 0);
		menuBarLayout->SetColumnDefinitions(menuBarColumnDefs);

		CreateFileDropDown(window);
		CreateSimulationDropDown(window);
		CreateViewDropDown(window);
	}
	void CreateFileDropDown(const std::shared_ptr<ContentWindow>& window)
	{
		std::shared_ptr<Layout> menuBarLayout = window->GetLayout()->GetSubLayout(0, 0);

		std::shared_ptr<DropDown> fileDropDown = menuBarLayout->CreateControl<DropDown>(0, 0);
		fileDropDown->SetDropDownSize(100, 200);
		fileDropDown->SetColorTheme(THEME_MENU_BAR_DROP_DOWN_COLOR);

		// For the main button, we don't need to set the rows/columns - already 1 of each
		std::shared_ptr<Button> fileDropDownMainButton = fileDropDown->GetMainLayout()->CreateControl<Button>();
		fileDropDownMainButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		fileDropDownMainButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
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
		fileDropDownNewSimulationButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		fileDropDownNewSimulationButton->GetLayout()->SetColumnDefinitions(fileDropDownButtonColumns);
		fileDropDownNewSimulationButton->Click(
			[weakWindow = std::weak_ptr<ContentWindow>(window)]()
			{
				auto window = weakWindow.lock();			

				// Get the layout for the pane on the right
				std::shared_ptr<Layout> layout = window->GetLayout()->GetSubLayout(2, 1);
				if (layout == nullptr)
					layout = window->GetLayout()->CreateSubLayout(2, 1);
				else
					layout->Clear();

				CreateNewSimulationControls(layout);
			}
		);

		std::shared_ptr<Text> fileDropDownNewSimulationGlyph = fileDropDownNewSimulationButton->GetLayout()->CreateControl<Text>(0, 0);
		fileDropDownNewSimulationGlyph->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_GLYPH);
		fileDropDownNewSimulationGlyph->SetText(L"\xE710");

		std::shared_ptr<Text> fileDropDownNewSimulationText = fileDropDownNewSimulationButton->GetLayout()->CreateControl<Text>(0, 1);
		fileDropDownNewSimulationText->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_TEXT);
		fileDropDownNewSimulationText->SetText(L"New Simulation");

		// Open Simulation
		std::shared_ptr<Button> fileDropDownOpenSimulationButton = fileDropDown->GetDropDownLayout()->CreateControl<Button>(1, 0);
		fileDropDownOpenSimulationButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		fileDropDownOpenSimulationButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		fileDropDownOpenSimulationButton->GetLayout()->SetColumnDefinitions(fileDropDownButtonColumns);

		std::shared_ptr<Text> fileDropDownOpenSimulationGlyph = fileDropDownOpenSimulationButton->GetLayout()->CreateControl<Text>(0, 0);
		fileDropDownOpenSimulationGlyph->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_GLYPH);
		fileDropDownOpenSimulationGlyph->SetText(L"\xE8E5");

		std::shared_ptr<Text> fileDropDownOpenSimulationText = fileDropDownOpenSimulationButton->GetLayout()->CreateControl<Text>(0, 1);
		fileDropDownOpenSimulationText->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_TEXT);
		fileDropDownOpenSimulationText->SetText(L"Open Simulation");

		// Save Simulation
		std::shared_ptr<Button> fileDropDownSaveSimulationButton = fileDropDown->GetDropDownLayout()->CreateControl<Button>(2, 0);
		fileDropDownSaveSimulationButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		fileDropDownSaveSimulationButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		fileDropDownSaveSimulationButton->GetLayout()->SetColumnDefinitions(fileDropDownButtonColumns);

		std::shared_ptr<Text> fileDropDownSaveSimulationGlyph = fileDropDownSaveSimulationButton->GetLayout()->CreateControl<Text>(0, 0);
		fileDropDownSaveSimulationGlyph->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_GLYPH);
		fileDropDownSaveSimulationGlyph->SetText(L"\xE74E");

		std::shared_ptr<Text> fileDropDownSaveSimulationText = fileDropDownSaveSimulationButton->GetLayout()->CreateControl<Text>(0, 1);
		fileDropDownSaveSimulationText->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_TEXT);
		fileDropDownSaveSimulationText->SetText(L"Save Simulation");

		// Save As
		std::shared_ptr<Button> fileDropDownSaveAsButton = fileDropDown->GetDropDownLayout()->CreateControl<Button>(3, 0);
		fileDropDownSaveAsButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		fileDropDownSaveAsButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		fileDropDownSaveAsButton->GetLayout()->SetColumnDefinitions(fileDropDownButtonColumns);

		std::shared_ptr<Text> fileDropDownSaveAsGlyph = fileDropDownSaveAsButton->GetLayout()->CreateControl<Text>(0, 0);
		fileDropDownSaveAsGlyph->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_GLYPH);
		fileDropDownSaveAsGlyph->SetText(L"\xE792");

		std::shared_ptr<Text> fileDropDownSaveAsText = fileDropDownSaveAsButton->GetLayout()->CreateControl<Text>(0, 1);
		fileDropDownSaveAsText->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_TEXT);
		fileDropDownSaveAsText->SetText(L"Save As...");
	}
	void CreateSimulationDropDown(const std::shared_ptr<ContentWindow>& window)
	{
		std::shared_ptr<Layout> menuBarLayout = window->GetLayout()->GetSubLayout(0, 0);

		std::shared_ptr<DropDown> dropDown = menuBarLayout->CreateControl<DropDown>(0, 1);
		dropDown->SetDropDownSize(75, 200);
		dropDown->SetColorTheme(THEME_MENU_BAR_DROP_DOWN_COLOR);

		// For the main button, we don't need to set the rows/columns - already 1 of each
		std::shared_ptr<Button> dropDownMainButton = dropDown->GetMainLayout()->CreateControl<Button>();
		dropDownMainButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		dropDownMainButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
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
		resetStateButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		resetStateButton->GetLayout()->SetColumnDefinitions(dropDownButtonColumns);

		std::shared_ptr<Text> resetStateGlyph = resetStateButton->GetLayout()->CreateControl<Text>(0, 0);
		resetStateGlyph->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_GLYPH);
		resetStateGlyph->SetText(L"\xE72C");

		std::shared_ptr<Text> resetStateText = resetStateButton->GetLayout()->CreateControl<Text>(0, 1);
		resetStateText->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_TEXT);
		resetStateText->SetText(L"Reset State");


		// Add Atom
		std::shared_ptr<Button> addAtomButton = dropDown->GetDropDownLayout()->CreateControl<Button>(1, 0);
		addAtomButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		addAtomButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		addAtomButton->GetLayout()->SetColumnDefinitions(dropDownButtonColumns);

		std::shared_ptr<Text> addAtomGlyph = addAtomButton->GetLayout()->CreateControl<Text>(0, 0);
		addAtomGlyph->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_GLYPH);
		addAtomGlyph->SetText(L"\xECC8");

		std::shared_ptr<Text> addAtomText = addAtomButton->GetLayout()->CreateControl<Text>(0, 1);
		addAtomText->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_TEXT);
		addAtomText->SetText(L"Add Atom");


		// Edit Atom
		std::shared_ptr<Button> editAtomButton = dropDown->GetDropDownLayout()->CreateControl<Button>(2, 0);
		editAtomButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		editAtomButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		editAtomButton->GetLayout()->SetColumnDefinitions(dropDownButtonColumns);

		std::shared_ptr<Text> editAtomGlyph = editAtomButton->GetLayout()->CreateControl<Text>(0, 0);
		editAtomGlyph->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_GLYPH);
		editAtomGlyph->SetText(L"\xEB7E");

		std::shared_ptr<Text> editAtomText = editAtomButton->GetLayout()->CreateControl<Text>(0, 1);
		editAtomText->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_TEXT);
		editAtomText->SetText(L"Edit Atom");

	}
	void CreateViewDropDown(const std::shared_ptr<ContentWindow>& window)
	{
		std::shared_ptr<Layout> menuBarLayout = window->GetLayout()->GetSubLayout(0, 0);

		std::shared_ptr<DropDown> dropDown = menuBarLayout->CreateControl<DropDown>(0, 2);
		dropDown->SetDropDownSize(25, 200);
		dropDown->SetColorTheme(THEME_MENU_BAR_DROP_DOWN_COLOR);

		// For the main button, we don't need to set the rows/columns - already 1 of each
		std::shared_ptr<Button> dropDownMainButton = dropDown->GetMainLayout()->CreateControl<Button>();
		dropDownMainButton->SetColorTheme(THEME_MENU_BAR_BUTTON_COLOR);
		dropDownMainButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
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
		atomSummaryButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		atomSummaryButton->GetLayout()->SetColumnDefinitions(dropDownButtonColumns);

		std::shared_ptr<Text> atomSummaryGlyph = atomSummaryButton->GetLayout()->CreateControl<Text>(0, 0);
		atomSummaryGlyph->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_GLYPH);
		atomSummaryGlyph->SetText(L"\xE762");

		std::shared_ptr<Text> atomSummaryText = atomSummaryButton->GetLayout()->CreateControl<Text>(0, 1);
		atomSummaryText->SetTextTheme(THEME_MENU_BAR_DROP_DOWN_TEXT);
		atomSummaryText->SetText(L"Atom Summary");
	}

	void CreateNewSimulationControls(const std::shared_ptr<Layout> layout)
	{
		RowColDefinitions rowDefs;
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f);		// Simulation Name sublayout
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 50.0f);		// Add Atom combo box
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 300.0f);	// Atom Position / Velocity
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 40.0f);		// Add atom button
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// Atom ListView
		layout->SetRowDefinitions(rowDefs);

		// ============================================================================================================
		// Sub Layout for simulation name controls
		std::shared_ptr<Layout> simulationNameSubLayout = layout->CreateSubLayout(0, 0);

		RowColDefinitions simulationNameColumns;
		simulationNameColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 110.0f);
		simulationNameColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		simulationNameSubLayout->SetColumnDefinitions(simulationNameColumns);

		// Text for Simulation Name
		std::shared_ptr<Text> simulationNameText = simulationNameSubLayout->CreateControl<Text>(0, 0);
		simulationNameText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		simulationNameText->SetText(L"Simulation Name:");
		simulationNameText->Margin(5.0f, 0.0f);

		// Text Input for Simulation Name
		std::shared_ptr<TextInput> simulationNameTextInput = simulationNameSubLayout->CreateControl<TextInput>(0, 1);
		simulationNameTextInput->Margin(0.0f, 0.0f, 5.0f, 0.0f);
		// ============================================================================================================
		// ============================================================================================================
		// Sub Layout for add atom controls
		std::shared_ptr<Layout> addAtomSubLayout = layout->CreateSubLayout(1, 0);

		RowColDefinitions addAtomColumns;
		addAtomColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 100.0f);
		addAtomColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		addAtomSubLayout->SetColumnDefinitions(addAtomColumns);

		// Text for Add atom
		std::shared_ptr<Text> addAtomText = addAtomSubLayout->CreateControl<Text>(0, 0);
		addAtomText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		addAtomText->SetText(L"Atom Type:");
		addAtomText->Margin(5.0f, 0.0f);

		std::shared_ptr<ComboBox> atomComboBox = addAtomSubLayout->CreateControl<ComboBox>(0, 1);
		atomComboBox->AddComboBoxItem(L"Hydrogen");
		atomComboBox->AddComboBoxItem(L"Helium");
		atomComboBox->AddComboBoxItem(L"Lithium");
		atomComboBox->AddComboBoxItem(L"Beryllium");
		atomComboBox->AddComboBoxItem(L"Boron");
		atomComboBox->AddComboBoxItem(L"Carbon");
		atomComboBox->AddComboBoxItem(L"Nitrogen");
		atomComboBox->AddComboBoxItem(L"Oxygen");
		atomComboBox->AddComboBoxItem(L"Flourine");
		atomComboBox->AddComboBoxItem(L"Neon");
		atomComboBox->Margin(5.0f, 10.0f);
		// atomComboBox->SelectionChanged() - defined below so it can capture the listview

		// ============================================================================================================
		// ============================================================================================================
		// Sub Layout for editing position and velocity
		std::shared_ptr<Layout> atomPositionVelocitySubLayout = layout->CreateSubLayout(2, 0);

		RowColDefinitions positionVelocityColumns;
		positionVelocityColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 60.0f);
		positionVelocityColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		atomPositionVelocitySubLayout->SetColumnDefinitions(positionVelocityColumns);

		RowColDefinitions positionVelocityRows;
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Box Dimensions:
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Box Dimensions Slider
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Position:
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Position X
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Position Y
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Position Z
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Velocity:
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Velocity X
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Velocity Y
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Velocity Z
		atomPositionVelocitySubLayout->SetRowDefinitions(positionVelocityRows);

		// Text for "Box Dimensions"
		std::shared_ptr<Text> boxDimensionsText = atomPositionVelocitySubLayout->CreateControl<Text>(0, 0, 1, 2);
		boxDimensionsText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		boxDimensionsText->SetText(L"Box Dimensions");
		boxDimensionsText->Margin(5.0f, 0.0f);

		// Box Dimension Slider
		std::shared_ptr<Slider> boxDimensionSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(1, 0, 1, 2);
		boxDimensionSlider->SetMin(2.0f);
		boxDimensionSlider->SetMax(100.0f);
		boxDimensionSlider->Margin(10.0f, 2.0f);
		boxDimensionSlider->ValueChanged(
			[](float value)
			{
				SimulationManager::BoxDimensions(value);
			}
		);


		// Text for "Position"
		std::shared_ptr<Text> positionText = atomPositionVelocitySubLayout->CreateControl<Text>(2, 0);
		positionText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		positionText->SetText(L"Position");
		positionText->Margin(5.0f, 0.0f);

		// Text for X Position
		std::shared_ptr<Text> positionXText = atomPositionVelocitySubLayout->CreateControl<Text>(3, 0);
		positionXText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		positionXText->SetText(L"X:");
		positionXText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

		// Slider for X Position
		std::shared_ptr<Slider> positionXSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(3, 1);
		positionXSlider->SetMin(-1.0f);
		positionXSlider->SetMax(1.0f);
		positionXSlider->Margin(0.0f, 2.0f);



		// Text for Y Position
		std::shared_ptr<Text> positionYText = atomPositionVelocitySubLayout->CreateControl<Text>(4, 0);
		positionYText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		positionYText->SetText(L"Y:");
		positionYText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

		// Slider for Y Position
		std::shared_ptr<Slider> positionYSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(4, 1);
		positionYSlider->SetMin(-1.0f);
		positionYSlider->SetMax(1.0f);
		positionYSlider->Margin(0.0f, 2.0f);



		// Text for Z Position
		std::shared_ptr<Text> positionZText = atomPositionVelocitySubLayout->CreateControl<Text>(5, 0);
		positionZText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		positionZText->SetText(L"Z:");
		positionZText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

		// Slider for Z Position
		std::shared_ptr<Slider> positionZSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(5, 1);
		positionZSlider->SetMin(-1.0f);
		positionZSlider->SetMax(1.0f);
		positionZSlider->Margin(0.0f, 2.0f);

		// ===========================================================
		// Now that all the position sliders have been created, we can 
		// reference them in the lambda for the box dimension slider
		// - Capture values by value, capturing shared_ptr's by reference
		//   will cause some kind of read access error
		boxDimensionSlider->ValueChanged(
			[weakPXSlider = std::weak_ptr<Slider>(positionXSlider),
			weakPYSlider = std::weak_ptr<Slider>(positionYSlider),
			weakPZSlider = std::weak_ptr<Slider>(positionZSlider)](float value)
		{
			auto pXSlider = weakPXSlider.lock();
			auto pYSlider = weakPYSlider.lock();
			auto pZSlider = weakPZSlider.lock();

			SimulationManager::BoxDimensions(value);
			float half = value / 2.0f;
			pXSlider->SetMin(-half);
			pXSlider->SetMax(half);
			pYSlider->SetMin(-half);
			pYSlider->SetMax(half);
			pZSlider->SetMin(-half);
			pZSlider->SetMax(half);
		}
		);
		// ===========================================================

		// Text for "Velocity"
		std::shared_ptr<Text> velocityText = atomPositionVelocitySubLayout->CreateControl<Text>(6, 0);
		velocityText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		velocityText->SetText(L"Velocity");
		velocityText->Margin(5.0f, 0.0f);

		// Text for X Velocity
		std::shared_ptr<Text> velocityXText = atomPositionVelocitySubLayout->CreateControl<Text>(7, 0);
		velocityXText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		velocityXText->SetText(L"X:");
		velocityXText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

		// Slider for X Velocity
		std::shared_ptr<Slider> velocityXSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(7, 1);
		velocityXSlider->SetMin(-100.0f);
		velocityXSlider->SetMax(100.0f);
		velocityXSlider->Margin(0.0f, 2.0f);


		// Text for Y Velocity
		std::shared_ptr<Text> velocityYText = atomPositionVelocitySubLayout->CreateControl<Text>(8, 0);
		velocityYText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		velocityYText->SetText(L"Y:");
		velocityYText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

		// Slider for Y Velocity
		std::shared_ptr<Slider> velocityYSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(8, 1);
		velocityYSlider->SetMin(-100.0f);
		velocityYSlider->SetMax(100.0f);
		velocityYSlider->Margin(0.0f, 2.0f);


		// Text for Z Velocity
		std::shared_ptr<Text> velocityZText = atomPositionVelocitySubLayout->CreateControl<Text>(9, 0);
		velocityZText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		velocityZText->SetText(L"Z:");
		velocityZText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

		// Slider for Z Velocity
		std::shared_ptr<Slider> velocityZSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(9, 1);
		velocityZSlider->SetMin(-100.0f);
		velocityZSlider->SetMax(100.0f);
		velocityZSlider->Margin(0.0f, 2.0f);

		// ============================================================================================================
		// ============================================================================================================
		// Add atom button
		std::shared_ptr<Button> addAtomButton = layout->CreateControl<Button>(3, 0);
		addAtomButton->SetColorTheme(THEME_NEW_SIMULATION_ADD_BUTTON_COLOR);
		addAtomButton->SetBorderTheme(THEME_NEW_SIMULATION_ADD_BUTTON_BORDER);
		addAtomButton->Margin(20.0f, 5.0f);

		std::shared_ptr<Layout> addAtomButtonLayout = addAtomButton->GetLayout();

		std::shared_ptr<Text> addAtomButtonText = addAtomButtonLayout->CreateControl<Text>();
		addAtomButtonText->SetTextTheme(THEME_NEW_SIMULATION_ADD_BUTTON_TEXT);
		addAtomButtonText->SetText(L"Add New Atom");

		// ============================================================================================================
		// ============================================================================================================
		// Atom List View
		std::shared_ptr<ListView<Atom>> atomListView = layout->CreateControl<ListView<Atom>>(4, 0);
		atomListView->SetItemHeight(40.0f);
		atomListView->SetFormatFunction(
			[weakDeviceResources = std::weak_ptr<DeviceResources>(layout->GetDeviceResources()),
			weakListView = std::weak_ptr<ListView<Atom>>(atomListView),
			weakPXSlider = std::weak_ptr<Slider>(positionXSlider),
			weakPYSlider = std::weak_ptr<Slider>(positionYSlider),
			weakPZSlider = std::weak_ptr<Slider>(positionZSlider),
			weakVXSlider = std::weak_ptr<Slider>(velocityXSlider),
			weakVYSlider = std::weak_ptr<Slider>(velocityYSlider),
			weakVZSlider = std::weak_ptr<Slider>(velocityZSlider),
			weakComboBox = std::weak_ptr<ComboBox>(atomComboBox)](std::shared_ptr<Atom> atom, bool highlighted)
		{
			auto deviceResources = weakDeviceResources.lock();
			auto listView = weakListView.lock();

			// Create a new layout object with the same height and a reasonable width (it will get resized)
			std::shared_ptr<Layout> newLayout = std::make_shared<Layout>(deviceResources, 0.0f, 0.0f, 40.0f, 1000.0f);

			std::shared_ptr<Button> newButton = newLayout->CreateControl<Button>();
			if (highlighted)
				newButton->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_HIGHLIGHTED_COLOR);
			else
				newButton->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_COLOR);
			newButton->SetBorderTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_BORDER);
			newButton->Margin(5.0f, 0.0f);
			std::shared_ptr<Layout> newButtonLayout = newButton->GetLayout();

			RowColDefinitions columnDefs;
			columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 75.0f);	// Name of the atom type
			columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 47.0f);	// Position: / Velocity:
			columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);	// Position / Velocity values
			columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f);	// Remove Button
			newButtonLayout->SetColumnDefinitions(columnDefs);

			RowColDefinitions rowDefs;
			rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
			rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
			newButtonLayout->SetRowDefinitions(rowDefs);

			// Text of the atom type
			std::shared_ptr<Text> atomNameText = newButtonLayout->CreateControl<Text>(0, 0, 2, 1);
			atomNameText->SetTextTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_TEXT);
			atomNameText->SetText(atom->Name());
			atomNameText->Margin(5.0f, 0.0f, 0.0f, 0.0f);


			std::shared_ptr<Text> positionText = newButtonLayout->CreateControl<Text>(0, 1);
			positionText->SetTextTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_POSITION_VELOCITY_TEXT);
			positionText->SetText(L"Position:");

			std::shared_ptr<Text> positionValueText = newButtonLayout->CreateControl<Text>(0, 2);
			positionValueText->SetTextTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_POSITION_VELOCITY_TEXT);

			std::ostringstream positionOSS;
			positionOSS.precision(3);
			positionOSS << std::fixed << "(" << atom->Position().x << ", " << atom->Position().y << ", " << atom->Position().z << ")";
			positionValueText->SetText(positionOSS.str());



			std::shared_ptr<Text> velocityText = newButtonLayout->CreateControl<Text>(1, 1);
			velocityText->SetTextTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_POSITION_VELOCITY_TEXT);
			velocityText->SetText(L"Velocity:");

			std::shared_ptr<Text> velocityValueText = newButtonLayout->CreateControl<Text>(1, 2);
			velocityValueText->SetTextTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_POSITION_VELOCITY_TEXT);

			std::ostringstream velocityOSS;
			velocityOSS.precision(3);
			velocityOSS << std::fixed << "(" << atom->Velocity().x << ", " << atom->Velocity().y << ", " << atom->Velocity().z << ")";
			velocityValueText->SetText(velocityOSS.str());


			// Remove button with "X" in it to remove this listview item
			std::shared_ptr<Button> removeButton = newButtonLayout->CreateControl<Button>(0, 3, 2, 1);
			removeButton->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_REMOVE_BUTTON_COLOR);
			removeButton->SetBorderTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_REMOVE_BUTTON_BORDER);
			removeButton->Margin(5.0f, 10.0f);

			removeButton->Click(
				[weakListView = std::weak_ptr<ListView<Atom>>(listView),
				weakAtom = std::weak_ptr<Atom>(atom),
				weakPXSlider,
				weakPYSlider,
				weakPZSlider,
				weakVXSlider,
				weakVYSlider,
				weakVZSlider,
				weakComboBox]()
			{
				auto listView = weakListView.lock();
				auto atom = weakAtom.lock();
				auto pXSlider = weakPXSlider.lock();
				auto pYSlider = weakPYSlider.lock();
				auto pZSlider = weakPZSlider.lock();
				auto vXSlider = weakVXSlider.lock();
				auto vYSlider = weakVYSlider.lock();
				auto vZSlider = weakVZSlider.lock();
				auto comboBox = weakComboBox.lock();

				// Do not allow deleting of the last item
				if (listView->ItemCount() == 1)
					return;

				// Get the index of the current atom and whether or not it was highlighted
				int index = listView->ItemIndex(atom);
				bool isHighlighted = listView->IsItemHighlighted(atom);

				listView->RemoveItem(atom);
				SimulationManager::RemoveAtom(atom);

				// If the atom to be deleted was the highlighted one, we must try to re-select another atom
				if (isHighlighted)
				{
					// If the index is now beyond the range of the total atoms, decrement the index
					if (index == listView->ItemCount())
						--index;

					listView->HighlightItem(index);
					SimulationManager::SelectAtom(index);

					std::shared_ptr<Atom> selectedAtom = SimulationManager::GetSelectedAtom();

					// Update the position/velocity sliders						
					pXSlider->SetValue(selectedAtom->Position().x);
					pYSlider->SetValue(selectedAtom->Position().y);
					pZSlider->SetValue(selectedAtom->Position().z);
					vXSlider->SetValue(selectedAtom->Velocity().x);
					vYSlider->SetValue(selectedAtom->Velocity().y);
					vZSlider->SetValue(selectedAtom->Velocity().z);

					// Update the atom type comboBox
					switch (selectedAtom->ElementType())
					{
					case Element::HYDROGEN:		comboBox->SelectItem(L"Hydrogen"); break;
					case Element::HELIUM:		comboBox->SelectItem(L"Helium"); break;
					case Element::LITHIUM:		comboBox->SelectItem(L"Lithium"); break;
					case Element::BERYLLIUM:	comboBox->SelectItem(L"Beryllium"); break;
					case Element::BORON:		comboBox->SelectItem(L"Boron"); break;
					case Element::CARBON:		comboBox->SelectItem(L"Carbon"); break;
					case Element::NITROGEN:		comboBox->SelectItem(L"Nitrogen"); break;
					case Element::OXYGEN:		comboBox->SelectItem(L"Oxygen"); break;
					case Element::FLOURINE:		comboBox->SelectItem(L"Flourine"); break;
					case Element::NEON:			comboBox->SelectItem(L"Neon"); break;
					}
				}
			}
			);

			std::shared_ptr<Layout> removeButtonLayout = removeButton->GetLayout();
			std::shared_ptr<Text> removeGlyph = removeButtonLayout->CreateControl<Text>();
			removeGlyph->SetTextTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_REMOVE_BUTTON_GLYPH);
			removeGlyph->SetText(L"\xE711");

			return newLayout;
		}
		);
		atomListView->SetHighlightItemLayoutMethod(
			[](std::shared_ptr<Layout> layout)
			{
				std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(layout->GetChildControl(0));
				button->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_HIGHLIGHTED_COLOR);
			}
		);
		atomListView->SetUnhighlightItemLayoutMethod(
			[](std::shared_ptr<Layout> layout)
			{
				std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(layout->GetChildControl(0));
				button->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_COLOR);
			}
		);
		atomListView->SetItemClickMethod(
			[weakPXSlider = std::weak_ptr<Slider>(positionXSlider),
			weakPYSlider = std::weak_ptr<Slider>(positionYSlider),
			weakPZSlider = std::weak_ptr<Slider>(positionZSlider),
			weakVXSlider = std::weak_ptr<Slider>(velocityXSlider),
			weakVYSlider = std::weak_ptr<Slider>(velocityYSlider),
			weakVZSlider = std::weak_ptr<Slider>(velocityZSlider),
			weakComboBox = std::weak_ptr<ComboBox>(atomComboBox)](std::shared_ptr<Atom> atom)
		{
			auto pXSlider = weakPXSlider.lock();
			auto pYSlider = weakPYSlider.lock();
			auto pZSlider = weakPZSlider.lock();
			auto vXSlider = weakVXSlider.lock();
			auto vYSlider = weakVYSlider.lock();
			auto vZSlider = weakVZSlider.lock();
			auto comboBox = weakComboBox.lock();

			SimulationManager::SelectAtom(atom);

			// Update the position/velocity sliders
			pXSlider->SetValue(atom->Position().x);
			pYSlider->SetValue(atom->Position().y);
			pZSlider->SetValue(atom->Position().z);
			vXSlider->SetValue(atom->Velocity().x);
			vYSlider->SetValue(atom->Velocity().y);
			vZSlider->SetValue(atom->Velocity().z);

			// Update the atom type comboBox
			switch (atom->ElementType())
			{
			case Element::HYDROGEN:		comboBox->SelectItem(L"Hydrogen"); break;
			case Element::HELIUM:		comboBox->SelectItem(L"Helium"); break;
			case Element::LITHIUM:		comboBox->SelectItem(L"Lithium"); break;
			case Element::BERYLLIUM:	comboBox->SelectItem(L"Beryllium"); break;
			case Element::BORON:		comboBox->SelectItem(L"Boron"); break;
			case Element::CARBON:		comboBox->SelectItem(L"Carbon"); break;
			case Element::NITROGEN:		comboBox->SelectItem(L"Nitrogen"); break;
			case Element::OXYGEN:		comboBox->SelectItem(L"Oxygen"); break;
			case Element::FLOURINE:		comboBox->SelectItem(L"Flourine"); break;
			case Element::NEON:			comboBox->SelectItem(L"Neon"); break;
			}
		}
		);
		atomListView->SetValueChangedUpdateLayoutMethod(
			[](std::shared_ptr<Atom> atom, std::shared_ptr<Layout> layout)
			{
				std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(layout->GetChildControl(0));
				std::shared_ptr<Layout> buttonLayout = button->GetLayout();

				// Text of the atom type
				std::shared_ptr<Text> atomNameText = std::dynamic_pointer_cast<Text>(buttonLayout->GetChildControl(0));
				atomNameText->SetText(atom->Name());

				// Atom position values
				std::shared_ptr<Text> positionValueText = std::dynamic_pointer_cast<Text>(buttonLayout->GetChildControl(2));
				std::ostringstream positionOSS;
				positionOSS.precision(3);
				positionOSS << std::fixed << "(" << atom->Position().x << ", " << atom->Position().y << ", " << atom->Position().z << ")";
				positionValueText->SetText(positionOSS.str());

				// Atom velocity values
				std::shared_ptr<Text> velocityValueText = std::dynamic_pointer_cast<Text>(buttonLayout->GetChildControl(4));
				std::ostringstream velocityOSS;
				velocityOSS.precision(3);
				velocityOSS << std::fixed << "(" << atom->Velocity().x << ", " << atom->Velocity().y << ", " << atom->Velocity().z << ")";
				velocityValueText->SetText(velocityOSS.str());
			}
		);



		addAtomButton->Click(
			[weakListView = std::weak_ptr<ListView<Atom>>(atomListView),
			weakPXSlider = std::weak_ptr<Slider>(positionXSlider),
			weakPYSlider = std::weak_ptr<Slider>(positionYSlider),
			weakPZSlider = std::weak_ptr<Slider>(positionZSlider),
			weakVXSlider = std::weak_ptr<Slider>(velocityXSlider),
			weakVYSlider = std::weak_ptr<Slider>(velocityYSlider),
			weakVZSlider = std::weak_ptr<Slider>(velocityZSlider),
			weakComboBox = std::weak_ptr<ComboBox>(atomComboBox)]()
		{
			auto listView = weakListView.lock();
			auto pXSlider = weakPXSlider.lock();
			auto pYSlider = weakPYSlider.lock();
			auto pZSlider = weakPZSlider.lock();
			auto vXSlider = weakVXSlider.lock();
			auto vYSlider = weakVYSlider.lock();
			auto vZSlider = weakVZSlider.lock();
			auto comboBox = weakComboBox.lock();

			// Add a new hydrogen atom to the center of the simulation and let
			// that be the newly selected atom
			XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
			XMFLOAT3 velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
			SimulationManager::SelectAtom(SimulationManager::AddNewAtom<Hydrogen>(position, velocity));

			// Update the position/velocity sliders
			pXSlider->SetValue(0.0f);
			pYSlider->SetValue(0.0f);
			pZSlider->SetValue(0.0f);
			vXSlider->SetValue(0.0f);
			vYSlider->SetValue(0.0f);
			vZSlider->SetValue(0.0f);

			// Add the new atom to the list view
			// NOTE: Must add it to the list view BEFORE updating the combobox selected item because
			//       it will trigger an update to the listview item, which will cause an error because
			//       it hasn't been added yet
			listView->AddItem(SimulationManager::GetSelectedAtom());
			listView->HighlightItem(SimulationManager::GetSelectedAtom());

			// Update the Atom type combo box to display "Hydrogen"
			comboBox->SelectItem(L"Hydrogen");
		}
		);

		positionXSlider->ValueChanged(
			[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](float value)
		{
			SimulationManager::SelectedAtomPositionX(value);

			auto listView = weakListView.lock();
			listView->ItemChanged(SimulationManager::GetSelectedAtom());
		}
		);
		positionYSlider->ValueChanged(
			[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](float value)
		{
			SimulationManager::SelectedAtomPositionY(value);

			auto listView = weakListView.lock();
			listView->ItemChanged(SimulationManager::GetSelectedAtom());
		}
		);
		positionZSlider->ValueChanged(
			[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](float value)
		{
			SimulationManager::SelectedAtomPositionZ(value);

			auto listView = weakListView.lock();
			listView->ItemChanged(SimulationManager::GetSelectedAtom());
		}
		);
		velocityXSlider->ValueChanged(
			[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](float value)
		{
			SimulationManager::SelectedAtomVelocityX(value);

			auto listView = weakListView.lock();
			listView->ItemChanged(SimulationManager::GetSelectedAtom());
		}
		);
		velocityYSlider->ValueChanged(
			[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](float value)
		{
			SimulationManager::SelectedAtomVelocityY(value);

			auto listView = weakListView.lock();
			listView->ItemChanged(SimulationManager::GetSelectedAtom());
		}
		);
		velocityZSlider->ValueChanged(
			[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](float value)
		{
			SimulationManager::SelectedAtomVelocityZ(value);

			auto listView = weakListView.lock();
			listView->ItemChanged(SimulationManager::GetSelectedAtom());
		}
		);

		atomComboBox->SelectionChanged(
			[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](std::wstring value)
		{
			auto listView = weakListView.lock();
			int index = listView->ItemIndex(SimulationManager::GetSelectedAtom());

			if (value == L"Hydrogen")
				SimulationManager::ChangeSelectedAtomType<Hydrogen>();
			else if (value == L"Helium")
				SimulationManager::ChangeSelectedAtomType<Helium>();
			else if (value == L"Lithium")
				SimulationManager::ChangeSelectedAtomType<Lithium>();
			else if (value == L"Beryllium")
				SimulationManager::ChangeSelectedAtomType<Beryllium>();
			else if (value == L"Boron")
				SimulationManager::ChangeSelectedAtomType<Boron>();
			else if (value == L"Carbon")
				SimulationManager::ChangeSelectedAtomType<Carbon>();
			else if (value == L"Nitrogen")
				SimulationManager::ChangeSelectedAtomType<Nitrogen>();
			else if (value == L"Oxygen")
				SimulationManager::ChangeSelectedAtomType<Oxygen>();
			else if (value == L"Flourine")
				SimulationManager::ChangeSelectedAtomType<Flourine>();
			else if (value == L"Neon")
				SimulationManager::ChangeSelectedAtomType<Neon>();

			// Must completely replace the item instead of updating it because the 
			// SimulationManager::ChangeSelectedAtomType method will delete the existing
			// selected atom and replace it with a new one, which the listview does not 
			// know about
			listView->ReplaceItemAt(SimulationManager::GetSelectedAtom(), index);

			// Must make to re-select the new atom
			listView->HighlightItem(SimulationManager::GetSelectedAtom());
		}
		);


		// ============================================================================================================
		// ============================================================================================================
		// Add a Hydrogen Atom to the simulation
		SimulationManager::Pause();
		XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
		XMFLOAT3 velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
		std::shared_ptr<Hydrogen> firstAtom = SimulationManager::AddNewAtom<Hydrogen>(position, velocity);
		SimulationManager::SelectAtom(firstAtom);

		// Add the atom to the list view
		atomListView->AddItem(firstAtom);

		// Select the first item so the button is highlighted
		atomListView->HighlightItem(firstAtom);
	}

	void CreateQuickBar(const std::shared_ptr<ContentWindow>& window)
	{
		std::shared_ptr<Layout> mainLayout = window->GetLayout();

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
		newSimulationButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		std::shared_ptr<Text> newSimulationText = newSimulationButton->GetLayout()->CreateControl<Text>();
		newSimulationText->SetTextTheme(THEME_QUICK_BAR_GLYPH);
		newSimulationText->SetText(L"\xE710");

		// Open Simulation
		std::shared_ptr<Button> openSimulationButton = layout->CreateControl<Button>(0, 1);
		openSimulationButton->SetColorTheme(THEME_QUICK_BAR_BUTTON_COLOR);
		openSimulationButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		std::shared_ptr<Text> openSimulationText = openSimulationButton->GetLayout()->CreateControl<Text>();
		openSimulationText->SetTextTheme(THEME_QUICK_BAR_GLYPH);
		openSimulationText->SetText(L"\xE8E5");

		// Save Simulation
		std::shared_ptr<Button> saveSimulationButton = layout->CreateControl<Button>(0, 2);
		saveSimulationButton->SetColorTheme(THEME_QUICK_BAR_BUTTON_COLOR);
		saveSimulationButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		std::shared_ptr<Text> saveSimulationText = saveSimulationButton->GetLayout()->CreateControl<Text>();
		saveSimulationText->SetTextTheme(THEME_QUICK_BAR_GLYPH);
		saveSimulationText->SetText(L"\xE74E");

		// Play / Pause Simulation
		std::shared_ptr<Button> playSimulationButton = layout->CreateControl<Button>(0, 3);
		playSimulationButton->SetColorTheme(THEME_QUICK_BAR_BUTTON_COLOR);
		playSimulationButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		std::shared_ptr<Text> playSimulationText = playSimulationButton->GetLayout()->CreateControl<Text>();
		playSimulationText->SetTextTheme(THEME_QUICK_BAR_GLYPH);
		playSimulationText->SetText(L"\xE769");
	}

	void CreateRightPane(const std::shared_ptr<ContentWindow>& window)
	{
		std::shared_ptr<Layout> mainLayout = window->GetLayout();

		std::shared_ptr<Layout> rightPaneLayout = mainLayout->CreateSubLayout(2, 1);
	}
}