#pragma once
#include "pch.h"
#include "Theme.h"


class LineTheme : public Theme
{
public:
	LineTheme(std::string themeName, const std::shared_ptr<DeviceResources>& deviceResources);

	ID2D1SolidColorBrush* GetBrush() const { return m_color.Get(); }
	ID2D1StrokeStyle* GetStrokeStyle() const;

	void SetStrokeStyle(const float* dashes, UINT32 dashesCount, D2D1_CAP_STYLE startCap, D2D1_CAP_STYLE endCap, D2D1_CAP_STYLE dashCap, D2D1_LINE_JOIN lineJoin, float miterLimit, D2D1_DASH_STYLE dashStyle, float dashOffset);

	float GetStrokeWidth() { return m_strokeWidth; }

	void SetColor(D2D1_COLOR_F color);
	void SetStrokeWidth(float width) { m_strokeWidth = width; }

private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_color;

	Microsoft::WRL::ComPtr<ID2D1StrokeStyle> m_strokeStyle;

	float m_strokeWidth;
};