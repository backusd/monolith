#pragma once
#include "pch.h"
#include "Theme.h"


class LineTheme : public Theme
{
public:
	LineTheme(std::string themeName, const std::shared_ptr<DeviceResources>& deviceResources);

	ID2D1SolidColorBrush* GetBrush() const { return m_color.Get(); }
	float GetStrokeWidth() { return m_strokeWidth; }

	void SetColor(D2D1_COLOR_F color);
	void SetStrokeWidth(float width) { m_strokeWidth = width; }

private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_color;

	float m_strokeWidth;
};