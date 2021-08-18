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
	//void OnPaint() override;


	void OnLayoutResize() override {  }
	void OnMarginChanged() override {  }

	void SetTextTheme(std::string name) { m_textTheme = std::static_pointer_cast<TextTheme>(ThemeManager::GetTheme(name)); }
	void SetText(std::wstring text) { m_text = text; }

private:
	std::shared_ptr<TextTheme>	m_textTheme;
	DWRITE_TEXT_METRICS			m_textMetrics;
	std::wstring				m_text;

};