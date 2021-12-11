#pragma once
#include "pch.h"
#include "Theme.h"
#include "MouseOverDown.h"


class BorderTheme : public Theme
{
public:
	BorderTheme(std::string themeName, const std::shared_ptr<DeviceResources>& deviceResources);

	ID2D1SolidColorBrush* GetBrush(MouseOverDown state) const;
	ID2D1StrokeStyle* GetStrokeStyle() const;
	
	float GetStrokeWidthLeft() { return m_strokeWidthLeft; }
	float GetStrokeWidthTop() { return m_strokeWidthTop; }
	float GetStrokeWidthRight() { return m_strokeWidthRight; }
	float GetStrokeWidthBottom() { return m_strokeWidthBottom; }

	void SetAllColors(D2D1_COLOR_F color);
	void SetColor(D2D1_COLOR_F color);
	void SetColorPointerOver(D2D1_COLOR_F color);
	void SetColorPointerDown(D2D1_COLOR_F color);

	void SetStrokeStyle(const float* dashes, UINT32 dashesCount, D2D1_CAP_STYLE startCap, D2D1_CAP_STYLE endCap, D2D1_CAP_STYLE dashCap, D2D1_LINE_JOIN lineJoin, float miterLimit, D2D1_DASH_STYLE dashStyle, float dashOffset);

	void SetStrokeWidth(float all) { SetStrokeWidth(all, all, all, all); }
	void SetStrokeWidth(float leftRight, float topBottom) { SetStrokeWidth(leftRight, topBottom, leftRight, topBottom); }
	void SetStrokeWidth(float left, float top, float right, float bottom) { m_strokeWidthLeft = left; m_strokeWidthTop = top; m_strokeWidthRight = right; m_strokeWidthBottom = bottom; }

private:
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_color;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_colorPointerOver;
	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_colorPointerDown;

	Microsoft::WRL::ComPtr<ID2D1StrokeStyle> m_strokeStyle;

	float m_strokeWidthLeft;
	float m_strokeWidthTop;
	float m_strokeWidthRight;
	float m_strokeWidthBottom;
};