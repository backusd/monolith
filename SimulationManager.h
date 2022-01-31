#pragma once
#include "pch.h"

#include "Bond.h"
#include "Simulation.h"

#include <functional>
#include <memory>


// Enumeration to track what state the user is in
enum class UserState
{
	VIEW,					// User is just viewing the simulation (mouse is used for dragging/spinning the simulation)
	EDIT_BONDS,				// User is creating new bonds (mouse is used to drag a new bond between two atoms)
	EDIT_VELOCITY_ARROWS	// User is editing velocity arrows for an atom
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
	static void DeleteBond(const std::shared_ptr<Bond>& bond);

	template<typename T>
	static std::shared_ptr<T> AddNewAtom(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity);

	template<typename T>
	static void ChangeSelectedAtomType();

	static void RemoveAtom(std::shared_ptr<Atom> atom);
	static void RemoveAllAtoms();

	// static void SelectAtom(int index);
	//static void SelectAtom(std::shared_ptr<Atom> atom);
	//static std::shared_ptr<Atom> GetSelectedAtom() { return m_selectedAtom; }

	static void SetPrimarySelectedAtomPositionX(float positionX) { if (m_primarySelectedAtom != nullptr) m_primarySelectedAtom->SetPositionX(positionX); }
	static void SetPrimarySelectedAtomPositionY(float positionY) { if (m_primarySelectedAtom != nullptr) m_primarySelectedAtom->SetPositionY(positionY); }
	static void SetPrimarySelectedAtomPositionZ(float positionZ) { if (m_primarySelectedAtom != nullptr) m_primarySelectedAtom->SetPositionZ(positionZ); }
	static void SetPrimarySelectedAtomVelocityX(float velocityX) { if (m_primarySelectedAtom != nullptr) m_primarySelectedAtom->SetVelocityX(velocityX); }
	static void SetPrimarySelectedAtomVelocityY(float velocityY) { if (m_primarySelectedAtom != nullptr) m_primarySelectedAtom->SetVelocityY(velocityY); }
	static void SetPrimarySelectedAtomVelocityZ(float velocityZ) { if (m_primarySelectedAtom != nullptr) m_primarySelectedAtom->SetVelocityZ(velocityZ); }

	static void BoxDimensions(float value) { m_simulation->BoxDimensions(value); }
	static void ExpandBoxDimensionsIfNecessary() { m_simulation->ExpandBoxDimensionsIfNecessary(); }
	static float BoxDimensionsMinimum() { return m_simulation->BoxDimensionsMinimum(); }


	static void AtomHoveredOver(std::shared_ptr<Atom> atom);
	static std::shared_ptr<Atom> AtomHoveredOver() { return m_atomHoveredOver; }

	static void BondHoveredOver(std::shared_ptr<Bond> bond);
	static std::shared_ptr<Bond> BondHoveredOver() { return m_bondHoveredOver; }

	static void SimulationClickDown();
	static void SimulationClickUp();

	static void ShowAllVelocityArrows() { m_simulation->ShowAllVelocityArrows(); }
	static void HideAllVelocityArrows() { m_simulation->HideAllVelocityArrows(); }

	static void SetUserState(UserState state);
	static UserState GetUserState() { return m_userState; }

	//static void SelectBond(std::shared_ptr<Bond> bond) { SelectedBondChangedEvent(bond); }
	
	//static std::shared_ptr<Bond> GetSelectedBond() { return m_selectedBond; }
	static std::shared_ptr<Bond> GetBondHoveredOver() { return m_bondHoveredOver; }





	// Methods for [Primary] Selected Atom(s)/Bond(s)
	static void SetPrimarySelectedAtom(std::shared_ptr<Atom> atom);
	static void SetPrimarySelectedBond(std::shared_ptr<Bond> bond);

	static void ClearPrimarySelectedAtom() { m_primarySelectedAtom = nullptr; }
	static void ClearPrimarySelectedBond() { m_primarySelectedBond = nullptr; }

	static std::shared_ptr<Atom> GetPrimarySelectedAtom() { return m_primarySelectedAtom; }
	static std::shared_ptr<Bond> GetPrimarySelectedBond() { return m_primarySelectedBond; }

	//       selected vector of atoms/bonds
	static bool AtomIsSelected(std::shared_ptr<Atom> atom);
	static bool BondIsSelected(std::shared_ptr<Bond> bond);

	static void SelectAtom(std::shared_ptr<Atom> atom);
	static void SelectBond(std::shared_ptr<Bond> bond);

	static void UnselectAtom(std::shared_ptr<Atom> atom);
	static void UnselectBond(std::shared_ptr<Bond> bond);

	static void SwitchAtomSelectedUnselected(std::shared_ptr<Atom> atom);
	static void SwitchBondSelectedUnselected(std::shared_ptr<Bond> bond);

	static void ClearSelectedAtoms() { m_selectedAtoms.clear(); }
	static void ClearSelectedBonds() { m_selectedBonds.clear(); }

	static std::vector<std::shared_ptr<Atom>> GetSelectedAtoms() { return m_selectedAtoms; }
	static std::vector<std::shared_ptr<Bond>> GetSelectedBonds() { return m_selectedBonds; }




	//static void TriggerPrimarySelectedBondChangedEvent(std::shared_ptr<Bond> bond) { PrimarySelectedBondChangedEvent(bond); }

	// Event setters
	static void SetPlayPauseChangedEvent(std::function<void(bool)> function) { PlayPauseChangedEvent = function; }
	
	static void SetAtomHoveredOverChangedEvent(std::function<void(std::shared_ptr<Atom>)> function) { AtomHoveredOverChangedEvent = function; }
	static void SetAtomClickedEvent(std::function<void(std::shared_ptr<Atom>)> function) { AtomClickedEvent = function; }
	static void SetPrimarySelectedAtomChangedEvent(std::function<void(std::shared_ptr<Atom>)> function) { PrimarySelectedAtomChangedEvent = function; }
	
	static void SetBondHoveredOverChangedEvent(std::function<void(std::shared_ptr<Bond>)> function) { BondHoveredOverChangedEvent = function; }
	static void SetBondClickedEvent(std::function<void(std::shared_ptr<Bond>)> function) { BondClickedEvent = function; }
	static void SetPrimarySelectedBondChangedEvent(std::function<void(std::shared_ptr<Bond>)> function) { PrimarySelectedBondChangedEvent = function; }

	// Clear all event handlers
	static void ClearAtomBondSelectionEvents();
private:
	// Disallow creation of a SimulationManager object
	SimulationManager() {}

	static UserState m_userState;

	static std::unique_ptr<Simulation> m_simulation;

	// The primary selected atom is the atom that will be highlighted/selected when on the Add Atoms page 
	// or the Edit Velocity Arrows page
	static std::shared_ptr<Atom> m_primarySelectedAtom;
	static std::shared_ptr<Bond> m_primarySelectedBond;

	static std::vector<std::shared_ptr<Atom>> m_selectedAtoms;
	static std::vector<std::shared_ptr<Bond>> m_selectedBonds;




	// Additional data regarding the user's interaction with the simulation
	// This does not belong in the simulation class itself because the simulation should 
	// only be responsible for itself and not hold state info regarding human interaction
	//static std::shared_ptr<Atom> m_selectedAtom;
	static std::shared_ptr<Atom> m_atomHoveredOver;
	static std::shared_ptr<Atom> m_atomSelectedOnClickDown;

	

	// Track which bond is being hovered over
	static std::shared_ptr<Bond> m_bondHoveredOver;
	static std::shared_ptr<Bond> m_bondSelectedOnClickDown;

	// Additional data to track which atoms the user is creating bonds for
	static std::shared_ptr<Bond> m_newBond;
	static bool m_bondAlreadyExisted;



	// Events
	// Play/Pause Simulation event - parameter = true if simulation is playing - triggered when play/pause changes
	static std::function<void(bool)> PlayPauseChangedEvent;

	static std::function<void(std::shared_ptr<Atom>)> AtomHoveredOverChangedEvent;
	static std::function<void(std::shared_ptr<Atom>)> AtomClickedEvent;
	static std::function<void(std::shared_ptr<Atom>)> PrimarySelectedAtomChangedEvent;

	static std::function<void(std::shared_ptr<Bond>)> BondHoveredOverChangedEvent;
	static std::function<void(std::shared_ptr<Bond>)> BondClickedEvent;
	static std::function<void(std::shared_ptr<Bond>)> PrimarySelectedBondChangedEvent;



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
	if (typeid(*m_primarySelectedAtom) == typeid(T))
		return;

	m_primarySelectedAtom = m_simulation->ChangeAtomType<T>(m_primarySelectedAtom);
}