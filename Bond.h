#pragma once
#include "pch.h"

#include "CylinderMesh.h"
#include "MeshManager.h"

#include <memory>

class Atom;

class Bond
{
public:
	Bond(const std::shared_ptr<Atom>& atom1, const std::shared_ptr<Atom>& atom2);

	void DeleteBonds() {
		m_atom1 = nullptr;
		m_atom2 = nullptr;
	}

	void Render(DirectX::XMMATRIX viewProjectionMatrix);

	bool IncludesAtom(const std::shared_ptr<Atom>& atom){ return m_atom1 == atom || m_atom2 == atom; }

	void SwitchAtom(const std::shared_ptr<Atom>& oldAtom, const std::shared_ptr<Atom>& newAtom);

private:
	std::shared_ptr<Atom> m_atom1;
	std::shared_ptr<Atom> m_atom2;

	std::shared_ptr<CylinderMesh> m_cylinderMesh;

	float m_springConstant;
};