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

				// Pause the simulation - this will automatically update the play/pause button glyph
				SimulationManager::Pause();

				// Clear out the quick bar dynamic controls and add new controls
				DisplayNewSimulationQuickBarControls(window);

				// If there are any atoms in the existing simulation, prompt the user for
				// if they would like to save the simulation before continuing
				if (SimulationManager::AtomCount() > 0)
				{
					DisplaySaveSimulationPromptControls(window);
				}
				else
				{
					// Add a single hydrogen atom to the simulation
					XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
					XMFLOAT3 velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
					std::shared_ptr<Hydrogen> firstAtom = SimulationManager::AddNewAtom<Hydrogen>(position, velocity);
					SimulationManager::SelectAtom(firstAtom);

					// Create the UI controls to add/edit atoms
					DisplayAddAtomsControls(window);
				}
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

	void DisplayAddAtomsControls(const std::shared_ptr<ContentWindow>& window)
	{
		// Make sure the simulation is just in VIEW mode
		SimulationManager::SetUserState(UserState::VIEW);


		std::shared_ptr<Layout> layout = std::dynamic_pointer_cast<Layout>(window->GetLayout()->GetSubLayout(L"RightSideLayout"));
		assert(layout != nullptr);

		// Clear the layout of any previous content
		layout->Clear();

		// Create two buttons that split the right side pane in half vertically
		// Top button will house the "Add New Atom" button and the list view of all existing atoms
		// Bottom button will house the all other controls for editing the "Selected Atom"
		// We use buttons simply to be able to color the background - there will be no click functionality
		RowColDefinitions rowDefs;
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// Top Button
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 340.0f);	// TabbedPane w/ selected atom info
		layout->SetRowDefinitions(rowDefs);

		std::shared_ptr<Button> topBackdropButton = layout->CreateControl<Button>(0, 0);
		topBackdropButton->SetColorTheme(THEME_NEW_SIMULATION_BACKDROP_BUTTON_COLOR);
		topBackdropButton->SetBorderTheme(THEME_NEW_SIMULATION_BACKDROP_BUTTON_BORDER);
		topBackdropButton->Margin(5.0f, 0.0f, 5.0f, 5.0f);



		

		//std::shared_ptr<Button> bottomBackdropButton = layout->CreateControl<Button>(1, 0);
		//bottomBackdropButton->SetColorTheme(THEME_NEW_SIMULATION_BACKDROP_BUTTON_COLOR);
		//bottomBackdropButton->SetBorderTheme(THEME_NEW_SIMULATION_BACKDROP_BUTTON_BORDER);
		//bottomBackdropButton->Margin(5.0f, 0.0f, 5.0f, 5.0f);




		// Top Section ================================================================================
		// Create "Add Atom Button" and atoms listview in the top backdrop
		std::shared_ptr<Layout> topLayout = topBackdropButton->GetLayout();
		RowColDefinitions topRowDefs;
		topRowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 40.0f);		// Add atom button
		topRowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// atoms listview
		topLayout->SetRowDefinitions(topRowDefs);

		// Add atom button
		std::shared_ptr<Button> addAtomButton = topLayout->CreateControl<Button>(0, 0);
		addAtomButton->SetColorTheme(THEME_NEW_SIMULATION_ADD_BUTTON_COLOR);
		addAtomButton->SetBorderTheme(THEME_NEW_SIMULATION_ADD_BUTTON_BORDER);
		addAtomButton->Margin(20.0f, 5.0f);
		std::shared_ptr<Layout> addAtomButtonLayout = addAtomButton->GetLayout();
		std::shared_ptr<Text> addAtomButtonText = addAtomButtonLayout->CreateControl<Text>();
		addAtomButtonText->SetTextTheme(THEME_NEW_SIMULATION_ADD_BUTTON_TEXT);
		addAtomButtonText->SetText(L"Add New Atom");

		// Atom ListView
		std::shared_ptr<ListView<Atom>> atomListView = topLayout->CreateControl<ListView<Atom>>(1, 0);
		atomListView->SetItemHeight(40.0f);
		atomListView->SetHighlightItemLayoutMethod(
			[](std::shared_ptr<Layout> layout)
			{
				std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(layout->GetChildControl(0));
				if (button != nullptr)
					button->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_HIGHLIGHTED_COLOR);
			}
		);
		atomListView->SetUnhighlightItemLayoutMethod(
			[](std::shared_ptr<Layout> layout)
			{
				std::shared_ptr<Button> button = std::dynamic_pointer_cast<Button>(layout->GetChildControl(0));
				if (button != nullptr)
					button->SetColorTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_COLOR);
			}
		);
		atomListView->SetItemClickMethod(
			[](std::shared_ptr<Atom> atom)
			{
				SimulationManager::SelectAtom(atom);
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
		// Add each atom to the list view
		for (std::shared_ptr<Atom> atom : SimulationManager::Atoms())
			atomListView->AddItem(atom);

		// Highlight the selected atom
		atomListView->HighlightItem(SimulationManager::GetSelectedAtom());
		// atomListView->SetFormatFunction() - defined below after the sliders and combobox are created

		

		// addAtomButton->Click method defined here so we can capture the listview
		addAtomButton->Click(
			[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)]()
			{
				auto listView = weakListView.lock();

				// Add a new hydrogen atom to the center of the simulation and let
				// that be the newly selected atom
				XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
				XMFLOAT3 velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

				std::shared_ptr<Atom> newAtom = SimulationManager::AddNewAtom<Hydrogen>(position, velocity);

				// Add the atom to the list view BEFORE selecting it so it gets highlighted when the selection is made
				listView->AddItem(newAtom);

				SimulationManager::SelectAtom(newAtom);
			}
		);

		// END Top Section ================================================================================
		// 
		// Bottom Section =================================================================================
		// TabbedPane to hold different details about the selected atom
		std::shared_ptr<TabbedPane> selectedAtomControlsTabbedPane = layout->CreateControl<TabbedPane>(1, 0);
		selectedAtomControlsTabbedPane->SetNumberOfTabs(2);
		selectedAtomControlsTabbedPane->Margin(5.0f, 0.0f, 5.0f, 5.0f);
		selectedAtomControlsTabbedPane->SetTabRowHeight(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 20.0f);

		// Tab 1: Atom Info (Atom Type / Position / Velocity)
		std::shared_ptr<Layout> selectedAtomControlsTab1 = selectedAtomControlsTabbedPane->GetTabLayout(0);
		std::shared_ptr<Text> selectedAtomControlsTab1Text = selectedAtomControlsTab1->CreateControl<Text>();
		selectedAtomControlsTab1Text->SetTextTheme(THEME_NEW_SIMULATION_ADD_BUTTON_TEXT);
		selectedAtomControlsTab1Text->SetText(L"Atom Info");

		// Tab 2: Bond Info
		std::shared_ptr<Layout> selectedAtomControlsTab2 = selectedAtomControlsTabbedPane->GetTabLayout(1);
		std::shared_ptr<Text> selectedAtomControlsTab2Text = selectedAtomControlsTab2->CreateControl<Text>();
		selectedAtomControlsTab2Text->SetTextTheme(THEME_NEW_SIMULATION_ADD_BUTTON_TEXT);
		selectedAtomControlsTab2Text->SetText(L"Bond Info");

		// Pane 1: Atom Info (Atom Type / Position / Velocity)
		std::shared_ptr<Layout> selectedAtomControlsPane1 = selectedAtomControlsTabbedPane->GetPaneLayout(0);

		RowColDefinitions bottomLayoutRowDefs;
		bottomLayoutRowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 60.0f);		// Atom Type combo box
		bottomLayoutRowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 250.0f);	// Atom Position / Velocity Sliders
		selectedAtomControlsPane1->SetRowDefinitions(bottomLayoutRowDefs);

		// Sublayout for atom type combobox
		std::shared_ptr<Layout> atomTypeSubLayout = selectedAtomControlsPane1->CreateSubLayout(0, 0);
		RowColDefinitions atomTypeSublayoutColumns;
		atomTypeSublayoutColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 80.0f);	// Text "Element:"
		atomTypeSublayoutColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// Combobox with types
		atomTypeSubLayout->SetColumnDefinitions(atomTypeSublayoutColumns);

		// Text "Element:"
		std::shared_ptr<Text> atomTypeHeaderText = atomTypeSubLayout->CreateControl<Text>(0, 0);
		atomTypeHeaderText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		atomTypeHeaderText->SetText(L"Element:");
		atomTypeHeaderText->Margin(5.0f, 0.0f);

		// Combobox with elements
		std::shared_ptr<ComboBox> atomTypeComboBox = atomTypeSubLayout->CreateControl<ComboBox>(0, 1);
		atomTypeComboBox->AddComboBoxItem(ElementStrings[Element::HYDROGEN]);
		atomTypeComboBox->AddComboBoxItem(ElementStrings[Element::HELIUM]);
		atomTypeComboBox->AddComboBoxItem(ElementStrings[Element::LITHIUM]);
		atomTypeComboBox->AddComboBoxItem(ElementStrings[Element::BERYLLIUM]);
		atomTypeComboBox->AddComboBoxItem(ElementStrings[Element::BORON]);
		atomTypeComboBox->AddComboBoxItem(ElementStrings[Element::CARBON]);
		atomTypeComboBox->AddComboBoxItem(ElementStrings[Element::NITROGEN]);
		atomTypeComboBox->AddComboBoxItem(ElementStrings[Element::OXYGEN]);
		atomTypeComboBox->AddComboBoxItem(ElementStrings[Element::FLOURINE]);
		atomTypeComboBox->AddComboBoxItem(ElementStrings[Element::NEON]);
		atomTypeComboBox->Margin(0.0f, 11.0f, 10.0f, 10.0f);

		if (SimulationManager::GetSelectedAtom() != nullptr)
			atomTypeComboBox->SelectItem(ElementStrings[SimulationManager::GetSelectedAtom()->ElementType()]);

		// Sub Layout for editing position and velocity
		std::shared_ptr<Layout> atomPositionVelocitySubLayout = selectedAtomControlsPane1->CreateSubLayout(1, 0);

		RowColDefinitions positionVelocityColumns;
		positionVelocityColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 60.0f); // Header Text
		positionVelocityColumns.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);	// Slider control
		atomPositionVelocitySubLayout->SetColumnDefinitions(positionVelocityColumns);

		RowColDefinitions positionVelocityRows;
		//positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Box Dimensions:
		//positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Box Dimensions Slider
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Position:
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Position X
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Position Y
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Position Z
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 40.0f); // Velocity:
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Velocity X
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Velocity Y
		positionVelocityRows.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Velocity Z
		atomPositionVelocitySubLayout->SetRowDefinitions(positionVelocityRows);

		/*
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
		*/

		DirectX::XMFLOAT3 selectedAtomPosition = SimulationManager::GetSelectedAtom() == nullptr ? XMFLOAT3(0.0f, 0.0f, 0.0f) : SimulationManager::GetSelectedAtom()->Position();

		// Text for "Position"
		std::shared_ptr<Text> positionText = atomPositionVelocitySubLayout->CreateControl<Text>(0, 0);
		positionText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		positionText->SetText(L"Position");
		positionText->Margin(5.0f, 0.0f);

		// Text for X Position
		std::shared_ptr<Text> positionXText = atomPositionVelocitySubLayout->CreateControl<Text>(1, 0);
		positionXText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		positionXText->SetText(L"X:");
		positionXText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

		// Slider for X Position
		std::shared_ptr<Slider> positionXSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(1, 1);
		positionXSlider->SetMin(-1.0f);
		positionXSlider->SetMax(1.0f);
		positionXSlider->Margin(0.0f, 2.0f);
		positionXSlider->SetValue(selectedAtomPosition.x);

		// Text for Y Position
		std::shared_ptr<Text> positionYText = atomPositionVelocitySubLayout->CreateControl<Text>(2, 0);
		positionYText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		positionYText->SetText(L"Y:");
		positionYText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

		// Slider for Y Position
		std::shared_ptr<Slider> positionYSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(2, 1);
		positionYSlider->SetMin(-1.0f);
		positionYSlider->SetMax(1.0f);
		positionYSlider->Margin(0.0f, 2.0f);
		positionYSlider->SetValue(selectedAtomPosition.y);

		// Text for Z Position
		std::shared_ptr<Text> positionZText = atomPositionVelocitySubLayout->CreateControl<Text>(3, 0);
		positionZText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		positionZText->SetText(L"Z:");
		positionZText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

		// Slider for Z Position
		std::shared_ptr<Slider> positionZSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(3, 1);
		positionZSlider->SetMin(-1.0f);
		positionZSlider->SetMax(1.0f);
		positionZSlider->Margin(0.0f, 2.0f);
		positionZSlider->SetValue(selectedAtomPosition.z);

		/*
		// Now that all the position sliders have been created, we can 
		// reference them in the lambda for the box dimension slider
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
		*/

		DirectX::XMFLOAT3 selectedAtomVelocity = SimulationManager::GetSelectedAtom() == nullptr ? XMFLOAT3(0.0f, 0.0f, 0.0f) : SimulationManager::GetSelectedAtom()->Velocity();

		// Text for "Velocity"
		std::shared_ptr<Text> velocityText = atomPositionVelocitySubLayout->CreateControl<Text>(4, 0);
		velocityText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		velocityText->SetText(L"Velocity");
		velocityText->Margin(5.0f, 10.0f, 5.0f, 0.0f);

		// Text for X Velocity
		std::shared_ptr<Text> velocityXText = atomPositionVelocitySubLayout->CreateControl<Text>(5, 0);
		velocityXText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		velocityXText->SetText(L"X:");
		velocityXText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

		// Slider for X Velocity
		std::shared_ptr<Slider> velocityXSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(5, 1);
		velocityXSlider->SetMin(-100.0f);
		velocityXSlider->SetMax(100.0f);
		velocityXSlider->Margin(0.0f, 2.0f);
		velocityXSlider->SetValue(selectedAtomVelocity.x);

		// Text for Y Velocity
		std::shared_ptr<Text> velocityYText = atomPositionVelocitySubLayout->CreateControl<Text>(6, 0);
		velocityYText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		velocityYText->SetText(L"Y:");
		velocityYText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

		// Slider for Y Velocity
		std::shared_ptr<Slider> velocityYSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(6, 1);
		velocityYSlider->SetMin(-100.0f);
		velocityYSlider->SetMax(100.0f);
		velocityYSlider->Margin(0.0f, 2.0f);
		velocityYSlider->SetValue(selectedAtomVelocity.y);

		// Text for Z Velocity
		std::shared_ptr<Text> velocityZText = atomPositionVelocitySubLayout->CreateControl<Text>(7, 0);
		velocityZText->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		velocityZText->SetText(L"Z:");
		velocityZText->Margin(40.0f, 0.0f, 0.0f, 0.0f);

		// Slider for Z Velocity
		std::shared_ptr<Slider> velocityZSlider = atomPositionVelocitySubLayout->CreateControl<Slider>(7, 1);
		velocityZSlider->SetMin(-100.0f);
		velocityZSlider->SetMax(100.0f);
		velocityZSlider->Margin(0.0f, 2.0f);
		velocityZSlider->SetValue(selectedAtomVelocity.z);


		// Pane 2: Bond Info
		std::shared_ptr<Layout> selectedAtomControlsPane2 = selectedAtomControlsTabbedPane->GetPaneLayout(1);
		std::shared_ptr<Text> selectedAtomControlsPane2Text = selectedAtomControlsPane2->CreateControl<Text>();
		selectedAtomControlsPane2Text->SetTextTheme(THEME_NEW_SIMULATION_ADD_BUTTON_TEXT);
		selectedAtomControlsPane2Text->SetText(L"Bond Info");

		// END Bottom Section =============================================================================
		//
		// Set callback functions for Top Section that rely on Bottom Section Controls ====================

		atomListView->SetFormatFunction(
			[weakDeviceResources = std::weak_ptr<DeviceResources>(layout->GetDeviceResources()),
			weakListView = std::weak_ptr<ListView<Atom>>(atomListView),
			weakPXSlider = std::weak_ptr<Slider>(positionXSlider),
			weakPYSlider = std::weak_ptr<Slider>(positionYSlider),
			weakPZSlider = std::weak_ptr<Slider>(positionZSlider),
			weakVXSlider = std::weak_ptr<Slider>(velocityXSlider),
			weakVYSlider = std::weak_ptr<Slider>(velocityYSlider),
			weakVZSlider = std::weak_ptr<Slider>(velocityZSlider),
			weakComboBox = std::weak_ptr<ComboBox>(atomTypeComboBox)](std::shared_ptr<Atom> atom, bool highlighted)
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
				// int index = listView->ItemIndex(atom);
				// bool isHighlighted = listView->IsItemHighlighted(atom);

				listView->RemoveItem(atom);

				// Removing the atom from the simulation will also delete any bonds associated with the atom
				SimulationManager::RemoveAtom(atom);
			}
			);

			std::shared_ptr<Layout> removeButtonLayout = removeButton->GetLayout();
			std::shared_ptr<Text> removeGlyph = removeButtonLayout->CreateControl<Text>();
			removeGlyph->SetTextTheme(THEME_NEW_SIMULATION_ATOM_LISTVIEW_REMOVE_BUTTON_GLYPH);
			removeGlyph->SetText(L"\xE711");

			return newLayout;
		}
		);

		// END Set callback functions for Top Section that rely on Bottom Section Controls ====================
		//
		// Set callback functions for Bottom Section that rely on Top Section Controls ====================

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

		atomTypeComboBox->SelectionChanged(
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



		// Assign the Simulation Atom click event function
		SimulationManager::SetAtomClickedEvent(
			[weakListView = std::weak_ptr<ListView<Atom>>(atomListView)](std::shared_ptr<Atom> atom)
		{
			auto listView = weakListView.lock();

			SimulationManager::SelectAtom(atom);
			// listView->HighlightItem(atom);





			// MUST update sliders and combobox.............

		}
		);

		// Whenever the selected atom changes, update the sliders, combobox, and listview
		SimulationManager::SetSelectedAtomChangedEvent(
			[weakListView = std::weak_ptr<ListView<Atom>>(atomListView),
			weakPXSlider = std::weak_ptr<Slider>(positionXSlider),
			weakPYSlider = std::weak_ptr<Slider>(positionYSlider),
			weakPZSlider = std::weak_ptr<Slider>(positionZSlider),
			weakVXSlider = std::weak_ptr<Slider>(velocityXSlider),
			weakVYSlider = std::weak_ptr<Slider>(velocityYSlider),
			weakVZSlider = std::weak_ptr<Slider>(velocityZSlider),
			weakComboBox = std::weak_ptr<ComboBox>(atomTypeComboBox)](std::shared_ptr<Atom> atom)
		{
			auto listView = weakListView.lock();
			auto pXSlider = weakPXSlider.lock();
			auto pYSlider = weakPYSlider.lock();
			auto pZSlider = weakPZSlider.lock();
			auto vXSlider = weakVXSlider.lock();
			auto vYSlider = weakVYSlider.lock();
			auto vZSlider = weakVZSlider.lock();
			auto comboBox = weakComboBox.lock();

			XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
			XMFLOAT3 velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);

			if (atom == nullptr)
			{
				comboBox->SelectItem(L"Hydrogen");
			}
			else
			{
				position = atom->Position();
				velocity = atom->Velocity();

				// Update the Atom type combo box to display the correct element type
				comboBox->SelectItem(ElementStrings[atom->ElementType()]);
			}

			// Update the position/velocity sliders
			pXSlider->SetValue(position.x);
			pYSlider->SetValue(position.y);
			pZSlider->SetValue(position.z);
			vXSlider->SetValue(velocity.x);
			vYSlider->SetValue(velocity.y);
			vZSlider->SetValue(velocity.z);

			// Highlight the correct atom in the listview
			listView->HighlightItem(atom);
		}
		);



		// Must set a layout cleared event for the right pane layout
		// If the layout is cleared, then we must undo the SimulationManager::AtomClickEvent
		// because the list view has been destroyed
		layout->SetLayoutClearedEvent([]()
			{
				SimulationManager::SetAtomClickedEvent([](std::shared_ptr<Atom> atom) {});
				SimulationManager::SetSelectedAtomChangedEvent([](std::shared_ptr<Atom> atom) {});
			}
		);
	}
	
	void DisplayAddMoleculeControls(const std::shared_ptr<ContentWindow>& window)
	{
		// Make sure the simulation is just in VIEW mode
		SimulationManager::SetUserState(UserState::VIEW);


		std::shared_ptr<Layout> layout = std::dynamic_pointer_cast<Layout>(window->GetLayout()->GetSubLayout(L"RightSideLayout"));
		assert(layout != nullptr);

		// Clear the layout of any previous content
		layout->Clear();

		std::shared_ptr<Text> text = layout->CreateControl<Text>();
		text->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		text->SetText(L"Display message to click on an atom, which should populate all atom/molecule details");





	}
	void DisplayCreateBondControls(const std::shared_ptr<ContentWindow>& window)
	{
		// Make sure the simulation is just in EDIT_BONDS mode
		SimulationManager::SetUserState(UserState::EDIT_BONDS);

		std::shared_ptr<Layout> layout = std::dynamic_pointer_cast<Layout>(window->GetLayout()->GetSubLayout(L"RightSideLayout"));
		assert(layout != nullptr);

		// Clear the layout of any previous content
		layout->Clear();

		RowColDefinitions rowDefs;
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 70.0f);	// Instructions text
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);	// Sublayout for selected bond info
		layout->SetRowDefinitions(rowDefs);

		std::shared_ptr<Text> instructions = layout->CreateControl<Text>(0, 0);
		instructions->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_INSTRUCTIONS_TEXT);
		instructions->SetText(L"To create a bond, click on an atom and drag to another atom. To edit, click on a bond.");

		std::shared_ptr<Layout> bondInfoLayout = layout->CreateSubLayout(1, 0);




		SimulationManager::SetSelectedBondChangedEvent(
			[weakLayout = std::weak_ptr<Layout>(bondInfoLayout)](std::shared_ptr<Bond> bond) 
			{
				auto layout = weakLayout.lock();
				layout->Clear();

				float rowHeight = 40.0f;

				RowColDefinitions rowDefs;
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, rowHeight);	// "Bond"
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, rowHeight);	// "Length: "
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, rowHeight);	// "Equilibrium Length: "
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, rowHeight); // "Spring Constant: "
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, rowHeight);	// "Atom 1"
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, rowHeight);	// Atom 1 element type
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, rowHeight);	// Atom 1 position
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, rowHeight);	// Atom 1 velocity
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, rowHeight);	// Atom 1 charge
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, rowHeight);	// "Atom 2
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, rowHeight);	// Atom 2 element type
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, rowHeight);	// Atom 2 position
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, rowHeight);	// Atom 2 velocity
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, rowHeight);	// Atom 2 charge
				rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		//
				layout->SetRowDefinitions(rowDefs);

				// Bond Header ===================================================================
				std::shared_ptr<Text> bondText = layout->CreateControl<Text>(0, 0);

				if (bond == nullptr)
				{
					bondText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_INSTRUCTIONS_TEXT);
					bondText->SetText(L"No bond selected");
					return;
				}
				
				bondText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				bondText->Margin(20.0f, 0.0f, 0.0f, 0.0f);
				bondText->SetText(L"Bond");
				
				// Bond Length ===================================================================
				std::shared_ptr<Layout> bondLengthSublayout = layout->CreateSubLayout(1, 0);
				RowColDefinitions bondLengthSublayoutColumnDefs;
				bondLengthSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 100.0f);	// "Length:"
				bondLengthSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// value
				bondLengthSublayout->SetColumnDefinitions(bondLengthSublayoutColumnDefs);

				std::shared_ptr<Text> bondLengthText = bondLengthSublayout->CreateControl<Text>(0, 0);
				bondLengthText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				bondLengthText->Margin(20.0f, 0.0f, 0.0f, 0.0f);
				bondLengthText->SetText(L"Length:");

				std::shared_ptr<Text> bondLengthValue = bondLengthSublayout->CreateControl<Text>(0, 1);
				bondLengthValue->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				bondLengthValue->SetText(std::to_wstring(bond->BondLength()));

				// Equilibrium Length ============================================================
				std::shared_ptr<Layout> equilibriumLengthSublayout = layout->CreateSubLayout(2, 0);
				RowColDefinitions equilibriumLengthSublayoutColumnDefs;
				equilibriumLengthSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 100.0f);	// "Equilibrium Length:"
				equilibriumLengthSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// value
				equilibriumLengthSublayout->SetColumnDefinitions(equilibriumLengthSublayoutColumnDefs);

				std::shared_ptr<Text> equilibriumLengthText = equilibriumLengthSublayout->CreateControl<Text>(0, 0);
				equilibriumLengthText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				equilibriumLengthText->Margin(20.0f, 0.0f, 0.0f, 0.0f);
				equilibriumLengthText->SetText(L"Equilibrium Length:");

				std::shared_ptr<Text> equilibriumLengthValue = equilibriumLengthSublayout->CreateControl<Text>(0, 1);
				equilibriumLengthValue->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				//equilibriumLengthValue->SetText(std::to_wstring(bond->EquilibriumLength()));
				equilibriumLengthValue->SetText(L"Not implemented");

				// Spring Constant ===============================================================
				std::shared_ptr<Layout> springConstantSublayout = layout->CreateSubLayout(3, 0);
				RowColDefinitions springConstantSublayoutColumnDefs;
				springConstantSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 100.0f);	// "Spring Constant:"
				springConstantSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// value
				springConstantSublayout->SetColumnDefinitions(springConstantSublayoutColumnDefs);

				std::shared_ptr<Text> springConstantText = springConstantSublayout->CreateControl<Text>(0, 0);
				springConstantText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				springConstantText->Margin(20.0f, 0.0f, 0.0f, 0.0f);
				springConstantText->SetText(L"Spring Constant:");

				std::shared_ptr<Text> springConstantValue = springConstantSublayout->CreateControl<Text>(0, 1);
				springConstantValue->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				springConstantValue->SetText(std::to_wstring(bond->SpringConstant()));

				// Atom 1 Header =================================================================
				std::shared_ptr<Text> atom1HeaderText = layout->CreateControl<Text>(4, 0);
				atom1HeaderText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				atom1HeaderText->Margin(20.0f, 0.0f, 0.0f, 0.0f);
				atom1HeaderText->SetText(L"Atom 1:");

				// Atom 1 Element type ===========================================================
				std::shared_ptr<Layout> atom1ElementTypeSublayout = layout->CreateSubLayout(5, 0);
				RowColDefinitions atom1ElementTypeSublayoutColumnDefs;
				atom1ElementTypeSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// Element type
				atom1ElementTypeSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 100.0f);	// Select Button
				atom1ElementTypeSublayout->SetColumnDefinitions(atom1ElementTypeSublayoutColumnDefs);

				std::shared_ptr<Text> atom1ElementTypeText = atom1ElementTypeSublayout->CreateControl<Text>(0, 0);
				atom1ElementTypeText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				atom1ElementTypeText->Margin(50.0f, 0.0f, 0.0f, 0.0f);
				atom1ElementTypeText->SetText(ElementStrings[bond->Atom1()->ElementType()]);

				std::shared_ptr<Button> atom1ElementSelectButton = atom1ElementTypeSublayout->CreateControl<Button>(0, 1);
				atom1ElementSelectButton->SetColorTheme(THEME_NEW_SIMULATION_CREATE_BONDS_SELECT_ATOM_BUTTON_COLOR);
				atom1ElementSelectButton->SetBorderTheme(THEME_NEW_SIMULATION_CREATE_BONDS_SELECT_ATOM_BUTTON_BORDER);
				atom1ElementSelectButton->Margin(10.0f, 10.0f);

				std::shared_ptr<Layout> atom1ElementSelectButtonLayout = atom1ElementSelectButton->GetLayout();
				std::shared_ptr<Text> atom1SelectButtonText = atom1ElementSelectButtonLayout->CreateControl<Text>(0, 0);
				atom1SelectButtonText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_SELECT_ATOM_BUTTON_TEXT);
				atom1SelectButtonText->SetText(L"Select");

				// Atom 1 position ===============================================================
				std::shared_ptr<Layout> atom1PositionSublayout = layout->CreateSubLayout(6, 0);
				RowColDefinitions atom1PositionSublayoutColumnDefs;
				atom1PositionSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 150.0f);	// "Position:"
				atom1PositionSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// value
				atom1PositionSublayout->SetColumnDefinitions(atom1PositionSublayoutColumnDefs);

				std::shared_ptr<Text> atom1PositionText = atom1PositionSublayout->CreateControl<Text>(0, 0);
				atom1PositionText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				atom1PositionText->Margin(50.0f, 0.0f, 0.0f, 0.0f);
				atom1PositionText->SetText(L"Position:");

				std::shared_ptr<Text> atom1PositionValue = atom1PositionSublayout->CreateControl<Text>(0, 1);
				atom1PositionValue->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				XMFLOAT3 atom1Position = bond->Atom1()->Position();
				std::ostringstream atom1PositionOSS;
				atom1PositionOSS.precision(3);
				atom1PositionOSS << std::fixed << "(" << atom1Position.x << ", " << atom1Position.y << ", " << atom1Position.z << ")";
				atom1PositionValue->SetText(atom1PositionOSS.str());

				// Atom 1 velocity ===============================================================
				std::shared_ptr<Layout> atom1VelocitySublayout = layout->CreateSubLayout(7, 0);
				RowColDefinitions atom1VelocitySublayoutColumnDefs;
				atom1VelocitySublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 150.0f);	// "Velocity:"
				atom1VelocitySublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// value
				atom1VelocitySublayout->SetColumnDefinitions(atom1VelocitySublayoutColumnDefs);

				std::shared_ptr<Text> atom1VelocityText = atom1VelocitySublayout->CreateControl<Text>(0, 0);
				atom1VelocityText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				atom1VelocityText->Margin(50.0f, 0.0f, 0.0f, 0.0f);
				atom1VelocityText->SetText(L"Velocity:");

				std::shared_ptr<Text> atom1VelocityValue = atom1VelocitySublayout->CreateControl<Text>(0, 1);
				atom1VelocityValue->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				XMFLOAT3 atom1Velocity = bond->Atom1()->Velocity();
				std::ostringstream atom1VelocityOSS;
				atom1VelocityOSS.precision(3);
				atom1VelocityOSS << std::fixed << "(" << atom1Velocity.x << ", " << atom1Velocity.y << ", " << atom1Velocity.z << ")";
				atom1VelocityValue->SetText(atom1VelocityOSS.str());

				// Atom 1 charge =================================================================
				std::shared_ptr<Layout> atom1ChargeSublayout = layout->CreateSubLayout(8, 0);
				RowColDefinitions atom1ChargeSublayoutColumnDefs;
				atom1ChargeSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 100.0f);	// "Charge:"
				atom1ChargeSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);	// value
				atom1ChargeSublayout->SetColumnDefinitions(atom1ChargeSublayoutColumnDefs);

				std::shared_ptr<Text> atom1ChargeText = atom1ChargeSublayout->CreateControl<Text>(0, 0);
				atom1ChargeText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				atom1ChargeText->Margin(50.0f, 0.0f, 0.0f, 0.0f);
				atom1ChargeText->SetText(L"Charge:");

				std::shared_ptr<Text> atom1ChargeValue = atom1ChargeSublayout->CreateControl<Text>(0, 1);
				atom1ChargeValue->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				atom1ChargeValue->SetText(std::to_wstring(bond->Atom1()->Charge()));

				// Atom 2 Header =================================================================
				std::shared_ptr<Text> atom2HeaderText = layout->CreateControl<Text>(9, 0);
				atom2HeaderText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				atom2HeaderText->Margin(20.0f, 0.0f, 0.0f, 0.0f);
				atom2HeaderText->SetText(L"Atom 2:");

				// Atom 2 Element type ===========================================================
				std::shared_ptr<Layout> atom2ElementTypeSublayout = layout->CreateSubLayout(10, 0);
				RowColDefinitions atom2ElementTypeSublayoutColumnDefs;
				atom2ElementTypeSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// Element type
				atom2ElementTypeSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 100.0f);	// Select Button
				atom2ElementTypeSublayout->SetColumnDefinitions(atom2ElementTypeSublayoutColumnDefs);

				std::shared_ptr<Text> atom2ElementTypeText = atom2ElementTypeSublayout->CreateControl<Text>(0, 0);
				atom2ElementTypeText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				atom2ElementTypeText->Margin(50.0f, 0.0f, 0.0f, 0.0f);
				atom2ElementTypeText->SetText(ElementStrings[bond->Atom2()->ElementType()]);

				std::shared_ptr<Button> atom2ElementSelectButton = atom2ElementTypeSublayout->CreateControl<Button>(0, 1);
				atom2ElementSelectButton->SetColorTheme(THEME_NEW_SIMULATION_CREATE_BONDS_SELECT_ATOM_BUTTON_COLOR);
				atom2ElementSelectButton->SetBorderTheme(THEME_NEW_SIMULATION_CREATE_BONDS_SELECT_ATOM_BUTTON_BORDER);
				atom2ElementSelectButton->Margin(10.0f, 10.0f);

				std::shared_ptr<Layout> atom2ElementSelectButtonLayout = atom2ElementSelectButton->GetLayout();
				std::shared_ptr<Text> atom2SelectButtonText = atom2ElementSelectButtonLayout->CreateControl<Text>(0, 0);
				atom2SelectButtonText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_SELECT_ATOM_BUTTON_TEXT);
				atom2SelectButtonText->SetText(L"Select");

				// Atom 2 position ===============================================================
				std::shared_ptr<Layout> atom2PositionSublayout = layout->CreateSubLayout(11, 0);
				RowColDefinitions atom2PositionSublayoutColumnDefs;
				atom2PositionSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 150.0f);	// "Position:"
				atom2PositionSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// value
				atom2PositionSublayout->SetColumnDefinitions(atom2PositionSublayoutColumnDefs);

				std::shared_ptr<Text> atom2PositionText = atom2PositionSublayout->CreateControl<Text>(0, 0);
				atom2PositionText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				atom2PositionText->Margin(50.0f, 0.0f, 0.0f, 0.0f);
				atom2PositionText->SetText(L"Position:");

				std::shared_ptr<Text> atom2PositionValue = atom2PositionSublayout->CreateControl<Text>(0, 1);
				atom2PositionValue->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				XMFLOAT3 atom2Position = bond->Atom2()->Position();
				std::ostringstream atom2PositionOSS;
				atom2PositionOSS.precision(3);
				atom2PositionOSS << std::fixed << "(" << atom2Position.x << ", " << atom2Position.y << ", " << atom2Position.z << ")";
				atom2PositionValue->SetText(atom2PositionOSS.str());

				// Atom 2 velocity ===============================================================
				std::shared_ptr<Layout> atom2VelocitySublayout = layout->CreateSubLayout(12, 0);
				RowColDefinitions atom2VelocitySublayoutColumnDefs;
				atom2VelocitySublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 150.0f);	// "Velocity:"
				atom2VelocitySublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// value
				atom2VelocitySublayout->SetColumnDefinitions(atom2VelocitySublayoutColumnDefs);

				std::shared_ptr<Text> atom2VelocityText = atom2VelocitySublayout->CreateControl<Text>(0, 0);
				atom2VelocityText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				atom2VelocityText->Margin(50.0f, 0.0f, 0.0f, 0.0f);
				atom2VelocityText->SetText(L"Velocity:");

				std::shared_ptr<Text> atom2VelocityValue = atom2VelocitySublayout->CreateControl<Text>(0, 1);
				atom2VelocityValue->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				XMFLOAT3 atom2Velocity = bond->Atom2()->Velocity();
				std::ostringstream atom2VelocityOSS;
				atom2VelocityOSS.precision(3);
				atom2VelocityOSS << std::fixed << "(" << atom2Velocity.x << ", " << atom2Velocity.y << ", " << atom2Velocity.z << ")";
				atom2VelocityValue->SetText(atom2VelocityOSS.str());

				// Atom 2 charge =================================================================
				std::shared_ptr<Layout> atom2ChargeSublayout = layout->CreateSubLayout(13, 0);
				RowColDefinitions atom2ChargeSublayoutColumnDefs;
				atom2ChargeSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 100.0f);	// "Charge:"
				atom2ChargeSublayoutColumnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);	// value
				atom2ChargeSublayout->SetColumnDefinitions(atom2ChargeSublayoutColumnDefs);

				std::shared_ptr<Text> atom2ChargeText = atom2ChargeSublayout->CreateControl<Text>(0, 0);
				atom2ChargeText->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				atom2ChargeText->Margin(50.0f, 0.0f, 0.0f, 0.0f);
				atom2ChargeText->SetText(L"Charge:");

				std::shared_ptr<Text> atom2ChargeValue = atom2ChargeSublayout->CreateControl<Text>(0, 1);
				atom2ChargeValue->SetTextTheme(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT);
				atom2ChargeValue->SetText(std::to_wstring(bond->Atom2()->Charge()));



			}
		);

	}
	void DisplayEditVelocityArrowsControls(const std::shared_ptr<ContentWindow>& window)
	{
		// Make sure the simulation is just in VIEW mode
		SimulationManager::SetUserState(UserState::VIEW);


		std::shared_ptr<Layout> layout = std::dynamic_pointer_cast<Layout>(window->GetLayout()->GetSubLayout(L"RightSideLayout"));
		assert(layout != nullptr);

		// Clear the layout of any previous content
		layout->Clear();

		RowColDefinitions rowDefs;
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 40.0f);	// Show All Velocity Arrows Button
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 40.0f);	// Hide All Velocity Arrows Button
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 100.0f);	// Text - "Click on atoms to show/hide velocity arrows"
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);		// 
		layout->SetRowDefinitions(rowDefs);

		// Show all arrows button
		std::shared_ptr<Button> showAllArrowsButton = layout->CreateControl<Button>(0, 0);
		showAllArrowsButton->SetColorTheme(THEME_NEW_SIMULATION_EDIT_VELOCITY_ARROWS_BUTTON_COLOR);
		showAllArrowsButton->SetBorderTheme(THEME_NEW_SIMULATION_EDIT_VELOCITY_ARROWS_BUTTON_BORDER);
		showAllArrowsButton->Margin(20.0f, 5.0f);
		showAllArrowsButton->Click([]() {
			SimulationManager::ShowAllVelocityArrows();
			});
		std::shared_ptr<Text> showAllArrowsButtonText = showAllArrowsButton->GetLayout()->CreateControl<Text>();
		showAllArrowsButtonText->SetTextTheme(THEME_NEW_SIMULATION_EDIT_VELOCITY_ARROWS_BUTTON_TEXT);
		showAllArrowsButtonText->SetText(L"Show all velocity arrows");

		// Hide all arrows button
		std::shared_ptr<Button> hideAllArrowsButton = layout->CreateControl<Button>(1, 0);
		hideAllArrowsButton->SetColorTheme(THEME_NEW_SIMULATION_EDIT_VELOCITY_ARROWS_BUTTON_COLOR);
		hideAllArrowsButton->SetBorderTheme(THEME_NEW_SIMULATION_EDIT_VELOCITY_ARROWS_BUTTON_BORDER);
		hideAllArrowsButton->Margin(20.0f, 5.0f);
		hideAllArrowsButton->Click([]() {
			SimulationManager::HideAllVelocityArrows();
			});
		std::shared_ptr<Text> hideAllArrowsButtonText = hideAllArrowsButton->GetLayout()->CreateControl<Text>();
		hideAllArrowsButtonText->SetTextTheme(THEME_NEW_SIMULATION_EDIT_VELOCITY_ARROWS_BUTTON_TEXT);
		hideAllArrowsButtonText->SetText(L"Hide all velocity arrows");

		// Text telling the user they can just click on atoms to show/hide arrows
		std::shared_ptr<Text> instructionsText = layout->CreateControl<Text>(2, 0);
		instructionsText->SetTextTheme(THEME_NEW_SIMULATION_EDIT_VELOCITY_ARROWS_BUTTON_TEXT);
		instructionsText->Margin(20, 0);
		instructionsText->SetText(L"Click on individual atoms to show/hide velocity arrows.");

	}
	void DisplayResetStateControls(const std::shared_ptr<ContentWindow>& window)
	{
		// Make sure the simulation is just in VIEW mode
		SimulationManager::SetUserState(UserState::VIEW);


		std::shared_ptr<Layout> layout = std::dynamic_pointer_cast<Layout>(window->GetLayout()->GetSubLayout(L"RightSideLayout"));
		assert(layout != nullptr);

		// Clear the layout of any previous content
		layout->Clear();

		std::shared_ptr<Text> text = layout->CreateControl<Text>();
		text->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		text->SetText(L"Reset State");





	}
	void DisplaySimulationPlayingControls(const std::shared_ptr<ContentWindow>& window)
	{
		// Make sure the simulation is just in VIEW mode
		SimulationManager::SetUserState(UserState::VIEW);


		std::shared_ptr<Layout> layout = std::dynamic_pointer_cast<Layout>(window->GetLayout()->GetSubLayout(L"RightSideLayout"));
		assert(layout != nullptr);

		// Clear the layout of any previous content
		layout->Clear();

		std::shared_ptr<Text> text = layout->CreateControl<Text>();
		text->SetTextTheme(THEME_NEW_SIMULATION_TEXT);
		text->SetText(L"Simulation is Playing...");
	}


	void DisplayNewSimulationQuickBarControls(const std::shared_ptr<ContentWindow>& window)
	{
		std::shared_ptr<Layout> layout = std::dynamic_pointer_cast<Layout>(window->GetLayout()->GetSubLayout(L"QuickBarDynamicControlsLayout"));
		assert(layout != nullptr);

		RowColDefinitions columnDefs;
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 110.0f);	// Add atoms
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 110.0f);	// Add molecules
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 110.0f);	// Create Bond
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 110.0f);	// Edit Velocity Arrows
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 110.0f);	// Reset State
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);
		layout->SetColumnDefinitions(columnDefs);

		// Add Atom Button
		std::shared_ptr<Button> addAtomButton = layout->CreateControl<Button>(0, 0);
		addAtomButton->SetColorTheme(THEME_QUICK_BAR_BUTTON_COLOR);
		addAtomButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		addAtomButton->Click(
			[weakWindow = std::weak_ptr<ContentWindow>(window)]() 
		{
			auto window = weakWindow.lock();

			// Pause the simulation first as it does not make sense to add an atom while the simulation is playing
			SimulationManager::Pause();

			// Just run the display add atoms function
			DisplayAddAtomsControls(window);
		});

		std::shared_ptr<Text> addAtomButtonText = addAtomButton->GetLayout()->CreateControl<Text>();
		addAtomButtonText->SetTextTheme(THEME_QUICK_BAR_TEXT);
		addAtomButtonText->SetText(L"Add Atoms");

		// Add Molecules Button
		std::shared_ptr<Button> addMoleculesButton = layout->CreateControl<Button>(0, 1);
		addMoleculesButton->SetColorTheme(THEME_QUICK_BAR_BUTTON_COLOR);
		addMoleculesButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		addMoleculesButton->Click(
			[weakWindow = std::weak_ptr<ContentWindow>(window)]()
		{
			auto window = weakWindow.lock();

			// Pause the simulation first as it does not make sense to add a molecule while the simulation is playing
			SimulationManager::Pause();

			// Just run the display add molecule function
			DisplayAddMoleculeControls(window);
		});

		std::shared_ptr<Text> addMoleculesButtonText = addMoleculesButton->GetLayout()->CreateControl<Text>();
		addMoleculesButtonText->SetTextTheme(THEME_QUICK_BAR_TEXT);
		addMoleculesButtonText->SetText(L"Add Molecules");


		// Create Bond Button
		std::shared_ptr<Button> createBondButton = layout->CreateControl<Button>(0, 2);
		createBondButton->SetColorTheme(THEME_QUICK_BAR_BUTTON_COLOR);
		createBondButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		createBondButton->Click(
			[weakWindow = std::weak_ptr<ContentWindow>(window)]()
		{
			auto window = weakWindow.lock();

			// Pause the simulation first as it does not make sense to create a bond while the simulation is playing
			SimulationManager::Pause();

			// Just run the display add molecule function
			DisplayCreateBondControls(window);
		});

		std::shared_ptr<Text> createBondButtonText = createBondButton->GetLayout()->CreateControl<Text>();
		createBondButtonText->SetTextTheme(THEME_QUICK_BAR_TEXT);
		createBondButtonText->SetText(L"Create Bond");


		// Edit Velocity Arrows
		std::shared_ptr<Button> editVelocityArrowsButton = layout->CreateControl<Button>(0, 3);
		editVelocityArrowsButton->SetColorTheme(THEME_QUICK_BAR_BUTTON_COLOR);
		editVelocityArrowsButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		editVelocityArrowsButton->Click(
			[weakWindow = std::weak_ptr<ContentWindow>(window)]()
		{
			auto window = weakWindow.lock();

			// Pause the simulation first as it will be generally easier to visualize which atoms get velocity arrows
			SimulationManager::Pause();

			// Just run the display add molecule function
			DisplayEditVelocityArrowsControls(window);

			// Modify the Atom Clicked event to show/hide velocity arrows
			SimulationManager::SetAtomClickedEvent([](std::shared_ptr<Atom> atom) {
				atom->SwitchVelocityArrowVisibility();
				});
		});

		std::shared_ptr<Text> editVelocityArrowsButtonText = editVelocityArrowsButton->GetLayout()->CreateControl<Text>();
		editVelocityArrowsButtonText->SetTextTheme(THEME_QUICK_BAR_TEXT);
		editVelocityArrowsButtonText->SetText(L"Edit Velocity Arrows");


		// Reset State Button
		std::shared_ptr<Button> resetStateButton = layout->CreateControl<Button>(0, 4);
		resetStateButton->SetColorTheme(THEME_QUICK_BAR_BUTTON_COLOR);
		resetStateButton->SetBorderTheme(THEME_MENU_BAR_BUTTON_BORDER);
		resetStateButton->Click(
			[weakWindow = std::weak_ptr<ContentWindow>(window)]()
		{
			auto window = weakWindow.lock();

			// Pause the simulation first as it does not make sense to add an atom while the simulation is playing
			SimulationManager::Pause();

			// Just run the display add molecule function
			DisplayResetStateControls(window);
		});

		std::shared_ptr<Text> resetStateButtonText = resetStateButton->GetLayout()->CreateControl<Text>();
		resetStateButtonText->SetTextTheme(THEME_QUICK_BAR_TEXT);
		resetStateButtonText->SetText(L"Reset State");
	}
	void DisplaySaveSimulationPromptControls(const std::shared_ptr<ContentWindow>& window)
	{
		std::shared_ptr<Layout> layout = std::dynamic_pointer_cast<Layout>(window->GetLayout()->GetSubLayout(L"RightSideLayout"));
		assert(layout != nullptr);

		// Make sure the layout is cleared before creating new content
		layout->Clear();

		// Create three columns that evenly divide the layout (one for each button)
		RowColDefinitions columnDefs;
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f); // Left Column
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f); // Middle Column
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f); // Right Column
		layout->SetColumnDefinitions(columnDefs);

		RowColDefinitions rowDefs;
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f); // Row for Prompt Text
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, 30.0f); // Row for Buttons
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f); // Space filling row
		layout->SetRowDefinitions(rowDefs);

		// Text Prompt
		std::shared_ptr<Text> promptText = layout->CreateControl<Text>(0, 0, 1, 3);
		promptText->SetTextTheme(THEME_NEW_SIMULATION_SAVE_SIMULATION_PROMPT_TEXT);
		promptText->SetText(L"Would you like to save the current simulation?");

		// Save Simulation Button
		std::shared_ptr<Button> saveButton = layout->CreateControl<Button>(1, 0);
		saveButton->SetColorTheme(THEME_NEW_SIMULATION_SAVE_SIMULATION_BUTTON_COLOR);
		saveButton->SetBorderTheme(THEME_NEW_SIMULATION_SAVE_SIMULATION_BUTTON_BORDER);
		saveButton->Margin(15.0f, 5.0f, 5.0f, 0.0f);
		saveButton->Click(
			[weakWindow = std::weak_ptr<ContentWindow>(window)]()
		{
			auto window = weakWindow.lock();

			// SAVE THE SIMULATION... (Assuming there are changes that need saving)
			// ???
			// ???

			// Update the simulation to be a single hydrogen atom in the center of the simulation
			SimulationManager::Pause();
			SimulationManager::RemoveAllAtoms();
			SimulationManager::BoxDimensions(2.0f);

			XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
			XMFLOAT3 velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
			std::shared_ptr<Hydrogen> firstAtom = SimulationManager::AddNewAtom<Hydrogen>(position, velocity);
			SimulationManager::SelectAtom(firstAtom);

			// Display the add atoms controls
			DisplayAddAtomsControls(window);
		}
		);

		std::shared_ptr<Text> saveButtonText = saveButton->GetLayout()->CreateControl<Text>();
		saveButtonText->SetTextTheme(THEME_NEW_SIMULATION_SAVE_SIMULATION_BUTTON_TEXT);
		saveButtonText->SetText(L"Save");


		// Discard Changes Button
		std::shared_ptr<Button> discardButton = layout->CreateControl<Button>(1, 1);
		discardButton->SetColorTheme(THEME_NEW_SIMULATION_SAVE_SIMULATION_BUTTON_COLOR);
		discardButton->SetBorderTheme(THEME_NEW_SIMULATION_SAVE_SIMULATION_BUTTON_BORDER);
		discardButton->Margin(5.0f, 5.0f, 5.0f, 0.0f);
		discardButton->Click(
			[weakWindow = std::weak_ptr<ContentWindow>(window)]()
		{
			auto window = weakWindow.lock();

			// Update the simulation to be a single hydrogen atom in the center of the simulation
			SimulationManager::Pause();
			SimulationManager::RemoveAllAtoms();
			SimulationManager::BoxDimensions(2.0f);

			XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
			XMFLOAT3 velocity = XMFLOAT3(0.0f, 0.0f, 0.0f);
			std::shared_ptr<Hydrogen> firstAtom = SimulationManager::AddNewAtom<Hydrogen>(position, velocity);
			SimulationManager::SelectAtom(firstAtom);

			// Display the add atoms controls
			DisplayAddAtomsControls(window);
		}
		);

		std::shared_ptr<Text> discardButtonText = discardButton->GetLayout()->CreateControl<Text>();
		discardButtonText->SetTextTheme(THEME_NEW_SIMULATION_SAVE_SIMULATION_BUTTON_TEXT);
		discardButtonText->SetText(L"Discard Changes");


		// Cancel Simulation Button
		std::shared_ptr<Button> cancelButton = layout->CreateControl<Button>(1, 2);
		cancelButton->SetColorTheme(THEME_NEW_SIMULATION_SAVE_SIMULATION_BUTTON_COLOR);
		cancelButton->SetBorderTheme(THEME_NEW_SIMULATION_SAVE_SIMULATION_BUTTON_BORDER);
		cancelButton->Margin(5.0f, 5.0f, 15.0f, 0.0f);
		cancelButton->Click(
			[weakWindow = std::weak_ptr<ContentWindow>(window)]()
		{
			auto window = weakWindow.lock();

			// Display the add atoms controls
			DisplayAddAtomsControls(window);
		}
		);

		std::shared_ptr<Text> cancelButtonText = cancelButton->GetLayout()->CreateControl<Text>();
		cancelButtonText->SetTextTheme(THEME_NEW_SIMULATION_SAVE_SIMULATION_BUTTON_TEXT);
		cancelButtonText->SetText(L"Cancel");
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
		columnDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_STAR, 1.0f);   // Remainder of bar - dynamic controls will be added here

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
		playSimulationButton->Name(L"PlayPauseButton");
		playSimulationButton->Click([]()
		{
			// Don't capture the button here - no need to modify the glyph here because it
			// will be modified by the SimulationManager::PlayPauseChangedEvent
			// 
			// Switch play / pause the simualation
			SimulationManager::SwitchPlayPause();
		}
		);
		std::shared_ptr<Text> playSimulationText = playSimulationButton->GetLayout()->CreateControl<Text>();
		playSimulationText->SetTextTheme(THEME_QUICK_BAR_GLYPH);
		playSimulationText->SetText(L"\xE768");

		// Update the play/pause button text in the event handler so that if the simulation is played/paused
		// from code (as opposed to a user clicking the play/pause button) the button's text will still get updated
		SimulationManager::SetPlayPauseChangedEvent(
			[weakText   = std::weak_ptr<Text>(playSimulationText),
			 weakWindow = std::weak_ptr<ContentWindow>(window)](bool isPlaying)
		{
			auto text = weakText.lock();
			auto window = weakWindow.lock();

			if (isPlaying)
			{
				text->SetText(L"\xE769"); // Show the pause button if the simulation is playing

				// if the simulation begins playing, then populate the IsPlaying menu
				DisplaySimulationPlayingControls(window);
			}
			else
			{
				text->SetText(L"\xE768"); // Show the play button if simulation is paused

				// if the simulation gets paused, populate the add atoms controls
				DisplayAddAtomsControls(window);
			}
		}
		);



		// Create a sublayout to house additional controls that will be dynamically added/removed
		std::shared_ptr<Layout> dynamicControlsLayout = layout->CreateSubLayout(0, 4);
		dynamicControlsLayout->Name(L"QuickBarDynamicControlsLayout");
	}

	void CreateRightPane(const std::shared_ptr<ContentWindow>& window)
	{
		std::shared_ptr<Layout> rightSideLayout = window->GetLayout()->CreateSubLayout(2, 1);
		rightSideLayout->Name(L"RightSideLayout");
	}
}