#pragma once
#include "pch.h"

#include "Control.h"

class SimulationRenderer : public Control
{
public:
	SimulationRenderer(const std::shared_ptr<DeviceResources>& deviceResources,
					   const std::shared_ptr<Layout>& parentLayout);

	SimulationRenderer(const std::shared_ptr<DeviceResources>& deviceResources,
					   const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan);

	void Update() override;

	// Don't override Render2D() because there is no 2D rendering needed here
	bool Render3D() override;


private:
	CD3D11_VIEWPORT m_viewport;

	DirectX::XMFLOAT3 m_boxDimensions;

};