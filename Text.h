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

	void OnPaint() override;
	void OnLayoutResize() override {  }
	void OnMarginChanged() override {  }

	void SetTextTheme(std::string name) { m_textTheme = std::static_pointer_cast<TextTheme>(ThemeManager::GetTheme(name)); }

	// Use default behavior defined by Control for these methods
	// 
	// void ClearContents() override;
	// bool MouseIsOver(int x, int y) override;
	// std::shared_ptr<OnMessageResult> OnLButtonDown(std::shared_ptr<MouseState> mouseState) override;
	// std::shared_ptr<OnMessageResult> OnLButtonUp(std::shared_ptr<MouseState> mouseState) override;
	// std::shared_ptr<OnMessageResult> OnMouseMove(std::shared_ptr<MouseState> mouseState) override;
	// std::shared_ptr<OnMessageResult> OnMouseLeave() override;
private:
	std::shared_ptr<TextTheme>	m_textTheme;
	DWRITE_TEXT_METRICS			m_textMetrics;
};