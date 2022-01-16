#pragma once
#include "pch.h"

#include "Constants.h"
#include "DeviceResources.h"
#include "Enums.h"
#include "HLSLStructures.h"

#include <memory>
#include <vector>


class CylinderMesh
{
private:
	std::shared_ptr<DeviceResources>		m_deviceResources;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cylinderVertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_cylinderIndexBuffer;
	uint32_t								m_cylinderIndexCount;

	Microsoft::WRL::ComPtr<ID3D11Buffer>	m_modelViewProjectionBuffer;
	ModelViewProjectionConstantBuffer		m_modelViewProjectionBufferData;

	DirectX::XMMATRIX m_modelMatrix;

	float m_xyScaling;

	void CreateModelViewProjectionBuffer();
	void LoadCylinderVertexAndIndexBuffers();

	DirectX::XMMATRIX ComputeRotationMatrix(DirectX::XMFLOAT3 velocity);


public:
	CylinderMesh(const std::shared_ptr<DeviceResources>& deviceResources);

	float Radius() { return m_xyScaling; }

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

		Returns: returns the computed model matrix. Because every bond will run Render and they all have a shared_ptr to the
		         same CylinderMesh, each call will update the model matrix. However, each bond will want to keep track of the
				 model matrix that was computed so we can easily test for mouse over the cylinder without re-computing the
				 model matrix.
	*/
	DirectX::XMMATRIX Render(DirectX::XMFLOAT3 position1, DirectX::XMFLOAT3 position2, DirectX::XMMATRIX viewProjection);

	DirectX::XMMATRIX ModelMatrix() { return m_modelMatrix; }
};