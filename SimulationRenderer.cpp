#include "SimulationRenderer.h"


using Microsoft::WRL::ComPtr;
using DirectX::XMMATRIX;
using DirectX::XMVECTOR;
using DirectX::XMFLOAT4X4;
using DirectX::XMFLOAT3;
using DirectX::XMFLOAT4;
using DirectX::XMVECTORF32;


SimulationRenderer::SimulationRenderer(const std::shared_ptr<DeviceResources>& deviceResources,
									   const std::shared_ptr<Layout>& parentLayout) :
	SimulationRenderer(deviceResources, parentLayout, 0, 0, 1, 1)
{

}

SimulationRenderer::SimulationRenderer(const std::shared_ptr<DeviceResources>& deviceResources,
									   const std::shared_ptr<Layout>& parentLayout, int row, int column, int rowSpan, int columnSpan) :
	Control(deviceResources, parentLayout, row, column, rowSpan, columnSpan),
	m_velocityArrowMaterial(nullptr),
	m_d3dDepthStencilState(nullptr),
	m_testCylinder(MeshManager::GetCylinderMesh()),
	m_rayOrigin(XMVECTOR()),
	m_rayEnd(XMVECTOR()),
	m_hoveredOver(false)
{
	// Create resources that will not change on window resizing and not device dependent
	// -- Must call this first because it will create the MoveLookController which will be used later
	CreateStaticResources();

	// Create resources that are dependent on the device
	CreateDeviceDependentResources();

	// Create Window size dependent resources
	CreateWindowSizeDependentResources();

	// Create background brush
	m_deviceResources->D2DDeviceContext()->CreateSolidColorBrush(
		D2D1::ColorF(D2D1::ColorF::CornflowerBlue, 1.0f), 
		m_backgroundColorBrush.ReleaseAndGetAddressOf()
	);
}

void SimulationRenderer::CreateDeviceDependentResources()
{
	CreateVertexShaderAndInputLayout();
	CreatePixelShader();
	CreateBuffers();
	CreateBox();
}

void SimulationRenderer::CreateWindowSizeDependentResources()
{
	D2D1_RECT_F rect = m_parentLayout->GetRect(m_row, m_column, m_rowSpan, m_columnSpan);

	// viewport	
	m_viewport = CD3D11_VIEWPORT(
		m_deviceResources->DIPSToPixels(rect.left + m_marginLeft),
		m_deviceResources->DIPSToPixels(rect.top + m_marginTop),
		m_deviceResources->DIPSToPixels(rect.right - m_marginRight - rect.left - m_marginLeft),
		m_deviceResources->DIPSToPixels(rect.bottom - m_marginBottom - rect.top - m_marginTop)
	);

	// Perspective Matrix
	float aspectRatio = (rect.right - rect.left) / (rect.bottom - rect.top); // width / height
	float fovAngleY = DirectX::XM_PI / 4;

	// This is a simple example of a change that can be made when the app is in portrait or snapped view
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// Note that the OrientationTransform3D matrix is post-multiplied here
	// in order to correctly orient the scene to match the display orientation.
	// This post-multiplication step is required for any draw calls that are
	// made to the swap chain render target. For draw calls to other targets,
	// this transform should not be applied.

	// This sample makes use of a right-handed coordinate system using row-major matrices.
	XMMATRIX perspectiveMatrix = DirectX::XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
	);

	XMFLOAT4X4 orientation = m_deviceResources->OrientationTransform3D();
	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	// Projection Matrix (No Transpose)
	m_projectionMatrix = perspectiveMatrix * orientationMatrix;

	// Set the view matrix
	m_viewMatrix = m_moveLookController->ViewMatrix();
}

void SimulationRenderer::CreateStaticResources()
{
	// box dimensions
	DirectX::XMFLOAT3 boxDimensions = SimulationManager::BoxDimensions();

	m_moveLookController = std::make_unique<MoveLookController>(boxDimensions);

	// Sphere Material

	// Load vector of material properties
	PhongMaterialProperties* dummy = new PhongMaterialProperties(); // Set a dummy property so that each element is at the index of its element type
	m_phongMaterialProperties.push_back(dummy);

	PhongMaterialProperties* hydrogen = new PhongMaterialProperties();
	hydrogen->Material.Emissive = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
	hydrogen->Material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	hydrogen->Material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	hydrogen->Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	hydrogen->Material.SpecularPower = 6.0f;

	m_phongMaterialProperties.push_back(hydrogen);

	PhongMaterialProperties* helium = new PhongMaterialProperties();
	helium->Material.Emissive = XMFLOAT4(0.4f, 0.14f, 0.14f, 1.0f);
	helium->Material.Ambient = XMFLOAT4(1.0f, 0.75f, 0.75f, 1.0f);
	helium->Material.Diffuse = XMFLOAT4(1.0f, 0.6f, 0.6f, 1.0f);
	helium->Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	helium->Material.SpecularPower = 6.0f;

	m_phongMaterialProperties.push_back(helium);

	PhongMaterialProperties* lithium = new PhongMaterialProperties();
	lithium->Material.Emissive = XMFLOAT4(0.15f, 0.0f, 0.15f, 1.0f);
	lithium->Material.Ambient = XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f);
	lithium->Material.Diffuse = XMFLOAT4(1.0f, 0.6f, 0.6f, 1.0f);
	lithium->Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	lithium->Material.SpecularPower = 6.0f;

	m_phongMaterialProperties.push_back(lithium);

	PhongMaterialProperties* beryllium = new PhongMaterialProperties();
	beryllium->Material.Emissive = XMFLOAT4(0.15f, 0.15f, 0.0f, 1.0f);
	beryllium->Material.Ambient = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	beryllium->Material.Diffuse = XMFLOAT4(1.0f, 1.0f, 0.0f, 1.0f);
	beryllium->Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	beryllium->Material.SpecularPower = 6.0f;

	m_phongMaterialProperties.push_back(beryllium);

	PhongMaterialProperties* boron = new PhongMaterialProperties();
	boron->Material.Emissive = XMFLOAT4(0.45f, 0.22f, 0.22f, 1.0f);
	boron->Material.Ambient = XMFLOAT4(1.0f, 0.45f, 0.45f, 1.0f);
	boron->Material.Diffuse = XMFLOAT4(1.0f, 0.8f, 0.8f, 1.0f);
	boron->Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	boron->Material.SpecularPower = 6.0f;

	m_phongMaterialProperties.push_back(boron);

	PhongMaterialProperties* carbon = new PhongMaterialProperties();
	carbon->Material.Emissive = XMFLOAT4(0.1f, 0.1f, 0.1f, 1.0f);
	carbon->Material.Ambient = XMFLOAT4(0.12f, 0.12f, 0.12f, 1.0f);
	carbon->Material.Diffuse = XMFLOAT4(0.8f, 0.8f, 0.8f, 1.0f);
	carbon->Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	carbon->Material.SpecularPower = 6.0f;

	m_phongMaterialProperties.push_back(carbon);

	PhongMaterialProperties* nitrogen = new PhongMaterialProperties();
	nitrogen->Material.Emissive = XMFLOAT4(0.0f, 0.0f, 0.3f, 1.0f);
	nitrogen->Material.Ambient = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	nitrogen->Material.Diffuse = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	nitrogen->Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	nitrogen->Material.SpecularPower = 6.0f;

	m_phongMaterialProperties.push_back(nitrogen);

	PhongMaterialProperties* oxygen = new PhongMaterialProperties();
	oxygen->Material.Emissive = XMFLOAT4(0.3f, 0.0f, 0.0f, 1.0f);
	oxygen->Material.Ambient = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	oxygen->Material.Diffuse = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	oxygen->Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	oxygen->Material.SpecularPower = 6.0f;

	m_phongMaterialProperties.push_back(oxygen);

	PhongMaterialProperties* flourine = new PhongMaterialProperties();
	flourine->Material.Emissive = XMFLOAT4(0.0f, 0.12f, 0.12f, 1.0f);
	flourine->Material.Ambient = XMFLOAT4(0.0f, 0.5f, 0.5f, 1.0f);
	flourine->Material.Diffuse = XMFLOAT4(0.0f, 0.2f, 1.0f, 1.0f);
	flourine->Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	flourine->Material.SpecularPower = 6.0f;

	m_phongMaterialProperties.push_back(flourine);

	PhongMaterialProperties* neon = new PhongMaterialProperties();
	neon->Material.Emissive = XMFLOAT4(0.1f, 0.3f, 0.3f, 1.0f);
	neon->Material.Ambient = XMFLOAT4(0.3f, 1.0f, 0.0f, 1.0f);
	neon->Material.Diffuse = XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f);
	neon->Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	neon->Material.SpecularPower = 6.0f;

	m_phongMaterialProperties.push_back(neon);

	// Velocity Arrow Material =================================================
	m_velocityArrowMaterial = std::make_unique<PhongMaterialProperties>();
	m_velocityArrowMaterial->Material.Emissive = XMFLOAT4(0.15f, 0.15f, 0.15f, 1.0f);
	m_velocityArrowMaterial->Material.Ambient = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_velocityArrowMaterial->Material.Diffuse = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);
	m_velocityArrowMaterial->Material.Specular = XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f);
	m_velocityArrowMaterial->Material.SpecularPower = 6.0f;

	// Box Material ============================================================
	m_boxMaterialProperties = PhongMaterialProperties();
	m_boxMaterialProperties.Material.Ambient = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_boxMaterialProperties.Material.Diffuse = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_boxMaterialProperties.Material.Specular = XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f);
	m_boxMaterialProperties.Material.SpecularPower = 10.0f;

	// Lighting ============================================================
	m_lightProperties = LightProperties();
	m_lightProperties.GlobalAmbient = XMFLOAT4(0.2f, 0.2f, 0.2f, 1.0f);

	// The initial eye position - you will want to modify MoveLookController so the Eye
	// position can be retrieved to also update the light position
	m_lightProperties.EyePosition = XMFLOAT4(0.0f, 0.0f, 2 * boxDimensions.z, 0.0f);

	// Add the lights
	static const XMVECTORF32 LightColors[MAX_LIGHTS] = {
		DirectX::Colors::White,
		DirectX::Colors::Orange,
		DirectX::Colors::Yellow,
		DirectX::Colors::Green,
		DirectX::Colors::Blue,
		DirectX::Colors::Indigo,
		DirectX::Colors::Violet,
		DirectX::Colors::White
	};

	static const LightType LightTypes[MAX_LIGHTS] = {
		PointLight, SpotLight, SpotLight, PointLight, SpotLight, SpotLight, SpotLight, PointLight
	};

	static const bool LightEnabled[MAX_LIGHTS] = {
		true, false, false, false, false, false, false, false
	};

	const int numLights = MAX_LIGHTS;
	for (int i = 0; i < numLights; ++i)
	{
		Light light;
		light.Enabled = static_cast<int>(LightEnabled[i]);
		light.LightType = LightTypes[i];
		light.Color = XMFLOAT4(LightColors[i]);
		light.SpotAngle = DirectX::XMConvertToRadians(45.0f);
		light.ConstantAttenuation = 1.0f;
		light.LinearAttenuation = 0.08f;
		light.QuadraticAttenuation = 0.0f;

		// Make the light slightly offset from the initial eye position
		//XMFLOAT4 LightPosition = XMFLOAT4(std::sin(totalTime + offset * i) * radius, 9.0f, std::cos(totalTime + offset * i) * radius, 1.0f);
		XMFLOAT4 LightPosition = XMFLOAT4(boxDimensions.x / 4, boxDimensions.y / 4, 2 * boxDimensions.z, 1.0f);
		light.Position = LightPosition;
		XMVECTOR LightDirection = DirectX::XMVectorSet(-LightPosition.x, -LightPosition.y, -LightPosition.z, 0.0f);
		XMStoreFloat4(&light.Direction, DirectX::XMVector3Normalize(LightDirection));

		m_lightProperties.Lights[i] = light;
	}

}

void SimulationRenderer::CreateVertexShaderAndInputLayout() 
{
	// SOLID shader =======================================================================
	
	ComPtr<ID3DBlob> solidBlob;
	ThrowIfFailed(
		D3DReadFileToBlob(L"SolidVertexShader.cso", solidBlob.ReleaseAndGetAddressOf())
	);
	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateVertexShader(
			solidBlob->GetBufferPointer(),
			solidBlob->GetBufferSize(),
			nullptr,
			m_solidVertexShader.ReleaseAndGetAddressOf()
		)
	);

	static const D3D11_INPUT_ELEMENT_DESC solidVertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateInputLayout(
			solidVertexDesc,
			ARRAYSIZE(solidVertexDesc),
			solidBlob->GetBufferPointer(),
			solidBlob->GetBufferSize(),
			m_solidInputLayout.ReleaseAndGetAddressOf()
		)
	);

	// PHONG shader =======================================================================
	// create vertex shader
	ComPtr<ID3DBlob> blob;
	ThrowIfFailed(
		D3DReadFileToBlob(L"PhongVertexShader.cso", blob.ReleaseAndGetAddressOf())
	);
	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateVertexShader(
			blob->GetBufferPointer(), 
			blob->GetBufferSize(), 
			nullptr, 
			m_phongVertexShader.ReleaseAndGetAddressOf()
		)
	);

	/* D3D11_INPUT_ELEMENT_DESC
	Parameter 1: Semantic Name           - must match the semantic used in VertexShaderInput
	Parameter 2: Semantic Index          - Only used when there are more than one element with the same semantic name
	Parameter 3: Format                  - DXGI_FORMAT_R32G32B32_FLOAT = a 3-component float for storing color info
	Parameter 4: Input Slot              - Value that identifies the input-assembler ???
	Parameter 5: Aligned Byte Offset     - Offset in bytes from the start of the vertex (Use D3D11_APPEND_ALIGNED_ELEMENT to have the value inferred)
	Parameter 6: Input Slot Class        - D3D11_INPUT_PER_VERTEX_DATA = Input data is per vertex
	Parameter 7: Instance Data Step Rate - Number of instance to draw using same instance data (must be 0 for per-vertex data)
	*/

	static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, D3D11_APPEND_ALIGNED_ELEMENT, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateInputLayout(
			vertexDesc,
			ARRAYSIZE(vertexDesc),
			blob->GetBufferPointer(),
			blob->GetBufferSize(),
			m_phongInputLayout.ReleaseAndGetAddressOf()
		)
	);
}

void SimulationRenderer::CreatePixelShader()
{
	// SOLID shader =======================================================================
	ComPtr<ID3DBlob> solidBlob;
	ThrowIfFailed(
		D3DReadFileToBlob(L"SolidPixelShader.cso", solidBlob.ReleaseAndGetAddressOf())
	);
	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreatePixelShader(
			solidBlob->GetBufferPointer(),
			solidBlob->GetBufferSize(),
			nullptr,
			m_solidPixelShader.ReleaseAndGetAddressOf()
		)
	);

	// PHONG shader =======================================================================
	// create pixel shader
	ComPtr<ID3DBlob> blob;
	ThrowIfFailed(
		D3DReadFileToBlob(L"PhongPixelShader.cso", blob.ReleaseAndGetAddressOf())
	);
	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreatePixelShader(
			blob->GetBufferPointer(), 
			blob->GetBufferSize(), 
			nullptr, 
			m_phongPixelShader.ReleaseAndGetAddressOf()
		)
	);
}

void SimulationRenderer::CreateBuffers()
{
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			m_modelViewProjectionBuffer.ReleaseAndGetAddressOf()
		)
	);

	// Phong Material constant buffer (Pixel Shader)
	CD3D11_BUFFER_DESC phongMaterialConstantBufferDesc(sizeof(PhongMaterialProperties), D3D11_BIND_CONSTANT_BUFFER);
	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&phongMaterialConstantBufferDesc,
			nullptr,
			m_phongMaterialPropertiesConstantBuffer.ReleaseAndGetAddressOf()
		)
	);

	// Solid material constant buffer (Pixel Shader)
	CD3D11_BUFFER_DESC solidMaterialConstantBufferDesc(sizeof(SolidMaterialProperties), D3D11_BIND_CONSTANT_BUFFER);
	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&solidMaterialConstantBufferDesc,
			nullptr,
			m_solidMaterialPropertiesConstantBuffer.ReleaseAndGetAddressOf()
		)
	);

	// Box Material constant buffer (Pixel Shader)
	CD3D11_BUFFER_DESC boxMaterialConstantBufferDesc(sizeof(PhongMaterialProperties), D3D11_BIND_CONSTANT_BUFFER);
	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&boxMaterialConstantBufferDesc,
			nullptr,
			m_boxMaterialPropertiesConstantBuffer.ReleaseAndGetAddressOf()
		)
	);

	// Lights constant buffer (Pixel Shader)
	CD3D11_BUFFER_DESC lightsConstantBufferDesc(sizeof(LightProperties), D3D11_BIND_CONSTANT_BUFFER);
	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&lightsConstantBufferDesc,
			nullptr,
			m_lightPropertiesConstantBuffer.ReleaseAndGetAddressOf()
		)
	);
}

void SimulationRenderer::CreateBox()
{
	// Create a unit cube - will be scaled up to size of simulation during rendering
	float x = 0.5f;
	float y = 0.5f;
	float z = 0.5f;

	std::vector<VertexPositionNormal> v(8); // box vertices
	v[0].position = XMFLOAT3(x, y, z);
	v[1].position = XMFLOAT3(-x, y, z);
	v[2].position = XMFLOAT3(x, -y, z);
	v[3].position = XMFLOAT3(x, y, -z);
	v[4].position = XMFLOAT3(-x, -y, z);
	v[5].position = XMFLOAT3(-x, y, -z);
	v[6].position = XMFLOAT3(x, -y, -z);
	v[7].position = XMFLOAT3(-x, -y, -z);

	for (VertexPositionNormal vertex : v)
		vertex.normal = XMFLOAT3(1.0f, 1.0f, 1.0f);

	std::vector<VertexPositionNormal> vertexList;
	// draw the square with all positive x
	vertexList.push_back(v[0]);
	vertexList.push_back(v[3]);
	vertexList.push_back(v[3]);
	vertexList.push_back(v[6]);
	vertexList.push_back(v[6]);
	vertexList.push_back(v[2]);
	vertexList.push_back(v[2]);
	vertexList.push_back(v[0]);

	// draw the square with all negative x
	vertexList.push_back(v[1]);
	vertexList.push_back(v[5]);
	vertexList.push_back(v[5]);
	vertexList.push_back(v[7]);
	vertexList.push_back(v[7]);
	vertexList.push_back(v[4]);
	vertexList.push_back(v[4]);
	vertexList.push_back(v[1]);

	// draw the four lines that connect positive x with negative x
	vertexList.push_back(v[0]);
	vertexList.push_back(v[1]);
	vertexList.push_back(v[3]);
	vertexList.push_back(v[5]);
	vertexList.push_back(v[6]);
	vertexList.push_back(v[7]);
	vertexList.push_back(v[2]);
	vertexList.push_back(v[4]);

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = vertexList.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC vertexBufferDesc(
		sizeof(VertexPositionNormal) * 24,
		D3D11_BIND_VERTEX_BUFFER
	);

	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			m_boxVertexBuffer.ReleaseAndGetAddressOf()
		)
	);
}


void SimulationRenderer::Update(StepTimer const& stepTimer)
{
	// There should only be a single simulation which is accessible via SimulationManager
	// Because multiple controls may need to read from the simulation data, the update to the simulation
	// must come from the main window, so that multiple controls don't try updating the simulation
	// 
	// So for now, all I need to do is update the MoveLookController
	D2D1_RECT_F rect = {};
	rect.left = m_viewport.TopLeftX;
	rect.top = m_viewport.TopLeftY;
	rect.right = rect.left + m_viewport.Width;
	rect.bottom = rect.top + m_viewport.Height;

	m_moveLookController->Update(stepTimer, rect);

	// If the move look controller is actively moving, update the view matrix
	if (m_moveLookController->IsMoving())
		m_viewMatrix = m_moveLookController->ViewMatrix();
}

bool SimulationRenderer::Render3D()
{
	// Compute the view-projection matrix at the beginning of each render
	m_viewProjectionMatrix = m_viewMatrix * m_projectionMatrix;

	// Fill in the background of the area to hold the 3D scene
	DrawBackground();

	// Set up pipeline configurations that will not change
 	ID3D11DeviceContext4* context = m_deviceResources->D3DDeviceContext();
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	context->UpdateSubresource(m_lightPropertiesConstantBuffer.Get(), 0, nullptr, &m_lightProperties, 0, 0);
	context->RSSetViewports(1, &m_viewport);


	this->SetShaderMode(ShaderMode::PHONG);


	// Draw everything that doesn't need special effects (ex. stenciling)
	DrawAtoms();
	DrawAtomVelocityArrows();
	DrawBonds();


	// Second rendering pass
	DrawHoveredAtom();
	DrawHoveredBond();
	DrawSelectedAtoms();
	DrawSelectedBonds();


	DrawBox();

	return true;
}

void SimulationRenderer::DrawBackground()
{
	// Fill the background of the 3D area to be rendered
	D2D1_RECT_F rect = {};
	rect.left = m_deviceResources->PixelsToDIPS(m_viewport.TopLeftX);
	rect.top = m_deviceResources->PixelsToDIPS(m_viewport.TopLeftY);
	rect.right = m_deviceResources->PixelsToDIPS(m_viewport.TopLeftX + m_viewport.Width);
	rect.bottom = m_deviceResources->PixelsToDIPS(m_viewport.TopLeftY + m_viewport.Height);

	ID2D1DeviceContext* context2 = m_deviceResources->D2DDeviceContext();
	context2->BeginDraw();
	context2->FillRectangle(rect, m_backgroundColorBrush.Get());
	context2->EndDraw();
}
void SimulationRenderer::DrawAtoms()
{
	// Set up the pipeline for drawing atoms
	this->SetStencilMode(StencilMode::NONE);	// Do not write to the stencil buffer
	this->SetShaderMode(ShaderMode::PHONG);		// Use Phong shading

	// Set the current element to invalid so that the first atom will set the material properties
	ELEMENT currentElement = Element::INVALID;

	for (std::shared_ptr<Atom> atom : SimulationManager::Atoms())
	{
		// Update the material type if necessary
		if (atom->ElementType() != currentElement)
		{
			currentElement = atom->ElementType();
			this->SetMaterialProperties(atom);
		}
		
		atom->Render(m_viewProjectionMatrix);
	}
}
void SimulationRenderer::DrawHoveredAtom()
{
	std::shared_ptr<Atom> atomHoveredOver = SimulationManager::AtomHoveredOver();

	// If there is an atom hovered over, draw an outline around the atom
	if (atomHoveredOver != nullptr)
	{
		// Step 1: Re-render the atom using the stencil buffer to set the pixels to mask the rendering in the next step
		this->SetMaterialProperties(atomHoveredOver);

		//		Set the stencil to mode to write so that we write 1's to the stencil buffer for the pixels that will be masked
		this->SetStencilMode(StencilMode::WRITE);

		this->SetShaderMode(ShaderMode::PHONG);		// Use Phong shading

		atomHoveredOver->Render(m_viewProjectionMatrix);


		// Step 2: Re-draw the atom slightly larger and in a different color using the stencil mask

		//		Set the stencil to mode to mask so that we don't render to pixels where the stencil buffer value is 1
		this->SetStencilMode(StencilMode::MASK);

		//		Set the shader mode to solid so we can draw solid color
		this->SetShaderMode(ShaderMode::SOLID);

		//     Change the color to solid purple
		this->SetMaterialProperties(DirectX::Colors::Purple);

		atomHoveredOver->RenderOutline(m_viewProjectionMatrix, 0.005f);
	}
}
void SimulationRenderer::DrawSelectedAtoms()
{

}
void SimulationRenderer::DrawAtomVelocityArrows()
{
	// Set up the pipeline for drawing the velocity arrows
	this->SetStencilMode(StencilMode::NONE);					// Do not write to the stencil buffer
	this->SetShaderMode(ShaderMode::PHONG);						// Use Phong shading
	this->SetMaterialProperties(m_velocityArrowMaterial.get()); // Set the velocity arrow material

	for (std::shared_ptr<Atom> atom : SimulationManager::Atoms())
	{
		atom->RenderVelocityArrow(m_viewProjectionMatrix);
	}
}
void SimulationRenderer::DrawBonds()
{
	// Set up the pipeline for drawing bonds
	this->SetStencilMode(StencilMode::NONE);	// Do not write to the stencil buffer
	this->SetShaderMode(ShaderMode::PHONG);		// Use Phong shading

	for (std::shared_ptr<Bond> bond : SimulationManager::Bonds())
	{
		// Set material property for first atom
		this->SetMaterialProperties(bond->Atom1());

		// Render atom1 to midpoint
		bond->RenderAtom1ToMidPoint(m_viewProjectionMatrix, m_moveLookController->Position());

		// Set material property for second atom
		this->SetMaterialProperties(bond->Atom2());

		// Render midpoint to atom2
		bond->RenderMidPointToAtom2(m_viewProjectionMatrix, m_moveLookController->Position());
	}
}
void SimulationRenderer::DrawHoveredBond()
{
	std::shared_ptr<Bond> bondHoveredOver = SimulationManager::GetBondHoveredOver();

	// If there is a bond hovered over, draw an outline around the bond
	if (bondHoveredOver != nullptr && bondHoveredOver->Atom1() != nullptr)
	{
		// Step 1: Re-render the bond using the stencil buffer to set the pixels to mask the rendering in the next step

		//		Set the stencil to mode to write so that we write 1's to the stencil buffer for the pixels that will be masked
		this->SetStencilMode(StencilMode::WRITE);

		this->SetShaderMode(ShaderMode::PHONG);

		//		Set the material to the first atom in the bond
		this->SetMaterialProperties(bondHoveredOver->Atom1());

		//		Render cylinder from atom1 to midpoint
		bondHoveredOver->RenderAtom1ToMidPoint(m_viewProjectionMatrix, m_moveLookController->Position());

		// Also re-draw the atoms so that they may also add to the stencil mask. This makes it so that the bond cylinder
		// inside the atom sphere is not visible
		//bondHoveredOver->Atom1()->Render(viewProjectionMatrix);


		//		Set the material to the second atom in the bond
		this->SetMaterialProperties(bondHoveredOver->Atom2());

		//		Render cylinder from midpoint to atom2
		bondHoveredOver->RenderMidPointToAtom2(m_viewProjectionMatrix, m_moveLookController->Position());

		// Also re-draw the atoms so that they may also add to the stencil mask. This makes it so that the bond cylinder
		// inside the atom sphere is not visible
		//bondHoveredOver->Atom2()->Render(viewProjectionMatrix);



		// Step 2: Re-draw the atom slightly larger and in a different color using the stencil mask

		//		Set the stencil to mode to mask so that we don't render to pixels where the stencil buffer value is 1
		this->SetStencilMode(StencilMode::MASK);

		//		Set the shader mode to solid so we can draw solid color
		this->SetShaderMode(ShaderMode::SOLID);

		//     Change the color to solid purple
		this->SetMaterialProperties(DirectX::Colors::Purple);

		//		Render the cylinder with the outline
		bondHoveredOver->RenderOutline(m_viewProjectionMatrix, m_moveLookController->Position(), 0.005f);
	}
}
void SimulationRenderer::DrawSelectedBonds()
{
	std::shared_ptr<Bond> selectedBond = SimulationManager::GetSelectedBond();

	// If there is a selected bond, draw an outline around the bond
	if (selectedBond != nullptr && selectedBond->Atom1() != nullptr)
	{
		// Step 1: Re-render the bond using the stencil buffer to set the pixels to mask the rendering in the next step

		//		Set the stencil to mode to write so that we write 1's to the stencil buffer for the pixels that will be masked
		this->SetStencilMode(StencilMode::WRITE);

		this->SetShaderMode(ShaderMode::PHONG);

		//		Set the material to the first atom in the bond
		this->SetMaterialProperties(selectedBond->Atom1());

		//		Render cylinder from atom1 to midpoint
		selectedBond->RenderAtom1ToMidPoint(m_viewProjectionMatrix, m_moveLookController->Position());

		// Also re-draw the atoms so that they may also add to the stencil mask. This makes it so that the bond cylinder
		// inside the atom sphere is not visible
		//bondHoveredOver->Atom1()->Render(viewProjectionMatrix);


		//		Set the material to the second atom in the bond
		this->SetMaterialProperties(selectedBond->Atom2());

		//		Render cylinder from midpoint to atom2
		selectedBond->RenderMidPointToAtom2(m_viewProjectionMatrix, m_moveLookController->Position());

		// Also re-draw the atoms so that they may also add to the stencil mask. This makes it so that the bond cylinder
		// inside the atom sphere is not visible
		//bondHoveredOver->Atom2()->Render(viewProjectionMatrix);



		// Step 2: Re-draw the atom slightly larger and in a different color using the stencil mask

		//		Set the stencil to mode to mask so that we don't render to pixels where the stencil buffer value is 1
		this->SetStencilMode(StencilMode::MASK);

		//		Set the shader mode to solid so we can draw solid color
		this->SetShaderMode(ShaderMode::SOLID);

		//     Change the color to solid purple
		this->SetMaterialProperties(DirectX::Colors::Green);

		//		Render the cylinder with the outline
		selectedBond->RenderOutline(m_viewProjectionMatrix, m_moveLookController->Position(), 0.005f);
	}
}
void SimulationRenderer::DrawBox()
{
	ID3D11DeviceContext4* context = m_deviceResources->D3DDeviceContext();

	UINT stride = sizeof(VertexPositionNormal);
	UINT offset = 0;
	ID3D11Buffer* const boxVertexBuffers[] = { m_boxVertexBuffer.Get() };
	context->IASetVertexBuffers(0, 1, boxVertexBuffers, &stride, &offset);
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_LINELIST);

	// Scale the box according to the current box dimensions
	XMFLOAT3 dims = SimulationManager::BoxDimensions();
	XMMATRIX model = DirectX::XMMatrixScaling(dims.x, dims.y, dims.z);

	DirectX::XMStoreFloat4x4(&m_modelViewProjectionBufferData.model, model);
	DirectX::XMStoreFloat4x4(&m_modelViewProjectionBufferData.modelViewProjection, model * m_viewProjectionMatrix);
	DirectX::XMStoreFloat4x4(&m_modelViewProjectionBufferData.inverseTransposeModel, XMMatrixTranspose(XMMatrixInverse(nullptr, model)));

	// Prepare the constant buffer to send it to the graphics device.
	context->UpdateSubresource1(m_modelViewProjectionBuffer.Get(), 0, NULL, &m_modelViewProjectionBufferData, 0, 0, 0);

	// Send the constant buffer to the graphics device.
	ID3D11Buffer* const boxConstantBuffers[] = { m_modelViewProjectionBuffer.Get() };
	context->VSSetConstantBuffers1(0, 1, boxConstantBuffers, nullptr, nullptr);

	this->SetMaterialProperties(&m_boxMaterialProperties);

	// Update the Material constant buffer for the box and then bind it to the pixel shader
	//context->UpdateSubresource(m_boxMaterialPropertiesConstantBuffer.Get(), 0, nullptr, &m_boxMaterialProperties, 0, 0);
	//ID3D11Buffer* const psBoxConstantBuffers[] = { m_boxMaterialPropertiesConstantBuffer.Get(), m_lightPropertiesConstantBuffer.Get() };
	//context->PSSetConstantBuffers1(0, 2, psBoxConstantBuffers, nullptr, nullptr);

	// Draw the objects.
	context->Draw(24, 0);
}

OnMessageResult SimulationRenderer::OnLButtonDown(std::shared_ptr<MouseState> mouseState)
{
	float _x = m_deviceResources->DIPSToPixels(static_cast<float>(mouseState->X()));
	float _y = m_deviceResources->DIPSToPixels(static_cast<float>(mouseState->Y()));
	
	// Make sure the move look controller is only updated when the user state allows it
	if (SimulationManager::GetUserState() != UserState::EDIT_BONDS)
		m_moveLookController->OnLButtonDown(_x, _y);

	// Inform the simulation manager in case we are clicking on an atom
	SimulationManager::SimulationClickDown();

	// There are no subcontrols or layouts, so we can just create the result directly
	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}

OnMessageResult SimulationRenderer::OnLButtonUp(std::shared_ptr<MouseState> mouseState)
{
	float _x = m_deviceResources->DIPSToPixels(static_cast<float>(mouseState->X()));
	float _y = m_deviceResources->DIPSToPixels(static_cast<float>(mouseState->Y()));

	m_moveLookController->OnLButtonUp(_x, _y);

	// Inform the simulation manager in case we are clicking on an atom
	SimulationManager::SimulationClickUp();

	// There are no subcontrols or layouts, so we can just create the result directly
	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}

OnMessageResult SimulationRenderer::OnLButtonDoubleClick(std::shared_ptr<MouseState> mouseState)
{
	// Set a MoveAction within the move look controller
	m_moveLookController->OnLButtonDoubleClick();

	// There are no subcontrols or layouts, so we can just create the result directly
	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}

OnMessageResult SimulationRenderer::OnMouseMove(std::shared_ptr<MouseState> mouseState)
{
	// If the mouse is no longer over the simulation contol, just return none
	if (!m_moveLookController->LButtonIsDown() && !MouseIsOver(mouseState->X(), mouseState->Y()))
		return OnMessageResult::NONE;

	float _x = m_deviceResources->DIPSToPixels(static_cast<float>(mouseState->X()));
	float _y = m_deviceResources->DIPSToPixels(static_cast<float>(mouseState->Y()));

	m_moveLookController->OnMouseMove(_x, _y);

	// if the simulation is paused -> perform picking
	if (SimulationManager::SimulationIsPaused())
		PerformPicking(_x, _y);

	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}

OnMessageResult SimulationRenderer::OnMouseLeave()
{
	m_moveLookController->OnMouseLeave();

	// There are no subcontrols or layouts, so we can just create the result directly
	return OnMessageResult::NONE;
}

OnMessageResult SimulationRenderer::OnMouseWheel(int wheelDelta)
{
	// Set a MoveAction within the move look controller
	m_moveLookController->OnMouseWheel(wheelDelta);

	// There are no subcontrols or layouts, so we can just create the result directly
	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}

OnMessageResult SimulationRenderer::OnKeyDown(unsigned char keycode)
{
	m_moveLookController->OnKeyDown(keycode);

	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}
OnMessageResult SimulationRenderer::OnKeyUp(unsigned char keycode)
{
	m_moveLookController->OnKeyUp(keycode);

	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}
OnMessageResult SimulationRenderer::OnChar(char key)
{
	switch (key)
	{
	case 'p': SimulationManager::SwitchPlayPause(); break;
	case 'c': m_moveLookController->CenterOnFace(); break;
	case 'w': m_moveLookController->RotateUp90(); break;
	case 'a': m_moveLookController->RotateLeft90(); break;
	case 's': m_moveLookController->RotateDown90(); break;
	case 'd': m_moveLookController->RotateRight90(); break;
	}

	return OnMessageResult::CAPTURE_MOUSE_AND_MESSAGE_HANDLED;
}


bool SimulationRenderer::MouseIsOver(int x, int y)
{
	float _x = m_deviceResources->DIPSToPixels(static_cast<float>(x));
	float _y = m_deviceResources->DIPSToPixels(static_cast<float>(y));

	return m_viewport.TopLeftX <= _x &&
		m_viewport.TopLeftY <= _y &&
		m_viewport.TopLeftX + m_viewport.Width >= _x &&
		m_viewport.TopLeftY + m_viewport.Height >= _y;
}

void SimulationRenderer::PerformPicking(float mouseX, float mouseY)
{
	/* Will update which atom the pointer is over
	*  To not affect performance, this method should only be called
	*  when the simulation is paused
	*/
	std::vector<std::shared_ptr<Atom>> atoms = SimulationManager::Atoms();

	XMFLOAT3 clickpointNear = XMFLOAT3(mouseX, mouseY, 0.0f);
	XMFLOAT3 clickpointFar = XMFLOAT3(mouseX, mouseY, 1.0f);

	XMVECTOR clickpointNearVector = XMLoadFloat3(&clickpointNear);
	XMVECTOR clickpointFarVector = XMLoadFloat3(&clickpointFar);

	float shortestDistance = FLT_MAX; // Set initial to the maximum possible float value
	float distance = FLT_MAX;
	
	std::shared_ptr<Atom> atomHoveredOver = nullptr;

	for (std::shared_ptr<Atom> atom : atoms)
	{
		if (atom->MouseIsOver(mouseX, mouseY, m_viewport, m_projectionMatrix, m_viewMatrix, distance))
		{
			if (distance < shortestDistance)
			{
				atomHoveredOver = atom;
				shortestDistance = distance;
			}
		}
	}

	// Perform cylinder/bond checking 

	float shortestBondDistance = FLT_MAX; // Set initial to the maximum possible float value
	float bondDistance = FLT_MAX;
	std::shared_ptr<Bond> bondHoveredOver = nullptr;

	for (std::shared_ptr<Bond> bond :  SimulationManager::Bonds())
	{
		// if an intersection is found, the distance will be returned in the 'distance' variable
		if (bond->MouseIsOver(mouseX, mouseY, m_viewport, m_projectionMatrix, m_viewMatrix, m_moveLookController->Position(), bondDistance))
		{
			if (bondDistance < shortestBondDistance)
			{
				bondHoveredOver = bond;
				shortestBondDistance = bondDistance;
			}
		}
	}

	// If a bond and an atom are both being hovered over, then find the closest to the camera and use that one
	if (bondHoveredOver != nullptr && atomHoveredOver != nullptr)
	{
		XMVECTOR cameraVector = m_moveLookController->Position();
		XMVECTOR bondCenterVector = bondHoveredOver->BondCenter();
		XMFLOAT3 atomCenter = atomHoveredOver->Position();
		XMVECTOR atomCenterVector = DirectX::XMLoadFloat3(&atomCenter);

		XMFLOAT3 cameraToBondDistance;
		DirectX::XMStoreFloat3(&cameraToBondDistance, DirectX::XMVector3Length(DirectX::XMVectorSubtract(cameraVector, bondCenterVector)));
		
		XMFLOAT3 cameraToAtomDistance;
		DirectX::XMStoreFloat3(&cameraToAtomDistance, DirectX::XMVector3Length(DirectX::XMVectorSubtract(cameraVector, atomCenterVector)));

		// Set the object not being hovered over to nullptr
		if (std::abs(cameraToBondDistance.x) < std::abs(cameraToAtomDistance.x))
			atomHoveredOver = nullptr;
		else
			bondHoveredOver = nullptr;
	}

	// Inform the SimulationManager - CAN be nullptr
	SimulationManager::AtomHoveredOver(atomHoveredOver);
	SimulationManager::BondHoveredOver(bondHoveredOver);
}



void SimulationRenderer::SetShaderMode(ShaderMode mode)
{
	ID3D11DeviceContext4* context = m_deviceResources->D3DDeviceContext();

	switch (mode)
	{
	case ShaderMode::PHONG:
		context->IASetInputLayout(m_phongInputLayout.Get());
		context->VSSetShader(m_phongVertexShader.Get(), nullptr, 0);
		context->PSSetShader(m_phongPixelShader.Get(), nullptr, 0);
		break;
	case ShaderMode::SOLID:
		context->IASetInputLayout(m_solidInputLayout.Get());
		context->VSSetShader(m_solidVertexShader.Get(), nullptr, 0);
		context->PSSetShader(m_solidPixelShader.Get(), nullptr, 0);
		break;
	}
}

void SimulationRenderer::SetStencilMode(StencilMode mode)
{
	D3D11_DEPTH_STENCIL_DESC dsDesc = CD3D11_DEPTH_STENCIL_DESC{ CD3D11_DEFAULT{} };

	/*
	* This is what the default settings are
	*
	dsDesc.DepthEnable    = TRUE;									// Depth testing is enabled
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;				// All depth bits can be written to
	dsDesc.DepthFunc      = D3D11_COMPARISON_LESS;					// Smaller depth values will be saved

	dsDesc.StencilEnable    = FALSE;								// Disable stencil testing
	dsDesc.StencilReadMask  = 0xFF;									// Read from entire stencil buffer
	dsDesc.StencilWriteMask = 0xFF;									// Write to entire stencil buffer

			// Pixels with normal facing the camera
	dsDesc.FrontFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;	// Stencil test will always pass
	dsDesc.FrontFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;	// Keep existing stencil data (do not overwrite)
	dsDesc.FrontFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;	//
	dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	//

			// Pixels with normal facing away from the camera
	dsDesc.BackFace.StencilFunc        = D3D11_COMPARISON_ALWAYS;	// Stencil test will always pass
	dsDesc.BackFace.StencilPassOp      = D3D11_STENCIL_OP_KEEP;		// Keep existing stencil data (do not overwrite)
	dsDesc.BackFace.StencilFailOp      = D3D11_STENCIL_OP_KEEP;		//
	dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;		//
	*/


	// This value will be the one that is used as the "source" stencil value that is compared
	// against the existing stencil value. We set it to 0 by default because that is what the
	// entire stencil buffered is set to (see ContentWindow->Render()).
	UINT referenceValue = 0;

	if (mode == StencilMode::WRITE)
	{
		// When we want to write to the stencil buffer, we want to set the stencil buffer value to 1
		referenceValue = 1;

		// Use compare_less_equal because an atom may be rendered twice BEFORE drawing the outline, 
		// and it is the second draw that will set the stencil buffer value to 1. So we need the second
		// draw to pass the depth test, which would fail if we just used compare_less because we are
		// draw the same object to the same location
		dsDesc.DepthFunc = D3D11_COMPARISON_LESS_EQUAL;

		// Enable stencil testing - write to entire stencil buffer
		dsDesc.StencilEnable = TRUE;
		dsDesc.StencilWriteMask = 0xFF;

		// For front facing, always pass the stencil test so each pixel sets the stencil value to 1
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;		// When stencil & depth pass, write to the stencil buffer
		dsDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;			// When stencil fails, don't write to stencil buffer

		//dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	// When stencil passes, but depth fails, don't write to stencil buffer
		dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;	// When stencil passes, but depth fails, do write to the stencil buffer
																		// So, when the selected atom is partially (or completely) behind other atoms
																		// the stencil mask will still be drawn so the outline will be visible through other objects

		// Don't modify any back facing pixels - never allow them to overwrite the stencil buffer
		//dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;




		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_REPLACE;		// When stencil & depth pass, write to the stencil buffer
		dsDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;			// When stencil fails, don't write to stencil buffer

		//dsDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_KEEP;	// When stencil passes, but depth fails, don't write to stencil buffer
		dsDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_REPLACE;


	}
	else if (mode == StencilMode::MASK)
	{
		// Disable depth testing so we are guaranteed to draw the pixels to the screen if it passes the stencil test
		dsDesc.DepthEnable = FALSE;

		// Enable stencil testing - Read from entire stencil buffer
		dsDesc.StencilEnable = TRUE;
		dsDesc.StencilReadMask = 0xFF;

		// We will leave the referenceValue to 0, so only pass the stencil test if we are on a pixel
		// where the stencil value has not been set to 1
		dsDesc.FrontFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		dsDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;		// In this case, keep the existing data (do not overwrite the mask)

		// Don't modify any back facing pixels - never allow them to overwrite the stencil buffer
		//dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_NEVER;





		dsDesc.BackFace.StencilFunc = D3D11_COMPARISON_EQUAL;
		dsDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;

	}

	ID3D11Device5* device = m_deviceResources->D3DDevice();
	ID3D11DeviceContext4* context = m_deviceResources->D3DDeviceContext();

	ThrowIfFailed(device->CreateDepthStencilState(&dsDesc, m_d3dDepthStencilState.ReleaseAndGetAddressOf()));
	context->OMSetDepthStencilState(m_d3dDepthStencilState.Get(), referenceValue);
}

void SimulationRenderer::SetMaterialProperties(std::shared_ptr<Atom> atom)
{
	// Set the material to that of the corresponding atom type
	SetMaterialProperties(m_phongMaterialProperties[atom->ElementType()]);
}

void SimulationRenderer::SetMaterialProperties(DirectXColor color)
{
	ID3D11DeviceContext4* context = m_deviceResources->D3DDeviceContext();

	// Create a new solid material and store the passed in color values
	SolidMaterialProperties* material = new SolidMaterialProperties();
	DirectX::XMStoreFloat4(&material->Color, color);

	context->UpdateSubresource(m_solidMaterialPropertiesConstantBuffer.Get(), 0, nullptr, material, 0, 0);
	ID3D11Buffer* const psConstantBuffers[] = { m_solidMaterialPropertiesConstantBuffer.Get() };
	context->PSSetConstantBuffers1(0, 1, psConstantBuffers, nullptr, nullptr);	

	delete material;
}

void SimulationRenderer::SetMaterialProperties(PhongMaterialProperties* material)
{
	ID3D11DeviceContext4* context = m_deviceResources->D3DDeviceContext();

	context->UpdateSubresource(m_phongMaterialPropertiesConstantBuffer.Get(), 0, nullptr, material, 0, 0);
	ID3D11Buffer* const psConstantBuffers[] = { m_phongMaterialPropertiesConstantBuffer.Get(), m_lightPropertiesConstantBuffer.Get() };
	context->PSSetConstantBuffers1(0, 2, psConstantBuffers, nullptr, nullptr);
}

