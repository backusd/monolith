#pragma once
#include "pch.h"
#include "Layout.h"
#include "OnMessageResult.h"
#include "MouseState.h"
#include "DeviceResources.h"
#include "StepTimer.h"

#include <memory>

class Control
{
public:
	Control(const std::shared_ptr<DeviceResources>& deviceResources, 
		    const std::shared_ptr<Layout>& parentLayout);

	Control(const std::shared_ptr<DeviceResources>& deviceResources, 
		    const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan);

	~Control();

	void Row(int row) { m_row = row; }
	void Column(int col) { m_column = col; }

	void RowSpan(int rowSpan) { m_rowSpan = rowSpan; }
	void ColumnSpan(int colSpan) { m_columnSpan = colSpan; }

	void Margin(float all);
	void Margin(float leftRight, float topBottom);
	void Margin(float left, float top, float right, float bottom);

	// Virtual functions
	virtual void ClearContents() {}
	// virtual void OnPaint() = 0;
	virtual void OnLayoutResize() {}
	virtual void OnMarginChanged() {}
	virtual bool MouseIsOver(int x, int y) { return false; }	// true if mouse is deemed over the control
	virtual std::shared_ptr<OnMessageResult> OnLButtonDown(std::shared_ptr<MouseState> mouseState) { return std::make_shared<OnMessageResult>(); }
	virtual std::shared_ptr<OnMessageResult> OnLButtonUp(std::shared_ptr<MouseState> mouseState)   { return std::make_shared<OnMessageResult>(); }
	virtual std::shared_ptr<OnMessageResult> OnMouseMove(std::shared_ptr<MouseState> mouseState)   { return std::make_shared<OnMessageResult>(); }
	virtual std::shared_ptr<OnMessageResult> OnMouseLeave() { return std::make_shared<OnMessageResult>(); }
	
	virtual std::shared_ptr<OnMessageResult> OnKeyDown(unsigned char keycode) { return std::make_shared<OnMessageResult>(); }
	virtual std::shared_ptr<OnMessageResult> OnKeyUp(unsigned char keycode)   { return std::make_shared<OnMessageResult>(); }

	// Should ONLY be overridden by 3D rendering controls
	virtual void Update(StepTimer const& stepTimer) {}

	// All 3D rendering is performed prior to 2D rendering so that any menu
	// will be rendered on top of 3D controls. In theory, a custom control could implement
	// both 2D and 3D features, but the 3D features will still get rendered first
	virtual bool Render3D() { return false; } // false indicates the control has not made a draw call
	virtual bool Render2D() { return false; }

protected:
	D2D1_RECT_F GetParentRect() { return m_parentLayout->GetRect(m_row, m_column, m_rowSpan, m_columnSpan); }

	// Pointer to device resources
	std::shared_ptr<DeviceResources> m_deviceResources;

	// Keep pointer to the layout the control resides within
	std::shared_ptr<Layout> m_parentLayout;

	int m_row;
	int m_column;

	int m_rowSpan;
	int m_columnSpan;

	float m_marginLeft;
	float m_marginTop;
	float m_marginRight;
	float m_marginBottom;

	
};