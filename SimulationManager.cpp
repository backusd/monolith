#include "SimulationManager.h"

std::unique_ptr<Simulation> SimulationManager::m_simulation = nullptr;
std::shared_ptr<Atom> SimulationManager::m_selectedAtom = nullptr;
std::shared_ptr<Atom> SimulationManager::m_atomHoveredOver = nullptr;
std::shared_ptr<Atom> SimulationManager::m_atomBeingClicked = nullptr;


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
}

void SimulationManager::SimulationClickDown()
{
	// On the down click, we simply want to keep track of what atom is being clicked
	// If the user is just grabbing the simulation to rotate it, the mouse will likely
	// be moved off the atom and therefore will not actually click the atom when the mouse
	// is released
	m_atomBeingClicked = m_atomHoveredOver;
}
void SimulationManager::SimulationClickUp()
{
	// if the atom that was hovered over when the mouse was clicked down is still being
	// hovered over, then trigger the necessary click event
	if (m_atomBeingClicked != nullptr && m_atomBeingClicked == m_atomHoveredOver)
	{
		AtomClickedEvent(m_atomBeingClicked);		
	}

	// Set the atom being clicked to nullptr
	m_atomBeingClicked = nullptr;
}

//void SimulationManager::SelectAtom(int index) 
//{
//	m_selectedAtom = m_simulation->GetAtom(index); 
//}
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