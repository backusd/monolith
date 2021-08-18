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

	static bool SimulationIsPaused() { return m_simulation->IsPaused(); }

	static void SwitchPlayPause() { m_simulation->SwitchPlayPause(); }

	static void DestroySimulation() { m_simulation = nullptr; }

	static DirectX::XMFLOAT3 BoxDimensions() { return m_simulation->BoxDimensions(); }

private:
	// Disallow creation of a SimulationManager object
	SimulationManager() {}

	static std::unique_ptr<Simulation> m_simulation;
};