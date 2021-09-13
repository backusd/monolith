#pragma once
#include "pch.h"

#include "Constants.h"
#include "DeviceResources.h"
#include "Electron.h"
#include "Enums.h"
#include "SphereMesh.h"
#include "ArrowMesh.h"

#include <memory>
#include <vector>

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

	// Render
	void Render(DirectX::XMMATRIX viewProjectionMatrix);
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

protected:

	std::shared_ptr<SphereMesh> m_sphereMesh;
	std::shared_ptr<ArrowMesh> m_arrowMesh;

	DirectX::XMFLOAT3		m_position;
	DirectX::XMFLOAT3		m_velocity;

	Element								   m_element;
	std::vector<std::shared_ptr<Electron>> m_electrons;

	int				m_neutronCount;

	float			m_radius;
};