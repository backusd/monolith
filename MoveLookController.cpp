#include "MoveLookController.h"

MoveLookController::MoveLookController(DirectX::XMFLOAT3 boxDimensions)
{
	m_eyeVec = { 0.0f, 0.0f, 2 * boxDimensions.z, 0.0f };
	m_atVec = { 0.0f, 0.0f, 0.0f, 0.0f };								// look at the origin
	m_upVec = { 0.0f, 1.0f, 0.0f, 0.0f };								// Up in the positive y-direction
}

DirectX::XMMATRIX MoveLookController::ViewMatrix()
{
	return DirectX::XMMatrixLookAtRH(m_eyeVec, m_atVec, m_upVec);
}