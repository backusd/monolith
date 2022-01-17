#include "Atom.h"

using DirectX::XMFLOAT3;
using DirectX::XMMATRIX;
using DirectX::XMVECTOR;


Atom::Atom(const std::shared_ptr<DeviceResources>& deviceResources, ELEMENT element, XMFLOAT3 position, XMFLOAT3 velocity) :
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

Atom::Atom(const std::shared_ptr<DeviceResources>& deviceResources, ELEMENT element, XMFLOAT3 position, XMFLOAT3 velocity, int neutronCount, int electronCount) :
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

Atom::Atom(const std::shared_ptr<DeviceResources>& deviceResources, ELEMENT element, XMFLOAT3 position, XMFLOAT3 velocity, int neutronCount, int electronCount, float radius) :
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

bool Atom::MouseIsOver(float mouseX, float mouseY, CD3D11_VIEWPORT viewport, DirectX::XMMATRIX projectionMatrix, DirectX::XMMATRIX viewMatrix, float& distance)
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
		DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z));

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
		DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z));

	rayDirectionVector = DirectX::XMVector3Normalize(DirectX::XMVectorSubtract(rayDestinationVector, rayOriginVector));

	//
	XMFLOAT3 origin, direction;
	XMStoreFloat3(&origin, rayOriginVector);
	XMStoreFloat3(&direction, rayDirectionVector);

	float a, b, c, discriminant;
	float radius = m_radius;

	// Calculate the a, b, and c coefficients.
	a = (direction.x * direction.x) + (direction.y * direction.y) + (direction.z * direction.z);
	b = ((direction.x * origin.x) + (direction.y * origin.y) + (direction.z * origin.z)) * 2.0f;
	c = ((origin.x * origin.x) + (origin.y * origin.y) + (origin.z * origin.z)) - (radius * radius);

	// Find the discriminant.
	discriminant = (b * b) - (4 * a * c);

	// if discriminant is negative the picking ray missed the sphere, otherwise it intersected the sphere.
	if (discriminant < 0.0f)
		return false;

	// determine the distance to the closest point
	float minRoot = (-b + std::sqrtf(discriminant)) / (2 * a);
	float maxRoot = (-b - std::sqrtf(discriminant)) / (2 * a);

	if (minRoot > maxRoot)
		std::swap(minRoot, maxRoot);

	// Get the smallest positive root
	if (minRoot < 0)
	{
		minRoot = maxRoot;
		if (minRoot < 0)
			return false;		// Both roots are negative so return false
	}

	// Return the distance to the sphere
	distance = minRoot;

	return true;
}
