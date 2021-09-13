#pragma once
#include "pch.h"

#include "DeviceResources.h"
#include "HLSLStructures.h"

#include <memory>
#include <vector>


/*
	The purpose of this class is to house the vertex and index
	buffers that are required to render a sphere

	It also is responsible for computing the model (world) matrix for
	the sphere and makes the Draw call to render it
	*/
class ArrowMesh
{
private:
	std::shared_ptr<DeviceResources>		m_deviceResources;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_indexBuffer;
	uint32_t								m_indexCount;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_modelViewProjectionBuffer;
	ModelViewProjectionConstantBuffer		m_modelViewProjectionBufferData;

	DirectX::XMMATRIX m_modelMatrix;

	void CreateModelViewProjectionBuffer();
	void CreateAndLoadVertexAndIndexBuffers();

	DirectX::XMMATRIX ComputeRotationMatrix(DirectX::XMFLOAT3 direction);


public:
	ArrowMesh(const std::shared_ptr<DeviceResources>& deviceResources);

	/* Render must be passed the following:
		1. XMFLOAT3 position       - position of the atom   -> used for translating to compute the model matrix
		2. float    radius         - radius of the atom     -> used for scaling to compute the model matrix
		3. XMMATRIX viewProjection - view projection matrix -> used for computing the final modelviewprojection matrix

		Upstream:
		1. Simulation Render should have already
				IASetInputLayout
				IASetPrimitiveTopology <- Only true as long as we are always rendered triangle lists
				VSSetShader
				PSSetShader
				PSSetConstantBuffers1  <- This will require a lookup for each atom to make sure the correct material is used

		Responsilities:
		1. Render will perform the following
				IASetVertexBuffers
				ISSetIndexBuffer
				VSSetConstantBuffers1
				DrawIndexed
	*/
	void Render(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, float radius, DirectX::XMMATRIX viewProjection);

	DirectX::XMMATRIX ModelMatrix() { return m_modelMatrix; }

};