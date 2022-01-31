#include "SimulationManager.h"

UserState SimulationManager::m_userState = UserState::VIEW;

std::unique_ptr<Simulation> SimulationManager::m_simulation = nullptr;
std::shared_ptr<Atom> SimulationManager::m_atomHoveredOver = nullptr;
std::shared_ptr<Atom> SimulationManager::m_atomSelectedOnClickDown = nullptr;

std::shared_ptr<Bond> SimulationManager::m_bondHoveredOver = nullptr;
std::shared_ptr<Bond> SimulationManager::m_bondSelectedOnClickDown = nullptr;
std::shared_ptr<Bond> SimulationManager::m_newBond = nullptr;
bool SimulationManager::m_bondAlreadyExisted = false;

std::shared_ptr<Atom> SimulationManager::m_primarySelectedAtom = nullptr;
std::shared_ptr<Bond> SimulationManager::m_primarySelectedBond = nullptr;

std::vector<std::shared_ptr<Atom>> SimulationManager::m_selectedAtoms = std::vector<std::shared_ptr<Atom>>();
std::vector<std::shared_ptr<Bond>> SimulationManager::m_selectedBonds = std::vector<std::shared_ptr<Bond>>();

std::function<void(bool)> SimulationManager::PlayPauseChangedEvent = [](bool value) {};
std::function<void(std::shared_ptr<Atom>)> SimulationManager::AtomHoveredOverChangedEvent = [](std::shared_ptr<Atom> atom) {};
std::function<void(std::shared_ptr<Atom>)> SimulationManager::AtomClickedEvent = [](std::shared_ptr<Atom> atom) {};
std::function<void(std::shared_ptr<Atom>)> SimulationManager::PrimarySelectedAtomChangedEvent = [](std::shared_ptr<Atom> atom) {};

std::function<void(std::shared_ptr<Bond>)> SimulationManager::BondHoveredOverChangedEvent = [](std::shared_ptr<Bond> bond) {};
std::function<void(std::shared_ptr<Bond>)> SimulationManager::BondClickedEvent = [](std::shared_ptr<Bond> bond) {};
std::function<void(std::shared_ptr<Bond>)> SimulationManager::PrimarySelectedBondChangedEvent = [](std::shared_ptr<Bond> bond) {};

void SimulationManager::ClearAtomBondSelectionEvents()
{
	AtomHoveredOverChangedEvent		= [](std::shared_ptr<Atom> atom) {};
	AtomClickedEvent				= [](std::shared_ptr<Atom> atom) {};
	PrimarySelectedAtomChangedEvent = [](std::shared_ptr<Atom> atom) {};

	BondHoveredOverChangedEvent		= [](std::shared_ptr<Bond> bond) {};
	BondClickedEvent				= [](std::shared_ptr<Bond> bond) {};
	PrimarySelectedBondChangedEvent = [](std::shared_ptr<Bond> bond) {};
}

void SimulationManager::DeleteBond(const std::shared_ptr<Bond>& bond)
{ 
	if (bond == m_primarySelectedBond)
		SimulationManager::ClearPrimarySelectedBond();
	
	if (SimulationManager::BondIsSelected(bond))
		SimulationManager::UnselectBond(bond);

	m_simulation->DeleteBond(bond); 
}

void SimulationManager::AtomHoveredOver(std::shared_ptr<Atom> atom)
{
	// Only update and trigger the event if the atom changed
	if (atom != m_atomHoveredOver)
	{
		m_atomHoveredOver = atom;
		AtomHoveredOverChangedEvent(atom); // CAN be nullptr
	}

	switch (m_userState)
	{
	case UserState::VIEW:
		// Nothing to do here
		break;

	case UserState::EDIT_BONDS:
		// If m_atomHoveredOver is not nullptr and is different from m_atomSelectedOnClickDown, first check if 
		// a bond already exists between both atoms. If a bond already exists, then just 
		// populate that bond in the right pane for editing. Otherwise, create a new bond and
		// populate the info on the right pane
		
		// if m_atomSelectedOnClickDown is not null, then we have begun dragging to attempt to create a bond
		if (m_atomSelectedOnClickDown != nullptr)
		{
			// if m_atomHoveredOver is nullptr then we are not hovering over an atom
			// AND check if we are hovering over the original atom 
			if (m_atomHoveredOver == nullptr || m_atomHoveredOver == m_atomSelectedOnClickDown)
			{
				// if the bond is not nullptr, then we must update m_newBond to be nullptr
				if (m_newBond != nullptr)
				{
					// if the bond did not previously exist, delete it
					if (!m_bondAlreadyExisted)
						SimulationManager::DeleteBond(m_newBond);

					// Stop tracking the bond being created
					m_newBond = nullptr;
					SetPrimarySelectedBond(m_newBond);
				}
			} // we are hovering over a different atom
			else
			{
				// if m_newBond is nullptr, we need to create a new bond
				if (m_newBond == nullptr)
				{
					// if a bond already exists between m_atomSelectedOnClickDown and m_atomHoveredOver, just load that bond info
					if (m_atomSelectedOnClickDown->HasBondWithAtom(m_atomHoveredOver))
					{
						m_newBond = m_atomSelectedOnClickDown->GetBondWithAtom(m_atomHoveredOver);
						m_bondAlreadyExisted = true;						
					}
					else
					{
						// Create a new bond 
						m_newBond = SimulationManager::CreateBond(m_atomSelectedOnClickDown, m_atomHoveredOver);
						m_bondAlreadyExisted = false;
					}

					SetPrimarySelectedBond(m_newBond);
				}
				else
				{
					// if the bond does not include the atom being hovered over, then we need to update the bond
					if (!m_newBond->IncludesAtom(m_atomHoveredOver))
					{
						// if the bond did not previously exist, delete it
						if (!m_bondAlreadyExisted)
							SimulationManager::DeleteBond(m_newBond);

						// Clear the current bond
						m_newBond = nullptr;

						// check if there is already a bond between m_atomSelectedOnClickDown and m_atomHoveredOver
						if (m_atomSelectedOnClickDown->HasBondWithAtom(m_atomHoveredOver))
						{
							m_newBond = m_atomSelectedOnClickDown->GetBondWithAtom(m_atomHoveredOver);
							m_bondAlreadyExisted = true;
						}
						else
						{
							// Create a new bond 
							m_newBond = SimulationManager::CreateBond(m_atomSelectedOnClickDown, m_atomHoveredOver);
							m_bondAlreadyExisted = false;
						}

						SetPrimarySelectedBond(m_newBond);
					}
				}
			}
		}

		break;

	case UserState::EDIT_VELOCITY_ARROWS:
		// Do nothing - in other words, all atoms to be hovered in this mode
		break;
	}
}

void SimulationManager::BondHoveredOver(std::shared_ptr<Bond> bond)
{
	// Regardless of what state we are in, update the hovered bond if it is different
	if (bond != m_bondHoveredOver)
	{
		m_bondHoveredOver = bond;
		BondHoveredOverChangedEvent(bond); // CAN be nullptr
	}
}

void SimulationManager::SimulationClickDown()
{
	// A click down should simply reset all of the selection variables that are used for all states
	//
	// Update the atom/bond that is being selected - CAN be nullptr
	m_atomSelectedOnClickDown = m_atomHoveredOver;
	m_bondSelectedOnClickDown = m_bondHoveredOver;

	m_bondAlreadyExisted = false;
	m_newBond = nullptr;
}
void SimulationManager::SimulationClickUp()
{
	// Regardless of state, test which variables are not nullptr and trigger the correct event
	//
	// Only select an atom/bond if the one being hovered over is the one that was hovered over when
	// the click down occurred
	if (m_bondSelectedOnClickDown != nullptr && m_bondSelectedOnClickDown == m_bondHoveredOver)
	{
		BondClickedEvent(m_bondSelectedOnClickDown);
	}
	else if (m_atomSelectedOnClickDown != nullptr && m_atomSelectedOnClickDown == m_atomHoveredOver)
	{
		AtomClickedEvent(m_atomSelectedOnClickDown);
	}


	// Reset variables
	m_newBond = nullptr;
	m_bondAlreadyExisted = false;

	// Set the atom/bond being clicked to nullptr
	m_atomSelectedOnClickDown = nullptr;
	m_bondSelectedOnClickDown = nullptr;
}

void SimulationManager::RemoveAtom(std::shared_ptr<Atom> atom) 
{ 
	// If we are removing the primary selected atom, then we must check what state we are in
	if (m_primarySelectedAtom == atom)
	{
		int atomIndex = m_simulation->GetAtomIndex(atom);

		switch (m_userState)
		{
			// If we are in VIEW, then we must make a new selection
		case UserState::VIEW:

			if (atomIndex == 0)
			{
				// If its the first atom, try getting the second if it exists
				m_primarySelectedAtom = (m_simulation->AtomCount() == 1) ? nullptr : m_simulation->GetAtomAtIndex(1);
			}
			else
			{
				// Get the atom one before the selected one
				m_primarySelectedAtom = m_simulation->GetAtomAtIndex(atomIndex - 1);
			}

			PrimarySelectedAtomChangedEvent(m_primarySelectedAtom);
			break;


			// Default should just be to set it to nullptr, although these modes should not allow
			// you to delete an atom
		default:
			m_primarySelectedAtom = nullptr;
			break;
		}
	}
	
	if (SimulationManager::AtomIsSelected(atom)) // Make sure the atom is no longer selected
		SimulationManager::UnselectAtom(atom);

	// Removing the atom from the simulation will also delete all bonds to the atom
	m_simulation->RemoveAtom(atom); 
}

void SimulationManager::SetUserState(UserState state) 
{ 
	m_userState = state; 

	// whenever there is a state change, make sure we null each atom pointer
	//m_selectedAtom = nullptr;
	//m_atomHoveredOver = nullptr;
	//m_atomBeingClicked = nullptr;
	//m_bondAtom1 = nullptr;
	//m_bondAtom2 = nullptr;
	//m_newBond = nullptr;
}


void SimulationManager::SetPrimarySelectedAtom(std::shared_ptr<Atom> atom)
{ 
	if (m_primarySelectedAtom != atom)
	{
		m_primarySelectedAtom = atom;

		if (atom != nullptr)
			PrimarySelectedAtomChangedEvent(atom);
	}
}
void SimulationManager::SetPrimarySelectedBond(std::shared_ptr<Bond> bond)
{	
	if (m_primarySelectedBond != bond)
	{
		m_primarySelectedBond = bond;

		if (bond != nullptr)
			PrimarySelectedBondChangedEvent(bond);
	} 
}



bool SimulationManager::AtomIsSelected(std::shared_ptr<Atom> atom)
{
	for (std::shared_ptr<Atom> a : m_selectedAtoms)
	{
		if (a == atom)
			return true;
	}
	return false;
}
bool SimulationManager::BondIsSelected(std::shared_ptr<Bond> bond)
{
	for (std::shared_ptr<Bond> b : m_selectedBonds)
	{
		if (b == bond)
			return true;
	}
	return false;
}

void SimulationManager::SelectAtom(std::shared_ptr<Atom> atom)
{
	if (!SimulationManager::AtomIsSelected(atom))
		m_selectedAtoms.push_back(atom);
}
void SimulationManager::SelectBond(std::shared_ptr<Bond> bond)
{
	if (!SimulationManager::BondIsSelected(bond))
		m_selectedBonds.push_back(bond);
}

void SimulationManager::UnselectAtom(std::shared_ptr<Atom> atom)
{
	for (unsigned int iii = 0; iii < m_selectedAtoms.size(); ++iii)
	{
		if (m_selectedAtoms[iii] == atom)
		{
			m_selectedAtoms.erase(m_selectedAtoms.begin() + iii);
			return;
		}
	}
}
void SimulationManager::UnselectBond(std::shared_ptr<Bond> bond)
{
	for (unsigned int iii = 0; iii < m_selectedBonds.size(); ++iii)
	{
		if (m_selectedBonds[iii] == bond)
		{
			m_selectedBonds.erase(m_selectedBonds.begin() + iii);
			return;
		}
	}
}

void SimulationManager::SwitchAtomSelectedUnselected(std::shared_ptr<Atom> atom)
{
	if (SimulationManager::AtomIsSelected(atom))
		SimulationManager::UnselectAtom(atom);
	else
		m_selectedAtoms.push_back(atom);
}
void SimulationManager::SwitchBondSelectedUnselected(std::shared_ptr<Bond> bond)
{
	if (SimulationManager::BondIsSelected(bond))
		SimulationManager::UnselectBond(bond);
	else
		m_selectedBonds.push_back(bond);
}

void SimulationManager::RemoveAllAtoms()
{ 
	SimulationManager::ClearPrimarySelectedAtom();
	SimulationManager::ClearPrimarySelectedBond();
	SimulationManager::ClearSelectedAtoms();
	SimulationManager::ClearSelectedBonds();

	m_simulation->RemoveAllAtoms();
}