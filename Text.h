#pragma once
#include "pch.h"
#include "Control.h"


class Text : public Control
{
public:
	Text(const std::shared_ptr<DeviceResources>& deviceResources, 
		 const std::shared_ptr<Layout>& parentLayout);

	void OnPaint(ID2D1HwndRenderTarget* renderTarget) override;


};