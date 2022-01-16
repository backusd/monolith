#include "Bond.h"
#include "Atom.h" // required because we call functions on Atom class which is forward declared in Bond.h

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;
using DirectX::XMVECTOR;


Bond::Bond(const std::shared_ptr<Atom>& atom1, const std::shared_ptr<Atom>& atom2) :
	m_atom1(atom1),
	m_atom2(atom2),
	m_type(BondType::SINGLE),
	m_cylinderMesh(MeshManager::GetCylinderMesh()),
	m_modelMatrix(DirectX::XMMatrixIdentity()),
	m_springConstant(1.0f)
{
}

void Bond::Render(XMMATRIX viewProjectionMatrix)
{
	// So as to not allow hovering over part of the cylinder that resides within the atom itself,
	// Only draw the cylinder from the surface of the atom to the other atom
	XMFLOAT3 p1 = m_atom1->Position();
	XMVECTOR p1Vector = DirectX::XMLoadFloat3(&p1);
	XMFLOAT3 p2 = m_atom2->Position();
	XMVECTOR p2Vector = DirectX::XMLoadFloat3(&p2);

	// Get the display radius, which may be smaller than actual radius if rendering in ball and stick style
	float r1 = m_atom1->DisplayRadius();
	float r2 = m_atom2->DisplayRadius();

	// Compute and normalize the vector between the two atoms
	XMVECTOR bondDirectionVector = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(p2Vector, p1Vector));
	XMFLOAT3 bondDirection;
	DirectX::XMStoreFloat3(&bondDirection, bondDirectionVector);

	// Adjust p1 by adding the radius amount along the bond direction
	float factor = 0.98f; // Multiple each by this factor so the bond does not appear outside the sphere
	r1 *= factor;
	r2 *= factor;

	p1.x += (bondDirection.x * r1);
	p1.y += (bondDirection.y * r1);
	p1.z += (bondDirection.z * r1);

	// Adjust p2 by subtracting the radius amount along the bond direction
	p2.x -= (bondDirection.x * r2);
	p2.y -= (bondDirection.y * r2);
	p2.z -= (bondDirection.z * r2);


	// Get back the model matrix for the cylinder mesh that was computed in the call to Render
	m_modelMatrix = m_cylinderMesh->Render(p1, p2, viewProjectionMatrix);
}

void Bond::SwitchAtom(const std::shared_ptr<Atom>& oldAtom, const std::shared_ptr<Atom>& newAtom)
{
	if (m_atom1 == oldAtom)
	{
		m_atom1 = newAtom;
		return;
	}

	if (m_atom2 == oldAtom)
		m_atom2 = newAtom;
}

float Bond::BondLength()
{
	XMFLOAT3 p1 = m_atom1->Position();
	XMFLOAT3 p2 = m_atom2->Position();
	XMVECTOR p1Vector = DirectX::XMLoadFloat3(&p1);
	XMVECTOR p2Vector = DirectX::XMLoadFloat3(&p2);
	XMVECTOR differenceVector = DirectX::XMVectorSubtract(p2Vector, p1Vector);
	XMFLOAT3 magnitude;
	DirectX::XMStoreFloat3(&magnitude, DirectX::XMVector3Length(differenceVector));
	return magnitude.x;
}

void Bond::SetBondType(BONDTYPE bondType)
{
	m_type = bondType;

	// Will need to look up what the equilibrium length should be and set that value as well
}

bool Bond::MouseIsOver(float mouseX, float mouseY, CD3D11_VIEWPORT viewport, DirectX::XMMATRIX projectionMatrix, DirectX::XMMATRIX viewMatrix, float& distance)
{
	XMVECTOR rayOriginVector, rayDestinationVector, rayDirectionVector;

	rayOriginVector = XMVector3Unproject(
		DirectX::XMVectorSet(mouseX, mouseY, 0.0f, 0.0f), // click point near vector
		viewport.TopLeftX,
		viewport.TopLeftY,
		viewport.Width,
		viewport.Height,
		0,
		1,
		projectionMatrix,
		viewMatrix,
		m_modelMatrix);

	rayDestinationVector = XMVector3Unproject(
		DirectX::XMVectorSet(mouseX, mouseY, 1.0f, 0.0f), // click point far vector
		viewport.TopLeftX,
		viewport.TopLeftY,
		viewport.Width,
		viewport.Height,
		0,
		1,
		projectionMatrix,
		viewMatrix,
		m_modelMatrix);

	rayDirectionVector = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(rayDestinationVector, rayOriginVector));

	//
	XMVECTOR cylinderDirectionVector = DirectX::XMVectorSet(0.0f, 0.0f, 1.0f, 1.0f);
	XMVECTOR joiningDirectionVector = DirectX::XMVector3Normalize(DirectX::XMVector3Cross(rayDirectionVector, cylinderDirectionVector));

	// Origin of cylinder
	XMVECTOR cylinderOriginVector = DirectX::XMVectorSet(0.0f, 0.0f, 0.0f, 1.0f);

	XMVECTOR joiningDistanceVector = DirectX::XMVector3Dot(joiningDirectionVector, DirectX::XMVectorSubtract(rayOriginVector, cylinderOriginVector));
	XMFLOAT3 joiningDistance;
	XMStoreFloat3(&joiningDistance, joiningDistanceVector);



	XMFLOAT3 rayOrigin, rayDirection;
	XMStoreFloat3(&rayOrigin, rayOriginVector);
	XMStoreFloat3(&rayDirection, rayDirectionVector);

	 

	// z distance should be the z value of the mouse ray when crossing the xy-plane
	// mouseRay_xyz = rayOrigin_xyz + t * rayDirection_xyz
	// 
	// mouseRay_y (set this to 0)  = rayOrigin_y + t * rayDirection_y
	// t = (0 - rayOrigin_y) / rayDirection_y
	float t = (0.0f - rayOrigin.y) / rayDirection.y;

	// mouseRay_z = rayOrigin_z + t * rayDirection_z
	//    (Variable to hold the z-coordinate of the point along the cylinder axis (0, 0, 1) where the
	//     shortest distance to the mouse ray is located)
	float cylinderZ = rayOrigin.z + t * rayDirection.z;

	// If the z value is less than 0, the mouse is below the cylinder
	// If the z value is greater than cylinder length (1 because we rescaled), the mouse is above the cylinder
	// If the joining distance is greater than the radius, we are outside the cylinder
	// Note: Using 1 as the radius and max z because we rescaled when calling XMVector3Unproject such that 
	//       now we must use the original cylinder radius which is 1 and height 1
	if (cylinderZ >= 0.0f && cylinderZ <= 1.0f && std::abs(joiningDistance.x) < 1.0f)
	{
		// Compute the point where the mouse is touching the cylinder
		// The middle of the cylinder goes from (0, 0, 0) to (0, 0, 1), so start with the point on 
		// the center line with the correct z value
		XMFLOAT3 touchPoint = XMFLOAT3(0.0f, 0.0f, cylinderZ);

		// Now for the x & y values, traverse one radius (1) along the joining vector
		// This vector is already normalized and has a z-component of 0, so just take the x & y values as-is
		XMFLOAT3 joiningDirection;
		XMStoreFloat3(&joiningDirection, joiningDirectionVector);

		touchPoint.x = joiningDirection.x;
		touchPoint.y = joiningDirection.y;

		XMVECTOR touchPointVector = XMLoadFloat3(&touchPoint);
		XMVECTOR distanceVector = DirectX::XMVector3Length(DirectX::XMVectorSubtract(touchPointVector, rayOriginVector));

		XMFLOAT3 _distance;
		XMStoreFloat3(&_distance, distanceVector);

		distance = _distance.x;

		return true;
	}

	return false;
}

XMVECTOR Bond::BondCenter()
{
	XMFLOAT3 a1 = m_atom1->Position();
	XMFLOAT3 a2 = m_atom2->Position();
	XMFLOAT3 middle = XMFLOAT3(
		(a1.x + a2.x) / 2.0f,
		(a1.y + a2.y) / 2.0f,
		(a1.z + a2.z) / 2.0f
	);

	return DirectX::XMLoadFloat3(&middle);
}