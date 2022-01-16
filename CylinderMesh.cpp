#include "CylinderMesh.h"

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;
using DirectX::XMVECTOR;

CylinderMesh::CylinderMesh(const std::shared_ptr<DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources),
	m_cylinderIndexCount(0),
	m_xyScaling(Constants::AtomicRadii[Element::HYDROGEN] / 3.0f)
{
	CreateModelViewProjectionBuffer();
	LoadCylinderVertexAndIndexBuffers();
}

void CylinderMesh::CreateModelViewProjectionBuffer()
{
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(ModelViewProjectionConstantBuffer), D3D11_BIND_CONSTANT_BUFFER);
	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			m_modelViewProjectionBuffer.ReleaseAndGetAddressOf()
		)
	);
}

void CylinderMesh::LoadCylinderVertexAndIndexBuffers()
{
	// Slices = number of points to place around the circular ends
	unsigned int slices = 40;

	// Number of vertices will be twice the number of slices because of the two ends
	unsigned int numVertices = slices * 2;

	// vector to hold the position and normal for each vertex
	std::vector<VertexPositionNormal> cylinderVertices(numVertices);

	XMFLOAT3 position;
	float angle;

	// Create cylinder vertices
	for (unsigned int a = 0; a < slices; ++a)
	{
		// Bottom cylinder has z = 0
		position.z = 0.0f;

		// Calculate x and y (radius is 1)
		angle = static_cast<float>(a) / static_cast<float>(slices) * DirectX::XM_2PI;
		position.x = static_cast<float>(cos(angle));
		position.y = static_cast<float>(sin(angle));

		// Assign vertex for lower circle
		// Position and normal are identical (radius is 1, so normal is already normalized)
		cylinderVertices[a].position = position;
		cylinderVertices[a].normal = position;

		// Top cylinder has z = 1
		position.z = 1.0f;

		// Assign vertex for top circle
		cylinderVertices[a + slices].position = position;
		cylinderVertices[a + slices].normal = position;
	}

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = cylinderVertices.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC vertexBufferDesc(
		sizeof(VertexPositionNormal) * numVertices,
		D3D11_BIND_VERTEX_BUFFER
	);

	m_cylinderVertexBuffer = nullptr;
	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			m_cylinderVertexBuffer.ReleaseAndGetAddressOf()
		)
	);



	// for each slice, there will be a rectangle formed between the top and bottom
	// set of vertices. Therefore 2 triangles must be made, each with 3 vertices
	unsigned int numIndices = slices * 2 * 3;
	std::vector<unsigned short> indices(numIndices);

	unsigned short i_1, i_2, i_3, i_4;

	for (unsigned short iii = 0; iii + 1 < static_cast<unsigned short>(slices); ++iii)
	{
		// Each slice consists of 4 vertices, two from the bottom and two from the top
		i_1 = iii;
		i_2 = iii + 1;
		i_3 = i_1 + slices;
		i_4 = i_2 + slices;

		// First triangle
		indices.push_back(i_1);
		indices.push_back(i_2);
		indices.push_back(i_4);

		// Second triangle
		indices.push_back(i_1);
		indices.push_back(i_3);
		indices.push_back(i_4);
	}

	// Add final two triangles where the cylinder wraps back around to the initial vertices
	// First triangle
	indices.push_back(slices - 1);
	indices.push_back(0);
	indices.push_back(slices);

	// Second triangle
	indices.push_back(slices - 1);
	indices.push_back(slices);
	indices.push_back(2 * slices - 1);

	m_cylinderIndexCount = static_cast<uint32_t>(indices.size());

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indices.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC indexBufferDesc(
		sizeof(unsigned short) * m_cylinderIndexCount,
		D3D11_BIND_INDEX_BUFFER
	);

	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			m_cylinderIndexBuffer.ReleaseAndGetAddressOf()
		)
	);
}

DirectX::XMMATRIX CylinderMesh::Render(XMFLOAT3 position1, XMFLOAT3 position2, XMMATRIX viewProjection)
{
	auto context = m_deviceResources->D3DDeviceContext();

	// Each vertex is one instance of the MyVertexPositionNormal struct.
	UINT stride = sizeof(VertexPositionNormal);
	UINT offset = 0;
	ID3D11Buffer* const vertexBuffers[] = { m_cylinderVertexBuffer.Get() };
	context->IASetVertexBuffers(0, 1, vertexBuffers, &stride, &offset);

	context->IASetIndexBuffer(m_cylinderIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// The cylinder circles are in the xy-plane, so we must scale them to an appropriate width for the atom radius
	// The z-direction must be scaled to match the distance between position1 and position2
	XMVECTOR p1Vector = DirectX::XMLoadFloat3(&position1);
	XMVECTOR p2Vector = DirectX::XMLoadFloat3(&position2);
	XMVECTOR differenceVector = DirectX::XMVectorSubtract(p2Vector, p1Vector);
	XMFLOAT3 magnitude;
	DirectX::XMStoreFloat3(&magnitude, DirectX::XMVector3Length(differenceVector));

	XMFLOAT3 difference;
	DirectX::XMStoreFloat3(&difference, differenceVector);

	if (magnitude.x > 0.0f)
	{
		m_modelMatrix = DirectX::XMMatrixScaling(m_xyScaling, m_xyScaling, magnitude.z) // scale the z-stretch to the length of the cylinder
			* ComputeRotationMatrix(difference)
			* DirectX::XMMatrixTranslation(position1.x, position1.y, position1.z);

		// Store model, modelviewprojection, and inversetransposemodel matrices
		XMStoreFloat4x4(&m_modelViewProjectionBufferData.model, m_modelMatrix);
		XMStoreFloat4x4(&m_modelViewProjectionBufferData.modelViewProjection, m_modelMatrix * viewProjection);
		XMStoreFloat4x4(&m_modelViewProjectionBufferData.inverseTransposeModel, XMMatrixTranspose(XMMatrixInverse(nullptr, m_modelMatrix)));

		// Prepare the constant buffer to send it to the graphics device.
		context->UpdateSubresource1(m_modelViewProjectionBuffer.Get(), 0, NULL, &m_modelViewProjectionBufferData, 0, 0, 0);

		// Send the constant buffer to the graphics device.
		ID3D11Buffer* const constantBuffers[] = { m_modelViewProjectionBuffer.Get() };
		context->VSSetConstantBuffers1(0, 1, constantBuffers, nullptr, nullptr);

		// Draw the objects.
		context->DrawIndexed(m_cylinderIndexCount, 0, 0);

		return m_modelMatrix;
	}
	
	return DirectX::XMMatrixIdentity();
}

XMMATRIX CylinderMesh::ComputeRotationMatrix(XMFLOAT3 velocity)
{
	XMMATRIX rotationMatrix = DirectX::XMMatrixIdentity();

	// If the velocity is only in the z direction, handle it manually
	if (velocity.x == 0.0f && velocity.y == 0.0f)
	{
		// If only in the negative z, just rotate over the x-axis
		if (velocity.z < 0.0f)
			rotationMatrix = DirectX::XMMatrixRotationAxis(DirectX::XMVectorSet(1.0f, 0.0f, 0.0f, 1.0f), DirectX::XM_PI);

		// If positive z, do nothing (just use the identity matrix)
	}
	else
	{
		// Compute the middle vector between the velocity vector and the z-axis
		XMVECTOR middleVector = DirectX::XMVectorAdd(DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f), DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&velocity)));
		rotationMatrix = DirectX::XMMatrixRotationAxis(middleVector, DirectX::XM_PI);
	}

	return rotationMatrix;
}