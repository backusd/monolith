#pragma once
#include "pch.h"
#include "Layout.h"
#include "OnMessageResult.h"
#include "MouseState.h"
#include "DeviceResources.h"
#include "StepTimer.h"

#include <memory>
#include <string>

class Control
{
public:
	Control(const std::shared_ptr<DeviceResources>& deviceResources, 
		    const std::shared_ptr<Layout>& parentLayout);

	Control(const std::shared_ptr<DeviceResources>& deviceResources, 
		    const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan);

	virtual ~Control() {}

	// Call OnLayoutResize when updating row/column to make sure the control resizes if necessary
	void Row(int row) { m_row = row; OnLayoutResize(); }
	void Column(int col) { m_column = col; OnLayoutResize(); }

	void RowSpan(int rowSpan) { m_rowSpan = rowSpan; }
	void ColumnSpan(int colSpan) { m_columnSpan = colSpan; }

	void Margin(float all);
	void Margin(float leftRight, float topBottom);
	void Margin(float left, float top, float right, float bottom);

	float MarginLeft() { return m_marginLeft; }
	float MarginTop() { return m_marginTop; }
	float MarginRight() { return m_marginRight; }
	float MarginBottom() { return m_marginBottom; }

	void Name(std::wstring name) { m_name = name; }
	std::wstring Name() { return m_name; }

	

	// Virtual functions

	virtual void ReleaseLayout() { m_deviceResources = nullptr; m_parentLayout = nullptr; }

	virtual void OnLayoutResize() {}
	virtual void OnMarginChanged() {}
	virtual bool MouseIsOver(int x, int y) { return false; }	// true if mouse is deemed over the control

	virtual OnMessageResult OnLButtonDown(std::shared_ptr<MouseState> mouseState) { return OnMessageResult::NONE; }
	virtual OnMessageResult OnLButtonUp(std::shared_ptr<MouseState> mouseState) { return OnMessageResult::NONE; }
	virtual OnMessageResult OnLButtonDoubleClick(std::shared_ptr<MouseState> mouseState) { return OnMessageResult::NONE; }

	virtual OnMessageResult OnMouseMove(std::shared_ptr<MouseState> mouseState) { return OnMessageResult::NONE; }
	virtual OnMessageResult OnMouseLeave() { return OnMessageResult::NONE; }
	virtual OnMessageResult OnMouseWheel(int wheelDelta) { return OnMessageResult::NONE; }

	virtual OnMessageResult OnKeyDown(unsigned char keycode) { return OnMessageResult::NONE; }
	virtual OnMessageResult OnKeyUp(unsigned char keycode) { return OnMessageResult::NONE; }
	virtual OnMessageResult OnChar(char key) { return OnMessageResult::NONE; }

	// Should ONLY be overridden by 3D rendering controls
	virtual void Update(StepTimer const& stepTimer) {}

	// All 3D rendering is performed prior to 2D rendering so that any menu
	// will be rendered on top of 3D controls. In theory, a custom control could implement
	// both 2D and 3D features, but the 3D features will still get rendered first
	virtual bool Render3D() { return false; } // false indicates the control has not made a draw call
	virtual bool Render2D() { return false; }

	// This function allows a control to only render the portion of itself that fits within the 
	// renderWindow parameter. This is useful for things like ListView which may need to clip
	// the first and/or last item that is visible
	virtual bool Render2D(D2D1_RECT_F renderWindow, bool clipTop = true) { return false; }

	D2D1_RECT_F GetParentRect() { return m_parentLayout->GetRect(m_row, m_column, m_rowSpan, m_columnSpan); }

protected:
	

	// Pointer to device resources
	std::shared_ptr<DeviceResources> m_deviceResources;

	// Keep pointer to the layout the control resides within
	std::shared_ptr<Layout> m_parentLayout;

	// (Unique) name for the control so it can be looked up later
	std::wstring m_name;

	int m_row;
	int m_column;

	int m_rowSpan;
	int m_columnSpan;

	float m_marginLeft;
	float m_marginTop;
	float m_marginRight;
	float m_marginBottom;

	
};