#include "SimulationManager.h"

UserState SimulationManager::m_userState = UserState::VIEW;

std::unique_ptr<Simulation> SimulationManager::m_simulation = nullptr;
std::shared_ptr<Atom> SimulationManager::m_selectedAtom = nullptr;
std::shared_ptr<Atom> SimulationManager::m_atomHoveredOver = nullptr;
std::shared_ptr<Atom> SimulationManager::m_atomBeingClicked = nullptr;

std::shared_ptr<Atom> SimulationManager::m_bondAtom1 = nullptr;
// std::shared_ptr<Atom> SimulationManager::m_bondAtom2 = nullptr;
std::shared_ptr<Bond> SimulationManager::m_newBond = nullptr;

std::function<void(bool)> SimulationManager::PlayPauseChangedEvent = [](bool value) {};
std::function<void(std::shared_ptr<Atom>)> SimulationManager::AtomHoveredOverChangedEvent = [](std::shared_ptr<Atom> atom) {};
std::function<void(std::shared_ptr<Atom>)> SimulationManager::AtomClickedEvent = [](std::shared_ptr<Atom> atom) {};
std::function<void(std::shared_ptr<Atom>)> SimulationManager::SelectedAtomChangedEvent = [](std::shared_ptr<Atom> atom) {};




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
			if (m_atomHoveredOver == nullptr)
			{
				// if the bond is not nullptr, then it needs to be deleted
				if (m_newBond != nullptr)
				{
					int iii = 0;



				}
			} // check if we are hovering over the original atom 
			else if (m_atomHoveredOver == m_bondAtom1)
			{
				// if the bond is not nullptr, then it needs to be deleted
				if (m_newBond != nullptr)
				{
					int iii = 0;



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
						int i = 1;
					}
					else
					{
						// Create a new bond 
						m_newBond = SimulationManager::CreateBond(m_bondAtom1, m_atomHoveredOver);
					}
				}
				else
				{
					// check to make sure m_newBond is between m_bondAtom1 and m_atomHoveredOver

					int iii = 0;





				}
			}
		}

		// Reset the atom variables
		// m_bondAtom1 = m_atomHoveredOver;
		break;
	}
}

void SimulationManager::SimulationClickDown()
{
	switch (m_userState)
	{
	case UserState::VIEW:
		// On the down click, we simply want to keep track of what atom is being clicked
		// If the user is just grabbing the simulation to rotate it, the mouse will likely
		// be moved off the atom and therefore will not actually click the atom when the mouse
		// is released
		m_atomBeingClicked = m_atomHoveredOver;
		break;

	case UserState::EDIT_BONDS:
		// If the user state is EDIT_BONDS, then the down click needs to signal a new bond is being made
		m_bondAtom1 = m_atomHoveredOver;
		m_newBond = nullptr;
		break;
	}
}
void SimulationManager::SimulationClickUp()
{
	switch (m_userState)
	{
	case UserState::EDIT_BONDS:		
		// Reset m_bondAtom1 to be the atom that is currently hovered over
		m_bondAtom1 = m_atomHoveredOver;

		// Reset m_newBond to be nullptr - don't need to it to be populated
		m_newBond = nullptr;

		break;

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

void SimulationManager::SelectAtom(std::shared_ptr<Atom> atom) 
{ 
	if (m_selectedAtom != atom)
	{
		m_selectedAtom = atom;
		SelectedAtomChangedEvent(atom);
	}
}

void SimulationManager::RemoveAtom(std::shared_ptr<Atom> atom) 
{ 
	// MUST do extra work if the atom being removed is the selected one
	if (m_selectedAtom == atom)
	{
		int atomIndex = m_simulation->GetAtomIndex(atom);
		
		if (atomIndex == 0)
		{
			// If its the first atom, try getting the second if it exists
			if (m_simulation->AtomCount() == 1)
				m_selectedAtom = nullptr;
			else
				m_selectedAtom = m_simulation->GetAtomAtIndex(1);
		}
		else
		{
			// Get the atom one before the selected one
			m_selectedAtom = m_simulation->GetAtomAtIndex(atomIndex - 1);
		}

		SelectedAtomChangedEvent(m_selectedAtom);
	}

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