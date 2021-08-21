#include "Atom.h"

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;


Atom::Atom(const std::shared_ptr<DeviceResources>& deviceResources, Element element, XMFLOAT3 position, XMFLOAT3 velocity) :
	m_element(element),
	m_position(position),
	m_velocity(velocity),
	m_neutronCount(element),
	m_radius(Constants::AtomicRadii[element]),
	m_mesh(nullptr)
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
	m_radius(Constants::AtomicRadii[element])
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
	m_radius(radius)
{
	// Populate the electrons
	for (int iii = 0; iii < electronCount; ++iii)
		m_electrons.push_back(std::shared_ptr<Electron>(new Electron()));
}


void Atom::Render(XMMATRIX viewProjectionMatrix)
{
	m_mesh->Render(m_position, m_radius, viewProjectionMatrix);
}