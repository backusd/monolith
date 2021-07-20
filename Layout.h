#pragma once
#include "pch.h"
#include "RowCol.h"
#include "MouseState.h"
#include "OnMessageResult.h"
#include "DeviceResources.h"

#include <memory>
#include <vector>
#include <tuple>

class ContentWindow;
class Control;

class Layout : public std::enable_shared_from_this<Layout>
{
public:
	Layout(const std::shared_ptr<DeviceResources>& deviceResources,
		   float top, float left, float height, float width);
	Layout(const std::shared_ptr<DeviceResources>& deviceResources, 
		   D2D1_RECT_F rect);
	~Layout();

	void ClearContents();

	void SetRowDefinitions(RowColDefinitions rowDefs)    { m_rowDefinitions = rowDefs; UpdateLayout(); }
	void SetColumnDefinitions(RowColDefinitions colDefs) { m_columnDefinitions = colDefs; UpdateLayout(); }
	
	int RowCount()		{ return m_rows.size(); }
	int ColumnCount()	{ return m_columns.size(); }

	float Left()	{ return m_columns[0].Left(); }
	float Right()	{ return m_columns[m_columns.size() - 1].Left() + m_columns[m_columns.size() - 1].Width(); }
	float Top()		{ return m_rows[0].Top(); }
	float Bottom()	{ return m_rows[m_rows.size() - 1].Top() + m_rows[m_rows.size() - 1].Height(); }

	void OnResize(float top, float left, float height, float width);
	void OnResize(D2D1_RECT_F rect);

	std::shared_ptr<Layout> CreateSubLayout(int rowIndex, int columnIndex);

	template<typename T>
	std::shared_ptr<T> CreateControl();

	template<typename T>
	std::shared_ptr<T> CreateControl(int row, int column);

	template<typename T>
	std::shared_ptr<T> CreateControl(int row, int column, int rowSpan, int columnSpan);

	template<typename T>
	void AddControl(std::shared_ptr<T> control) { m_controls.push_back(control); }

	D2D1_RECT_F GetRect(int rowIndex, int columnIndex, int rowSpan, int columnSpan);

	void OnPaint();
	void PaintBorders();
	
	bool MouseIsOver(int x, int y) { return x >= Left() && x <= Right() && y >= Top() && y <= Bottom(); }

	std::shared_ptr<OnMessageResult> OnLButtonDown(const std::shared_ptr<MouseState>& mouseState);
	std::shared_ptr<OnMessageResult> OnLButtonUp(const std::shared_ptr<MouseState>& mouseState);
	std::shared_ptr<OnMessageResult> OnMouseMove(const std::shared_ptr<MouseState>& mouseState);
	std::shared_ptr<OnMessageResult> OnMouseLeave();

private:
	void UpdateLayout();

	std::shared_ptr<DeviceResources> m_deviceResources;

	// Row and Column definitions
	RowColDefinitions m_rowDefinitions;
	RowColDefinitions m_columnDefinitions;

	// Keep track of top, left, height and width to apply the layout
	float m_top;
	float m_left;
	float m_height;
	float m_width;

	// List of all controls
	std::vector<std::shared_ptr<Control>> m_controls;
	std::shared_ptr<Control> m_mouseCapturedControl;
	std::shared_ptr<Layout> m_mouseCapturedLayout;

	// Lists of all rows and columns in actual pixels within the layout
	std::vector<RowCol> m_rows;
	std::vector<RowCol> m_columns;

	// Keep a List of all sub-layouts, including the grid entry they belong to
	std::vector<std::tuple<std::shared_ptr<Layout>, int, int>> m_subLayouts;

};

template<typename T>
std::shared_ptr<T> Layout::CreateControl()
{
	return CreateControl<T>(0, 0, 1, 1);
}

template<typename T>
std::shared_ptr<T> Layout::CreateControl(int row, int column)
{
	return CreateControl<T>(row, column, 1, 1);
}

template<typename T>
std::shared_ptr<T> Layout::CreateControl(int row, int column, int rowSpan, int columnSpan)
{
	// Create the control - must use shared_from_this to pass shared pointer to Layout
	std::shared_ptr<T> control = std::make_shared<T>(m_deviceResources, shared_from_this(), row, column, rowSpan, columnSpan);

	// Add control to the list within the layout
	AddControl(std::static_pointer_cast<Control>(control));

	// return the control
	return control;
}