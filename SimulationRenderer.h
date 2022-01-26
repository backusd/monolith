#pragma once
#include "pch.h"

#include "Control.h"
#include "HLSLStructures.h"
#include "MoveLookController.h"
#include "SimulationManager.h"

#include <memory>
#include <vector>
#include <sstream>

typedef DirectX::XMVECTORF32 DirectXColor;

enum class StencilMode
{
	NONE,
	WRITE,
	MASK
};

enum class ShaderMode
{
	PHONG,
	SOLID
};

class SimulationRenderer : public Control
{
public:
	SimulationRenderer(const std::shared_ptr<DeviceResources>& deviceResources,
					   const std::shared_ptr<Layout>& parentLayout);

	SimulationRenderer(const std::shared_ptr<DeviceResources>& deviceResources,
					   const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan);

	~SimulationRenderer() override
	{
		int iii = 0;
	}

	void Update(StepTimer const& stepTimer) override;

	// Don't override Render2D() because there is no 2D rendering needed here
	bool Render3D() override;

	void OnLayoutResize() override { CreateWindowSizeDependentResources(); }
	void OnMarginChanged() override { CreateWindowSizeDependentResources(); }

	OnMessageResult OnLButtonDown(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnLButtonUp(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnLButtonDoubleClick(std::shared_ptr<MouseState> mouseState) override;

	OnMessageResult OnMouseMove(std::shared_ptr<MouseState> mouseState) override;
	OnMessageResult OnMouseLeave() override;
	OnMessageResult OnMouseWheel(int wheelDelta) override;

	OnMessageResult OnKeyDown(unsigned char keycode) override;
	OnMessageResult OnKeyUp(unsigned char keycode) override;
	OnMessageResult OnChar(char key) override;

	bool MouseIsOver(int x, int y) override;


private:
	void CreateDeviceDependentResources();
	void CreateWindowSizeDependentResources();
	void CreateStaticResources();

	void CreateVertexShaderAndInputLayout();
	void CreatePixelShader();
	void CreateBuffers();
	void CreateBox();

	void PerformPicking(float mouseX, float mouseY);

	// Pipeline Modification functions
	void SetShaderMode(ShaderMode mode);
	void SetStencilMode(StencilMode mode);

	void SetMaterialProperties(std::shared_ptr<Atom> atom); // Set pixel shader buffer to use atom specific material
	void SetMaterialProperties(DirectXColor color);			// Set pixel shader buffer to use a solid color
	void SetMaterialProperties(PhongMaterialProperties* material); // Pass a material pointer in directly (used for setting arrow material)

	// Render steps
	void DrawBackground();
	void DrawAtoms();
	//void DrawHoveredAtom();
	//void DrawSelectedAtoms();
	void DrawAtomVelocityArrows();
	void DrawBonds();
	//void DrawHoveredBond();
	//void DrawSelectedBonds();
	void DrawBox();

	void DrawStencilMask();
	void DrawHoveredAndSelectedAtomsAndBonds();

	Microsoft::WRL::ComPtr<ID2D1SolidColorBrush> m_backgroundColorBrush;

	std::unique_ptr<MoveLookController> m_moveLookController;

	CD3D11_VIEWPORT m_viewport;

	// Picking parameters
	DirectX::XMFLOAT4X4 m_projection;
	DirectX::XMFLOAT4X4 m_view;
	DirectX::XMFLOAT4X4 m_inverseView;
	DirectX::XMFLOAT3   m_direction;
	DirectX::XMVECTOR   m_eyeVector;
	DirectX::XMFLOAT4X4 m_model;
	DirectX::XMFLOAT4X4 m_translation;

	// Object Independent Resources
	Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_phongVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_phongPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_phongInputLayout;

	Microsoft::WRL::ComPtr<ID3D11VertexShader>	m_solidVertexShader;
	Microsoft::WRL::ComPtr<ID3D11PixelShader>	m_solidPixelShader;
	Microsoft::WRL::ComPtr<ID3D11InputLayout>	m_solidInputLayout;

	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_modelViewProjectionBuffer;
	ModelViewProjectionConstantBuffer			m_modelViewProjectionBufferData;
	DirectX::XMMATRIX							m_viewMatrix;
	DirectX::XMMATRIX							m_projectionMatrix;
	DirectX::XMMATRIX							m_viewProjectionMatrix;


	// Light Properties
	LightProperties								m_lightProperties;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_lightPropertiesConstantBuffer;

	// MaterialProperties
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_phongMaterialPropertiesConstantBuffer;
	std::vector<PhongMaterialProperties*>		m_phongMaterialProperties;
	std::unique_ptr<PhongMaterialProperties>	m_velocityArrowMaterial;

	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_solidMaterialPropertiesConstantBuffer;


	// Box Resources
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_boxVertexBuffer;
	PhongMaterialProperties						m_boxMaterialProperties;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		m_boxMaterialPropertiesConstantBuffer;

	// Pipeline configuration
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> m_d3dDepthStencilState;



	std::shared_ptr<CylinderMesh> m_testCylinder;
	XMVECTOR m_rayOrigin;
	XMVECTOR m_rayEnd;
	bool m_hoveredOver;
};