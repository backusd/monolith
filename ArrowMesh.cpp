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



	// ==================================================================
	/*
	unsigned int segments = 26;
	unsigned int slices = segments / 2;
	unsigned int numVertices = (slices + 1) * (segments + 1) + 1;
	unsigned int numIndices = slices * segments * 3 * 2;

	std::vector<VertexPositionNormal> sphereVertices(numVertices);

	// To make the texture look right on the top and bottom of the sphere
	// each slice will have 'segments + 1' vertices.  The top and bottom
	// vertices will all be coincident, but have different U texture cooordinates.
	unsigned int p = 0;
	XMFLOAT3 positionNormal; // We are creating a unit sphere so the position is the same as the normal
	for (unsigned int a = 0; a <= slices; a++)
	{
		float angle1 = static_cast<float>(a) / static_cast<float>(slices) * DirectX::XM_PI;
		float z = static_cast<float>(cos(angle1));
		float r = static_cast<float>(sin(angle1));

		for (unsigned int b = 0; b <= segments; b++)
		{
			float angle2 = static_cast<float>(b) / static_cast<float>(segments) * DirectX::XM_2PI;

			// We are working with the unit sphere so the position and normal
			// vectors are the same
			positionNormal = XMFLOAT3(
				static_cast<float>(r * cos(angle2)),
				static_cast<float>(r * sin(angle2)),
				z
			);
			sphereVertices[p].position = positionNormal;
			sphereVertices[p].normal = positionNormal;

			p++;
		}
	}
	unsigned int vertexCount = p;

	D3D11_SUBRESOURCE_DATA vertexBufferDataSphere = { 0 };
	vertexBufferDataSphere.pSysMem = sphereVertices.data();
	vertexBufferDataSphere.SysMemPitch = 0;
	vertexBufferDataSphere.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC vertexBufferDescSphere(
		sizeof(VertexPositionNormal) * vertexCount,
		D3D11_BIND_VERTEX_BUFFER
	);

	m_vertexBuffer = nullptr;
	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&vertexBufferDescSphere,
			&vertexBufferDataSphere,
			m_vertexBuffer.ReleaseAndGetAddressOf()
		)
	);

	// Load mesh indices. Each trio of indices represents
	// a triangle to be rendered on the screen.
	// For example: 0,2,1 means that the vertices with indexes
	// 0, 2 and 1 from the vertex buffer compose the 
	// first triangle of this mesh.

	std::vector<unsigned short> sphereIndices(numIndices);

	p = 0;
	for (unsigned short a = 0; a < slices; a++)
	{
		unsigned short p1 = a * (segments + 1);
		unsigned short p2 = (a + 1) * (segments + 1);

		// Generate two triangles for each segment around the slice.
		for (unsigned short b = 0; b < segments; b++)
		{
			if (a < (slices - 1))
			{
				// For all but the bottom slice add the triangle with one
				// vertex in the a slice and two vertices in the a + 1 slice.
				// Skip it for the bottom slice since the triangle would be
				// degenerate as all the vertices in the bottom slice are coincident.
				sphereIndices[p] = b + p1;
				sphereIndices[p + 1] = b + p2;
				sphereIndices[p + 2] = b + p2 + 1;
				p = p + 3;
			}
			if (a > 0)
			{
				// For all but the top slice add the triangle with two
				// vertices in the a slice and one vertex in the a + 1 slice.
				// Skip it for the top slice since the triangle would be
				// degenerate as all the vertices in the top slice are coincident.
				sphereIndices[p] = b + p1;
				sphereIndices[p + 1] = b + p2 + 1;
				sphereIndices[p + 2] = b + p1 + 1;
				p = p + 3;
			}
		}
	}
	m_indexCount = p;

	D3D11_SUBRESOURCE_DATA indexBufferDataSphere = { 0 };
	indexBufferDataSphere.pSysMem = sphereIndices.data();
	indexBufferDataSphere.SysMemPitch = 0;
	indexBufferDataSphere.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC indexBufferDescSphere(
		sizeof(unsigned short) * m_indexCount,
		D3D11_BIND_INDEX_BUFFER
	);

	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&indexBufferDescSphere,
			&indexBufferDataSphere,
			m_indexBuffer.ReleaseAndGetAddressOf()
		)
	);
	*/
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

	// Compute the model matrix
	//m_modelMatrix = DirectX::XMMatrixScaling(radius, radius, radius) * DirectX::XMMatrixTranslation(position.x, position.y, position.z);
	
	// The cylinder circles are in the xy-plane, so we must scale them to an appropriate width for the atom radius
	// The z-direction must be scaled to match the velocity of the atom
	XMVECTOR velocityVector = DirectX::XMLoadFloat3(&velocity);
	XMVECTOR magnitudeVector = DirectX::XMVector3Length(velocityVector);
	XMFLOAT3 _magnitude;
	DirectX::XMStoreFloat3(&_magnitude, magnitudeVector);
	float magnitude = _magnitude.x;

	float xyScaling = radius / 3.0f;
	m_modelMatrix = DirectX::XMMatrixScaling(xyScaling, xyScaling, magnitude / 100.0f) // scale down the z-stretch by 100
		* DirectX::XMMatrixTranslation(position.x, position.y, position.z) 
		* DirectX::XMMatrixRotationX(DirectX::XM_PIDIV2);



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