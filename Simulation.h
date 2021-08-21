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

	void AddNewAtom(std::shared_ptr<Atom> atom);
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
	template<typename T>
	std::shared_ptr<T> CreateAtom(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity);

	std::shared_ptr<DeviceResources> m_deviceResources;

	// Box
	DirectX::XMFLOAT3	m_boxDimensions;		// 3 floats to hold the MAX x,y,z dimensions for the simulation box (ex. if x = 10, then x-axis = [-10, 10])
	bool				m_boxVisible;			// If true, the dimension box will be outlined

	// Time
	float		m_elapsedTime;

	// Atoms
	std::vector<std::shared_ptr<Atom>> m_atoms;			// List of Atoms active in the simulation

	// State
	bool m_paused;
};

template<typename T>
std::shared_ptr<T> Simulation::CreateAtom(DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity)
{
	std::shared_ptr<T> atom = std::make_shared<T>(m_deviceResources, position, velocity);
	atom->SetMesh(MeshManager::GetSphereMesh());
	return atom;
}