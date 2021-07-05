#pragma once
#include "pch.h"
#include "Layout.h"
#include "OnMessageResult.h"
#include "MouseState.h"
#include "DeviceResources.h"

#include <memory>

class Control
{
public:
	Control(const std::shared_ptr<Layout>& parentLayout);
	Control(const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan);

	void Row(int row) { m_row = row; }
	void Column(int col) { m_column = col; }

	void RowSpan(int rowSpan) { m_rowSpan = rowSpan; }
	void ColumnSpan(int colSpan) { m_columnSpan = colSpan; }

	void Margin(float all);
	void Margin(float leftRight, float topBottom);
	void Margin(float left, float top, float right, float bottom);

	// Virtual functions
	virtual void OnPaint(ID2D1HwndRenderTarget* renderTarget) = 0;
	virtual void OnLayoutResize() {}
	virtual void OnMarginChanged() {}
	virtual bool MouseIsOver(int x, int y) { return false; }	// true if mouse is deemed over the control
	virtual std::shared_ptr<OnMessageResult> OnLButtonDown(std::shared_ptr<MouseState> mouseState) { return std::make_shared<OnMessageResult>(); }
	virtual std::shared_ptr<OnMessageResult> OnLButtonUp(std::shared_ptr<MouseState> mouseState)   { return std::make_shared<OnMessageResult>(); }
	virtual std::shared_ptr<OnMessageResult> OnMouseMove(std::shared_ptr<MouseState> mouseState)   { return std::make_shared<OnMessageResult>(); }
	virtual std::shared_ptr<OnMessageResult> OnMouseLeave() { return std::make_shared<OnMessageResult>(); }

protected:

	// Add shared pointer to device resources

	//...

	// Keep pointer to the layout the control resides within
	const std::shared_ptr<Layout> m_parentLayout;

	int m_row;
	int m_column;

	int m_rowSpan;
	int m_columnSpan;

	float m_marginLeft;
	float m_marginTop;
	float m_marginRight;
	float m_marginBottom;

	
};