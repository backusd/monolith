#pragma once
#include "pch.h"

#include "Simulation.h"

#include <functional>
#include <memory>

class SimulationManager
{
public:
	static void CreateSimulation(const std::shared_ptr<DeviceResources>& deviceResources)
	{
		m_simulation = std::make_unique<Simulation>(deviceResources);
	}

	static std::vector<std::shared_ptr<Atom>> Atoms() { return m_simulation->Atoms(); }
	static unsigned int AtomCount() { return m_simulation->AtomCount(); }

	static void Update(StepTimer const& timer) { m_simulation->Update(timer); }

	// Pause the simulation and trigger the event - parameter = true -> simulation is playing
	static void Pause() { m_simulation->PauseSimulation(); PlayPauseChangedEvent(false); }
	static bool SimulationIsPaused() { return m_simulation->IsPaused(); }

	// Switch play/pause and trigger the event - parameter = true -> simulation is playing
	static void SwitchPlayPause() { m_simulation->SwitchPlayPause(); PlayPauseChangedEvent(!m_simulation->IsPaused()); }

	static DirectX::XMFLOAT3 BoxDimensions() { return m_simulation->BoxDimensions(); }

	template<typename T>
	static std::shared_ptr<T> AddNewAtom(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity);

	template<typename T>
	static void ChangeSelectedAtomType();

	static void RemoveAtom(std::shared_ptr<Atom> atom) { m_simulation->RemoveAtom(atom); }
	static void RemoveAllAtoms() { m_simulation->RemoveAllAtoms(); }

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


	// Event setters
	static void SetPlayPauseChangedEvent(std::function<void(bool)> function) { PlayPauseChangedEvent = function; }
	

private:
	// Disallow creation of a SimulationManager object
	SimulationManager() {}

	static std::unique_ptr<Simulation> m_simulation;

	// Events
	// Play/Pause Simulation event - parameter = true if simulation is playing - triggered when play/pause changes
	static std::function<void(bool)> PlayPauseChangedEvent;

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