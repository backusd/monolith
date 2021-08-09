#pragma once
#include "pch.h"

class MoveLookController
{
public:
	MoveLookController(DirectX::XMFLOAT3 boxDimensions);

	DirectX::XMMATRIX ViewMatrix();

private:
	// Eye/at/up vectors
	DirectX::XMVECTOR m_eyeVec;
	DirectX::XMVECTOR m_atVec;
	DirectX::XMVECTOR m_upVec;
};