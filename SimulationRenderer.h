#pragma once
#include "pch.h"

#include "Control.h"
#include "HLSLStructures.h"
#include "MoveLookController.h"
#include "SimulationManager.h"

#include <memory>
#include <vector>

class SimulationRenderer : public Control
{
public:
	SimulationRenderer(const std::shared_ptr<DeviceResources>& deviceResources,
					   const std::shared_ptr<Layout>& parentLayout);

	SimulationRenderer(const std::shared_ptr<DeviceResources>& deviceResources,
					   const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan);

	void Update(StepTimer const& stepTimer) override;

	// Don't override Render2D() because there is no 2D rendering needed here
	bool Render3D() override;

	void OnLayoutResize() override { CreateWindowSizeDependentResources(); }
	void OnMarginChanged() override { CreateWindowSizeDependentResources(); }


private:
	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();
	void CreateStaticResources();

	void CreateVertexShaderAndInputLayout();
	void CreatePixelShader();
	void CreateBuffers();
	void CreateBox();



	std::unique_ptr<MoveLookController> m_moveLookController;

	CD3D11_VIEWPORT m_viewport;

	DirectX::XMFLOAT4X4 m_projection;
	DirectX::XMFLOAT4X4 m_view;
	DirectX::XMFLOAT4X4 m_inverseView;
	DirectX::XMFLOAT3   m_direction;
	DirectX::XMVECTOR   m_eyeVector;
	DirectX::XMFLOAT4X4 m_model;
	DirectX::XMFLOAT4X4 m_translation;

	// Object Independent Resources
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_vertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_pixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_inputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_modelViewProjectionBuffer;
	ModelViewProjectionConstantBuffer			m_modelViewProjectionBufferData;
	DirectX::XMMATRIX							m_viewMatrix;
	DirectX::XMMATRIX							m_projectionMatrix;

	// Light Properties
	LightProperties								m_lightProperties;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_lightPropertiesConstantBuffer;

	// MaterialProperties
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_materialPropertiesConstantBuffer;
	std::vector<MaterialProperties*>			m_materialProperties;

	// Box Resources
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_boxVertexBuffer;
	MaterialProperties							m_boxMaterialProperties;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_boxMaterialPropertiesConstantBuffer;
	DirectX::XMFLOAT3							m_boxDimensions;

};