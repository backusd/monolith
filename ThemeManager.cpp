#include "ThemeManager.h"

std::map<std::string, std::shared_ptr<Theme>> ThemeManager::m_themesMap;
bool ThemeManager::m_initialized = false;

void ThemeManager::Initialize(const std::shared_ptr<DeviceResources>& deviceResources)
{
	if (!m_initialized)
	{
		CreateDefaultThemes(deviceResources);

		CreateMenuBarThemes(deviceResources);
		CreateQuickBarThemes(deviceResources);

		CreateNewSimulationThemes(deviceResources);

		m_initialized = true;
	}
}

void ThemeManager::CreateDefaultThemes(const std::shared_ptr<DeviceResources>& deviceResources)
{
	// Text Control - Use class defaults
	std::shared_ptr<TextTheme> defaultTextTheme = ThemeManager::CreateTheme<TextTheme>(THEME_DEFAULT_TEXT, deviceResources);

	// Button Control - Background
	std::shared_ptr<ColorTheme> buttonColor = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_BUTTON_COLOR, deviceResources);
	buttonColor->SetAllColors(D2D1::ColorF(D2D1::ColorF::White));

	// Button Control - Border
	std::shared_ptr<BorderTheme> buttonBorder = ThemeManager::CreateTheme<BorderTheme>(THEME_DEFAULT_BUTTON_BORDER, deviceResources);
	buttonBorder->SetAllColors(D2D1::ColorF(D2D1::ColorF::White));
	buttonBorder->SetStrokeWidth(1.0f);

	// Drop Down Control - Background
	std::shared_ptr<ColorTheme> dropDownColor = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_DROP_DOWN_COLOR, deviceResources);
	dropDownColor->SetAllColors(D2D1::ColorF(D2D1::ColorF::White));

	// TextInput Control - Background Color
	std::shared_ptr<ColorTheme> textInputBackgroundColor = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_TEXT_INPUT_BACKGROUND_COLOR, deviceResources);
	textInputBackgroundColor->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));
	textInputBackgroundColor->SetColorPointerOver(D2D1::ColorF(90.0f / 255.0f, 90.0f / 255.0f, 96.0f / 255.0f));
	textInputBackgroundColor->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::DarkGray));

	// TextInput Control - Border Color
	std::shared_ptr<ColorTheme> textInputBorderColor = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_TEXT_INPUT_BORDER_COLOR, deviceResources);
	textInputBorderColor->SetAllColors(D2D1::ColorF(D2D1::ColorF::White));

	// TextInputControl - Text
	std::shared_ptr<TextTheme> textInputText = ThemeManager::CreateTheme<TextTheme>(THEME_DEFAULT_TEXT_INPUT_TEXT, deviceResources);
	textInputText->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	textInputText->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	textInputText->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	textInputText->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	textInputText->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	textInputText->SetFontSize(13.0f);
	textInputText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
	textInputText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	textInputText->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	textInputText->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	// TextInputControl - Placeholder Text
	std::shared_ptr<TextTheme> textInputPlaceholderText = ThemeManager::CreateTheme<TextTheme>(THEME_DEFAULT_TEXT_INPUT_PLACEHOLDER_TEXT, deviceResources);
	textInputPlaceholderText->SetColor(D2D1::ColorF(D2D1::ColorF::LightCyan));
	textInputPlaceholderText->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	textInputPlaceholderText->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	textInputPlaceholderText->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	textInputPlaceholderText->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	textInputPlaceholderText->SetFontSize(13.0f);
	textInputPlaceholderText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
	textInputPlaceholderText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	textInputPlaceholderText->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	textInputPlaceholderText->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	// ComboBox Control - Down Arrow Glyph
	std::shared_ptr<TextTheme> comboBoxDownArrowGlyph = ThemeManager::CreateTheme<TextTheme>(THEME_DEFAULT_COMBO_BOX_DOWN_ARROW_GLYPH, deviceResources);
	comboBoxDownArrowGlyph->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	comboBoxDownArrowGlyph->SetFontFamily(FONT_FAMILY::SEGOE_MDL2_ASSETS);
	comboBoxDownArrowGlyph->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	comboBoxDownArrowGlyph->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	comboBoxDownArrowGlyph->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	comboBoxDownArrowGlyph->SetFontSize(13.0f);
	comboBoxDownArrowGlyph->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	comboBoxDownArrowGlyph->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	comboBoxDownArrowGlyph->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	comboBoxDownArrowGlyph->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	// ComboBox Control - Default Text
	std::shared_ptr<TextTheme> comboBoxText = ThemeManager::CreateTheme<TextTheme>(THEME_DEFAULT_COMBO_BOX_TEXT, deviceResources);
	comboBoxText->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	comboBoxText->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	comboBoxText->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	comboBoxText->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	comboBoxText->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	comboBoxText->SetFontSize(13.0f);
	comboBoxText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
	comboBoxText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	comboBoxText->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	comboBoxText->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	// ComboBox Control - Background Color
	std::shared_ptr<ColorTheme> comboBoxBackgroundColor = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_COMBO_BOX_BACKGROUND_COLOR, deviceResources);
	comboBoxBackgroundColor->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));
	comboBoxBackgroundColor->SetColorPointerOver(D2D1::ColorF(90.0f / 255.0f, 90.0f / 255.0f, 96.0f / 255.0f));
	comboBoxBackgroundColor->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::DarkGray));

	// ComboBox Control - Border Color
	std::shared_ptr<ColorTheme> comboBoxBorderColor = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_COMBO_BOX_BORDER_COLOR, deviceResources);
	comboBoxBorderColor->SetAllColors(D2D1::ColorF(D2D1::ColorF::White));

	// ComboBox Control - main button color
	std::shared_ptr<ColorTheme> comboBoxButtonColor = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_COMBO_BOX_BUTTON_COLOR, deviceResources);
	comboBoxButtonColor->SetColor(D2D1::ColorF(45.0f / 255.0f, 45.0f / 255.0f, 48.0f / 255.0f));
	comboBoxButtonColor->SetColorPointerOver(D2D1::ColorF(90.0f / 255.0f, 90.0f / 255.0f, 96.0f / 255.0f));
	comboBoxButtonColor->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Black));

	// ComboBox Control - Drop down item button color
	std::shared_ptr<ColorTheme> comboBoxItemButtonColor = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_COMBO_BOX_ITEM_BUTTON_COLOR, deviceResources);
	comboBoxItemButtonColor->SetColor(D2D1::ColorF(45.0f / 255.0f, 45.0f / 255.0f, 48.0f / 255.0f));
	comboBoxItemButtonColor->SetColorPointerOver(D2D1::ColorF(90.0f / 255.0f, 90.0f / 255.0f, 96.0f / 255.0f));
	comboBoxItemButtonColor->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Black));

	// ComboBox Control - Drop down item button border
	std::shared_ptr<BorderTheme> comboBoxItemButtonBorder = ThemeManager::CreateTheme<BorderTheme>(THEME_DEFAULT_COMBO_BOX_ITEM_BUTTON_BORDER, deviceResources);
	comboBoxItemButtonBorder->SetStrokeWidth(0.0f);

	// Line - line color
	std::shared_ptr<LineTheme> line = ThemeManager::CreateTheme<LineTheme>(THEME_DEFAULT_LINE_COLOR, deviceResources);
	line->SetColor(D2D1::ColorF(D2D1::ColorF::White));

	// Slider Control - Rect color
	std::shared_ptr<ColorTheme> sliderRectColor = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_SLIDER_RECT_COLOR, deviceResources);
	sliderRectColor->SetColor(D2D1::ColorF(D2D1::ColorF::Blue));
	sliderRectColor->SetColorPointerOver(D2D1::ColorF(D2D1::ColorF::LightBlue));
	sliderRectColor->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::DarkBlue));

	// Tabbed Pane - Tab selected color
	std::shared_ptr<ColorTheme> tabbedPaneSelectedTabColor = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_TABBED_PANE_TAB_SELECTED_COLOR, deviceResources);
	tabbedPaneSelectedTabColor->SetAllColors(D2D1::ColorF(25.0f / 255.0f, 25.0f / 255.0f, 28.0f / 255.0f));

	// Tabbed Pane - Tab unselected color
	std::shared_ptr<ColorTheme> tabbedPaneUnselectedTabColor = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_TABBED_PANE_TAB_UNSELECTED_COLOR, deviceResources);
	tabbedPaneUnselectedTabColor->SetColor(D2D1::ColorF(55.0f / 255.0f, 55.0f / 255.0f, 58.0f / 255.0f));
	tabbedPaneUnselectedTabColor->SetColorPointerOver(D2D1::ColorF(75.0f / 255.0f, 75.0f / 255.0f, 78.0f / 255.0f));
	tabbedPaneUnselectedTabColor->SetColorPointerDown(D2D1::ColorF(95.0f / 255.0f, 95.0f / 255.0f, 98.0f / 255.0f));

	// Tabbed Pane - Tab selected border
	std::shared_ptr<BorderTheme> tabbedPaneSelectedTabBorder = ThemeManager::CreateTheme<BorderTheme>(THEME_DEFAULT_TABBED_PANE_TAB_SELECTED_BORDER, deviceResources);
	tabbedPaneSelectedTabBorder->SetStrokeWidth(1.5f, 1.5f, 1.5f, 0.5f);
	tabbedPaneSelectedTabBorder->SetAllColors(D2D1::ColorF(85.0f / 255.0f, 85.0f / 255.0f, 88.0f / 255.0f));

	// Tabbed Pane - Tab unselected border
	std::shared_ptr<BorderTheme> tabbedPaneUnselectedTabBorder = ThemeManager::CreateTheme<BorderTheme>(THEME_DEFAULT_TABBED_PANE_TAB_UNSELECTED_BORDER, deviceResources);
	tabbedPaneUnselectedTabBorder->SetStrokeWidth(1.5f, 1.5f, 1.5f, 0.5f);
	tabbedPaneUnselectedTabBorder->SetAllColors(D2D1::ColorF(85.0f / 255.0f, 85.0f / 255.0f, 88.0f / 255.0f));

	// Tabbed Pane - Pane color
	std::shared_ptr<ColorTheme> tabbedPanePaneColor = ThemeManager::CreateTheme<ColorTheme>(THEME_DEFAULT_TABBED_PANE_PANE_COLOR, deviceResources);
	tabbedPanePaneColor->SetAllColors(D2D1::ColorF(25.0f / 255.0f, 25.0f / 255.0f, 28.0f / 255.0f));

	// Tabbed Pane - Pane border
	std::shared_ptr<BorderTheme> tabbedPanePaneBorder = ThemeManager::CreateTheme<BorderTheme>(THEME_DEFAULT_TABBED_PANE_PANE_BORDER, deviceResources);
	tabbedPanePaneBorder->SetStrokeWidth(1.5f, 0.0f, 1.5f, 1.5f);
	tabbedPanePaneBorder->SetAllColors(D2D1::ColorF(85.0f / 255.0f, 85.0f / 255.0f, 88.0f / 255.0f));
}

void ThemeManager::CreateMenuBarThemes(const std::shared_ptr<DeviceResources>& deviceResources)
{
	// Text Control - Text for main drop down button
	std::shared_ptr<TextTheme> menuButtonTextTheme = ThemeManager::CreateTheme<TextTheme>(THEME_MENU_BAR_TEXT, deviceResources);
	menuButtonTextTheme->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	menuButtonTextTheme->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	menuButtonTextTheme->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	menuButtonTextTheme->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	menuButtonTextTheme->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	menuButtonTextTheme->SetFontSize(13.0f);
	menuButtonTextTheme->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	menuButtonTextTheme->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	menuButtonTextTheme->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	menuButtonTextTheme->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	// Text Control - Text for drop down item
	std::shared_ptr<TextTheme> menuDropDownTextTheme = ThemeManager::CreateTheme<TextTheme>(THEME_MENU_BAR_DROP_DOWN_TEXT, deviceResources);
	menuDropDownTextTheme->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	menuDropDownTextTheme->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	menuDropDownTextTheme->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	menuDropDownTextTheme->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	menuDropDownTextTheme->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	menuDropDownTextTheme->SetFontSize(13.0f);
	menuDropDownTextTheme->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
	menuDropDownTextTheme->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	menuDropDownTextTheme->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	menuDropDownTextTheme->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	// Text Control - Text for drop down glyph
	std::shared_ptr<TextTheme> menuDropDownGlyphTheme = ThemeManager::CreateTheme<TextTheme>(THEME_MENU_BAR_DROP_DOWN_GLYPH, deviceResources);
	menuDropDownGlyphTheme->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	menuDropDownGlyphTheme->SetFontFamily(FONT_FAMILY::SEGOE_MDL2_ASSETS);
	menuDropDownGlyphTheme->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	menuDropDownGlyphTheme->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	menuDropDownGlyphTheme->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	menuDropDownGlyphTheme->SetFontSize(13.0f);
	menuDropDownGlyphTheme->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	menuDropDownGlyphTheme->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	menuDropDownGlyphTheme->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	menuDropDownGlyphTheme->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	// Button Control - Background color
	std::shared_ptr<ColorTheme> menuButtonColorTheme = ThemeManager::CreateTheme<ColorTheme>(THEME_MENU_BAR_BUTTON_COLOR, deviceResources);
	menuButtonColorTheme->SetColor(D2D1::ColorF(45.0f / 255.0f, 45.0f / 255.0f, 48.0f / 255.0f));
	menuButtonColorTheme->SetColorPointerOver(D2D1::ColorF(90.0f / 255.0f, 90.0f / 255.0f, 96.0f / 255.0f));
	menuButtonColorTheme->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Black));

	// Button Control - Border
	std::shared_ptr<BorderTheme> menuButtonBorder = ThemeManager::CreateTheme<BorderTheme>(THEME_MENU_BAR_BUTTON_BORDER, deviceResources);
	menuButtonBorder->SetStrokeWidth(0.0f);

	// Drop Down Control - Background color
	std::shared_ptr<ColorTheme> menuDropDownColorTheme = ThemeManager::CreateTheme<ColorTheme>(THEME_MENU_BAR_DROP_DOWN_COLOR, deviceResources);
	menuDropDownColorTheme->SetColor(D2D1::ColorF(45.0f / 255.0f, 45.0f / 255.0f, 48.0f / 255.0f));

}

void ThemeManager::CreateQuickBarThemes(const std::shared_ptr<DeviceResources>& deviceResources)
{
	// Text Control - Text for quick bar glyphs
	std::shared_ptr<TextTheme> quickBarGlyphTheme = ThemeManager::CreateTheme<TextTheme>(THEME_QUICK_BAR_GLYPH, deviceResources);
	quickBarGlyphTheme->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	quickBarGlyphTheme->SetFontFamily(FONT_FAMILY::SEGOE_MDL2_ASSETS);
	quickBarGlyphTheme->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	quickBarGlyphTheme->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	quickBarGlyphTheme->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	quickBarGlyphTheme->SetFontSize(13.0f);
	quickBarGlyphTheme->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	quickBarGlyphTheme->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	quickBarGlyphTheme->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	quickBarGlyphTheme->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	// Button Control - Background color
	std::shared_ptr<ColorTheme> quickBarButtonColorTheme = ThemeManager::CreateTheme<ColorTheme>(THEME_QUICK_BAR_BUTTON_COLOR, deviceResources);
	quickBarButtonColorTheme->SetColor(D2D1::ColorF(45.0f / 255.0f, 45.0f / 255.0f, 48.0f / 255.0f));
	quickBarButtonColorTheme->SetColorPointerOver(D2D1::ColorF(90.0f / 255.0f, 90.0f / 255.0f, 96.0f / 255.0f));
	quickBarButtonColorTheme->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Black));

	// Text Control - Text for buttons in the quick bar that aren't glyphs
	std::shared_ptr<TextTheme> quickBarTextTheme = ThemeManager::CreateTheme<TextTheme>(THEME_QUICK_BAR_TEXT, deviceResources);
	quickBarTextTheme->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	quickBarTextTheme->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	quickBarTextTheme->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	quickBarTextTheme->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	quickBarTextTheme->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	quickBarTextTheme->SetFontSize(12.0f);
	quickBarTextTheme->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	quickBarTextTheme->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	quickBarTextTheme->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	quickBarTextTheme->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

}

void ThemeManager::CreateNewSimulationThemes(const std::shared_ptr<DeviceResources>& deviceResources)
{
	// Backdrop color
	std::shared_ptr<ColorTheme> backdropButtonColor = ThemeManager::CreateTheme<ColorTheme>(THEME_NEW_SIMULATION_BACKDROP_BUTTON_COLOR, deviceResources);
	backdropButtonColor->SetColor(D2D1::ColorF(25.0f / 255.0f, 25.0f / 255.0f, 28.0f / 255.0f));
	backdropButtonColor->SetColorPointerOver(D2D1::ColorF(25.0f / 255.0f, 25.0f / 255.0f, 28.0f / 255.0f));
	backdropButtonColor->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Black));

	std::shared_ptr<BorderTheme> backdropButtonBorder = ThemeManager::CreateTheme<BorderTheme>(THEME_NEW_SIMULATION_BACKDROP_BUTTON_BORDER, deviceResources);
	backdropButtonBorder->SetStrokeWidth(0.75f);
	backdropButtonBorder->SetAllColors(D2D1::ColorF(D2D1::ColorF::Gray));

	// Basic text
	std::shared_ptr<TextTheme> textTheme = ThemeManager::CreateTheme<TextTheme>(THEME_NEW_SIMULATION_TEXT, deviceResources);
	textTheme->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	textTheme->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	textTheme->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	textTheme->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	textTheme->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	textTheme->SetFontSize(13.0f);
	textTheme->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
	textTheme->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	textTheme->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	textTheme->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	// Add New Atom Button
	std::shared_ptr<ColorTheme> addButtonColor = ThemeManager::CreateTheme<ColorTheme>(THEME_NEW_SIMULATION_ADD_BUTTON_COLOR, deviceResources);
	addButtonColor->SetColor(D2D1::ColorF(0.0f / 255.0f, 70.0f / 255.0f, 180.0f / 255.0f));
	addButtonColor->SetColorPointerOver(D2D1::ColorF(0.0f / 255.0f, 40.0f / 255.0f, 105.0f / 255.0f));
	addButtonColor->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Black));

	std::shared_ptr<BorderTheme> addButtonBorder = ThemeManager::CreateTheme<BorderTheme>(THEME_NEW_SIMULATION_ADD_BUTTON_BORDER, deviceResources);
	addButtonBorder->SetStrokeWidth(1.0f);
	addButtonBorder->SetAllColors(D2D1::ColorF(D2D1::ColorF::White));

	std::shared_ptr<TextTheme> addButtonText = ThemeManager::CreateTheme<TextTheme>(THEME_NEW_SIMULATION_ADD_BUTTON_TEXT, deviceResources);
	addButtonText->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	addButtonText->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	addButtonText->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	addButtonText->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	addButtonText->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	addButtonText->SetFontSize(13.0f);
	addButtonText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	addButtonText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	addButtonText->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	addButtonText->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);


	// Atom ListView
	std::shared_ptr<ColorTheme> atomListViewButtonColor = ThemeManager::CreateTheme<ColorTheme>(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_COLOR, deviceResources);
	atomListViewButtonColor->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));
	atomListViewButtonColor->SetColorPointerOver(D2D1::ColorF(D2D1::ColorF::DarkGray));
	atomListViewButtonColor->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Black));

	std::shared_ptr<ColorTheme> atomListViewButtonHighlightedColor = ThemeManager::CreateTheme<ColorTheme>(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_HIGHLIGHTED_COLOR, deviceResources);
	atomListViewButtonHighlightedColor->SetColor(D2D1::ColorF(D2D1::ColorF::Black));
	atomListViewButtonHighlightedColor->SetColorPointerOver(D2D1::ColorF(D2D1::ColorF::DarkGray));
	atomListViewButtonHighlightedColor->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Black));

	std::shared_ptr<BorderTheme> atomListViewButtonBorder = ThemeManager::CreateTheme<BorderTheme>(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_BORDER, deviceResources);
	atomListViewButtonBorder->SetStrokeWidth(1.0f);
	atomListViewButtonBorder->SetAllColors(D2D1::ColorF(D2D1::ColorF::White));

	std::shared_ptr<TextTheme> atomListViewButtonText = ThemeManager::CreateTheme<TextTheme>(THEME_NEW_SIMULATION_ATOM_LISTVIEW_BUTTON_TEXT, deviceResources);
	atomListViewButtonText->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	atomListViewButtonText->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	atomListViewButtonText->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_BOLD);
	atomListViewButtonText->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	atomListViewButtonText->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	atomListViewButtonText->SetFontSize(13.0f);
	atomListViewButtonText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
	atomListViewButtonText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	atomListViewButtonText->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	atomListViewButtonText->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	std::shared_ptr<TextTheme> atomListViewPositionVelocityText = ThemeManager::CreateTheme<TextTheme>(THEME_NEW_SIMULATION_ATOM_LISTVIEW_POSITION_VELOCITY_TEXT, deviceResources);
	atomListViewPositionVelocityText->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	atomListViewPositionVelocityText->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	atomListViewPositionVelocityText->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	atomListViewPositionVelocityText->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	atomListViewPositionVelocityText->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	atomListViewPositionVelocityText->SetFontSize(11.0f);
	atomListViewPositionVelocityText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
	atomListViewPositionVelocityText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	atomListViewPositionVelocityText->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	atomListViewPositionVelocityText->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	// Atom ListView - Remove Button
	std::shared_ptr<ColorTheme> atomListViewRemoveButtonColor = ThemeManager::CreateTheme<ColorTheme>(THEME_NEW_SIMULATION_ATOM_LISTVIEW_REMOVE_BUTTON_COLOR, deviceResources);
	atomListViewRemoveButtonColor->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));
	atomListViewRemoveButtonColor->SetColorPointerOver(D2D1::ColorF(D2D1::ColorF::DarkGray));
	atomListViewRemoveButtonColor->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Black));

	std::shared_ptr<BorderTheme> atomListViewRemoveButtonBorder = ThemeManager::CreateTheme<BorderTheme>(THEME_NEW_SIMULATION_ATOM_LISTVIEW_REMOVE_BUTTON_BORDER, deviceResources);
	atomListViewRemoveButtonBorder->SetStrokeWidth(0.0f);
	atomListViewRemoveButtonBorder->SetAllColors(D2D1::ColorF(D2D1::ColorF::White));

	std::shared_ptr<TextTheme> atomListViewRemoveButtonGlyph = ThemeManager::CreateTheme<TextTheme>(THEME_NEW_SIMULATION_ATOM_LISTVIEW_REMOVE_BUTTON_GLYPH, deviceResources);
	atomListViewRemoveButtonGlyph->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	atomListViewRemoveButtonGlyph->SetFontFamily(FONT_FAMILY::SEGOE_MDL2_ASSETS);
	atomListViewRemoveButtonGlyph->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	atomListViewRemoveButtonGlyph->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	atomListViewRemoveButtonGlyph->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	atomListViewRemoveButtonGlyph->SetFontSize(13.0f);
	atomListViewRemoveButtonGlyph->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	atomListViewRemoveButtonGlyph->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	atomListViewRemoveButtonGlyph->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	atomListViewRemoveButtonGlyph->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	// Save Simulation Prompt
	std::shared_ptr<TextTheme> saveSimulationPromptText = ThemeManager::CreateTheme<TextTheme>(THEME_NEW_SIMULATION_SAVE_SIMULATION_PROMPT_TEXT, deviceResources);
	saveSimulationPromptText->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	saveSimulationPromptText->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	saveSimulationPromptText->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	saveSimulationPromptText->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	saveSimulationPromptText->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	saveSimulationPromptText->SetFontSize(14.0f);
	saveSimulationPromptText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	saveSimulationPromptText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_FAR);
	saveSimulationPromptText->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	saveSimulationPromptText->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	std::shared_ptr<ColorTheme> saveSimulationButtonColor = ThemeManager::CreateTheme<ColorTheme>(THEME_NEW_SIMULATION_SAVE_SIMULATION_BUTTON_COLOR, deviceResources);
	saveSimulationButtonColor->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));
	saveSimulationButtonColor->SetColorPointerOver(D2D1::ColorF(D2D1::ColorF::DarkGray));
	saveSimulationButtonColor->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Black));

	std::shared_ptr<BorderTheme> saveSimulationButtonBorder = ThemeManager::CreateTheme<BorderTheme>(THEME_NEW_SIMULATION_SAVE_SIMULATION_BUTTON_BORDER, deviceResources);
	saveSimulationButtonBorder->SetStrokeWidth(1.0f);
	saveSimulationButtonBorder->SetAllColors(D2D1::ColorF(D2D1::ColorF::White));

	std::shared_ptr<TextTheme> saveSimulationButtonGlyph = ThemeManager::CreateTheme<TextTheme>(THEME_NEW_SIMULATION_SAVE_SIMULATION_BUTTON_TEXT, deviceResources);
	saveSimulationButtonGlyph->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	saveSimulationButtonGlyph->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	saveSimulationButtonGlyph->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	saveSimulationButtonGlyph->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	saveSimulationButtonGlyph->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	saveSimulationButtonGlyph->SetFontSize(12.0f);
	saveSimulationButtonGlyph->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	saveSimulationButtonGlyph->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	saveSimulationButtonGlyph->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_NO_WRAP);
	saveSimulationButtonGlyph->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);


	// Create Bonds Themes
	std::shared_ptr<TextTheme> createBondsInstructionsText = ThemeManager::CreateTheme<TextTheme>(THEME_NEW_SIMULATION_CREATE_BONDS_INSTRUCTIONS_TEXT, deviceResources);
	createBondsInstructionsText->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	createBondsInstructionsText->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	createBondsInstructionsText->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	createBondsInstructionsText->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	createBondsInstructionsText->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	createBondsInstructionsText->SetFontSize(14.0f);
	createBondsInstructionsText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	createBondsInstructionsText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	createBondsInstructionsText->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_WHOLE_WORD);
	createBondsInstructionsText->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	std::shared_ptr<TextTheme> createBondsHeadersText = ThemeManager::CreateTheme<TextTheme>(THEME_NEW_SIMULATION_CREATE_BONDS_HEADERS_TEXT, deviceResources);
	createBondsHeadersText->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	createBondsHeadersText->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	createBondsHeadersText->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	createBondsHeadersText->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	createBondsHeadersText->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	createBondsHeadersText->SetFontSize(14.0f);
	createBondsHeadersText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_LEADING);
	createBondsHeadersText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	createBondsHeadersText->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_WHOLE_WORD);
	createBondsHeadersText->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	std::shared_ptr<ColorTheme> createBondsSelectAtomButtonColor = ThemeManager::CreateTheme<ColorTheme>(THEME_NEW_SIMULATION_CREATE_BONDS_SELECT_ATOM_BUTTON_COLOR, deviceResources);
	createBondsSelectAtomButtonColor->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));
	createBondsSelectAtomButtonColor->SetColorPointerOver(D2D1::ColorF(D2D1::ColorF::DarkGray));
	createBondsSelectAtomButtonColor->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Black));

	std::shared_ptr<BorderTheme> createBondsSelectAtomButtonBorder = ThemeManager::CreateTheme<BorderTheme>(THEME_NEW_SIMULATION_CREATE_BONDS_SELECT_ATOM_BUTTON_BORDER, deviceResources);
	createBondsSelectAtomButtonBorder->SetStrokeWidth(1.0f);
	createBondsSelectAtomButtonBorder->SetAllColors(D2D1::ColorF(D2D1::ColorF::White));

	std::shared_ptr<TextTheme> createBondsSelectAtomButtonText = ThemeManager::CreateTheme<TextTheme>(THEME_NEW_SIMULATION_CREATE_BONDS_SELECT_ATOM_BUTTON_TEXT, deviceResources);
	createBondsSelectAtomButtonText->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	createBondsSelectAtomButtonText->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	createBondsSelectAtomButtonText->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	createBondsSelectAtomButtonText->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	createBondsSelectAtomButtonText->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	createBondsSelectAtomButtonText->SetFontSize(14.0f);
	createBondsSelectAtomButtonText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	createBondsSelectAtomButtonText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	createBondsSelectAtomButtonText->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_WHOLE_WORD);
	createBondsSelectAtomButtonText->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);

	// Edit Velocity Arrows Themes
	std::shared_ptr<ColorTheme> editVelocityArrowsButtonColor = ThemeManager::CreateTheme<ColorTheme>(THEME_NEW_SIMULATION_EDIT_VELOCITY_ARROWS_BUTTON_COLOR, deviceResources);
	editVelocityArrowsButtonColor->SetColor(D2D1::ColorF(D2D1::ColorF::Gray));
	editVelocityArrowsButtonColor->SetColorPointerOver(D2D1::ColorF(D2D1::ColorF::DarkGray));
	editVelocityArrowsButtonColor->SetColorPointerDown(D2D1::ColorF(D2D1::ColorF::Black));

	std::shared_ptr<BorderTheme> editVelocityArrowsButtonBorder = ThemeManager::CreateTheme<BorderTheme>(THEME_NEW_SIMULATION_EDIT_VELOCITY_ARROWS_BUTTON_BORDER, deviceResources);
	editVelocityArrowsButtonBorder->SetStrokeWidth(1.0f);
	editVelocityArrowsButtonBorder->SetAllColors(D2D1::ColorF(D2D1::ColorF::White));

	std::shared_ptr<TextTheme> editVelocityArrowsButtonText = ThemeManager::CreateTheme<TextTheme>(THEME_NEW_SIMULATION_EDIT_VELOCITY_ARROWS_BUTTON_TEXT, deviceResources);
	editVelocityArrowsButtonText->SetColor(D2D1::ColorF(D2D1::ColorF::White));
	editVelocityArrowsButtonText->SetFontFamily(FONT_FAMILY::SEGOE_UI);
	editVelocityArrowsButtonText->SetFontWeight(DWRITE_FONT_WEIGHT::DWRITE_FONT_WEIGHT_LIGHT);
	editVelocityArrowsButtonText->SetFontStyle(DWRITE_FONT_STYLE::DWRITE_FONT_STYLE_NORMAL);
	editVelocityArrowsButtonText->SetFontStretch(DWRITE_FONT_STRETCH::DWRITE_FONT_STRETCH_NORMAL);
	editVelocityArrowsButtonText->SetFontSize(14.0f);
	editVelocityArrowsButtonText->SetTextAlignment(DWRITE_TEXT_ALIGNMENT::DWRITE_TEXT_ALIGNMENT_CENTER);
	editVelocityArrowsButtonText->SetParagraphAlignment(DWRITE_PARAGRAPH_ALIGNMENT::DWRITE_PARAGRAPH_ALIGNMENT_CENTER);
	editVelocityArrowsButtonText->SetWordWrapping(DWRITE_WORD_WRAPPING::DWRITE_WORD_WRAPPING_WHOLE_WORD);
	editVelocityArrowsButtonText->SetTrimmingGranularity(DWRITE_TRIMMING_GRANULARITY::DWRITE_TRIMMING_GRANULARITY_NONE);
}

std::shared_ptr<Theme> ThemeManager::GetTheme(std::string name)
{
	if (m_themesMap.find(name) != m_themesMap.end())
		return m_themesMap[name];

	return nullptr;
}

void ThemeManager::AddTheme(std::shared_ptr<Theme> theme)
{
	m_themesMap.insert(std::pair(theme->Name(), theme));
}
