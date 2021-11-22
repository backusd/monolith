#include "ArrowMesh.h"

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;
using DirectX::XMVECTOR;

ArrowMesh::ArrowMesh(const std::shared_ptr<DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources),
	m_cylinderIndexCount(0),
	m_xyScaling(Constants::AtomicRadii[Element::HYDROGEN] / 3.0f)
{
	CreateModelViewProjectionBuffer();
	LoadCylinderVertexAndIndexBuffers();
	LoadConeVertexAndIndexBuffers();
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

void ArrowMesh::LoadCylinderVertexAndIndexBuffers()
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

void ArrowMesh::LoadConeVertexAndIndexBuffers()
{
	// Slices = number of points to place around the circle
	unsigned int slices = 40;

	// Number of vertices will be the number of slices plus one for the center
	// of the circle and one for the top of the cone
	unsigned int numVertices = slices + 2;

	// vector to hold the position and normal for each vertex
	std::vector<VertexPositionNormal> coneVertices(numVertices);

	XMFLOAT3 position = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 normal = DirectX::XMFLOAT3(0.0f, 0.0f, -1.0f);
	float angle;

	// Add the center of the cone vertex first
	coneVertices[0].position = position;
	coneVertices[0].normal = normal;

	// Bottom of the cone vertices
	for (unsigned int a = 0; a < slices; ++a)
	{
		// Z is 0 so that the circle lies in the xy plane
		position.z = 0.0f;

		// Calculate x and y (radius is 1)
		angle = static_cast<float>(a) / static_cast<float>(slices) * DirectX::XM_2PI;
		position.x = static_cast<float>(cos(angle));
		position.y = static_cast<float>(sin(angle));

		// Normal is same as position except no z component
		normal = position;
		normal.z = 0.0f;

		// Must normalize the normal
		DirectX::XMStoreFloat3(&normal, DirectX::XMVector3Normalize(DirectX::XMLoadFloat3(&normal)));

		// Add one to the index because the circle center vertex is at index 0
		coneVertices[a + 1].position = position;
		coneVertices[a + 1].normal = normal;
	}

	// Top of the cone vertex
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = 0.5f;

	normal = position;
	normal.z = 1.0f;

	coneVertices[numVertices - 1].position = position;
	coneVertices[numVertices - 1].normal = normal;

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = coneVertices.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC vertexBufferDesc(
		sizeof(VertexPositionNormal) * numVertices,
		D3D11_BIND_VERTEX_BUFFER
	);

	m_coneVertexBuffer = nullptr;
	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			m_coneVertexBuffer.ReleaseAndGetAddressOf()
		)
	);


	// for each slice, we need to create two triangles: 1) triangle to the top of the
	// cone and 2) triangle to the center of the circle
	unsigned int numIndices = slices * 2 * 3;
	std::vector<unsigned short> indices(numIndices);

	unsigned short circleCenter = 0;
	unsigned short coneTop = numVertices - 1;
	unsigned short i_1, i_2;

	// Start at index 1 because circle center is at index 0
	for (unsigned short iii = 1; iii <= slices; ++iii)
	{
		i_1 = iii;

		// if we are one the last index, the next index needs to be 1
		i_2 = (iii == slices) ? 1 : iii + 1;

		// Bottom triangle
		indices.push_back(circleCenter);
		indices.push_back(i_1);
		indices.push_back(i_2);

		// Top triangle
		indices.push_back(coneTop);
		indices.push_back(i_1);
		indices.push_back(i_2);
	}

	m_coneIndexCount = static_cast<uint32_t>(indices.size());

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indices.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	CD3D11_BUFFER_DESC indexBufferDesc(
		sizeof(unsigned short) * m_coneIndexCount,
		D3D11_BIND_INDEX_BUFFER
	);

	ThrowIfFailed(
		m_deviceResources->D3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			m_coneIndexBuffer.ReleaseAndGetAddressOf()
		)
	);
}

void ArrowMesh::Render(XMFLOAT3 position, XMFLOAT3 velocity, float radius, XMMATRIX viewProjection)
{
	RenderCylinder(position, velocity, radius, viewProjection);
	RenderCone(position, velocity, radius, viewProjection);
}

void ArrowMesh::RenderCylinder(XMFLOAT3 position, XMFLOAT3 velocity, float radius, XMMATRIX viewProjection)
{
	auto context = m_deviceResources->D3DDeviceContext();

	// Each vertex is one instance of the MyVertexPositionNormal struct.
	UINT stride = sizeof(VertexPositionNormal);
	UINT offset = 0;
	ID3D11Buffer* const vertexBuffers[] = { m_cylinderVertexBuffer.Get() };
	context->IASetVertexBuffers(0, 1, vertexBuffers, &stride, &offset);

	context->IASetIndexBuffer(m_cylinderIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// The cylinder circles are in the xy-plane, so we must scale them to an appropriate width for the atom radius
	// The z-direction must be scaled to match the velocity of the atom
	XMVECTOR velocityVector = DirectX::XMLoadFloat3(&velocity);
	XMFLOAT3 magnitude;
	DirectX::XMStoreFloat3(&magnitude, DirectX::XMVector3Length(velocityVector));

	// Compute the position on the surface of sphere
	// This should the the center of the sphere vector plus the velocity vector scaled down to the size of the radius
	XMVECTOR velocityVectorScaled = DirectX::XMVectorScale(DirectX::XMVector3Normalize(velocityVector), radius - 0.005f); // Take a little bit off the radius otherwise there will be a small gap
	XMVECTOR positionVector = DirectX::XMLoadFloat3(&position);
	XMVECTOR surfacePositionVector = DirectX::XMVectorAdd(positionVector, velocityVectorScaled);

	XMFLOAT3 surfacePosition;
	DirectX::XMStoreFloat3(&surfacePosition, surfacePositionVector);

	if (magnitude.x > 0.0f)
	{
		m_modelMatrix = DirectX::XMMatrixScaling(m_xyScaling, m_xyScaling, magnitude.x / 100.0f) // scale down the z-stretch by 100
			* ComputeRotationMatrix(velocity)
			* DirectX::XMMatrixTranslation(surfacePosition.x, surfacePosition.y, surfacePosition.z);

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
	}
}

void ArrowMesh::RenderCone(XMFLOAT3 position, XMFLOAT3 velocity, float radius, XMMATRIX viewProjection)
{
	auto context = m_deviceResources->D3DDeviceContext();

	// Each vertex is one instance of the MyVertexPositionNormal struct.
	UINT stride = sizeof(VertexPositionNormal);
	UINT offset = 0;
	ID3D11Buffer* const vertexBuffers[] = { m_coneVertexBuffer.Get() };
	context->IASetVertexBuffers(0, 1, vertexBuffers, &stride, &offset);

	context->IASetIndexBuffer(m_coneIndexBuffer.Get(), DXGI_FORMAT_R16_UINT, 0);

	// The cylinder circles are in the xy-plane, so we must scale them to an appropriate width for the atom radius
	// The z-direction must be scaled to match the velocity of the atom
	XMVECTOR velocityVector = DirectX::XMLoadFloat3(&velocity);
	XMFLOAT3 magnitude;
	DirectX::XMStoreFloat3(&magnitude, DirectX::XMVector3Length(velocityVector));

	// Compute the position on the surface of sphere
	// This should the the center of the sphere vector plus the velocity vector scaled down to the size of the radius
	XMVECTOR velocityVectorScaled = DirectX::XMVectorScale(DirectX::XMVector3Normalize(velocityVector), radius - 0.005f); // Take a little bit off the radius otherwise there will be a small gap
	XMVECTOR positionVector = DirectX::XMLoadFloat3(&position);
	XMVECTOR surfacePositionVector = DirectX::XMVectorAdd(positionVector, velocityVectorScaled);

	XMFLOAT3 surfacePosition;
	DirectX::XMStoreFloat3(&surfacePosition, surfacePositionVector);

	if (magnitude.x > 0.0f)
	{
		// 1. Scale the cone so it is twice as wide as the cylinder
		// 2. Translate the cone along the z-axis so that it will reside at the end of the cylinder
		// 3. Rotate the cone through space so it points in the correct direction
		// 4. Translate the cone to the correct location
		m_modelMatrix = DirectX::XMMatrixScaling(2 * m_xyScaling, 2 * m_xyScaling, 0.1f) // scale down the z-stretch by 100
			* DirectX::XMMatrixTranslation(0.0f, 0.0f, magnitude.x / 100.0f)
			* ComputeRotationMatrix(velocity)
			* DirectX::XMMatrixTranslation(surfacePosition.x, surfacePosition.y, surfacePosition.z);

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
		context->DrawIndexed(m_coneIndexCount, 0, 0);
	}
}

XMMATRIX ArrowMesh::ComputeRotationMatrix(XMFLOAT3 velocity)
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