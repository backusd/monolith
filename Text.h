#pragma once
#include "pch.h"
#include "Control.h"
#include "TextTheme.h"
#include "ThemeManager.h"

#include <string>
#include <memory>

class Text : public Control
{
public:
	Text(const std::shared_ptr<DeviceResources>& deviceResources, 
		 const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan);


	bool Render2D() override;

	std::wstring GetText() { return m_text; }


	void OnLayoutResize() override { TextChanged(); }
	void OnMarginChanged() override { TextChanged(); }

	void SetTextTheme(std::string name) { m_textTheme = std::static_pointer_cast<TextTheme>(ThemeManager::GetTheme(name)); }
	void SetText(std::wstring text) { m_text = text; TextChanged(); }

	void AddChar(char key) { m_text.push_back(key); TextChanged(); }
	void Pop();

	float Width() { return m_textMetrics.widthIncludingTrailingWhitespace; }

private:
	void TextChanged();


	std::shared_ptr<TextTheme>	m_textTheme;
	DWRITE_TEXT_METRICS			m_textMetrics;
	std::wstring				m_text;
	D2D1::Matrix3x2F			m_screenTranslation;

	Microsoft::WRL::ComPtr<IDWriteTextLayout4>	m_textLayout;
};