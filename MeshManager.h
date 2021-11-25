#pragma once

#include "SphereMesh.h"
#include "ArrowMesh.h"
#include "CylinderMesh.h"

#include <memory>


class MeshManager
{
public:
	static void CreateMeshes(const std::shared_ptr<DeviceResources>& deviceResources)
	{
		m_sphereMesh = std::make_shared<SphereMesh>(deviceResources);
		m_arrowMesh = std::make_shared<ArrowMesh>(deviceResources);
		m_cylinderMesh = std::make_shared<CylinderMesh>(deviceResources);
	}

	static std::shared_ptr<SphereMesh> GetSphereMesh() { return m_sphereMesh; }
	static std::shared_ptr<ArrowMesh> GetArrowMesh() { return m_arrowMesh; }
	static std::shared_ptr<CylinderMesh> GetCylinderMesh() { return m_cylinderMesh; }

private:
	// Disallow creation of a SimulationManager object
	MeshManager() {}

	static std::shared_ptr<SphereMesh> m_sphereMesh;
	static std::shared_ptr<ArrowMesh> m_arrowMesh;
	static std::shared_ptr<CylinderMesh> m_cylinderMesh;
};