#include "ArrowMesh.h"

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;
using DirectX::XMVECTOR;

ArrowMesh::ArrowMesh(const std::shared_ptr<DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources),
	m_indexCount(0)
{
	CreateModelViewProjectionBuffer();
	CreateAndLoadVertexAndIndexBuffers();
}

void ArrowMesh::CreateModelViewProjectionBuffer()
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

void ArrowMesh::CreateAndLoadVertexAndIndexBuffers()
{
	unsigned int slices = 40;
	
	// unsigned int numVertices = slices * 3 + 1; // vertices around the base and top of cylinder + base of cone + 1 for top of cone
	unsigned int numVertices = slices * 2;
	
	std::vector<VertexPositionNormal> arrowVertices(numVertices);

	XMFLOAT3 position;
	XMFLOAT3 normal;
	float angle;

	// Bottom of the cylinder vertices
	for (unsigned int a = 0; a < slices; ++a)
	{
		position.z = 0.0f;

		angle = static_cast<float>(a) / static_cast<float>(slices) * DirectX::XM_2PI;
		position.x = static_cast<float>(cos(angle));
		position.y = static_cast<float>(sin(angle));

		arrowVertices[a].position = position;
		arrowVertices[a].normal = position; // Normal will be the same as position because it is basically the unit circle
	}

	// Top of the cylinder vertices
	for (unsigned int a = 0; a < slices; ++a)
	{
		position.z = 1.0f;

		angle = static_cast<float>(a) / static_cast<float>(slices) * DirectX::XM_2PI;
		position.x = static_cast<float>(cos(angle));
		position.y = static_cast<float>(sin(angle));

		// Normal is same as position except no z component
		normal = position;
		normal.z = 0.0f;

		arrowVertices[a + slices].position = position;
		arrowVertices[a + slices].normal = normal;
	}

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = arrowVertices.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC vertexBufferDesc(
		sizeof(VertexPositionNormal) * numVertices,
		D3D11_BIND_VERTEX_BUFFER
	);

	m_vertexBuffer = nullptr;
	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			m_vertexBuffer.ReleaseAndGetAddressOf()
		)
	);

	// Load mesh indices. Each trio of indices represents
	// a triangle to be rendered on the screen.
	// For example: 0,2,1 means that the vertices with indexes
	// 0, 2 and 1 from the vertex buffer compose the 
	// first triangle of this mesh.

	// for each slice, there will be a rectangle formed between the top and bottom
	// set of vertices. Therefore 2 triangles must be made, each with 3 vertices
	unsigned int numIndices = slices * 2 * 3;
	std::vector<unsigned short> indices(numIndices);

	unsigned short i_1, i_2, i_3, i_4;

	for (unsigned short iii = 0; iii + 1 < slices; ++iii)
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
	indices.push_back(numVertices - 1);

	m_indexCount = indices.size();

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indices.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC indexBufferDesc(
		sizeof(unsigned short) * m_indexCount,
		D3D11_BIND_INDEX_BUFFER
	);

	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			m_indexBuffer.ReleaseAndGetAddressOf()
		)
	);

}

void ArrowMesh::Render(XMFLOAT3 position, XMFLOAT3 velocity, float radius, XMMATRIX viewProjection)
{
	auto context = m_deviceResources->D3DDeviceContext();

	// Each vertex is one instance of the MyVertexPositionNormal struct.
	UINT stride = sizeof(VertexPositionNormal);
	UINT offset = 0;
	ID3D11Buffer* const vertexBuffers[] = { m_vertexBuffer.Get() };
	context->IASetVertexBuffers(0, 1, vertexBuffers, &stride, &offset);

	context->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// The cylinder circles are in the xy-plane, so we must scale them to an appropriate width for the atom radius
	// The z-direction must be scaled to match the velocity of the atom
	XMVECTOR velocityVector = DirectX::XMLoadFloat3(&velocity);
	XMVECTOR magnitudeVector = DirectX::XMVector3Length(velocityVector);
	XMFLOAT3 magnitude;
	DirectX::XMStoreFloat3(&magnitude, magnitudeVector);

	if (magnitude.x > 0.0f)
	{
		XMMATRIX rotationMatrix = DirectX::XMMatrixIdentity();

		// If the velocity is only in the z direction, handle it manually
		if (velocity.x == 0.0f && velocity.y == 0.0f)
		{
			// If only in the negative z, just rotate over the x-axis
			if (velocity.z < 0.0f)
			{
				XMFLOAT3 xAxis = DirectX::XMFLOAT3(1.0f, 0.0f, 0.0f);
				XMVECTOR xAxisVector = DirectX::XMLoadFloat3(&xAxis);
				rotationMatrix = DirectX::XMMatrixRotationAxis(xAxisVector, DirectX::XM_PI);
			}

			// If positive z, do nothing (just use the identity matrix
		}
		else
		{
			XMFLOAT3 zAxis = DirectX::XMFLOAT3(0.0f, 0.0f, 1.0f);
			XMVECTOR zAxisVector = DirectX::XMLoadFloat3(&zAxis);

			XMVECTOR middleVector = DirectX::XMVectorAdd(zAxisVector, DirectX::XMVector3Normalize(velocityVector));
			rotationMatrix = DirectX::XMMatrixRotationAxis(middleVector, DirectX::XM_PI);
		}

		float xyScaling = radius / 3.0f;

		m_modelMatrix = DirectX::XMMatrixScaling(xyScaling, xyScaling, magnitude.x / 100.0f) // scale down the z-stretch by 100
			* rotationMatrix
			* DirectX::XMMatrixTranslation(position.x, position.y, position.z);


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
		context->DrawIndexed(m_indexCount, 0, 0);
	}
}