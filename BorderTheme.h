#pragma once
#include "pch.h"
#include "Theme.h"
#include "MouseOverDown.h"


class BorderTheme : public Theme
{
public:
	BorderTheme(std::string themeName, const std::shared_ptr<DeviceResources>& deviceResources);

	ID2D1SolidColorBrush* GetBrush(MouseOverDown state) const;
	float GetStrokeWidth() { return m_strokeWidth; }

	void SetAllColors(D2D1_COLOR_F color);
	void SetColor(D2D1_COLOR_F color);
	void SetColorPointerOver(D2D1_COLOR_F color);
	void SetColorPointerDown(D2D1_COLOR_F color);
	void SetStrokeWidth(float width) { m_strokeWidth = width; }

private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_color;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_colorPointerOver;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_colorPointerDown;

	float m_strokeWidth;
};