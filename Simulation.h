#pragma once
#include "pch.h"

#include "Atom.h"
#include "Elements.h"
#include "MeshManager.h"
#include "StepTimer.h"

#include <cmath>
#include <vector>
#include <memory>


class Simulation
{
public:
	Simulation(const std::shared_ptr<DeviceResources>& deviceResources);

	template<typename T>
	void AddNewAtom(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity);

	void RemoveAtom();

	void PlaySimulation() { m_paused = false; }
	void PauseSimulation() { m_paused = true; m_elapsedTime = -1.0f; }
	bool IsPaused() { return m_paused; }

	void StartRecording();
	void StopRecording();

	void LoadSimulationFromFile();
	void SaveSimulationToFile();

	void ClearSimulation();	// Completely delete the entire active simulation
	void ResetSimulation(); // Reset the simulation state to where it was before ever pressing Play

	void Update(StepTimer const& timer);
	void SwitchPlayPause() { m_paused = !m_paused; if (m_paused) m_elapsedTime = -1.0f; }

	void SelectAtom(int index) { m_selectedAtomIndex = index; }

	void SelectedAtomPositionX(float positionX) { m_atoms[m_selectedAtomIndex]->SetPositionX(positionX); }
	void SelectedAtomPositionY(float positionY) { m_atoms[m_selectedAtomIndex]->SetPositionY(positionY); }
	void SelectedAtomPositionZ(float positionZ) { m_atoms[m_selectedAtomIndex]->SetPositionZ(positionZ); }
	void SelectedAtomVelocityX(float velocityX) { m_atoms[m_selectedAtomIndex]->SetVelocityX(velocityX); }
	void SelectedAtomVelocityY(float velocityY) { m_atoms[m_selectedAtomIndex]->SetVelocityY(velocityY); }
	void SelectedAtomVelocityZ(float velocityZ) { m_atoms[m_selectedAtomIndex]->SetVelocityZ(velocityZ); }

	// GET
	std::vector<std::shared_ptr<Atom>> Atoms() { return m_atoms; }

	DirectX::XMFLOAT3	BoxDimensions() { return m_boxDimensions; }

	bool		BoxVisible() { return m_boxVisible; }

	float		ElapsedTime() { return m_elapsedTime; }

	// SET
	void BoxDimensions(DirectX::XMFLOAT3 dimensions) { m_boxDimensions = dimensions; }

	void BoxVisible(bool visible) { m_boxVisible = visible; }

	void ElapsedTime(float time) { m_elapsedTime = time; }

private:

	std::shared_ptr<DeviceResources> m_deviceResources;

	// Box
	DirectX::XMFLOAT3	m_boxDimensions;		// 3 floats to hold the MAX x,y,z dimensions for the simulation box (ex. if x = 10, then x-axis = [-10, 10])
	bool				m_boxVisible;			// If true, the dimension box will be outlined

	// Time
	float		m_elapsedTime;

	// Atoms
	std::vector<std::shared_ptr<Atom>> m_atoms;			// List of Atoms active in the simulation
	int m_selectedAtomIndex;							// Index of atom that is selected

	// State
	bool m_paused;


};

template<typename T>
void Simulation::AddNewAtom(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity)
{
	std::shared_ptr<T> atom = std::make_shared<T>(m_deviceResources, position, velocity);
	atom->SetSphereMesh(MeshManager::GetSphereMesh());
	atom->SetArrowMesh(MeshManager::GetArrowMesh());
	

	// We need to make sure that the atoms are sorted by element type
	// So insert the new atom in the first spot after all of the elements with smaller
	// element numbers

	// Get the first index where the current element is greater than or equal to the new atom
	unsigned int index;
	for (index = 0; index < m_atoms.size(); ++index)
	{
		if (m_atoms[index]->ElementType() >= atom->ElementType())
			break;
	}

	// if we got to the end, just add the new atom
	// else, insert it at the appropriate spot in the vector
	if (index == m_atoms.size())
		m_atoms.push_back(atom);
	else
		m_atoms.insert(m_atoms.begin() + index, atom);
}