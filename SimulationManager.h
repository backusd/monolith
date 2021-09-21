#pragma once
#include "pch.h"

#include "Simulation.h"

#include <memory>

class SimulationManager
{
public:
	static void CreateSimulation(const std::shared_ptr<DeviceResources>& deviceResources)
	{
		m_simulation = std::make_unique<Simulation>(deviceResources);
	}

	static std::vector<std::shared_ptr<Atom>> Atoms() { return m_simulation->Atoms(); }

	static void Update(StepTimer const& timer) { m_simulation->Update(timer); }

	static void Pause() { m_simulation->PauseSimulation(); }
	static bool SimulationIsPaused() { return m_simulation->IsPaused(); }

	static void SwitchPlayPause() { m_simulation->SwitchPlayPause(); }

	static DirectX::XMFLOAT3 BoxDimensions() { return m_simulation->BoxDimensions(); }

	template<typename T>
	static std::shared_ptr<T> AddNewAtom(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity);

	template<typename T>
	static void ChangeSelectedAtomType();

	static void SelectAtom(int index) { m_simulation->SelectAtom(index); }
	static void SelectAtom(std::shared_ptr<Atom> atom) { m_simulation->SelectAtom(atom); }
	static std::shared_ptr<Atom> GetSelectedAtom() { return m_simulation->GetSelectedAtom(); }

	static void SelectedAtomPositionX(float positionX) { m_simulation->SelectedAtomPositionX(positionX); }
	static void SelectedAtomPositionY(float positionY) { m_simulation->SelectedAtomPositionY(positionY); }
	static void SelectedAtomPositionZ(float positionZ) { m_simulation->SelectedAtomPositionZ(positionZ); }
	static void SelectedAtomVelocityX(float velocityX) { m_simulation->SelectedAtomVelocityX(velocityX); }
	static void SelectedAtomVelocityY(float velocityY) { m_simulation->SelectedAtomVelocityY(velocityY); }
	static void SelectedAtomVelocityZ(float velocityZ) { m_simulation->SelectedAtomVelocityZ(velocityZ); }

	static void BoxDimensions(float value) { m_simulation->BoxDimensions(value); }

private:
	// Disallow creation of a SimulationManager object
	SimulationManager() {}

	static std::unique_ptr<Simulation> m_simulation;
};

template<typename T>
std::shared_ptr<T> SimulationManager::AddNewAtom(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity)
{
	return m_simulation->AddNewAtom<T>(position, velocity);
}

template<typename T>
void SimulationManager::ChangeSelectedAtomType()
{
	m_simulation->ChangeSelectedAtomType<T>();
}