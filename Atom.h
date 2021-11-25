#pragma once
#include "pch.h"

#include "Bond.h"
#include "Constants.h"
#include "DeviceResources.h"
#include "Electron.h"
#include "Enums.h"
#include "SphereMesh.h"
#include "ArrowMesh.h"

#include <string>
#include <memory>
#include <vector>

#include <typeinfo>

class Atom
{
public:
	// Constructors
	Atom(const std::shared_ptr<DeviceResources>& deviceResources,
		Element element,
		DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity);

	Atom(const std::shared_ptr<DeviceResources>& deviceResources,
		Element element,
		DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity,
		int neutronCount, int electronCount);

	// If you want to explicitly set the radius
	Atom(const std::shared_ptr<DeviceResources>& deviceResources,
		Element element,
		DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity,
		int neutronCount, int electronCount,
		float radius);

	// Virtual destructor
	virtual ~Atom() {}

	// Update
	virtual void Update(double timeDelta, const std::vector<std::shared_ptr<Atom>>& atoms, DirectX::XMFLOAT3 boxDimensions) = 0;

	// Show / hide velocity arrows
	void ShowVelocityArrow() { m_showVelocityArrow = true; }
	void HideVelocityArrow() { m_showVelocityArrow = false; }
	void SwitchVelocityArrowVisibility() { m_showVelocityArrow = !m_showVelocityArrow; }

	// Render
	void Render(DirectX::XMMATRIX viewProjectionMatrix);
	void RenderVelocityArrow(DirectX::XMMATRIX viewProjectionMatrix);
	DirectX::XMMATRIX ModelMatrix() { return m_sphereMesh->ModelMatrix(); }
	DirectX::XMMATRIX TranslationMatrix() { return DirectX::XMMatrixTranslation(m_position.x, m_position.y, m_position.z); }

	// Get
	DirectX::XMFLOAT3 Position() { return m_position; };
	DirectX::XMFLOAT3 Velocity() { return m_velocity; };
	Element ElementType() { return m_element; }
	float Mass() { return static_cast<float>(m_element + m_neutronCount); }
	int ProtonsCount() { return m_element; }
	int NeutronsCount() { return m_neutronCount; }
	int ElectronsCount() { return static_cast<int>(m_electrons.size()); }
	float Radius() { return m_radius; }
	int Charge() { return ProtonsCount() - ElectronsCount(); }

	std::wstring Name();

	std::vector<std::shared_ptr<Bond>> Bonds() { return m_bonds; }

	bool HasBondWithAtom(const std::shared_ptr<Atom>& atom);

	// Set
	void Velocity(DirectX::XMFLOAT3 velocity) { m_velocity = velocity; }
	void SetSphereMesh(const std::shared_ptr<SphereMesh>& mesh) { m_sphereMesh = mesh; }
	void SetArrowMesh(const std::shared_ptr<ArrowMesh>& mesh) { m_arrowMesh = mesh; }

	void SetPositionX(float positionX) { m_position.x = positionX; }
	void SetPositionY(float positionY) { m_position.y = positionY; }
	void SetPositionZ(float positionZ) { m_position.z = positionZ; }
	void SetVelocityX(float velocityX) { m_velocity.x = velocityX; }
	void SetVelocityY(float velocityY) { m_velocity.y = velocityY; }
	void SetVelocityZ(float velocityZ) { m_velocity.z = velocityZ; }

	//void AddBond(std::vector<std::shared_ptr<Bond>> bonds);
	void AddBond(const std::shared_ptr<Bond>& bond) { m_bonds.push_back(bond); }

protected:

	std::shared_ptr<SphereMesh> m_sphereMesh;
	std::shared_ptr<ArrowMesh> m_arrowMesh;

	DirectX::XMFLOAT3		m_position;
	DirectX::XMFLOAT3		m_velocity;

	Element								   m_element;
	std::vector<std::shared_ptr<Electron>> m_electrons;

	std::vector<std::shared_ptr<Bond>>	m_bonds;

	int				m_neutronCount;

	float			m_radius;

	bool			m_showVelocityArrow;
};