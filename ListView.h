#pragma once
#include "pch.h"

#include "Control.h"

#include <memory>
#include <vector>
#include <functional>

template <class T>
class ListView : public Control
{
public:
	ListView(const std::shared_ptr<DeviceResources>& deviceResources,
			 const std::shared_ptr<Layout>& parentLayout);

	ListView(const std::shared_ptr<DeviceResources>& deviceResources,
			 const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan);

	// To release the layout, just pass along to the button's layout
	void ReleaseLayout() override;

	// Add item to list and call FormatAddedItem to add it to the list
	void AddItem(std::shared_ptr<T> item);

	// Set the item height - modify the rowCol structure
	void SetItemHeight(float height);


private:
	std::vector<std::shared_ptr<T>> m_items;

	// vector to hold the layouts for each item contents
	std::vector<std::shared_ptr<T>> m_itemLayouts;

	// Layout for holding all controls
	std::shared_ptr<Layout> m_layout;

	// Must manually set the height of the items so the rows can be sized appropriately
	float m_itemHeight;

	// Method that gets executed when a new item is added
	// This allows for custom formatting of the objects in the list view
	// Input Parameter: Pointer to the item that was just added
	// Return Value:    Pointer to the layout with the item UI contents
	std::function<std::shared_ptr<Layout>(std::shared_ptr<T>) FormatAddedItem;
};

template <class T>
void ListView<T>::SetItemHeight(float height)
{
	m_itemHeight = height;

	// Add the appropriate number of rows
	D2D1_RECT_F rect = m_parentLayout->GetRect(
		0,
		0,
		m_parentLayout->RowCount(),
		m_parentLayout->ColumnCount()
	);

	float listViewHeight = rect.bottom - rect.top;
	int numberOfItems = static_cast<int>(listViewHeight / m_itemHeight);

	RowColDefinitions rowDefs;
	for (int iii = 0; iii < numberOfItems; ++iii)
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, m_itemHeight);

	m_layout->SetRowDefinitions(rowDefs);
}

template <class T>
void ListView<T>::AddItem(std::shared_ptr<T> item)
{
	// Add the item to the list of items
	m_items.push_back(item);

	// Create the list view item UI element
	std::shared_ptr<Layout> itemLayout = FormatAddedItem(item);

	// Add the layout to the list of layouts
	m_itemLayouts.push_back(itemLayout);

	// If there is room in the list view to present this item, add it to the appropriate row

}