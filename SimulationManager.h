#pragma once
#include "pch.h"

#include "Bond.h"
#include "Simulation.h"

#include <functional>
#include <memory>


// Enumeration to track what state the user is in
enum class UserState
{
	VIEW,		// User is just viewing the simulation (mouse is used for dragging/spinning the simulation)
	EDIT_BONDS	// User is creating new bonds (mouse is used to drag a new bond between two atoms)
};



class SimulationManager
{
public:
	static void CreateSimulation(const std::shared_ptr<DeviceResources>& deviceResources)
	{
		m_simulation = std::make_unique<Simulation>(deviceResources);
	}

	static void DestroyBonds() { m_simulation->DestroyBonds(); }

	static std::vector<std::shared_ptr<Atom>> Atoms() { return m_simulation->Atoms(); }
	static unsigned int AtomCount() { return m_simulation->AtomCount(); }

	static std::vector<std::shared_ptr<Bond>> Bonds() { return m_simulation->Bonds(); }

	static void Update(StepTimer const& timer) { m_simulation->Update(timer); }

	// Pause the simulation and trigger the event - parameter = true -> simulation is playing
	static void Pause() { m_simulation->PauseSimulation(); PlayPauseChangedEvent(false); }
	static bool SimulationIsPaused() { return m_simulation->IsPaused(); }

	// Switch play/pause and trigger the event - parameter = true -> simulation is playing
	static void SwitchPlayPause() { m_simulation->SwitchPlayPause(); PlayPauseChangedEvent(!m_simulation->IsPaused()); }

	static DirectX::XMFLOAT3 BoxDimensions() { return m_simulation->BoxDimensions(); }

	static std::shared_ptr<Bond> CreateBond(const std::shared_ptr<Atom>& atom1, const std::shared_ptr<Atom>& atom2) { return m_simulation->CreateBond(atom1, atom2); }
	static void DeleteBond(const std::shared_ptr<Bond>& bond) { m_simulation->DeleteBond(bond); }

	template<typename T>
	static std::shared_ptr<T> AddNewAtom(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity);

	template<typename T>
	static void ChangeSelectedAtomType();

	static void RemoveAtom(std::shared_ptr<Atom> atom);
	static void RemoveAllAtoms() { m_simulation->RemoveAllAtoms(); m_selectedAtom = nullptr; }

	// static void SelectAtom(int index);
	static void SelectAtom(std::shared_ptr<Atom> atom);
	static std::shared_ptr<Atom> GetSelectedAtom() { return m_selectedAtom; }

	static void SelectedAtomPositionX(float positionX) { if (m_selectedAtom != nullptr) m_selectedAtom->SetPositionX(positionX); }
	static void SelectedAtomPositionY(float positionY) { if (m_selectedAtom != nullptr) m_selectedAtom->SetPositionY(positionY); }
	static void SelectedAtomPositionZ(float positionZ) { if (m_selectedAtom != nullptr) m_selectedAtom->SetPositionZ(positionZ); }
	static void SelectedAtomVelocityX(float velocityX) { if (m_selectedAtom != nullptr) m_selectedAtom->SetVelocityX(velocityX); }
	static void SelectedAtomVelocityY(float velocityY) { if (m_selectedAtom != nullptr) m_selectedAtom->SetVelocityY(velocityY); }
	static void SelectedAtomVelocityZ(float velocityZ) { if (m_selectedAtom != nullptr) m_selectedAtom->SetVelocityZ(velocityZ); }

	static void BoxDimensions(float value) { m_simulation->BoxDimensions(value); }
	static void ExpandBoxDimensionsIfNecessary() { m_simulation->ExpandBoxDimensionsIfNecessary(); }
	static float BoxDimensionsMinimum() { return m_simulation->BoxDimensionsMinimum(); }


	static void AtomHoveredOver(std::shared_ptr<Atom> atom);
	static std::shared_ptr<Atom> AtomHoveredOver() { return m_atomHoveredOver; }

	static void SimulationClickDown();
	static void SimulationClickUp();

	static void ShowAllVelocityArrows() { m_simulation->ShowAllVelocityArrows(); }
	static void HideAllVelocityArrows() { m_simulation->HideAllVelocityArrows(); }

	static void SetUserState(UserState state);
	static UserState GetUserState() { return m_userState; }

	// Event setters
	static void SetPlayPauseChangedEvent(std::function<void(bool)> function) { PlayPauseChangedEvent = function; }
	static void SetAtomHoveredOverChangedEvent(std::function<void(std::shared_ptr<Atom>)> function) { AtomHoveredOverChangedEvent = function; }
	static void SetAtomClickedEvent(std::function<void(std::shared_ptr<Atom>)> function) { AtomClickedEvent = function; }
	static void SetSelectedAtomChangedEvent(std::function<void(std::shared_ptr<Atom>)> function) { SelectedAtomChangedEvent = function; }
	static void SetSelectedBondChangedEvent(std::function<void(std::shared_ptr<Bond>)> function) { SelectedBondChangedEvent = function; }

private:
	// Disallow creation of a SimulationManager object
	SimulationManager() {}

	static UserState m_userState;

	static std::unique_ptr<Simulation> m_simulation;

	// Additional data regarding the user's interaction with the simulation
	// This does not belong in the simulation class itself because the simulation should 
	// only be responsible for itself and not hold state info regarding human interaction
	static std::shared_ptr<Atom> m_selectedAtom;
	static std::shared_ptr<Atom> m_atomHoveredOver;
	static std::shared_ptr<Atom> m_atomBeingClicked;

	// Additional data to track which atoms the user is creating bonds for
	static std::shared_ptr<Atom> m_bondAtom1;
	static std::shared_ptr<Bond> m_newBond;
	static bool m_bondAlreadyExisted;



	// Events
	// Play/Pause Simulation event - parameter = true if simulation is playing - triggered when play/pause changes
	static std::function<void(bool)> PlayPauseChangedEvent;

	static std::function<void(std::shared_ptr<Atom>)> AtomHoveredOverChangedEvent;
	static std::function<void(std::shared_ptr<Atom>)> AtomClickedEvent;
	static std::function<void(std::shared_ptr<Atom>)> SelectedAtomChangedEvent;
	static std::function<void(std::shared_ptr<Bond>)> SelectedBondChangedEvent;



};

template<typename T>
std::shared_ptr<T> SimulationManager::AddNewAtom(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity)
{
	return m_simulation->AddNewAtom<T>(position, velocity);
}

template<typename T>
void SimulationManager::ChangeSelectedAtomType()
{
	// Only change the type if it differs
	if (typeid(*m_selectedAtom) == typeid(T))
		return;

	m_selectedAtom = m_simulation->ChangeAtomType<T>(m_selectedAtom);
}