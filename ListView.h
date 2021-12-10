#pragma once
#include "pch.h"

#include "Control.h"
#include "WindowManager.h"

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


	void OnLayoutResize() override { Resize(); }
	void OnMarginChanged() override { Resize(); }
	bool MouseIsOver(int x, int y) override;

	OnMessageResult OnLButtonDown(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnLButtonUp(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseMove(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseLeave() override;
	OnMessageResult OnMouseWheel(int wheelDelta) override;

	bool Render2D() override;


	// Set the format function
	void SetFormatFunction(std::function<std::shared_ptr<Layout>(std::shared_ptr<T>, bool)> function) 
	{
		FormatAddedItem = function;

		// if the item count is greater than 0, re-format all of the items
		if (ItemCount() > 0)
		{
			// First, clear all existing layouts
			m_itemLayouts.clear();

			// Second, add each item back with the new format
			for (unsigned int iii = 0; iii < m_items.size(); ++iii)
			{
				// Add the layout to the list of layouts (make sure to highlight the selected item)
				m_itemLayouts.push_back(this->FormatAddedItem(m_items[iii], iii == m_highlightedItemIndex));
			}
			
			// Update the sublayouts list
			this->UpdateSubLayouts();
		}
	}

	// Set the value changed update layout method
	void SetValueChangedUpdateLayoutMethod(std::function<void(std::shared_ptr<T>, std::shared_ptr<Layout>)> function) { ValueChangedUpdateLayoutMethod = function; }

	// Set the item click method
	void SetItemClickMethod(std::function<void(std::shared_ptr<T>)> function) { ItemClickMethod = function; }

	// Set highlighting methods
	void SetHighlightItemLayoutMethod(std::function<void(std::shared_ptr<Layout>)> function) { HighlightItemLayoutMethod = function; }
	void SetUnhighlightItemLayoutMethod(std::function<void(std::shared_ptr<Layout>)> function) { UnhighlightItemLayoutMethod = function; }

	// Add item to list and call FormatAddedItem to add it to the list
	void AddItem(std::shared_ptr<T> item);

	// Remove item from listview
	void RemoveItem(std::shared_ptr<T> item);

	// Set the item height - modify the rowCol structure
	void SetItemHeight(float height);

	// Function to call when an item changes in some way and the UI may need updating
	void ItemChanged(std::shared_ptr<T> item);

	// Function to replace a list view item with another
	void ReplaceItemAt(std::shared_ptr<T> newItem, int index);

	// Function to return the index of an item
	int ItemIndex(std::shared_ptr<T> item);

	// Function to select/highlight a specific item
	void HighlightItem(std::shared_ptr<T> item);
	void HighlightItem(int index);

	// Return true if item is the selected one
	bool IsItemHighlighted(std::shared_ptr<T> item);

	// Get the count of all items
	unsigned int ItemCount() { return static_cast<unsigned int>(m_items.size()); }


private:
	// Get the number of items that can fit inside the list view
	int MaxItemsCount();

	// Update the layouts that are bound as sublayouts in the main layout
	void UpdateSubLayouts();

	void Resize();

	std::vector<std::shared_ptr<T>> m_items;

	// vector to hold the layouts for each item contents
	std::vector<std::shared_ptr<Layout>> m_itemLayouts;

	// Layout for holding all controls
	std::shared_ptr<Layout> m_layout;

	// Represents the first index into the itemLayouts that is the top layout to be rendered
	unsigned int m_scrollOffset;

	// Must manually set the height of the items so the rows can be sized appropriately
	float m_itemHeight;

	// Index of selected/highlighted item
	int m_highlightedItemIndex;

	// Method that gets executed when a new item is added
	// This allows for custom formatting of the objects in the list view
	// Input Parameter 1: Pointer to the item that was just added
	// Input Parameter 2: Boolean - true if the item to be format is the selected/highlighted item
	// Return Value:    Pointer to the layout with the item UI contents
	std::function<std::shared_ptr<Layout>(std::shared_ptr<T>, bool)> FormatAddedItem;

	// Function to invoke when a value has changed and the corresponding layout needs updating
	// Input parameter 1: pointer to the item itself that has changed
	// Input parameter 2: pointer to the layout that holds the UI for the item
	std::function<void(std::shared_ptr<T>, std::shared_ptr<Layout>)> ValueChangedUpdateLayoutMethod;



	// Item click function
	std::function<void(std::shared_ptr<T>)> ItemClickMethod;

	// Methods to allow customization of highlighting/unhighlighting of items
	std::function<void(std::shared_ptr<Layout>)> HighlightItemLayoutMethod;
	std::function<void(std::shared_ptr<Layout>)> UnhighlightItemLayoutMethod;






};

template <class T>
ListView<T>::ListView(const std::shared_ptr<DeviceResources>& deviceResources, 
					  const std::shared_ptr<Layout>& parentLayout) :
	ListView<T>(deviceResources, parentLayout, 0, 0, 1, 1)
{
}

template <class T>
ListView<T>::ListView(const std::shared_ptr<DeviceResources>& deviceResources,
					  const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan) :
	Control(deviceResources, parentLayout, row, column, rowSpan, columnSpan),
	m_scrollOffset(0),
	m_highlightedItemIndex(-1),
	FormatAddedItem([](std::shared_ptr<T> item, bool highlighted) { return std::make_shared<Layout>(nullptr, 0.0f, 0.0f, 0.0f, 0.0f); }), // return an empty layout
	ValueChangedUpdateLayoutMethod([](std::shared_ptr<T> item, std::shared_ptr<Layout> layout) {}),
	ItemClickMethod([](std::shared_ptr<T> item) {}),
	HighlightItemLayoutMethod([](std::shared_ptr<Layout> layout) {}),
	UnhighlightItemLayoutMethod([](std::shared_ptr<Layout> layout) {})
{
	// Create its own layout not as a child of the parent
	// Because the default will be to have no margins, and row/column index = 0 and row/columnSpan = 1
	// we can automatically assign the layout to be the same as the (0,0) rectangle of the parent layout
	m_layout = std::make_shared<Layout>(deviceResources, GetParentRect());

	// Set a default height of 20 pixels
	// MUST call this after creating the layout
	SetItemHeight(20.0f);
}

template <class T>
void ListView<T>::Resize()
{
	// Call SetItemHeight with the previously set height
	// This will update the row/col definitions for the main layout
	this->SetItemHeight(m_itemHeight);

	// Next, resize the layout, which will use the new row / col definitions
	// to create the physical locations for each row
	D2D1_RECT_F rect = GetParentRect();
	rect.left += m_marginLeft;
	rect.right -= m_marginRight;
	rect.top += m_marginTop;
	rect.bottom -= m_marginBottom;

	m_layout->OnResize(rect);
}

template <class T>
int ListView<T>::MaxItemsCount()
{
	D2D1_RECT_F rect = this->GetParentRect();

	// adjust for margins
	rect.top += m_marginTop;
	rect.bottom -= m_marginBottom;

	// Return total height / item height
	return static_cast<int>((rect.bottom - rect.top) / m_itemHeight);
}

template <class T>
void ListView<T>::SetItemHeight(float height)
{
	m_itemHeight = height;

	// Get the max number of items that can fit
	int maxItems = this->MaxItemsCount();

	RowColDefinitions rowDefs;
	for (int iii = 0; iii < maxItems; ++iii)
		rowDefs.AddDefinition(ROW_COL_TYPE::ROW_COL_TYPE_FIXED, m_itemHeight);

	m_layout->SetRowDefinitions(rowDefs);
}

template <class T>
void ListView<T>::AddItem(std::shared_ptr<T> item)
{
	// Add the item to the list of items
	m_items.push_back(item);

	// Add the layout to the list of layouts (don't highlight the item by default)
	m_itemLayouts.push_back(this->FormatAddedItem(item, false));

	// If there is room in the list view to present this item, update the layouts 
	// that are bound to the main layout
	if (m_items.size() <= this->MaxItemsCount())
	{
		this->UpdateSubLayouts();
	}
}

template <class T>
void ListView<T>::RemoveItem(std::shared_ptr<T> item)
{
	int removeIndex = -1;

	for (unsigned int iii = 0; iii < m_items.size(); ++iii)
	{
		if (m_items[iii] == item)
		{
			removeIndex = iii;
			break;
		}
	}

	if (removeIndex != -1)
	{
		m_items.erase(m_items.begin() + removeIndex);
		m_itemLayouts[removeIndex]->ReleaseLayout();
		m_itemLayouts.erase(m_itemLayouts.begin() + removeIndex);

		// If we are removing the selected/highlighted item, then we must set the highlighted index
		// back to -1 so that we don't try to Unhighlight the corresponding layout
		// Otherwise, if the index to be removed is at a lower index, then we must decrement
		// the highlighted index accordingly
		if (removeIndex == m_highlightedItemIndex)
			m_highlightedItemIndex = -1;
		else if (removeIndex < m_highlightedItemIndex)
			--m_highlightedItemIndex;


		// if the scroll offset would leave an empty spot, then we must decrement the offset
		if (m_scrollOffset > 0 && m_scrollOffset + MaxItemsCount() > m_itemLayouts.size())
			--m_scrollOffset;

		UpdateSubLayouts();
	}
}

template <class T>
void ListView<T>::UpdateSubLayouts()
{
	// Must clear the sublayouts and then reassign
	m_layout->ClearSubLayouts();

	int maxItems = this->MaxItemsCount();

	for (int iii = 0; iii < maxItems; ++iii)
	{
		// break if there aren't enough items to fill the main layout
		if (iii == m_itemLayouts.size())
			break;
		
		// Assign the layout to the appropriate row within the list view
		m_layout->SetSubLayout(m_itemLayouts[iii + m_scrollOffset], iii, 0);
	}
}

template <class T>
void ListView<T>::ReleaseLayout()
{
	// Release the layout for the layouts in the main layout
	m_layout->ReleaseLayout();

	// Release all layouts in the list of layouts
	//		NOTE: this will cause duplicate release of any layouts that are bound as sublayouts
	//		but I think calling ReleaseLayout twice is fine
	for (std::shared_ptr<Layout> layout : m_itemLayouts)
		layout->ReleaseLayout();

	m_parentLayout = nullptr;
}

template <class T>
bool ListView<T>::Render2D()
{
	m_layout->Render2DControls();

	return true;
}

template <class T>
OnMessageResult ListView<T>::OnMouseMove(std::shared_ptr<MouseState> mouseState)
{
	return m_layout->OnMouseMove(mouseState);
}

template <class T>
OnMessageResult ListView<T>::OnLButtonDown(std::shared_ptr<MouseState> mouseState)
{
	return m_layout->OnLButtonDown(mouseState);
}

template <class T>
OnMessageResult ListView<T>::OnLButtonUp(std::shared_ptr<MouseState> mouseState)
{
	OnMessageResult result = m_layout->OnLButtonUp(mouseState);

	if (result != OnMessageResult::NONE)
	{
		std::shared_ptr<Layout> capturedLayout = m_layout->GetMouseCapturedLayout();

		if (capturedLayout != nullptr)
		{
			for (unsigned int iii = 0; iii < m_itemLayouts.size(); ++iii)
			{
				if (m_itemLayouts[iii] == capturedLayout)
				{
					// Highlight the clicked item
					this->HighlightItem(m_items[iii]);

					// Call the click method
					ItemClickMethod(m_items[iii]);
					break;
				}
			}
		}
	}

	return result;
}

template <class T>
OnMessageResult ListView<T>::OnMouseLeave()
{
	return m_layout->OnMouseLeave();
}

template <class T>
bool ListView<T>::MouseIsOver(int x, int y)
{
	return	x >= m_layout->Left() &&
			x <= m_layout->Right() &&
			y >= m_layout->Top() &&
			y <= m_layout->Bottom();
}

template <class T>
OnMessageResult ListView<T>::OnMouseWheel(int wheelDelta)
{
	// Only scroll if delta is >120 or <-120
	if (abs(wheelDelta) >= 120)
	{
		// If scrolling up, just decrement the offset by 1 and update layouts
		if (wheelDelta < 0)
		{
			if (m_scrollOffset > 0)
			{
				--m_scrollOffset;
				UpdateSubLayouts();
			}
		}
		else if (m_itemLayouts.size() > MaxItemsCount() + m_scrollOffset)
		{
			++m_scrollOffset;
			UpdateSubLayouts();
		}
	}

	//std::ostringstream oss;
	//oss << wheelDelta << '\n';
	//OutputDebugString(oss.str().c_str());

	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}

template <class T>
void ListView<T>::ItemChanged(std::shared_ptr<T> item)
{
	// Determine the item index
	for (unsigned int iii = 0; iii < m_items.size(); ++iii)
	{
		if (m_items[iii] == item)
		{
			// Get the layout for the item
			std::shared_ptr<Layout> itemLayout = m_itemLayouts[iii];

			// Call ValueChangedUpdateLayoutMethod to update the layout contents 
			// (even if the layout is not visible, because scrolling may make it visible)
			ValueChangedUpdateLayoutMethod(item, itemLayout);

			break;
		}
	}
}

template <class T>
void ListView<T>::ReplaceItemAt(std::shared_ptr<T> newItem, int index)
{
	m_items[index] = newItem;

	// Release the existing layout
	m_itemLayouts[index]->ReleaseLayout();

	// Add the new layout
	m_itemLayouts[index] = this->FormatAddedItem(newItem, index == m_highlightedItemIndex);

	// Update the bound layouts so the change can be rendered
	this->UpdateSubLayouts();
}

template <class T>
int ListView<T>::ItemIndex(std::shared_ptr<T> item)
{
	for (unsigned int iii = 0; iii < m_items.size(); ++iii)
	{
		if (m_items[iii] == item)
			return iii;
	}

	return -1;
}

template <class T>
void ListView<T>::HighlightItem(std::shared_ptr<T> item)
{
	int index = this->ItemIndex(item);
	if (index != -1)
	{
		this->HighlightItem(index);
	}
}

template <class T>
void ListView<T>::HighlightItem(int index)
{
	if (index != m_highlightedItemIndex)
	{
		if (m_highlightedItemIndex != -1)
		{
			// Un-highlight the previously highlighted item layout
			this->UnhighlightItemLayoutMethod(m_itemLayouts[m_highlightedItemIndex]);
		}

		m_highlightedItemIndex = index;

		// Highlight the new item
		this->HighlightItemLayoutMethod(m_itemLayouts[index]);
	}
}

template <class T>
bool ListView<T>::IsItemHighlighted(std::shared_ptr<T> item)
{
	return (m_highlightedItemIndex != -1) ? m_items[m_highlightedItemIndex] == item : false;
}