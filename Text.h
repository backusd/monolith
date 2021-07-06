#pragma once
#include "pch.h"
#include "Control.h"

#include <string>

class Text : public Control
{
public:
	Text(const std::shared_ptr<DeviceResources>& deviceResources, 
		 const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan);

	void OnPaint(ID2D1HwndRenderTarget* renderTarget) override;


};