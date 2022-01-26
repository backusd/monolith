#include "SimulationManager.h"

UserState SimulationManager::m_userState = UserState::VIEW;

std::unique_ptr<Simulation> SimulationManager::m_simulation = nullptr;
//std::shared_ptr<Atom> SimulationManager::m_selectedAtom = nullptr;
std::shared_ptr<Atom> SimulationManager::m_atomHoveredOver = nullptr;
std::shared_ptr<Atom> SimulationManager::m_atomBeingClicked = nullptr;

//std::shared_ptr<Bond> SimulationManager::m_selectedBond = nullptr;
std::shared_ptr<Bond> SimulationManager::m_bondHoveredOver = nullptr;

std::shared_ptr<Atom> SimulationManager::m_bondAtom1 = nullptr;
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
	else if (SimulationManager::BondIsSelected(bond))
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
		// If m_atomHoveredOver is not nullptr and is different from m_bondAtom1, first check if 
		// a bond already exists between both atoms. If a bond already exists, then just 
		// populate that bond in the right pane for editing. Otherwise, create a new bond and
		// populate the info on the right pane
		
		// if m_bondAtom1 is not null, then we have begun dragging to attempt to create a bond
		if (m_bondAtom1 != nullptr)
		{
			// m_atomHoveredOver is nullptr then we are not hovering over an atom
			// AND check if we are hovering over the original atom 
			if (m_atomHoveredOver == nullptr || m_atomHoveredOver == m_bondAtom1)
			{
				// if the bond is not nullptr, then we must update m_newBond
				if (m_newBond != nullptr)
				{
					// if the bond did not previously exist, delete it
					if (!m_bondAlreadyExisted)
						SimulationManager::DeleteBond(m_newBond);

					// Stop tracking the bond being created
					m_newBond = nullptr;
					PrimarySelectedBondChangedEvent(m_newBond);
				}
			} // we are hovering over a different atom
			else
			{
				// if m_newBond is nullptr, we need to create a new bond
				if (m_newBond == nullptr)
				{
					// if a bond already exists between m_bondAtom1 and m_atomHoveredOver, just load that bond info
					if (m_bondAtom1->HasBondWithAtom(m_atomHoveredOver))
					{
						m_newBond = m_bondAtom1->GetBondWithAtom(m_atomHoveredOver);
						m_bondAlreadyExisted = true;						
					}
					else
					{
						// Create a new bond 
						m_newBond = SimulationManager::CreateBond(m_bondAtom1, m_atomHoveredOver);
						m_bondAlreadyExisted = false;
					}

					PrimarySelectedBondChangedEvent(m_newBond);
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

						// check if there is already a bond between m_bondAtom1 and m_atomHoveredOver
						if (m_bondAtom1->HasBondWithAtom(m_atomHoveredOver))
						{
							m_newBond = m_bondAtom1->GetBondWithAtom(m_atomHoveredOver);
							m_bondAlreadyExisted = true;
						}
						else
						{
							// Create a new bond 
							m_newBond = SimulationManager::CreateBond(m_bondAtom1, m_atomHoveredOver);
							m_bondAlreadyExisted = false;
						}

						PrimarySelectedBondChangedEvent(m_newBond);
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
	switch (m_userState)
	{
		// In VIEW and EDIT_BONDS, allow the bond to be hovered over
	case UserState::VIEW:
	case UserState::EDIT_BONDS:
		// Only update and trigger the event if the atom changed
		if (bond != m_bondHoveredOver)
		{
			m_bondHoveredOver = bond;
			BondHoveredOverChangedEvent(bond); // CAN be nullptr
		}
		break;
	
	case UserState::EDIT_VELOCITY_ARROWS:
		// In the edit velocity arrows, don't allow bonds to be hovered over and selected
		m_bondHoveredOver = nullptr;
		break;
	}
}

void SimulationManager::SimulationClickDown()
{
	switch (m_userState)
	{
		// For both the EDIT_VELOCTY_ARROWS and VIEW state, just set the atomBeingClicked to the one that is hovered
	case UserState::EDIT_VELOCITY_ARROWS:
	case UserState::VIEW:
		// On the down click, we simply want to keep track of what atom is being clicked
		// If the user is just grabbing the simulation to rotate it, the mouse will likely
		// be moved off the atom and therefore will not actually click the atom when the mouse
		// is released
		m_atomBeingClicked = m_atomHoveredOver;

		// During the VIEW state, don't worry about bond hovering/selection
		break;

	case UserState::EDIT_BONDS:
		// Set bondAtom1 to atomHoveredOver
		//		If atomHoveredOver is not nullptr, then we will start making a bond. Otherwise, bondAtom1 will
		//		be nullptr and we will just move the scene around and not make any bond
		m_bondAtom1 = m_atomHoveredOver;
		m_bondAlreadyExisted = false;
		m_newBond = nullptr;
		break;
	}
}
void SimulationManager::SimulationClickUp()
{
	switch (m_userState)
	{
	case UserState::EDIT_BONDS:		
		// Reset variables
		m_bondAtom1 = nullptr;
		m_newBond = nullptr;
		m_bondAlreadyExisted = false;

		// Update the selected bond
		SimulationManager::SetPrimarySelectedBond(m_bondHoveredOver);

		break;

	case UserState::EDIT_VELOCITY_ARROWS:
	case UserState::VIEW:
		// if the atom that was hovered over when the mouse was clicked down is still being
		// hovered over, then trigger the necessary click event
		if (m_atomBeingClicked != nullptr && m_atomBeingClicked == m_atomHoveredOver)
		{
			AtomClickedEvent(m_atomBeingClicked);
		}

		// Set the atom being clicked to nullptr
		m_atomBeingClicked = nullptr;
		break;
	}
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
	else if (SimulationManager::AtomIsSelected(atom)) // Make sure the atom is no longer selected
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
		PrimarySelectedAtomChangedEvent(atom);
	}
}
void SimulationManager::SetPrimarySelectedBond(std::shared_ptr<Bond> bond)
{	
	if (m_primarySelectedBond != bond)
	{
		m_primarySelectedBond = bond;
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