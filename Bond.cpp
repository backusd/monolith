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
	m_springConstant(1.0f)
{
}

void Bond::Render(XMMATRIX viewProjectionMatrix)
{
	m_cylinderMesh->Render(
		m_atom1->Position(),
		m_atom2->Position(),
		viewProjectionMatrix
	);
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