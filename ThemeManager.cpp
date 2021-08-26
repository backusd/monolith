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


}

void ThemeManager::CreateNewSimulationThemes(const std::shared_ptr<DeviceResources>& deviceResources)
{
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
