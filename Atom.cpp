#include "Atom.h"

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;


Atom::Atom(const std::shared_ptr<DeviceResources>& deviceResources, Element element, XMFLOAT3 position, XMFLOAT3 velocity) :
	m_element(element),
	m_position(position),
	m_velocity(velocity),
	m_neutronCount(element),
	m_radius(Constants::AtomicRadii[element]),
	m_sphereMesh(nullptr),
	m_arrowMesh(nullptr),
	m_showVelocityArrow(false)
{
	// Populate the electrons
	for (int iii = 0; iii < element; ++iii)
		m_electrons.push_back(std::shared_ptr<Electron>(new Electron()));
}

Atom::Atom(const std::shared_ptr<DeviceResources>& deviceResources, Element element, XMFLOAT3 position, XMFLOAT3 velocity, int neutronCount, int electronCount) :
	m_element(element),
	m_position(position),
	m_velocity(velocity),
	m_neutronCount(neutronCount),
	m_radius(Constants::AtomicRadii[element]),
	m_sphereMesh(nullptr),
	m_arrowMesh(nullptr),
	m_showVelocityArrow(false)
{
	// Populate the electrons
	for (int iii = 0; iii < electronCount; ++iii)
		m_electrons.push_back(std::shared_ptr<Electron>(new Electron()));
}

Atom::Atom(const std::shared_ptr<DeviceResources>& deviceResources, Element element, XMFLOAT3 position, XMFLOAT3 velocity, int neutronCount, int electronCount, float radius) :
	m_element(element),
	m_position(position),
	m_velocity(velocity),
	m_neutronCount(neutronCount),
	m_radius(radius),
	m_sphereMesh(nullptr),
	m_arrowMesh(nullptr),
	m_showVelocityArrow(false)
{
	// Populate the electrons
	for (int iii = 0; iii < electronCount; ++iii)
		m_electrons.push_back(std::shared_ptr<Electron>(new Electron()));
}


void Atom::Render(XMMATRIX viewProjectionMatrix)
{
	m_sphereMesh->Render(m_position, m_radius, viewProjectionMatrix);
}

void Atom::RenderVelocityArrow(XMMATRIX viewProjectionMatrix)
{
	if (m_showVelocityArrow)
		m_arrowMesh->Render(m_position, m_velocity, m_radius, viewProjectionMatrix);
}

std::wstring Atom::Name()
{
	// This will return something like "class Hydrogen"
	std::string name = typeid(*this).name();

	// Erase everything up to and including the first space char
	name.erase(0, name.find(" ") + 1);

	// Create the wide string
	std::wstring w(name.begin(), name.end());

	return w;
}

bool Atom::HasBondWithAtom(const std::shared_ptr<Atom>& atom)
{
	for (std::shared_ptr<Bond> bond : m_bonds)
	{
		if (bond->IncludesAtom(atom))
			return true;
	}

	return false;
}

std::shared_ptr<Bond> Atom::GetBondWithAtom(const std::shared_ptr<Atom>& atom)
{
	for (std::shared_ptr<Bond> bond : m_bonds)
	{
		if (bond->IncludesAtom(atom))
			return bond;
	}

	return  nullptr;
}

/*
void Atom::AddBond(std::vector<std::shared_ptr<Bond>> bonds)
{
	for (std::shared_ptr<Bond> bond : bonds)
		m_bonds.push_back(bond);
}
*/

void Atom::RemoveBond(const std::shared_ptr<Bond>& bond)
{
	int bondIndex = -1;
	for (unsigned int iii = 0; iii < m_bonds.size(); ++iii)
	{
		if (m_bonds[iii] == bond)
		{
			bondIndex = iii;
			break;
		}
	}

	if (bondIndex != -1)
		m_bonds.erase(m_bonds.begin() + bondIndex);
}