#pragma once
#include "pch.h"

#include "Atom.h"
#include "DeviceResources.h"
#include "Enums.h"
#include "Elements.h"		// <-- includes header files for all elements

#include <memory>

class AtomGenerator
{
private:
	std::shared_ptr<DeviceResources> m_deviceResources;


public:
	AtomGenerator(const std::shared_ptr<DeviceResources>& deviceResources);

	Atom* CreateAtom(Element element, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity);
	Atom* CreateAtom(Element element, DirectX::XMFLOAT3 position, DirectX::XMFLOAT3 velocity, int neutronCount, int charge);
};