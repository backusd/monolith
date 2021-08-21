#pragma once

#include "SphereMesh.h"

#include <memory>


class MeshManager
{
public:
	static void CreateSphereMesh(const std::shared_ptr<DeviceResources>& deviceResources)
	{
		m_mesh = std::make_shared<SphereMesh>(deviceResources);
	}

	static std::shared_ptr<SphereMesh> GetSphereMesh() { return m_mesh; }

private:
	// Disallow creation of a SimulationManager object
	MeshManager() {}

	static std::shared_ptr<SphereMesh> m_mesh;
};