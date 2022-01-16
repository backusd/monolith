#pragma once
#include "pch.h"

#include "CylinderMesh.h"
#include "MeshManager.h"
#include "Enums.h"

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

	std::shared_ptr<Atom> Atom1() { return m_atom1; }
	std::shared_ptr<Atom> Atom2() { return m_atom2; }

	float BondLength();
	float EquilibriumLength();
	float SpringConstant() { return m_springConstant; }

	BONDTYPE GetBondType() { return m_type; }
	void SetBondType(BONDTYPE bondType);

	DirectX::XMMATRIX ModelMatrix() { return m_modelMatrix; }
	bool MouseIsOver(float mouseX, float mouseY, CD3D11_VIEWPORT viewport, DirectX::XMMATRIX projectionMatrix, DirectX::XMMATRIX viewMatrix, float& distance);

	DirectX::XMVECTOR BondCenter();

private:
	std::shared_ptr<Atom> m_atom1;
	std::shared_ptr<Atom> m_atom2;

	BONDTYPE m_type;

	std::shared_ptr<CylinderMesh> m_cylinderMesh;
	DirectX::XMMATRIX m_modelMatrix;

	float m_springConstant;
};