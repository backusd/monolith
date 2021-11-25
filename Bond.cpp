#include "Bond.h"
#include "Atom.h" // required because we call functions on Atom class which is forward declared in Bond.h

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;


Bond::Bond(const std::shared_ptr<Atom>& atom1, const std::shared_ptr<Atom>& atom2) :
	m_atom1(atom1),
	m_atom2(atom2),
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