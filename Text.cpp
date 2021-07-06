#include "Text.h"



Text::Text(const std::shared_ptr<DeviceResources>& deviceResources, 
		   const std::shared_ptr<Layout>& parentLayout) :
	Control(deviceResources, parentLayout)
{

}


void Text::OnPaint(ID2D1HwndRenderTarget* renderTarget)
{

}