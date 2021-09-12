#pragma once

#include "SphereMesh.h"
#include "ArrowMesh.h"

#include <memory>


class MeshManager
{
public:
	static void CreateMeshes(const std::shared_ptr<DeviceResources>& deviceResources)
	{
		m_sphereMesh = std::make_shared<SphereMesh>(deviceResources);
		m_arrowMesh = std::make_shared<ArrowMesh>(deviceResources);
	}

	static std::shared_ptr<SphereMesh> GetSphereMesh() { return m_sphereMesh; }
	static std::shared_ptr<ArrowMesh> GetArrowMesh() { return m_arrowMesh; }

private:
	// Disallow creation of a SimulationManager object
	MeshManager() {}

	static std::shared_ptr<SphereMesh> m_sphereMesh;
	static std::shared_ptr<ArrowMesh> m_arrowMesh;
};