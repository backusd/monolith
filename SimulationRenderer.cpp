#include "SimulationRenderer.h"


SimulationRenderer::SimulationRenderer(const std::shared_ptr<DeviceResources>& deviceResources,
									   const std::shared_ptr<Layout>& parentLayout) :
	SimulationRenderer(deviceResources, parentLayout, 0, 0, 1, 1)
{

}

SimulationRenderer::SimulationRenderer(const std::shared_ptr<DeviceResources>& deviceResources,
									   const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan) :
	Control(deviceResources, parentLayout, row, column, rowSpan, columnSpan)
{
	// viewport
	D2D1_RECT_F rect = m_parentLayout->GetRect(m_row, m_column, m_rowSpan, m_columnSpan);
	m_viewport = CD3D11_VIEWPORT(
		rect.left,
		rect.top,
		rect.right - rect.left,
		rect.bottom - rect.top
	);

	// box dimensions
	m_boxDimensions = DirectX::XMFLOAT3(2.0f, 2.0f, 2.0f);
}

void SimulationRenderer::Update()
{

}

bool SimulationRenderer::Render3D()
{
	// Don't try to render anything before the first Update.
	/*
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}
	

	ID3D11DeviceContext3* context = m_deviceResources->D3DDeviceContext();

	// Set a custom view port
	context->RSSetViewports(1, &m_viewport);

	// Reset render targets to the screen.
	ID3D11RenderTargetView* const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	const float color[] = { 0.0f, 0.0f, 1.0f, 1.0f }; // RGBA
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), color);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);
	*/



	return true;
}

